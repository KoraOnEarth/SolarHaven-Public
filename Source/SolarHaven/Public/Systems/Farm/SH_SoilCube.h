// LightLine Studios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SH_WorldTime.h"

#include "GameFramework/Actor.h"

#include "Interfaces/SH_PlantableInterface.h"

#include "Items/PDAs/SH_SeedItemPDA.h"

#include "SH_SoilCube.generated.h"

USTRUCT()
struct FSH_PlantData
{
	GENERATED_BODY()
	
public:
	void PlantSeed(USH_SeedItemPDA* SeedItemData, const FSH_WorldTime& WorldTime);
	void UpdateGrow(const FSH_WorldTime& WorldTime);
	
	bool GetHasPlantAlready() const { return PlantedSeed ? true : false; }
	bool GetIsReadyToCrop() const { return bIsReadyToCrop; }
	USH_SeedItemPDA* GetPlantedSeed() const { return PlantedSeed; };
	
	void UpdateGrowProcess(UStaticMeshComponent* PlantMeshComponent, const FSH_WorldTime& WorldTime);
	
	void ClearPlant();

private:
	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<USH_SeedItemPDA> PlantedSeed;
	UPROPERTY(VisibleInstanceOnly)
	TArray<UStaticMesh*> PlantStageMeshes;
	UPROPERTY(VisibleInstanceOnly)
	FSH_WorldTime PlantTime;
	UPROPERTY(VisibleInstanceOnly)
	float CurrentGrowProcess = 0.f;
	UPROPERTY(VisibleInstanceOnly)
	float CurrentGrowModifier = 1.f;
	UPROPERTY(VisibleInstanceOnly)
	float MinutesLeft = 0.f;
	UPROPERTY(VisibleInstanceOnly)
	int32 CurrentStage = 0;
	UPROPERTY(VisibleInstanceOnly)
	bool bIsReadyToCrop = false;

	float GetCurrentSeedSeasonModifier(const USH_SeedItemPDA* SeedItemData, ESH_Seasons CurrentSeason);
};

UCLASS()
class SOLARHAVEN_API ASH_SoilCube : public AActor, public ISH_PlantableInterface
{
	GENERATED_BODY()

public:
	ASH_SoilCube();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// -------------------
	// Plant Controls
	// -------------------
	bool GetIsReadyToCrop() { return PlantData.GetIsReadyToCrop(); }
	void HandleWorldSmallTick(const FSH_WorldTime& WorldTime);
	/** 
	 * @return true - if the plant has sufficient humidity to grow.
	 */
	bool CheckHumidity() { return HumidityLevel > 0.f; }
	void LowerHumidityLevelOnTick();
	
	UPROPERTY(VisibleInstanceOnly, Category = "Humidity")
	float HumidityLevel = 1.f;
	UPROPERTY(VisibleInstanceOnly, Category = "Humidity")
	float HumidityUsageMultiplier = 1.f;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> Root;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SoilMeshComponent;
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> PlantMeshComp;
	UPROPERTY(VisibleInstanceOnly, meta=(AllowPrivateAccess = true))
	FSH_PlantData PlantData;

public:
	// ----------------
	// INTERFACES
	// ----------------	
	virtual bool Plant(USH_SeedItemPDA* SeedItemPDA) override;
	virtual USH_ItemPDA* Crop() override;
	virtual void ToWater() override { HumidityLevel = 1.f; }
};
