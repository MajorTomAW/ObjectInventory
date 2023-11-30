// Copyright © 2023 MajorT and DevilCrab . All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ManagedHUD.generated.h"

struct FWidgetControllerParams;
class UOverlayController;
class UInventoryComponent;
class UManagedWidget;

/**
 * 
 */
UCLASS()
class OBJECTINVENTORY_API AManagedHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<class UManagedWidget> OverlayWidget;

	UOverlayController* GetOverlayController(const FWidgetControllerParams& InParams);
	void InitOverlay(UInventoryComponent* AC);

private:
	UPROPERTY(EditAnywhere, Category = "_ManagedHUD")
	TSubclassOf<UOverlayController> OverlayControllerClass;

	UPROPERTY()
	TObjectPtr<UOverlayController> OverlayController;

	UPROPERTY(EditAnywhere, Category = "_ManagedHUD")
	TSubclassOf<UManagedWidget> OverlayWidgetClass;
};
