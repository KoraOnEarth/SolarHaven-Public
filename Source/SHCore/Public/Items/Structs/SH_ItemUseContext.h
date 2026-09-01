#pragma once

#include "CoreMinimal.h"

#include "SH_ItemUseContext.generated.h"

USTRUCT(BlueprintType)
struct FSH_ItemUseContext
{
	GENERATED_BODY()
	
	FSH_ItemUseContext():
		Instigator(nullptr), 
		InteractionDistance(400.f)
	{	}
	
	UPROPERTY()
	TObjectPtr<AActor> Instigator;
	UPROPERTY()
	FVector ViewLocation;
	UPROPERTY()
	FVector ViewDirection;
	UPROPERTY()
	float InteractionDistance;
};
