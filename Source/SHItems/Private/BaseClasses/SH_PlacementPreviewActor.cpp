// Copyright Lightline Studios. All rights reserved 


#include "BaseClasses/SH_PlacementPreviewActor.h"


ASH_PlacementPreviewActor::ASH_PlacementPreviewActor()
{
	PrimaryActorTick.bCanEverTick = false;

	PreviewSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("PreviewScene"));
	SetRootComponent(PreviewSceneComponent);
	
	PreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMesh"));
	PreviewMesh->SetupAttachment(PreviewSceneComponent);

	PreviewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PreviewMesh->SetGenerateOverlapEvents(false);

	PreviewMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.1f));
}

void ASH_PlacementPreviewActor::SetPreviewMaterialColor(FLinearColor Color)
{
	if (PreviewMaterialDynamic)
	{
		PreviewMaterialDynamic->SetVectorParameterValue(TEXT("BaseColor"), Color);
	}
}

void ASH_PlacementPreviewActor::BeginPlay()
{
	Super::BeginPlay();

	if (PreviewMesh)
	{
		PreviewMesh->SetTranslucentSortPriority(10);
		PreviewMaterialDynamic = PreviewMesh->CreateAndSetMaterialInstanceDynamic(0);
		PreviewMaterialDynamic->SetScalarParameterValue(TEXT("Opacity"), 0.7f);
	}
}

