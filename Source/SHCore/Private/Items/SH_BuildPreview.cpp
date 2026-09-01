// LightLine Studios. All rights reserved.


#include "Items/SH_BuildPreview.h"

#include "Items/Behaviors/SH_BaseItemBehavior.h"


ASH_BuildPreview::ASH_BuildPreview()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	
	PreviewMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("PreviewMeshComponent");
	PreviewMeshComponent->SetupAttachment(RootComponent);
	PreviewMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PreviewMeshComponent->SetGenerateOverlapEvents(false);
	PreviewMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.1f));
}

void ASH_BuildPreview::Init(const USH_BaseItemBehavior* ItemBehavior)
{
	if (!PreviewMaterial) return;
	if (!PreviewMeshComponent) return;
	if (const TSoftObjectPtr<UStaticMesh>* MeshPtr = PreviewMeshes.Find(ItemBehavior->GetClass()))
	{
		UStaticMesh* Mesh = MeshPtr->LoadSynchronous();
		if (Mesh) PreviewMeshComponent->SetStaticMesh(Mesh);
	}
	SetMaterialDefaultParams();
}

void ASH_BuildPreview::Init(const USH_BaseItemBehavior* ItemBehavior, int xSize, int ySize)
{
	if (!PreviewMaterial) return;
	if (!PreviewMeshComponent) return;
	if (const TSoftObjectPtr<UStaticMesh>* MeshPtr = BuildCertificatePreviews.Find({xSize, ySize}))
	{
		UStaticMesh* Mesh = MeshPtr->LoadSynchronous();
		if (Mesh) PreviewMeshComponent->SetStaticMesh(Mesh);
	}
	SetMaterialDefaultParams();
}

void ASH_BuildPreview::SetPreviewMaterialColor(bool CanBuild)
{
	if (!PreviewMaterialDynamic) return;
	if (CanBuild) PreviewMaterialDynamic->SetVectorParameterValue(TEXT("BaseColor"), FLinearColor::Green);
	else PreviewMaterialDynamic->SetVectorParameterValue(TEXT("BaseColor"), FLinearColor::Red);
}

void ASH_BuildPreview::SetPreviewMaterialColor(FLinearColor Color)
{
	if (PreviewMaterialDynamic) PreviewMaterialDynamic->SetVectorParameterValue(TEXT("BaseColor"), Color);
}

void ASH_BuildPreview::SetMaterialDefaultParams()
{
	PreviewMeshComponent->SetTranslucentSortPriority(10);
	PreviewMaterialDynamic = UMaterialInstanceDynamic::Create(PreviewMaterial, this);
	PreviewMaterialDynamic->SetScalarParameterValue(TEXT("Opacity"), 0.7f);
	PreviewMeshComponent->SetMaterial(0, PreviewMaterialDynamic);
}

