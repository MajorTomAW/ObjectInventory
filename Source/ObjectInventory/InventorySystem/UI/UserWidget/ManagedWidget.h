// Copyright © 2023 MajorT and DevilCrab . All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectInventory/InventorySystem/ItemObject/ItemObject.h"
#include "ManagedWidget.generated.h"

class UOverlayController;
/**
 * 
 */
UCLASS()
class OBJECTINVENTORY_API UManagedWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UOverlayController> WidgetController;

	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UOverlayController* InWidgetController);

	UFUNCTION()
	void OnInventoryChanged(UItemObject* NewInventory);

	UFUNCTION()
	void OnItemStackChanged(UItemObject* Item);

	UFUNCTION()
	void OnItemAdded(UItemObject* NewItem);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();

	UFUNCTION(BlueprintImplementableEvent)
	void InventoryChanged(UItemObject* NewInventory);

	UFUNCTION(BlueprintImplementableEvent)
	void ItemAdded(UItemObject* NewItem);

	UFUNCTION(BlueprintImplementableEvent)
	void ItemStackChanged(UItemObject* Item);

	UFUNCTION(BlueprintImplementableEvent)
	void InventoryInitialized(UItemObject* Inventory);
};
