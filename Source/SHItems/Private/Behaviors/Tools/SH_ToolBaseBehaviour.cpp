// Copyright Lightline Studios. All rights reserved 


#include "Behaviors/Tools/SH_ToolBaseBehaviour.h"
#include "PrimaryDataAssets/SH_ToolItemPDA.h"

void USH_ToolBaseBehaviour::Init(AActor* InPlayerCharacter, USH_ItemPDA* InItemPDA)
{
	if (!InPlayerCharacter) return;
	
	ItemData = CastChecked<USH_ToolItemPDA>(InItemPDA);
	CachedCharacter = InPlayerCharacter;
}

void USH_ToolBaseBehaviour::Use(const FSH_ItemUseContext& Context)
{
	UE_LOG(LogTemp, Log, TEXT("USH_ToolBaseBehaviour::Use"));
}
