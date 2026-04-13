// Copyright Lightline Studios. All rights reserved 

#include "Farm/Soils/SH_BaseSoil.h"

#include "SH_GameTimeSubsystem.h"
#include "BaseComponents/SH_SnapPointComponent.h"
#include "Components/BoxComponent.h"
#include "Enums/SH_SnapDirection.h"
#include "Structs/SH_TimeSpan.h"

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
}

void FSH_PlantData::UpdateCurrentGrowProcess(const FSH_WorldTime& WorldTime)
{
	if (!PlantedSeed || bIsReadyToCrop) return;

	FSH_TimeSpan TimeElapsed = WorldTime - PlantTime;

	float TotalGrowth = TimeElapsed.GetTotalMinutes() * CurrentGrowModifier;
	CurrentGrowProcess = FMath::Min(TotalGrowth, PlantedSeed->TimeToGrowInMinutes);
	MinutesLeft = PlantedSeed->TimeToGrowInMinutes - CurrentGrowProcess;

	float Progress = CurrentGrowProcess / PlantedSeed->TimeToGrowInMinutes;

	int32 NewStage = FMath::Clamp(
		FMath::FloorToInt(Progress * (PlantedSeed->GrowVisualStages.GetMaxStages() - 1)),
		0,
		PlantedSeed->GrowVisualStages.GetMaxStages() - 1
	);

	if (CurrentStage != NewStage) CurrentStage = NewStage;

	if (MinutesLeft <= 0)
	{
		MinutesLeft = 0;
		bIsReadyToCrop = true;
		UE_LOG(LogTemp, Warning, TEXT("ReadyToCrop"));
	}
}

USH_SeedItemPDA* FSH_PlantData::GetPlantedSeed() const
{
	return PlantedSeed;
}
float FSH_PlantData::GetCurrentSeedSeasonModifier(USH_SeedItemPDA* SeedItemData, ESH_Seasons CurrentSeason)
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

ASH_BaseSoil::ASH_BaseSoil()
{
	PrimaryActorTick.bCanEverTick = false;

	SoilMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SoilMeshComponent"));
	RootComponent = SoilMeshComponent;

	PlantMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlantMeshComponent"));
	PlantMeshComp->SetupAttachment(RootComponent);

	// Сразу установка меша
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SoilMesh(TEXT("/Game/Gameplay/Farm/Assets/StaticMeshes/SM_BaseSoil.SM_BaseSoil"));
	if (SoilMesh.Succeeded()) SoilMeshComponent->SetStaticMesh(SoilMesh.Object);
	else UE_LOG(LogTemp, Error, TEXT("SoilMesh for %s did not found!"), *this->GetFName().ToString());

	SoilMeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);

	auto CreateSnap = [&](ESH_SnapDirection Direction, const FString& Name)
	{
		FSH_SoilSnapData& Data = SnapMap.Add(Direction);
		Data.SnapDirection = Direction;

		Data.SnapPoint = CreateDefaultSubobject<USH_SnapPointComponent>(
			*FString::Printf(TEXT("Snap_%s"), *Name));

		Data.TriggerBox = CreateDefaultSubobject<UBoxComponent>(
			*FString::Printf(TEXT("Trigger_%s"), *Name));

		Data.SnapPoint->SetupAttachment(RootComponent);
		Data.TriggerBox->SetupAttachment(RootComponent);
	};

	CreateSnap(ESH_SnapDirection::Top, "Top");
	CreateSnap(ESH_SnapDirection::Bottom, "Bottom");
	CreateSnap(ESH_SnapDirection::Left, "Left");
	CreateSnap(ESH_SnapDirection::Right, "Right");

	for (auto& [Dir, Data] : SnapMap)
	{
		Data.TriggerBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
		Data.TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
		Data.TriggerBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);

		Data.SnapPoint->SetMobility(EComponentMobility::Type::Stationary);
		Data.TriggerBox->SetMobility(EComponentMobility::Type::Stationary);
	}

	for (auto& [Dir, Data] : SnapMap)
	{
		Data.SnapPoint->SetRelativeLocation(GetSnapOffset(Dir));
		
		Data.TriggerBox->SetRelativeLocation(GetTriggerBoxOffset(Dir));
		Data.TriggerBox->SetBoxExtent(GetTriggerBoxExtent(Dir));
		Data.TriggerBox->ComponentTags.Add(GetTriggerBoxTag(Dir));
	}
}

