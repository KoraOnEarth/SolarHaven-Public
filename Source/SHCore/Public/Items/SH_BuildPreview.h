// LightLine Studios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SH_BuildPreview.generated.h"

class USH_BaseItemBehavior;

UCLASS()
class SHCORE_API ASH_BuildPreview : public AActor
{
	GENERATED_BODY()

public:
	ASH_BuildPreview();
	void Init(const USH_BaseItemBehavior* ItemBehavior);
	void Init(const USH_BaseItemBehavior* ItemBehavior, int xSize, int ySize);
	void SetPreviewMaterialColor(bool CanBuild);
	void SetPreviewMaterialColor(FLinearColor Color);

private:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> Root;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> PreviewMeshComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Appearance")
	TObjectPtr<UMaterial> PreviewMaterial;
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> PreviewMaterialDynamic;
	
	// Used when a Behavior can have only one preview (e.g., a hoe can only have a soil cube as its preview).
	UPROPERTY(EditDefaultsOnly, Category = "Appearance")
	TMap<TSubclassOf<USH_BaseItemBehavior>, TSoftObjectPtr<UStaticMesh>> PreviewMeshes;
	// Used to determine the size of the building preview when using a building certificate.
	UPROPERTY(EditDefaultsOnly, Category = "Appearance")
	TMap<FIntVector2, TSoftObjectPtr<UStaticMesh>> BuildCertificatePreviews;
	
	void SetMaterialDefaultParams();
};
