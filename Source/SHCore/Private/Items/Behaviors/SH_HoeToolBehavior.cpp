// LightLine Studios. All rights reserved.


#include "Items/Behaviors/SH_HoeToolBehavior.h"

#include "Interfaces/SH_BuildableGridInterface.h"

#include "Items/SH_BuildPreview.h"
#include "Items/PDAs/SH_ToolItemPDA.h"
#include "Items/Structs/SH_ItemUseContext.h"

void USH_HoeToolBehavior::Tick(const FSH_ItemUseContext& Context)
{
	UpdatePreviewActorLocation(Context);
}
bool USH_HoeToolBehavior::Use(const FSH_ItemUseContext& Context)
{
	CreateSoilBlock(Context);
	return true;
}

void USH_HoeToolBehavior::UpdatePreviewActorLocation(const FSH_ItemUseContext& Context)
{
	if (!IsValid(PreviewActor)) return;
	UWorld* World = CachedCharacter->GetWorld();
	if (!World) return;
	
	const FVector Start = Context.ViewLocation;
	const FVector End = Start + Context.ViewDirection * Context.InteractionDistance;
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Context.Instigator);
	
	// GameTraceChannel1 == TraceBuild
	bool bHit = World->LineTraceSingleByChannel(Hit, Start, End, ECC_GameTraceChannel1, Params);
	// Hides the preview if the InteractionDistance is insufficient.
	if (!bHit)
	{
		if (!PreviewActor->IsHidden()) PreviewActor->SetActorHiddenInGame(true);
		return;
	}
	if (PreviewActor->IsHidden()) PreviewActor->SetActorHiddenInGame(false);
	
	ISH_BuildableGridInterface* GridInterface = Cast<ISH_BuildableGridInterface>(Hit.GetActor());
	if (!GridInterface)
	{
		PreviewActor->SetActorHiddenInGame(true);
		return;
	}
	const FVector LocationToPlace = GridInterface->GetLocationToBuild(Hit.ImpactPoint, Context.Instigator->GetActorRotation());
	const bool bIsCellOccupied = GridInterface->GetIsCellOccupied(Hit.ImpactPoint);
	PreviewActor->SetActorLocation(LocationToPlace);
	PreviewActor->SetPreviewMaterialColor(!bIsCellOccupied);
}
void USH_HoeToolBehavior::CreateSoilBlock(const FSH_ItemUseContext& Context)
{
	if (!CachedCharacter) return;
	const USH_ToolItemPDA* CurrentData = Cast<USH_ToolItemPDA>(ItemData);
	UWorld* World = CachedCharacter->GetWorld();
	if (!World) return;
	
	FVector Start = Context.ViewLocation;
	FVector End = Start + Context.ViewDirection * Context.InteractionDistance;
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Context.Instigator);
	
	// GameTraceChannel1 == TraceBuild
	const bool bHit = World->LineTraceSingleByChannel(Hit, Start, End, ECC_GameTraceChannel1, Params);
	if (!bHit) return;
	
	ISH_BuildableGridInterface* GridInterface = Cast<ISH_BuildableGridInterface>(Hit.GetActor());
	if (!GridInterface) return;
	const FVector LocationToPlace = GridInterface->GetLocationToBuild(Hit.ImpactPoint, Context.Instigator->GetActorRotation());
	const bool bIsCellOccupied = GridInterface->GetIsCellOccupied(Hit.ImpactPoint);
	if (bIsCellOccupied) return;
	AActor* SpawnedActor = World->SpawnActor<AActor>(CurrentData->SpawnActorClass, LocationToPlace, FRotator::ZeroRotator);
	if (!SpawnedActor) return;
	GridInterface->SetCellOccupation(LocationToPlace, true);
}
