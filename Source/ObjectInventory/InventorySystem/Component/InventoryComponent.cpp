// Copyright © 2023 MajorT and DevilCrab . All rights reserved.


#include "InventoryComponent.h"

#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "ObjectInventory/InventorySystem/ItemObject/ItemObject.h"
#include "ObjectInventory/InventorySystem/UI/ManagedHUD.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
}

/** Interface */






void UInventoryComponent::PreRemoveItem_Implementation(UItemObject* ItemToRemove)
{
}

void UInventoryComponent::RemoveItemFromInventory(UItemObject* const InSlot)
{
}

void UInventoryComponent::PreAddItem_Implementation(UItemObject* InItem, UItemObject* TargetDestination)
{
	if (!InItem || !TargetDestination) return;
	UE_LOG(LogTemp, Warning, TEXT(" -->  Adding item: %s"), *InItem->GetItemTag().ToString());
	int StackSizeAfter = InItem->GetItemValues().StackCount;
	
	for (UItemObject* Item : TargetDestination->GetItemContainer())
	{
		if (Item->GetItemValues().ItemTag == InItem->GetItemValues().ItemTag)
		{
			const int MaxStackSize = Item->GetItemValues().MaxStackSize <= 0 ? 2147483647 : Item->GetItemValues().MaxStackSize;
			const int CurrentStackSize = Item->GetItemValues().StackCount + InItem->GetItemValues().StackCount;
			const bool bCanStack = Item->GetItemValues().bIsStackable;
			
			if (bCanStack)
			{
				StackSizeAfter = CurrentStackSize - MaxStackSize;
				int NewStackSize = CurrentStackSize;
				if (CurrentStackSize > MaxStackSize)
				{
					NewStackSize = MaxStackSize;
				}
				
				Item->SetStackCount(NewStackSize, false);
				OnItemStacked.Broadcast(Item);
				OnInventoryChanged.Broadcast(TargetDestination);

				if (StackSizeAfter <= 0) return;
			}
		}
	}
	FItemValues ItemValues = InItem->GetItemValues();
	ItemValues.StackCount = StackSizeAfter;
	ItemValues.SerialNumber = FGuid::NewGuid();
	InItem->SetItemValues(ItemValues);
	
	AddItemToInventory(InItem, TargetDestination);
}

void UInventoryComponent::AddItemToInventory(UItemObject* InItem, UItemObject* TargetDestination)
{
	TArray<UItemObject*> Objects = TargetDestination->GetItemContainer();
	Objects.Add(InItem);
	TargetDestination->SetItemContainer(Objects);
	
	InItem->SetItemOwner(this);
	InItem->SetParentContainer(TargetDestination);

	InItem->GetParentContainer()->OnItemAddedToContainer.Broadcast(InItem);
	OnItemAdded.Broadcast(InItem);
	OnInventoryChanged.Broadcast(Inventory);

	UE_LOG(LogTemp, Warning, TEXT("Values Broadcastet"));
}



void UInventoryComponent::DestroyItemObject(UItemObject* const InItem)
{
	
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInventoryComponent, Inventory);
}

UItemObject* UInventoryComponent::CreateItemObject(TSubclassOf<UItemObject> ItemObjectClass)
{
	if (ItemObjectClass && !ItemObjectClass->HasAnyClassFlags(CLASS_Abstract))
	{
		AActor* InOwner = GetOwner();
		checkf(InOwner != nullptr, TEXT("CreateLinkedItemObject called on ReplicatedItem with no owner!"));
		checkf(InOwner->HasAuthority(), TEXT("Only the server can create LinkedItemObjects!"));

		UItemObject* ItemObject = NewObject<UItemObject>(InOwner, ItemObjectClass);
		checkf(ItemObject != nullptr, TEXT("Failed to create slot!"));
		//AddReplicatedSubObject(ItemObject);

		return ItemObject;
	}
	return nullptr;
}

void UInventoryComponent::TransferItem(UItemObject* ItemToTransfer, UItemObject* TargetDestination)
{
	if (!ItemToTransfer ||!TargetDestination) return;

	// Remove item from Source
	UItemObject* Parent = ItemToTransfer->GetParentContainer();
	
	for (UItemObject* Item : Parent->GetItemValues().ItemContainer)
	{
		if (Item->GetItemValues() == ItemToTransfer->GetItemValues())
		{
			UE_LOG(LogTemp, Warning, TEXT("Item Found, now transfer"));
			TArray<UItemObject*> ItemContainer = ItemToTransfer->GetParentContainer()->GetItemValues().ItemContainer;
			ItemContainer.RemoveSingle(Item);
			ItemToTransfer->GetParentContainer()->SetItemContainer(ItemContainer);
			
			ItemToTransfer->OnItemChangedContainer.Broadcast();	
		}
	}

	// Add item to target destination
	PreAddItem(ItemToTransfer, TargetDestination);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	const ACharacter* Owner = Cast<ACharacter>(GetOwner());
	APlayerController* PC = Cast<APlayerController>(Owner->GetController());
	if (!PC) return;
	if (AManagedHUD* HUD = Cast<AManagedHUD>(PC->GetHUD()))
	{
		HUD->InitOverlay(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD is not a ManagedHUD!"));
	}
	InitParentBag();
}

void UInventoryComponent::InitParentBag()
{
	checkf(ParentBagClass, TEXT("ParentBagClass is invalid in InventoryComponent, please set a valid class!"));
	Inventory = CreateItemObject(ParentBagClass);
	OnInventoryInitialized.Broadcast(Inventory);
}
