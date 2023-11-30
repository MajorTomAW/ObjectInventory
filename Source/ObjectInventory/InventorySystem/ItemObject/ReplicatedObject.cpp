// Copyright © 2023 MajorT and DevilCrab . All rights reserved.


#include "ReplicatedObject.h"

UReplicatedObject::UReplicatedObject() {}
void UReplicatedObject::OnDestroyed_Implementation() {}


UWorld* UReplicatedObject::GetWorld() const
{
	if (const UObject* Outer = GetOuter())
	{
		return Outer->GetWorld();
	}
	return nullptr;
}

int32 UReplicatedObject::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
{
	checkf(GetOuter() != nullptr, TEXT("GetFunctionCallspace called on ReplicatedObject with no Outer!"));
	return GetOuter()->GetFunctionCallspace(Function, Stack);
}

bool UReplicatedObject::CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack)
{
	checkf(!HasAnyFlags(RF_ClassDefaultObject), TEXT("CallRemoteFunction called on ReplicatedObject with RF_ClassDefaultObject!"));

	AActor* OwningActor = GetOwningActor();
	if (UNetDriver* NetDriver = OwningActor->GetNetDriver())
	{
		NetDriver->ProcessRemoteFunction(OwningActor, Function, Parms, OutParms, Stack, this);
		return true;
	}
	return false;
}

void UReplicatedObject::DestroyObject()
{
	UE_LOG(LogTemp, Error, TEXT("Attempting to destroy object"));
	if (IsValid(this))
	{
		checkf(GetOwningActor()->HasAuthority() == true, TEXT("DestroyObject called on ReplicatedObject with no authority!"));
		MarkAsGarbage();
		OnDestroyed();
	}
}

void UReplicatedObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	if (const UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass()))
	{
		BPClass->GetLifetimeReplicatedProps(OutLifetimeProps);
	}
}
