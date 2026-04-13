// Copyright Lightline Studios. All rights reserved 


#include "BaseClasses/SH_ItemVisual.h"


ASH_ItemVisual::ASH_ItemVisual()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMeshComponent);
}

void ASH_ItemVisual::SetStaticMesh(UStaticMesh* InMesh)
{
	StaticMeshComponent->SetStaticMesh(InMesh);
}

void ASH_ItemVisual::BeginPlay()
{
	Super::BeginPlay();

	//
}

