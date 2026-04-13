// Copyright Lightline Studios. All rights reserved 


#include "Behaviors/Seeds/SH_SeedBaseBehavior.h"

#include "Interfaces/SH_PlantableInterface.h"
#include "PrimaryDataAssets/SH_SeedItemPDA.h"
#include "Structs/SH_ItemUseContext.h"

void USH_SeedBaseBehavior::Init(AActor* InPlayerCharacter, USH_ItemPDA* InItemPDA)
{
	if (!InPlayerCharacter) return;
	
	ItemData = CastChecked<USH_SeedItemPDA>(InItemPDA);
	CachedCharacter = InPlayerCharacter;
}

void USH_SeedBaseBehavior::Use(const FSH_ItemUseContext& Context)
{
	USH_SeedItemPDA* CurrentData = Cast<USH_SeedItemPDA>(ItemData);
	if (!CurrentData)
	{
		UE_LOG(LogTemp, Error, TEXT("CurrentData is nullptr!"));
		return;
	}
	
	UWorld* World = CachedCharacter->GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("World is nullptr!"));
		return;
	}
	
	FVector Start = Context.ViewLocation;
	FVector End = Start + Context.ViewDirection * Context.InteractionDistance;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Context.Instigator);

	bool bHit = World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
	if (!bHit) return;
	
	if (ISH_PlantableInterface* Interface = Cast<ISH_PlantableInterface>(Hit.GetActor()))
	{
		USH_SeedItemPDA* SeedItem = Cast<USH_SeedItemPDA>(ItemData.Get());
		if (!ItemData || !SeedItem) return;
		
		Interface->PlantSeed(Cast<USH_SeedItemPDA>(ItemData.Get()));
	}
}
