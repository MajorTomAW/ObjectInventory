// Copyright © 2023 MajorT and DevilCrab . All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UObject/UObjectBase.h"
#include "ReplicatedObject.generated.h"

/**
 * ReplicatedObject is a UObject that can be replicated over the network.
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class OBJECTINVENTORY_API UReplicatedObject : public UObject
{
	GENERATED_BODY()

public:
	UReplicatedObject();

	// Getting a valid UWorld from it's outer
	virtual UWorld* GetWorld() const override;

	// Blueprint: Allows us to get the owning actor of this object
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ReplicatedObject")
	FORCEINLINE AActor* GetOwningActor() const {return GetTypedOuter<AActor>(); }

	// Enables replication for this object
	FORCEINLINE virtual bool IsSupportedForNetworking() const override {return true; }

	// Called to replicate properties
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called when a property is about to be replicated
	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;

	// Called when a property is about to be replicated
	virtual bool CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack) override;

	// Called to destroy this object
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void DestroyObject();

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnDestroyed();
};
