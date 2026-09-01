// LightLine Studios. All rights reserved.

#include "Systems/SH_GridSystem.h"

#include "Components/BoxComponent.h"


ASH_GridSystem::ASH_GridSystem()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;
	
	GridVolume = CreateDefaultSubobject<UBoxComponent>("GridVolume");
	GridVolume->SetupAttachment(RootComponent);
	
	GridVolume->SetRelativeLocation(FVector(CellSize * 0.5f, CellSize * 0.5f, 0.f));
	GridVolume->SetBoxExtent(FVector(CellSize, CellSize, GridHeight));
}

void ASH_GridSystem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	UpdateGridVolume();
}

void ASH_GridSystem::BeginPlay()
{
	Super::BeginPlay();
	
	UpdateGridStruct();
}

void ASH_GridSystem::UpdateGridVolume()
{
	const float Width = CellSize * XCount;
	const float Height = CellSize * YCount;
	
	GridVolume->SetRelativeLocation(FVector(Width * 0.5f, Height * 0.5f, 0.f));
	GridVolume->SetBoxExtent(FVector(Width * 0.5f, Height * 0.5f, GridHeight));
}

void ASH_GridSystem::UpdateGridStruct()
{
	GridCells.Empty();
	for (int32 i = 0; i < XCount; i++)
	{
		for (int32 j = 0; j < YCount; j++)
		{
			GridCells.Add(FSH_GridCell(i, j));
		}
	}
}

FIntVector2 ASH_GridSystem::GetCellXY(const FVector& WorldLocation)
{
	const UE::Math::TVector<double> Locals = GetActorTransform().InverseTransformPosition(WorldLocation);

	const int32 CellX = FMath::FloorToInt(Locals.X / CellSize);
	const int32 CellY = FMath::FloorToInt(Locals.Y / CellSize);
	
	return FIntVector2(CellX, CellY);
}

FSH_GridCell* ASH_GridSystem::GetCell(int32 x, int32 y)
{
	if (x < 0 || x >= XCount || y < 0 || y >= YCount) return nullptr;
	return &GridCells[x * YCount + y];
}

FVector ASH_GridSystem::GetCellCenterLocation(const FVector& HitLocation)
{
	const FIntVector2 XYGridIndex = GetCellXY(HitLocation);
	const FVector LocalCellCenter = FVector(XYGridIndex.X * CellSize + CellSize / 2, XYGridIndex.Y * CellSize + CellSize / 2, GridHeight);
	const FVector WorldCellCenter = GetActorTransform().TransformPosition(LocalCellCenter);
	return WorldCellCenter;
}

FVector ASH_GridSystem::GetLocationToBuild(const FVector& HitLocation, const FRotator& CharacterRotation, int32 xSize, int32 ySize)
{
	if (xSize <= 0 || ySize <= 0) return FVector::ZeroVector;

	const FVector CellCenter = GetCellCenterLocation(HitLocation);
	const float GridYaw = FMath::GridSnap(FMath::UnwindDegrees(CharacterRotation.Yaw), 90.f);

	FVector Offset;
	Offset.X = -(xSize - 1) * (CellSize * 0.5f) + CellSize * ((xSize - 1) / 2);
	Offset.Y = (ySize - 1) * (CellSize * 0.5f);
	Offset.Z = 0.f;
	Offset = Offset.RotateAngleAxis(GridYaw, FVector::UpVector);

	return CellCenter - Offset;
}

bool ASH_GridSystem::GetIsCellOccupied(const FVector& HitLocation, int32 xSize, int32 ySize)
{
	TArray<FSH_GridCell*> CellsToCheck;
	for (int i = 0; i < xSize; i++)
	{
		for (int j = 0; j < ySize; j++)
		{
			FVector CurrentCellLocation(HitLocation.X - i * CellSize, HitLocation.Y + j * CellSize, GridHeight);
			const FIntVector2 XYGridIndex = GetCellXY(CurrentCellLocation);
			CellsToCheck.Add(GetCell(XYGridIndex.X, XYGridIndex.Y));
		}
	}
	if (CellsToCheck.Num() == 0) return true;
	for (const FSH_GridCell* i : CellsToCheck)
	{
		if (i->GetIsOccupied()) return true;
	}
	return false;
}

void ASH_GridSystem::SetCellOccupation(const FVector& HitLocation, bool bIsOccupied, int32 xSize, int32 ySize)
{
	const FIntVector2 XYGridIndex = GetCellXY(HitLocation);
	FSH_GridCell* Cell = GetCell(XYGridIndex.X, XYGridIndex.Y);
	
	if (!Cell) return;
	Cell->SetOccupation(bIsOccupied);
}

