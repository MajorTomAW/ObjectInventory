// Copyright © 2023 MajorT and DevilCrab . All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ReplicatedObject.h"
#include "ItemObject.generated.h"

class UInventoryComponent;
class UItemObject;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemChangedSignature, const UItemObject*, ItemObject);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemChangedContainer);

UENUM(Blueprintable)
enum class EItemType : uint8
{
	IT_Weapon UMETA(DisplayName = "Weapon"),
	IT_Food UMETA(DisplayName = "Food"),
	IT_Armor UMETA(DisplayName = "Armor"),
	IT_Consumable UMETA(DisplayName = "Container"),
	IT_Quest UMETA(DisplayName = "Quest"),
	IT_Misc UMETA(DisplayName = "Misc")
};

USTRUCT(BlueprintType, Blueprintable, DisplayName = "Item Info")
struct FItemValues
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText ItemName = FText::FromString("");

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FName ItemTag = FName("");

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText ItemDescription = FText::FromString("");

	UPROPERTY(BlueprintReadWrite, EditAnywhere, DisplayName = "ItemStackCount")
	int StackCount = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ItemWeight = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D* ItemIcon = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D* ItemIconActive = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsStackable = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxStackSize = -1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EItemType ItemType = EItemType::IT_Misc;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UItemObject*> ItemContainer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGuid SerialNumber = FGuid();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D WidgetSize = FVector2D(0.f, 0.f);


	bool operator==(const FItemValues& B) const
	{
		return SerialNumber == B.SerialNumber;
	}
};
/**
 * 
 */
UCLASS()
class OBJECTINVENTORY_API UItemObject : public UReplicatedObject
{
	GENERATED_BODY()

public:
	UItemObject();

	/** Ownership */
	UFUNCTION(BlueprintCallable, Category = "_Item|Ownership")
	void SetItemOwner(UInventoryComponent* NewOwner);

	UFUNCTION(BlueprintCallable, Category = "_Item|Ownership")
	void SetParentContainer(UItemObject* InParentContainer);

	/** Value Setter */
	UFUNCTION(BlueprintCallable, Category = "_Item|Info")
	void SetItemValues(FItemValues NewItemValues);

	UFUNCTION(BlueprintCallable, Category = "_Item|Info")
	void SetStackCount(int NewStackCount, bool bNotifyInventoryComponent = true /* Notify the Inventory component about this change and destroy the item if StackCount is less or equal than 0 */ );

	void SetItemContainer(const TArray<UItemObject*>& NewContainer);

	/** Value Getter */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "_Item|Ownership")
	FORCEINLINE UItemObject* GetParentContainer() const {return ParentContainer; }

	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName = "Get Item Info", Category = "_Item|Info")
	FORCEINLINE FItemValues GetItemValues() {return ItemValues; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "_Item|Info")
	FORCEINLINE TArray<UItemObject*>& GetItemContainer() {return ItemValues.ItemContainer; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "_Item|Info")
	FORCEINLINE int GetStackCount() const {return ItemValues.StackCount; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "_Item|Ownership")
	FORCEINLINE UInventoryComponent* GetItemOwner() const {return ItemOwner; }

	UFUNCTION()
	FORCEINLINE FName GetItemTag() const {return ItemValues.ItemTag; }

	/** Delegates */
	UPROPERTY(BlueprintAssignable)
	FOnItemChangedSignature OnItemChanged;

	UPROPERTY(BlueprintAssignable)
	FOnItemChangedContainer OnItemChangedContainer;

	UPROPERTY(BlueprintAssignable)
	FOnItemChangedSignature OnItemAddedToContainer;

	virtual void OnDestroyed_Implementation() override;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostCDOCompiled(const FPostCDOCompiledContext& Context) override;

	
private:
	UPROPERTY(Replicated, EditAnywhere)
	FItemValues ItemValues;

	UPROPERTY()
	TObjectPtr<UInventoryComponent> ItemOwner;

	UPROPERTY()
	TObjectPtr<UItemObject> ParentContainer;
};
