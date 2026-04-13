// Copyright Lightline Studios. All rights reserved 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SH_ItemBaseBehavior.generated.h"

struct FSH_ItemUseContext;
class USH_ItemPDA;

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class SHITEMS_API USH_ItemBaseBehavior : public UObject
{
	GENERATED_BODY()

public:
	virtual void Init(AActor* InPlayerCharacter, USH_ItemPDA* InItemPDA);

	virtual void Tick(const FSH_ItemUseContext& Context);
	
	virtual void OnEquipped();
	virtual void OnUnEquipped();
	virtual void Use(const FSH_ItemUseContext& Context);

	UPROPERTY()
	TObjectPtr<USH_ItemPDA> ItemData;

	UPROPERTY()
	TObjectPtr<AActor> CachedCharacter;
};
