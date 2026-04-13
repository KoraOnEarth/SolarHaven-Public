// Copyright Lightline Studios. All rights reserved 

#include "Behaviors/Tools/SH_HoeBehavior.h"

#include "BaseClasses/SH_PlacementPreviewActor.h"
#include "Engine/OverlapResult.h"
#include "Enums/SH_SnapDirection.h"
#include "Interfaces/SH_PlantableInterface.h"
#include "PrimaryDataAssets/SH_ToolItemPDA.h"
#include "Structs/SH_ItemUseContext.h"

class USH_WorldSubsystem;

void USH_HoeBehavior::Init(AActor* InPlayerCharacter, USH_ItemPDA* InItemPDA)
{
	Super::Init(InPlayerCharacter, InItemPDA);
}
void USH_HoeBehavior::Tick(const FSH_ItemUseContext& Context)
{
	UpdatePreviewActorPosition(Context);
}

// ------------------------
// Base Functions
// ------------------------
void USH_HoeBehavior::OnEquipped()
{
	if (!CachedCharacter) return;
	
	UWorld* World = CachedCharacter->GetWorld();
	if (!World || !PreviewActorClass) return;

	PreviewActor = World->SpawnActor<ASH_PlacementPreviewActor>(PreviewActorClass);
}
void USH_HoeBehavior::OnUnEquipped()
{
	if (IsValid(PreviewActor))
	{
		PreviewActor->Destroy();
		PreviewActor = nullptr;
	}
}
void USH_HoeBehavior::Use(const FSH_ItemUseContext& Context)
{
	CreateNewSoilBlock(Context);
}

