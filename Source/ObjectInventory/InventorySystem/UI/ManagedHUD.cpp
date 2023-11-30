// Copyright © 2023 MajorT and DevilCrab . All rights reserved.


#include "ManagedHUD.h"
#include "Blueprint/UserWidget.h"
#include "WidgetController/Overlay/OverlayController.h"
#include "ObjectInventory/InventorySystem/Component/InventoryComponent.h"
#include "UserWidget/ManagedWidget.h"
#include "WidgetController/WidgetController.h"


UOverlayController* AManagedHUD::GetOverlayController(const FWidgetControllerParams& InParams)
{
	if (OverlayController == nullptr)
	{
		OverlayController = NewObject<UOverlayController>(this, OverlayControllerClass);
		OverlayController->SetWidgetControllerParams(InParams);
		OverlayController->BindCallbacksToDependencies();
	}
	return OverlayController;
}

void AManagedHUD::InitOverlay(UInventoryComponent* AC)
{
	UManagedWidget* Overlay = CreateWidget<UManagedWidget>(GetWorld(), OverlayWidgetClass);
	const FWidgetControllerParams Params(AC);
	UOverlayController* WidgetController = GetOverlayController(Params);

	Overlay->SetWidgetController(WidgetController);
	WidgetController->BroadCastInitialValues();
	Overlay->AddToViewport();
}
