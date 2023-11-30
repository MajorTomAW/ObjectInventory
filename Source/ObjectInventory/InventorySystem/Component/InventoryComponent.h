// Copyright © 2023 MajorT and DevilCrab . All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


class UItemObject;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChangedSignature, UItemObject* , Item);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class OBJECTINVENTORY_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	

	/* Called before an Item gets added to the Inventory
	 * Used to manage stacking etc..*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintAuthorityOnly, DisplayName = "PreAddItem")
	void PreAddItem(UItemObject* InItem, UItemObject* TargetDestination);

	/* Called before an Item gets removed from the Inventory
	 * Used to manage stacking etc..*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintAuthorityOnly, DisplayName = "PreRemoveItem")
	void PreRemoveItem(UItemObject* ItemToRemove);

	UFUNCTION(BlueprintCallable)
	void AddItemToInventory(UItemObject* InItem, UItemObject* TargetDestination);

	UFUNCTION(BlueprintCallable)
	void RemoveItemFromInventory(UItemObject* const InSlot);

	UFUNCTION(BlueprintCallable)
	void DestroyItemObject(UItemObject* const InItem);

	UFUNCTION(BlueprintCallable)
	UItemObject* CreateItemObject(TSubclassOf<UItemObject> ItemObjectClass);

	/** Value getter */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UItemObject* GetInventory() const { return Inventory; }

	UFUNCTION(BlueprintCallable)
	void TransferItem(UItemObject* ItemToTransfer, UItemObject* TargetDestination);

	/** Delegates */
	UPROPERTY(BlueprintAssignable)
	FOnInventoryChangedSignature OnInventoryChanged;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryChangedSignature OnItemAdded;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryChangedSignature OnInventoryInitialized;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryChangedSignature OnItemStacked;

protected:
	virtual void BeginPlay() override;
	void InitParentBag();

private:
	UPROPERTY(Replicated)
	TObjectPtr<UItemObject> Inventory;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UItemObject> ParentBagClass;
};
