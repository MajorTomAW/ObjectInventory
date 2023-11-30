// Copyright © 2023 MajorT and DevilCrab . All rights reserved.


#include "ManagedWidget.h"

#include "ObjectInventory/InventorySystem/UI/WidgetController/Overlay/OverlayController.h"

void UManagedWidget::SetWidgetController(UOverlayController* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetController->OnInventoryChanged.AddDynamic(this, &UManagedWidget::OnInventoryChanged);
	WidgetController->OnItemAdded.AddDynamic(this, &UManagedWidget::ItemAdded);
	WidgetController->OnItemStackChanged.AddDynamic(this, &UManagedWidget::OnItemStackChanged);
	WidgetController->OnInventoryInitialized.AddDynamic(this, &UManagedWidget::InventoryInitialized);
	WidgetControllerSet();
}

void UManagedWidget::OnInventoryChanged(UItemObject* NewInventory)
{
	InventoryChanged(NewInventory);
}

void UManagedWidget::OnItemStackChanged(UItemObject* NewInventory)
{
	ItemStackChanged(NewInventory);
}

void UManagedWidget::OnItemAdded(UItemObject* NewInventory)
{
	ItemAdded(NewInventory);
}