void ASH_BaseSoil::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Рандомный поворот для бОльшего разнообразия растений
	FRotator PlantCompRotator(0.f, FMath::FRandRange(0.f, 180.f), 0.f);
	PlantMeshComp->SetRelativeRotation(PlantCompRotator);
}
void ASH_BaseSoil::BeginPlay()
{
	Super::BeginPlay();
	
	USH_GameTimeSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USH_GameTimeSubsystem>();
	if (Subsystem)
	{
		Subsystem->OnMinuteChanged.AddUObject(this, &ASH_BaseSoil::HandleWorldSmallTick);
	}
}
void ASH_BaseSoil::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	USH_GameTimeSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USH_GameTimeSubsystem>();
	if (Subsystem)
	{
		Subsystem->OnMinuteChanged.RemoveAll(this);
	}
	Super::EndPlay(EndPlayReason);
}


// -------------------
// Plant Controls
// -------------------
bool ASH_BaseSoil::PlantSeed(USH_SeedItemPDA* SeedItem)
{
	if (PlantData.GetHasPlantAlready())
	{
		UE_LOG(LogTemp, Warning, TEXT("This soil is already busy"));
		return false;
	}
	
	if (!SeedItem)
	{
		UE_LOG(LogTemp, Error, TEXT("SeedItem is nullptr"));
		return false;
	}

	USH_GameTimeSubsystem* GTSubsystem = GetGameInstance()->GetSubsystem<USH_GameTimeSubsystem>();
	if (!GTSubsystem) return false;

	PlantData.PlantSeed(SeedItem, GTSubsystem->GetCurrentTime());

	PreloadMeshes();
	PlantMeshComp->SetStaticMesh(PlantData.GetPlantedSeed()->GrowVisualStages.GetMeshAtStage(0).LoadSynchronous());
	
	return true;
}
void ASH_BaseSoil::HandleWorldSmallTick(const FSH_WorldTime& WorldTime)
{
	if (!PlantData.GetPlantedSeed()) return;
	
	int32 CachedStage = PlantData.GetCurrentStage();
	PlantData.UpdateCurrentGrowProcess(WorldTime);

	int32 CurrentStage = PlantData.GetCurrentStage();
	if (CachedStage == CurrentStage) return;

	PlantMeshComp->SetStaticMesh(LoadedMeshes[CurrentStage]);
}
void ASH_BaseSoil::PreloadMeshes()
{
	if (!PlantData.GetPlantedSeed()) return;
	USH_SeedItemPDA* PlantedSeed = PlantData.GetPlantedSeed();

	for (int32 i = 0; i < PlantedSeed->GrowVisualStages.GetMaxStages(); i++)
	{
		TSoftObjectPtr<UStaticMesh> Mesh = PlantedSeed->GrowVisualStages.GetMeshAtStage(i);

		if (!Mesh.IsNull())
		{
			LoadedMeshes.Add(Mesh.LoadSynchronous());
		}
	}
}


