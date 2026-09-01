// LightLine Studios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SH_GridCell.h"

#include "GameFramework/Actor.h"

#include "Interfaces/SH_BuildableGridInterface.h"

#include "SH_GridSystem.generated.h"

class UBoxComponent;

/**
 * Grid system. It is not yet clear whether it will be used in the final version. We'll see.
 */
UCLASS()
class SOLARHAVEN_API ASH_GridSystem : public AActor, public ISH_BuildableGridInterface
{
	GENERATED_BODY()

public:
	ASH_GridSystem();
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> GridVolume;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (ClampMin = 1, ClampMax = 2048, AllowPrivateAccess = true), Category="00 GridSettings")
	int32 XCount = 1;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (ClampMin = 1, ClampMax = 2048, AllowPrivateAccess = true), Category="00 GridSettings")
	int32 YCount = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 1.f, AllowPrivateAccess = true), Category="00 GridSettings")
	float CellSize = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 1.f, AllowPrivateAccess = true), Category="00 GridSettings")
	float GridHeight = 1.f;
	
	UPROPERTY()
	TArray<FSH_GridCell> GridCells;
	
	void UpdateGridVolume();
	void UpdateGridStruct();
	
	FIntVector2 GetCellXY(const FVector& WorldLocation);
	FSH_GridCell* GetCell(int32 x, int32 y);
	FVector GetCellCenterLocation(const FVector& HitLocation);

public:
	// Interface functions
	virtual FVector GetLocationToBuild(const FVector& HitLocation, const FRotator& CharacterRotation, int32 xSize = 1, int32 ySize = 1) override;
	virtual bool GetIsCellOccupied(const FVector& HitLocation, int32 xSize = 1, int32 ySize = 1) override;
	virtual void SetCellOccupation(const FVector& HitLocation, bool bIsOccupied, int32 xSize = 1, int32 ySize = 1) override;
};