// ------------------------
// Soil Building Functions
// ------------------------
void USH_HoeBehavior::CreateNewSoilBlock(const FSH_ItemUseContext& Context)
{
	USH_ToolItemPDA* CurrentData = Cast<USH_ToolItemPDA>(ItemData);
    if (!CurrentData) return;
	
    UWorld* World = CachedCharacter->GetWorld();
    if (!World) return;

    FVector Start = Context.ViewLocation;
    FVector End = Start + Context.ViewDirection * Context.InteractionDistance;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Context.Instigator);

    bool bHit = World->LineTraceSingleByChannel(Hit, Start, End, ECC_GameTraceChannel1, Params);
    if (!bHit) return;

	
    if (ISH_PlantableInterface* PlantInterface = Cast<ISH_PlantableInterface>(Hit.GetActor()))
    {
        FVector LocationToPlace = FVector::ZeroVector;
    	ESH_SnapDirection CurrentDirection = {};

        if (Hit.Component->ComponentHasTag("TriggerBoxTop"))
        {
        	CurrentDirection = ESH_SnapDirection::Top;
            LocationToPlace = PlantInterface->GetTransformToPlaceSoil(CurrentDirection);
        }
        else if (Hit.Component->ComponentHasTag("TriggerBoxBottom"))
        {
        	CurrentDirection = ESH_SnapDirection::Bottom;
            LocationToPlace = PlantInterface->GetTransformToPlaceSoil(CurrentDirection);
        }
        else if (Hit.Component->ComponentHasTag("TriggerBoxLeft"))
        {
        	CurrentDirection = ESH_SnapDirection::Left;
            LocationToPlace = PlantInterface->GetTransformToPlaceSoil(CurrentDirection);
        }
        else if (Hit.Component->ComponentHasTag("TriggerBoxRight"))
        {
        	CurrentDirection = ESH_SnapDirection::Right;
            LocationToPlace = PlantInterface->GetTransformToPlaceSoil(CurrentDirection);
        }
    	
        if (IsLocationOccupied(LocationToPlace)) return;

        AActor* NewSoil = World->SpawnActor<AActor>(
            CurrentData->SpawnActorClass,
            LocationToPlace,
            FRotator::ZeroRotator
        );

    	// На этом месте нужно "старой" клетке дать ссылку на нового соседа
    	PlantInterface->SetOccupiedSlotByDirection(CurrentDirection);

    	// А потом ещё где-то дать новой клетке ссылку на старого соседа
    	ISH_PlantableInterface* NewSoilPlantInterface = Cast<ISH_PlantableInterface>(NewSoil);
    	if (!NewSoilPlantInterface || CurrentDirection == ESH_SnapDirection::None)
    	{
    		return;
    	}

    	switch (CurrentDirection)
    	{
    	case ESH_SnapDirection::Top:
    		NewSoilPlantInterface->SetOccupiedSlotByDirection(ESH_SnapDirection::Bottom);
    		break;

    	case ESH_SnapDirection::Bottom:
    		NewSoilPlantInterface->SetOccupiedSlotByDirection(ESH_SnapDirection::Top);
    		break;

    	case ESH_SnapDirection::Left:
    		NewSoilPlantInterface->SetOccupiedSlotByDirection(ESH_SnapDirection::Right);
    		break;

    	case ESH_SnapDirection::Right:
    		NewSoilPlantInterface->SetOccupiedSlotByDirection(ESH_SnapDirection::Left);
    		break;

    	default:
    		break;
    	}
    	
        return;
    }
	
    if (IsLocationOccupied(Hit.ImpactPoint))
    {
        return;
    }

    World->SpawnActor<AActor>(
        CurrentData->SpawnActorClass,
        Hit.ImpactPoint,
        FRotator::ZeroRotator
    );
}
void USH_HoeBehavior::UpdatePreviewActorPosition(const FSH_ItemUseContext& Context)
{
	if (!IsValid(PreviewActor))
	{
		return;
	}
	
	UWorld* World = CachedCharacter->GetWorld();
	if (!World) return;
	
	FVector Start = Context.ViewLocation;
	FVector End = Start + Context.ViewDirection * Context.InteractionDistance;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Context.Instigator);

	bool bHit = World->LineTraceSingleByChannel(Hit, Start, End, ECC_GameTraceChannel1, Params);
	if (!bHit)
	{
		if (!PreviewActor->IsHidden())
		{
			PreviewActor->SetActorHiddenInGame(true);
		}
		return;
	}
	
	if (PreviewActor->IsHidden())
	{
		PreviewActor->SetActorHiddenInGame(false);
	}
	
	FVector LocationToPlace = FVector::ZeroVector;
	bool bIsSockedOccupied = false;

	// Snapping-логика
	if (ISH_PlantableInterface* PlantInterface = Cast<ISH_PlantableInterface>(Hit.GetActor()))
	{
		if (Hit.Component->ComponentHasTag("TriggerBoxTop"))
		{
			bIsSockedOccupied = PlantInterface->IsOccupiedSlotByDirection(ESH_SnapDirection::Top);
			if (bIsSockedOccupied) return;
			LocationToPlace = PlantInterface->GetTransformToPlaceSoil(ESH_SnapDirection::Top);
		}
		else if (Hit.Component->ComponentHasTag("TriggerBoxBottom"))
		{
			bIsSockedOccupied = PlantInterface->IsOccupiedSlotByDirection(ESH_SnapDirection::Bottom);
			if (bIsSockedOccupied) return;
			LocationToPlace = PlantInterface->GetTransformToPlaceSoil(ESH_SnapDirection::Bottom);
		}
		else if (Hit.Component->ComponentHasTag("TriggerBoxLeft"))
		{
			bIsSockedOccupied = PlantInterface->IsOccupiedSlotByDirection(ESH_SnapDirection::Left);
			if (bIsSockedOccupied) return;
			LocationToPlace = PlantInterface->GetTransformToPlaceSoil(ESH_SnapDirection::Left);
		}
		else if (Hit.Component->ComponentHasTag("TriggerBoxRight"))
		{
			bIsSockedOccupied = PlantInterface->IsOccupiedSlotByDirection(ESH_SnapDirection::Right);
			if (bIsSockedOccupied) return;
			LocationToPlace = PlantInterface->GetTransformToPlaceSoil(ESH_SnapDirection::Right);
		}
	}
	else
	{
		LocationToPlace = Hit.ImpactPoint;
	}

	bool bOccupied = IsLocationOccupied(LocationToPlace) || bIsSockedOccupied;
	
	PreviewActor->SetActorLocation(LocationToPlace);

	// Визуальная часть
	PreviewActor->SetPreviewMaterialColor(
		bOccupied ? FLinearColor::Red : FLinearColor::Green
	);
}
bool USH_HoeBehavior::IsLocationOccupied(const FVector& Location)
{
	UWorld* World = CachedCharacter->GetWorld();
	if (!World) return true;

	TArray<FOverlapResult> Overlaps;

	// Чуть меньше, поскольку иначе блоки, стоящие вплотную, могут считаться пересекающимися
	FCollisionShape Shape = FCollisionShape::MakeBox(FVector(74.f, 74.f, 50.f));

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(CachedCharacter);

	bool bHit = World->OverlapMultiByChannel(
		Overlaps,
		Location,
		FQuat::Identity,
		ECC_WorldDynamic,
		Shape,
		Params
	);

	if (!bHit) return false;

	for (const FOverlapResult& Result : Overlaps)
	{
		if (Result.GetActor())
		{
			if (Cast<ISH_PlantableInterface>(Result.GetActor()))
			{
				return true;
			}
		}
	}

	return false;
}
