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
	
	for (UItemObject* Item : TargetDestination->GetItemContainer())
	{
		UE_LOG(LogTemp, Error, TEXT("Items: %s"), *Item->GetItemValues().ItemName.ToString());
		
		if (Item->GetItemValues().ItemTag == InItem->GetItemValues().ItemTag && Item->GetItemValues().bIsStackable)
		{
			const int NewStackCount = Item->GetItemValues().StackCount + InItem->GetItemValues().StackCount;
			Item->SetStackCount(NewStackCount, false);
			OnItemStacked.Broadcast(Item);
			OnInventoryChanged.Broadcast(TargetDestination);
			return;
		}
	}
	FItemValues ItemValues = InItem->GetItemValues();
	ItemValues.SerialNumber = FGuid::NewGuid();
	InItem->SetItemValues(ItemValues);
	
	AddItemToInventory(InItem, Inventory);
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
	for (UItemObject* Item : TargetDestination->GetItemContainer())
	{
		if (Item->GetItemValues().ItemTag == ItemToTransfer->GetItemValues().ItemTag && Item->GetItemValues().bIsStackable)
		{
			const int NewStackCount = Item->GetItemValues().StackCount + ItemToTransfer->GetItemValues().StackCount;
			Item->SetStackCount(NewStackCount, false);
			OnItemStacked.Broadcast(Item);
			OnInventoryChanged.Broadcast(Inventory);
			UE_LOG(LogTemp, Warning, TEXT("--> TRANSFER: Item already exists, increasing the stack count"));
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("--> TRANSFER: Adding the Item to the container"));
	
	TArray<UItemObject*> Objects = TargetDestination->GetItemValues().ItemContainer;
	Objects.Add(ItemToTransfer);
	TargetDestination->SetItemContainer(Objects);
	ItemToTransfer->SetParentContainer(TargetDestination);
	
	TargetDestination->OnItemAddedToContainer.Broadcast(ItemToTransfer);
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
