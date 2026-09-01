// LightLine Studios. All rights reserved.

#include "Systems/Farm/SH_SoilCube.h"

#include "SH_GameTimeSubsystem.h"

#include "Items/PDAs/SH_SeedItemPDA.h"


// ------------------------------
// PlantData-structure functions
// ------------------------------
#pragma region PlantData
void FSH_PlantData::PlantSeed(USH_SeedItemPDA* SeedItemData, const FSH_WorldTime& WorldTime)
{
	if (!SeedItemData)
	{
		UE_LOG(LogTemp, Error, TEXT("SeedItemData is NULL"));
		return;
	}

	PlantedSeed = SeedItemData;
	PlantTime = WorldTime;
	CurrentStage = 0;
	CurrentGrowModifier = GetCurrentSeedSeasonModifier(SeedItemData, PlantTime.Season);  // Сейчас по умолчанию весна всегда
	bIsReadyToCrop = false;
	CurrentGrowProcess = 0.f;
	MinutesLeft = SeedItemData->TimeToGrowInMinutes;

	for (int32 i = 0; i < PlantedSeed->GrowVisualStages.GetMaxStages(); i++)
	{
		TSoftObjectPtr<UStaticMesh> Mesh = PlantedSeed->GrowVisualStages.GetMeshAtStage(i);
		if (!Mesh.IsNull()) PlantStageMeshes.Add(Mesh.LoadSynchronous());
	}
}
void FSH_PlantData::UpdateGrowProcess(UStaticMeshComponent* PlantMeshComponent, const FSH_WorldTime& WorldTime)
{
	if (IsValid(PlantedSeed))
	{
		int32 CachedStage = CurrentStage;
		UpdateGrow(WorldTime);
		if (CurrentStage >= PlantStageMeshes.Num())
		{
			UE_LOG(LogTemp, Warning, TEXT("No mesh for this stage of growing"))
			return;
		}
		if (CachedStage != CurrentStage) PlantMeshComponent->SetStaticMesh(PlantStageMeshes[CurrentStage]);
	}
}

void FSH_PlantData::UpdateGrow(const FSH_WorldTime& WorldTime)
{
	if (bIsReadyToCrop) return;

	FSH_TimeSpan TimeElapsed = WorldTime - PlantTime;
	float TotalGrowth = TimeElapsed.GetTotalMinutes() * CurrentGrowModifier;
	CurrentGrowProcess = FMath::Min(TotalGrowth, PlantedSeed->TimeToGrowInMinutes);
	MinutesLeft = PlantedSeed->TimeToGrowInMinutes - CurrentGrowProcess;
	float Progress = CurrentGrowProcess / PlantedSeed->TimeToGrowInMinutes;
	int32 NewGrowStage = FMath::Clamp(
		FMath::FloorToInt(Progress * (PlantedSeed->GrowVisualStages.GetMaxStages() - 1)),
		0,
		PlantedSeed->GrowVisualStages.GetMaxStages() - 1
	);
	if (CurrentStage != NewGrowStage) CurrentStage = NewGrowStage;
	if (MinutesLeft <= 0)
	{
		MinutesLeft = 0;
		bIsReadyToCrop = true;
	}
}

void FSH_PlantData::ClearPlant()
{
	PlantedSeed = nullptr;
	PlantTime.ClearTime();
	CurrentGrowProcess = 0.f;
	CurrentGrowModifier = 1.f;
	MinutesLeft = 0.f;
	CurrentStage = 0;
	bIsReadyToCrop = false;
	PlantStageMeshes.Empty();
}

float FSH_PlantData::GetCurrentSeedSeasonModifier(const USH_SeedItemPDA* SeedItemData, ESH_Seasons CurrentSeason)
{
	switch (CurrentSeason)
	{
		case ESH_Seasons::Spring: return SeedItemData->GrowSeasons.SpringGrowModifier;
		case ESH_Seasons::Summer: return SeedItemData->GrowSeasons.SummerGrowModifier;
		case ESH_Seasons::Autumn: return SeedItemData->GrowSeasons.AutumnGrowModifier;
		case ESH_Seasons::Winter: return SeedItemData->GrowSeasons.WinterGrowModifier;
		default: return 0.f;
	}
}
#pragma endregion

