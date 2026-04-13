// Copyright Lightline Studios. All rights reserved 

#pragma once

#include "CoreMinimal.h"
#include "SH_ToolBaseBehaviour.h"
#include "SH_HoeBehavior.generated.h"

class ASH_PlacementPreviewActor;
/**
 * 
 */
UCLASS()
class SHITEMS_API USH_HoeBehavior : public USH_ToolBaseBehaviour
{
	GENERATED_BODY()
	
public:
	virtual void Init(AActor* InPlayerCharacter, USH_ItemPDA* InItemPDA) override;
	virtual void Tick(const FSH_ItemUseContext& Context) override;

	// ------------------------
	// Base Functions
	// ------------------------
	virtual void OnEquipped() override;
	virtual void OnUnEquipped() override;
	virtual void Use(const FSH_ItemUseContext& Context) override;

	// ------------------------
	// Soil Building Functions
	// ------------------------
	void CreateNewSoilBlock(const FSH_ItemUseContext& Context);
	void UpdatePreviewActorPosition(const FSH_ItemUseContext& Context);
	bool IsLocationOccupied(const FVector& Location);

private:
	UPROPERTY()
	TObjectPtr<ASH_PlacementPreviewActor> PreviewActor;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASH_PlacementPreviewActor> PreviewActorClass;
};
