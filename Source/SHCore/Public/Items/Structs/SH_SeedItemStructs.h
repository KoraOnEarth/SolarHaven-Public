#pragma once

#include "CoreMinimal.h"

#include "SH_SeedItemStructs.generated.h"

USTRUCT()
struct FSH_GrowSeasons
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	float SpringGrowModifier = 1.f;
	UPROPERTY(EditDefaultsOnly)
	float SummerGrowModifier = 1.f;
	UPROPERTY(EditDefaultsOnly)
	float AutumnGrowModifier = 1.f;
	UPROPERTY(EditDefaultsOnly)
	float WinterGrowModifier = 1.f;
};

USTRUCT()
struct FSH_GrowVisualStages
{
	GENERATED_BODY()
	
public:
	TSoftObjectPtr<UStaticMesh> GetMeshAtStage(int Stage) const { return Meshes[Stage]; };
	int32 GetMaxStages() const { return Meshes.Num(); }
	
private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TArray<TSoftObjectPtr<UStaticMesh>> Meshes;
};
