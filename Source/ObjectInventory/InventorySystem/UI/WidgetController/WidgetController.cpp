// Copyright © 2023 MajorT and DevilCrab . All rights reserved.


#include "WidgetController.h"

void UWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& InParams)
{
	InventoryComponent = InParams.InventoryComponent;
}

void UWidgetController::BroadCastInitialValues()
{
}

void UWidgetController::BindCallbacksToDependencies()
{
}
