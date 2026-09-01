// LightLine Studios. All rights reserved.


#include "Items/Behaviors/SH_BaseItemBehavior.h"

#include "Items/SH_BuildPreview.h"
#include "Items/Behaviors/SH_BuildCertificateBehavior.h"
#include "Items/PDAs/SH_BuildCertificatePDA.h"

void USH_BaseItemBehavior::Init(AActor* PlayerCharacter, USH_ItemPDA* ItemPDA)
{
	ItemData = ItemPDA;
	CachedCharacter = PlayerCharacter;
}

void USH_BaseItemBehavior::Tick(const FSH_ItemUseContext& Context)
{
	//
}

void USH_BaseItemBehavior::BeginDestroy()
{
	UE_LOG(LogTemp, Log, TEXT("USH_BaseItemBehavior: BeginDestroy(): %s"), *GetName())
	Super::BeginDestroy();
}

void USH_BaseItemBehavior::OnEquipped()
{
	if (!IsValid(CachedCharacter)) return;
	if (!PreviewActorClass) return;
	UWorld* World = CachedCharacter->GetWorld();
	if (!World) return;
	
	PreviewActor = World->SpawnActor<ASH_BuildPreview>(PreviewActorClass);
	if (this->IsA<USH_BuildCertificateBehavior>())
	{
		USH_BuildCertificatePDA* CertificatePDA = Cast<USH_BuildCertificatePDA>(ItemData);
		FIntVector2 Size = CertificatePDA->GetSize();
		PreviewActor->Init(this, Size.X, Size.Y);
	}
	else
	{
		PreviewActor->Init(this);
	}
}

void USH_BaseItemBehavior::OnUnEquipped()
{
	if (PreviewActor)
	{
		PreviewActor->Destroy();
		PreviewActor = nullptr;
	}
}

bool USH_BaseItemBehavior::Use(const FSH_ItemUseContext& Context)
{
	return false;
}

void USH_BaseItemBehavior::UpdateItemData(AActor* PlayerCharacter, USH_ItemPDA* ItemPDA)
{
	Init(PlayerCharacter, ItemPDA);
}
