// Copyright © 2023 MajorT and DevilCrab . All rights reserved.


#include "ItemObject.h"
#include "ObjectInventory/InventorySystem/Component/InventoryComponent.h"
#include "Kismet/KismetStringLibrary.h"
#include "Net/UnrealNetwork.h"

UItemObject::UItemObject()
{
	ItemValues.ItemTag = UKismetStringLibrary::Conv_StringToName(ItemValues.ItemName.ToString());
}

void UItemObject::SetItemValues(FItemValues NewItemValues)
{
	ItemValues = NewItemValues;
}

void UItemObject::SetStackCount(int NewStackCount, bool bNotifyInventoryComponent)
{
	ItemValues.StackCount = NewStackCount;
	OnItemChanged.Broadcast(this);

	if (!bNotifyInventoryComponent || !ItemOwner) return;
	ItemOwner->OnItemStacked.Broadcast(this);
}

void UItemObject::SetItemOwner(UInventoryComponent* NewOwner)
{
	ItemOwner = NewOwner;
}

void UItemObject::SetParentContainer(UItemObject* InParentContainer)
{
	ParentContainer = InParentContainer;
}

void UItemObject::SetItemContainer(const TArray<UItemObject*>& NewContainer)
{
	ItemValues.ItemContainer = NewContainer;
	OnItemChanged.Broadcast(this);
}

void UItemObject::OnDestroyed_Implementation()
{
	OnItemChangedContainer.Broadcast();
}

void UItemObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UItemObject, ItemValues);
}


void UItemObject::PostCDOCompiled(const FPostCDOCompiledContext& Context)
{
	Super::PostCDOCompiled(Context);
	FString ItemTag = "UItemBase_" + ItemValues.ItemName.ToString() + "_t";
	ItemTag.RemoveSpacesInline();
	ItemValues.ItemTag = UKismetStringLibrary::Conv_StringToName(ItemTag);
}