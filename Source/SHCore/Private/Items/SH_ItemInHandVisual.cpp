// LightLine Studios. All rights reserved.

#include "Items/SH_ItemInHandVisual.h"


ASH_ItemInHandVisual::ASH_ItemInHandVisual()
{
	PrimaryActorTick.bCanEverTick = false;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);
	StaticMeshComponent->SetCastShadow(false);
}

void ASH_ItemInHandVisual::SetMesh(UStaticMesh* Mesh)
{
	StaticMeshComponent->SetStaticMesh(Mesh);
}