// -------------------
// Slots Controls
// -------------------
FVector ASH_BaseSoil::GetSnapOffset(ESH_SnapDirection Direction)
{
	switch (Direction)
	{
	case ESH_SnapDirection::Top:      return FVector( -150.f,   0.f, 0.f );
	case ESH_SnapDirection::Bottom:   return FVector(  150.f,   0.f, 0.f );
	case ESH_SnapDirection::Left:     return FVector(    0.f, 150.f, 0.f );
	case ESH_SnapDirection::Right:    return FVector(    0.f,-150.f, 0.f );
		
	default: return FVector(0.f, 0.f, 0.f);
	}
}
FVector ASH_BaseSoil::GetTriggerBoxOffset(ESH_SnapDirection Direction)
{
	switch (Direction)
	{
	case ESH_SnapDirection::Top:      return FVector( -90.f,   0.f, 15.f );
	case ESH_SnapDirection::Bottom:   return FVector(  90.f,   0.f, 15.f );
	case ESH_SnapDirection::Left:     return FVector(   0.f,  90.f, 15.f );
	case ESH_SnapDirection::Right:    return FVector(   0.f, -90.f, 15.f );
			
	default: return FVector(0.f, 0.f, 0.f);
	}
}
FVector ASH_BaseSoil::GetTriggerBoxExtent(ESH_SnapDirection Direction)
{
	switch (Direction)
	{
	case ESH_SnapDirection::Top:      return FVector( 30.f, 50.f, 10.f );
	case ESH_SnapDirection::Bottom:   return FVector( 30.f, 50.f, 10.f );
	case ESH_SnapDirection::Left:     return FVector( 50.f, 30.f, 10.f );
	case ESH_SnapDirection::Right:    return FVector( 50.f, 30.f, 10.f );
				
	default: return FVector(0.f, 0.f, 0.f);
	}
}
FName ASH_BaseSoil::GetTriggerBoxTag(ESH_SnapDirection Direction)
{
	switch (Direction)
	{
	case ESH_SnapDirection::Top:      return FName("TriggerBoxTop");
	case ESH_SnapDirection::Bottom:   return FName("TriggerBoxBottom");
	case ESH_SnapDirection::Left:     return FName("TriggerBoxLeft");
	case ESH_SnapDirection::Right:    return FName("TriggerBoxRight");
					
	default: return FName("None");
	}
}
bool ASH_BaseSoil::IsOccupiedSlotByDirection(ESH_SnapDirection Direction)
{
	check(SnapMap[ESH_SnapDirection::Top].SnapPoint);
	check(SnapMap[ESH_SnapDirection::Bottom].SnapPoint);
	check(SnapMap[ESH_SnapDirection::Left].SnapPoint);
	check(SnapMap[ESH_SnapDirection::Right].SnapPoint);

	switch (Direction)
	{
		case ESH_SnapDirection::Top:
			return OccupiedSlots.bIsOccupiedSlotTop;
		case ESH_SnapDirection::Bottom:
			return OccupiedSlots.bIsOccupiedSlotBottom;
		case ESH_SnapDirection::Left:
			return OccupiedSlots.bIsOccupiedSlotLeft;
		case ESH_SnapDirection::Right:
			return OccupiedSlots.bIsOccupiedSlotRight;
		default:
			UE_LOG(LogTemp, Error, TEXT("Wrong TriggerBoxName"));
			return false;
		}
}
void ASH_BaseSoil::SetOccupiedSlotByDirection(ESH_SnapDirection Direction)
{
	switch (Direction)
	{
		case ESH_SnapDirection::Top:
		{
			OccupiedSlots.bIsOccupiedSlotTop = true;
			SnapMap[ESH_SnapDirection::Top].TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			return;
		}
		case ESH_SnapDirection::Bottom:
		{
			OccupiedSlots.bIsOccupiedSlotBottom = true;
			SnapMap[ESH_SnapDirection::Bottom].TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			return;
		}
		case ESH_SnapDirection::Left:
		{
			OccupiedSlots.bIsOccupiedSlotLeft = true;
			SnapMap[ESH_SnapDirection::Left].TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			return;
		}
		case ESH_SnapDirection::Right:
		{
			OccupiedSlots.bIsOccupiedSlotRight = true;
			SnapMap[ESH_SnapDirection::Right].TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			return;
		}
		case ESH_SnapDirection::None:
		{
			return;
		}
		default:
		{
			UE_LOG(LogTemp, Error, TEXT("Wrong SnapDirection"));
			break;
		}
	}
}
FVector ASH_BaseSoil::GetTransformToPlaceSoil(ESH_SnapDirection Direction)
{
	check(SnapMap[ESH_SnapDirection::Top].SnapPoint);
	check(SnapMap[ESH_SnapDirection::Bottom].SnapPoint);
	check(SnapMap[ESH_SnapDirection::Left].SnapPoint);
	check(SnapMap[ESH_SnapDirection::Right].SnapPoint);
	
	switch (Direction)
	{
		case ESH_SnapDirection::Top:
				return SnapMap[ESH_SnapDirection::Top].SnapPoint->GetComponentLocation();
		case ESH_SnapDirection::Bottom:
				return SnapMap[ESH_SnapDirection::Bottom].SnapPoint->GetComponentLocation();
		case ESH_SnapDirection::Left:
				return SnapMap[ESH_SnapDirection::Left].SnapPoint->GetComponentLocation();
		case ESH_SnapDirection::Right:
				return SnapMap[ESH_SnapDirection::Right].SnapPoint->GetComponentLocation();
		default:
				UE_LOG(LogTemp, Error, TEXT("Wrong TriggerBoxName"));
				return FVector(0, 0, 0);
	}
}