// LightLine Studios. All rights reserved.


#include "Items/Behaviors/SH_BaseSeedBehavior.h"

#include "Interfaces/SH_PlantableInterface.h"


#include "Items/PDAs/SH_SeedItemPDA.h"
#include "Items/Structs/SH_ItemUseContext.h"

class ISH_PlantableInterface;

void USH_BaseSeedBehavior::Init(AActor* PlayerCharacter, USH_ItemPDA* ItemPDA)
{
	Super::Init(PlayerCharacter, ItemPDA);
	ItemData = CastChecked<USH_SeedItemPDA>(ItemPDA);
}
bool USH_BaseSeedBehavior::Use(const FSH_ItemUseContext& Context)
{
	if (!ItemData || !CachedCharacter) return false;
	UWorld* World = CachedCharacter->GetWorld();
	if (!World) return false;
	
	FVector Start = Context.ViewLocation;
	FVector End = Start + Context.ViewDirection * Context.InteractionDistance;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Context.Instigator);
	FHitResult Hit;
	const bool bHit = World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
	if (!bHit) return false;
	
	ISH_PlantableInterface* PlantInterface = Cast<ISH_PlantableInterface>(Hit.GetActor());
	if (!PlantInterface) return false;
	
	USH_SeedItemPDA* SeedPDA = CastChecked<USH_SeedItemPDA>(ItemData);
	const bool SuccessfulPlant = PlantInterface->Plant(SeedPDA);
	return SuccessfulPlant;
}
