// Copyright Lightline Studios. All rights reserved 


#include "BaseClasses/SH_ItemBaseBehavior.h"

void USH_ItemBaseBehavior::Init(AActor* InPlayerCharacter, USH_ItemPDA* InItemPDA)
{
	ItemData = InItemPDA;
}

void USH_ItemBaseBehavior::Tick(const FSH_ItemUseContext& Context)
{
	UE_LOG(LogTemp, Log, TEXT("ItemBaseBehavior::Tick"));
}

void USH_ItemBaseBehavior::OnEquipped()
{
	UE_LOG(LogTemp, Log, TEXT("ItemBaseBehavior::OnEquipped"));
}

void USH_ItemBaseBehavior::OnUnEquipped()
{
	UE_LOG(LogTemp, Log, TEXT("ItemBaseBehavior::OnUnEquipped"));
}

void USH_ItemBaseBehavior::Use(const FSH_ItemUseContext& Context)
{
	UE_LOG(LogTemp, Log, TEXT("ItemBaseBehavior::Use"));
}
