// LightLine Studios. All rights reserved.


#include "Items/Behaviors/SH_BuildCertificateBehavior.h"

#include "Interfaces/SH_BuildableGridInterface.h"

#include "Items/SH_BuildPreview.h"
#include "Items/PDAs/SH_BuildCertificatePDA.h"
#include "Items/Structs/SH_ItemUseContext.h"


void USH_BuildCertificateBehavior::Tick(const FSH_ItemUseContext& Context)
{
	UpdatePreviewActorLocation(Context);
}

bool USH_BuildCertificateBehavior::Use(const FSH_ItemUseContext& Context)
{
	UE_LOG(LogTemp, Warning, TEXT("USH_BuildCertificateBehavior::Use"));
	return true;
}

void USH_BuildCertificateBehavior::RotatePreview(int32 RotateDirection)
{
	if (RotateDirection == -1) CurrentRotation.Yaw -= 90.f;
	else CurrentRotation.Yaw += 90.f;
	UE_LOG(LogTemp, Log, TEXT("Current preview rotation: %s"), *CurrentRotation.ToString());
}

void USH_BuildCertificateBehavior::UpdatePreviewActorLocation(const FSH_ItemUseContext& Context)
{
	if (!IsValid(PreviewActor)) return;
	UWorld* World = CachedCharacter->GetWorld();
	if (!World) return;
	
	const FVector Start = FVector(Context.ViewLocation.X, Context.ViewLocation.Y, Context.ViewLocation.Z);
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
	USH_BuildCertificatePDA* CertificatePDA = Cast<USH_BuildCertificatePDA>(ItemData);
	if (!CertificatePDA) return;
	FIntVector2 Size = CertificatePDA->GetSize();
	const FVector LocationToPlaceWithoutRotation = GridInterface->GetLocationToBuild(Hit.ImpactPoint, Context.Instigator->GetActorRotation(), Size.X, Size.Y);
	PreviewActor->SetActorLocation(LocationToPlaceWithoutRotation);
	PreviewActor->SetActorRotation(CurrentRotation);
	
	const bool bIsCellOccupied = GridInterface->GetIsCellOccupied(LocationToPlaceWithoutRotation, Size.X, Size.Y);
	PreviewActor->SetPreviewMaterialColor(!bIsCellOccupied);
}
