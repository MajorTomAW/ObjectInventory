// Copyright © 2023 MajorT and DevilCrab . All rights reserved.


#include "OverlayController.h"

#include "ObjectInventory/InventorySystem/Component/InventoryComponent.h"

void UOverlayController::BroadCastInitialValues()
{
	OnInventoryChanged.Broadcast(InventoryComponent->GetInventory());
}

void UOverlayController::BindCallbacksToDependencies()
{
	InventoryComponent->OnInventoryChanged.AddDynamic(this, &UOverlayController::InventoryChanged);
	InventoryComponent->OnItemAdded.AddDynamic(this, &UOverlayController::ItemAdded);
	InventoryComponent->OnItemStacked.AddDynamic(this, &UOverlayController::ItemStacked);
	InventoryComponent->OnInventoryInitialized.AddDynamic(this, &UOverlayController::InventoryInitialized);
}

void UOverlayController::ItemAdded(UItemObject* ItemObject)
{
	OnItemAdded.Broadcast(ItemObject);
}

void UOverlayController::InventoryChanged(UItemObject* ItemObject)
{
	OnInventoryChanged.Broadcast(ItemObject);
}

void UOverlayController::ItemStacked(UItemObject* ItemObject)
{
	OnItemStackChanged.Broadcast(ItemObject);
}

void UOverlayController::InventoryInitialized(UItemObject* Inventory)
{
	OnInventoryInitialized.Broadcast(Inventory);
}
