// Copyright Lightline Studios. All rights reserved 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SH_ItemVisual.generated.h"

UCLASS()
class SHITEMS_API ASH_ItemVisual : public AActor
{
	GENERATED_BODY()

public:
	ASH_ItemVisual();

	void SetStaticMesh(UStaticMesh* InMesh);
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
};