ASH_SoilCube::ASH_SoilCube()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Root = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(Root);
	
	SoilMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("SoilMeshComponent");
	SoilMeshComponent->SetupAttachment(RootComponent);
	
	PlantMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("PlantMeshComp");
	PlantMeshComp->SetupAttachment(SoilMeshComponent);
}

void ASH_SoilCube::BeginPlay()
{
	if (USH_GameTimeSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USH_GameTimeSubsystem>())
	{
		// Humidity updates will eventually be changed to run once per hour (tentatively).
		Subsystem->OnMinuteChanged.AddUObject(this, &ThisClass::HandleWorldSmallTick);
	}
	
	// Random rotation for greater variety among plants.
	const FRotator PlantCompRotator(0.f, FMath::FRandRange(0.f, 180.f), 0.f);
	PlantMeshComp->SetRelativeRotation(PlantCompRotator);
}
void ASH_SoilCube::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (USH_GameTimeSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USH_GameTimeSubsystem>())
	{
		Subsystem->OnMinuteChanged.RemoveAll(this);
	}
	Super::EndPlay(EndPlayReason);
}

void ASH_SoilCube::HandleWorldSmallTick(const FSH_WorldTime& WorldTime)
{
	// Update the soil moisture.
	LowerHumidityLevelOnTick();
	if (!PlantData.GetHasPlantAlready() || PlantData.GetIsReadyToCrop()) return;
	if (!CheckHumidity())
	{ 
		UE_LOG(LogTemp, Warning, TEXT("Humidity level check failed at block: %s "), *this->GetName());
		return;
	}
	// Update the growth parameters.
	PlantData.UpdateGrowProcess(PlantMeshComp, WorldTime);
}

void ASH_SoilCube::LowerHumidityLevelOnTick()
{
	HumidityLevel = FMath::Clamp(HumidityLevel - 0.05f * HumidityUsageMultiplier, 0.f, 1.f);
}

bool ASH_SoilCube::Plant(USH_SeedItemPDA* SeedItemPDA)
{
	if (!SeedItemPDA) return false;
	if (PlantData.GetHasPlantAlready())
	{
		UE_LOG(LogTemp, Warning, TEXT("This soil is already busy"));
		return false;
	}
	if (!SeedItemPDA)
	{
		UE_LOG(LogTemp, Error, TEXT("SeedItem is nullptr"));
		return false;
	}

	USH_GameTimeSubsystem* GTSubsystem = GetGameInstance()->GetSubsystem<USH_GameTimeSubsystem>();
	if (!GTSubsystem) return false;

	PlantData.PlantSeed(SeedItemPDA, GTSubsystem->GetCurrentTime());
	
	if (SeedItemPDA->GrowVisualStages.GetMaxStages() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Visual Stages of grow of %s were not set!"), *SeedItemPDA->Name.ToString())
		return true;
	}
	PlantMeshComp->SetStaticMesh(PlantData.GetPlantedSeed()->GrowVisualStages.GetMeshAtStage(0).LoadSynchronous());
	
	return true;
}
USH_ItemPDA* ASH_SoilCube::Crop()
{
	if (!PlantData.GetPlantedSeed())
	{
		UE_LOG(LogTemp, Log, TEXT("!PlantData.GetPlantedSeed()"));
		return nullptr;
	}
	if (!PlantData.GetIsReadyToCrop())
	{
		UE_LOG(LogTemp, Log, TEXT("Plant is not ready to crop!"));
		return nullptr;
	}
	if (!PlantData.GetPlantedSeed()->CropItemPDA) UE_LOG(LogTemp, Warning, TEXT("CropItemPDA is not set in %s"), *PlantData.GetPlantedSeed()->GetName())
	
	USH_SeedItemPDA* SeedItem = PlantData.GetPlantedSeed();
	PlantData.ClearPlant();
	PlantMeshComp->SetStaticMesh(nullptr);
	return SeedItem;
}
