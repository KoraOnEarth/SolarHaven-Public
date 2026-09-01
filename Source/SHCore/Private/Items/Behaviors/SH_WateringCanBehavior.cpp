// LightLine Studios. All rights reserved.


#include "Items/Behaviors/SH_WateringCanBehavior.h"

#include "Interfaces/SH_PlantableInterface.h"

#include "Items/Structs/SH_ItemUseContext.h"


bool USH_WateringCanBehavior::Use(const FSH_ItemUseContext& Context)
{
	UWorld* World = CachedCharacter->GetWorld();
	if (!World) return false;
	
	FVector Start = Context.ViewLocation;
	FVector End = Start + Context.ViewDirection * Context.InteractionDistance;
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Context.Instigator);
	const bool bHit = World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
	if (!bHit)
	{
		UE_LOG(LogTemp, Log, TEXT("!bHit from WateringCanBeh"));
		return false;
	}
	ISH_PlantableInterface* PlantableInterface = Cast<ISH_PlantableInterface>(Hit.GetActor());
	if (!PlantableInterface) return false;
	PlantableInterface->ToWater();
	return true;
}
