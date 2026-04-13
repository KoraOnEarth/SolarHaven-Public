// Copyright Lightline Studios. All rights reserved 

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SH_SnapPointComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHCORE_API USH_SnapPointComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	USH_SnapPointComponent();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	FName SnapName;  // "Snap_Top", "Snap_Bottom", и т.д.
};
