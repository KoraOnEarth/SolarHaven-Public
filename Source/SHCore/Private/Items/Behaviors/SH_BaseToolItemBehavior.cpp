// LightLine Studios. All rights reserved.


#include "Items/Behaviors/SH_BaseToolItemBehavior.h"

#include "Items/PDAs/SH_ToolItemPDA.h"

void USH_BaseToolItemBehavior::Init(AActor* PlayerCharacter, USH_ItemPDA* ItemPDA)
{
	Super::Init(PlayerCharacter, ItemPDA);
	ItemData = CastChecked<USH_ToolItemPDA>(ItemPDA);
}

bool USH_BaseToolItemBehavior::Use(const FSH_ItemUseContext& Context)
{
	return false;
}
