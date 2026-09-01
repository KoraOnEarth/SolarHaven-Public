// LightLine Studios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SH_BaseItemBehavior.generated.h"

class ASH_BuildPreview;
struct FSH_ItemUseContext;
class USH_ItemPDA;

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class SHCORE_API USH_BaseItemBehavior : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void Init(AActor* PlayerCharacter, USH_ItemPDA* ItemPDA);
	virtual void Tick(const FSH_ItemUseContext& Context);
	virtual void BeginDestroy() override;
	
	// Creates the BuildPreview after all checks have passed.
	virtual void OnEquipped();
	// Removes the BuildPreview.
	virtual void OnUnEquipped();
	// Item use function.
	virtual bool Use(const FSH_ItemUseContext& Context);
	
	// Get pointer ao DataAsset in hand
	USH_ItemPDA* GetItemData() const { return ItemData; };
	// Reinitialization. Intended to be used when a Behavior class already exists. Currently, it is simply a duplicate of Init().
	void UpdateItemData(AActor* PlayerCharacter, USH_ItemPDA* ItemPDA);

protected:
	UPROPERTY()
	TObjectPtr<USH_ItemPDA> ItemData;
	UPROPERTY()
	TObjectPtr<AActor> CachedCharacter;
	UPROPERTY()
	TObjectPtr<ASH_BuildPreview> PreviewActor;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASH_BuildPreview> PreviewActorClass;
};
