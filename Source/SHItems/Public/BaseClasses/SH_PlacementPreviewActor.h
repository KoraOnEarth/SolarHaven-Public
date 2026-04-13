// Copyright Lightline Studios. All rights reserved 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SH_PlacementPreviewActor.generated.h"

UCLASS()
class SHITEMS_API ASH_PlacementPreviewActor : public AActor
{
	GENERATED_BODY()

public:
	ASH_PlacementPreviewActor();

	void SetPreviewMaterialColor(FLinearColor Color);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Preview", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USceneComponent> PreviewSceneComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Preview", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UStaticMeshComponent> PreviewMesh;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> PreviewMaterialDynamic;
};
