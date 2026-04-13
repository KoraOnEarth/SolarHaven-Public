// Copyright Lightline Studios. All rights reserved 

#pragma once

#include "CoreMinimal.h"
#include "Enums/SH_Seasons.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SH_PlantableInterface.h"
#include "PrimaryDataAssets/SH_SeedItemPDA.h"
#include "Structs/SH_WorldTime.h"
#include "SH_BaseSoil.generated.h"

enum class ESH_SnapDirection : uint8;
class UBoxComponent;
class USH_SnapPointComponent;

USTRUCT()
struct FSH_PlantData
{
	GENERATED_BODY()

public:
	FSH_PlantData() :
		CurrentGrowProcess(0.f),
		CurrentGrowModifier(0.f),
		MinutesLeft(0),
		CurrentStage(-1),
		bIsReadyToCrop(false)
	{	}

	void PlantSeed(USH_SeedItemPDA* SeedItemData, const FSH_WorldTime& WorldTime);

	void UpdateCurrentGrowProcess(const FSH_WorldTime& WorldTime);

	const FSH_WorldTime& GetPlantTime() const { return PlantTime; }
	int32 GetCurrentStage() const { return CurrentStage; }
	bool GetHasPlantAlready() const { return PlantedSeed ? true : false; }
	float GetCurrentGrowProcess() const { return CurrentGrowProcess; }
	float GetCurrentGrowModifier() const { return CurrentGrowModifier; }
	bool GetIsReadyToCrop() const { return bIsReadyToCrop; }
	USH_SeedItemPDA* GetPlantedSeed() const;

private:
	UPROPERTY(VisibleInstanceOnly, meta=(AllowPrivateAccess = true))
	TObjectPtr<USH_SeedItemPDA> PlantedSeed;
	
	UPROPERTY(VisibleInstanceOnly, meta=(AllowPrivateAccess = true))
	FSH_WorldTime PlantTime;

	UPROPERTY(VisibleInstanceOnly, meta=(AllowPrivateAccess = true))
	float CurrentGrowProcess;
	
	UPROPERTY(VisibleInstanceOnly, meta=(AllowPrivateAccess = true))
	float CurrentGrowModifier;

	UPROPERTY(VisibleInstanceOnly, meta=(AllowPrivateAccess = true))
	float MinutesLeft;

	UPROPERTY(VisibleInstanceOnly, meta=(AllowPrivateAccess = true))
	int32 CurrentStage;
	
	UPROPERTY(VisibleInstanceOnly, meta=(AllowPrivateAccess = true))
	bool bIsReadyToCrop;

	float GetCurrentSeedSeasonModifier(USH_SeedItemPDA* SeedItemData, ESH_Seasons CurrentSeason);
};

USTRUCT()
struct FSH_SoilSnapData
{
	GENERATED_BODY()
	
	UPROPERTY()
	TObjectPtr<USH_SnapPointComponent> SnapPoint;

	UPROPERTY()
	TObjectPtr<UBoxComponent> TriggerBox;

	UPROPERTY()
	ESH_SnapDirection SnapDirection = ESH_SnapDirection::None;
};

USTRUCT(BlueprintType)
struct FSH_OccupiedSlots
{
	GENERATED_BODY()

	FSH_OccupiedSlots() :
		bIsOccupiedSlotTop(false),
		bIsOccupiedSlotBottom(false),
		bIsOccupiedSlotLeft(false),
		bIsOccupiedSlotRight(false)
	{	}
	
	UPROPERTY(VisibleInstanceOnly)
	bool bIsOccupiedSlotTop;
	
	UPROPERTY(VisibleInstanceOnly)
	bool bIsOccupiedSlotBottom;

	UPROPERTY(VisibleInstanceOnly)
	bool bIsOccupiedSlotLeft;

	UPROPERTY(VisibleInstanceOnly)
	bool bIsOccupiedSlotRight;
};

UCLASS()
class SOLARHAVEN_API ASH_BaseSoil : public AActor, public ISH_PlantableInterface
{
	GENERATED_BODY()

public:
	ASH_BaseSoil();

	// -------------------
	// Plant Controls
	// -------------------
	virtual bool PlantSeed(USH_SeedItemPDA* SeedItem) override;
	
	// -------------------
	// Slots Controls
	// -------------------
	virtual bool IsOccupiedSlotByDirection(ESH_SnapDirection Direction) override;
	virtual void SetOccupiedSlotByDirection(ESH_SnapDirection Direction) override;
	virtual FVector GetTransformToPlaceSoil(ESH_SnapDirection Direction) override;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// -------------------
	// Plant Controls
	// -------------------
	void HandleWorldSmallTick(const FSH_WorldTime& WorldTime);
	void PreloadMeshes();
	
private:
	
	// -------------------
	// Plant Controls
	// -------------------
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> SoilMeshComponent;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> PlantMeshComp;

	UPROPERTY(VisibleInstanceOnly, meta=(AllowPrivateAccess = true))
	FSH_PlantData PlantData;

	UPROPERTY(VisibleInstanceOnly, meta=(AllowPrivateAccess = true))
	TArray<UStaticMesh*> LoadedMeshes;

	// -------------------
	// Slots Controls
	// -------------------
#pragma region Slots

	UPROPERTY()
	TMap<ESH_SnapDirection, FSH_SoilSnapData> SnapMap;

	UPROPERTY(VisibleInstanceOnly, meta=(AllowPrivateAccess = true))
	FSH_OccupiedSlots OccupiedSlots;

	FVector GetSnapOffset(ESH_SnapDirection Direction);
	FVector GetTriggerBoxOffset(ESH_SnapDirection Direction);
	FVector GetTriggerBoxExtent(ESH_SnapDirection Direction);
	FName GetTriggerBoxTag(ESH_SnapDirection Direction);

#pragma endregion 
};
