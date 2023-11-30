// Copyright © 2023 MajorT and DevilCrab . All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WidgetController.generated.h"

class UInventoryComponent;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(UInventoryComponent* AC)
		: InventoryComponent(AC) {}
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UInventoryComponent> InventoryComponent = nullptr;
};

/**
 * 
 */
UCLASS()
class OBJECTINVENTORY_API UWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& InParams);

	virtual void BroadCastInitialValues();
	virtual void BindCallbacksToDependencies();

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UInventoryComponent> InventoryComponent;
};
