// LightLine Studios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SH_ItemInHandVisual.generated.h"

UCLASS()
class SHCORE_API ASH_ItemInHandVisual : public AActor
{
	GENERATED_BODY()

public:
	ASH_ItemInHandVisual();
	
	void SetMesh(UStaticMesh* Mesh);

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
};
