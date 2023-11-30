// Copyright © 2023 MajorT and DevilCrab . All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ObjectInventory/InventorySystem/ItemObject/ItemObject.h"
#include "ObjectInventory/InventorySystem/UI/WidgetController/WidgetController.h"
#include "OverlayController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged, UItemObject*, ItemObject);

/**
 * 
 */
UCLASS()
class OBJECTINVENTORY_API UOverlayController : public UWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadCastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	/** Delegates */
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChanged OnItemAdded;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChanged OnInventoryChanged;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChanged OnItemStackChanged;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryChanged OnInventoryInitialized;

protected:
	UFUNCTION()
	void ItemAdded(UItemObject* ItemObject);

	UFUNCTION()
	void InventoryChanged(UItemObject* ItemObject);

	UFUNCTION()
	void ItemStacked(UItemObject* ItemObject);

	UFUNCTION()
	void InventoryInitialized(UItemObject* Inventory);
};
