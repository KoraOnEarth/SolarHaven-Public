// LightLine Studios. All rights reserved.


#include "Items/SH_ItemLibrary.h"

#include "Engine/AssetManager.h"

#include "Items/PDAs/SH_ItemPDA.h"

USH_ItemPDA* USH_ItemLibrary::GetItemByID(FName ItemID)
{
	FPrimaryAssetId AssetId("Item", ItemID);
	UAssetManager& Manager = UAssetManager::Get();
	
	UObject* Asset = Manager.GetPrimaryAssetObject(AssetId);
	if (!Asset)
	{
		// LoadPrimaryAsset - начинает загрузку
		// Handle - управляет процессом загрузки
		TSharedPtr<FStreamableHandle> Handle = Manager.LoadPrimaryAsset(AssetId);

		if (Handle.IsValid())
		{
			Handle->WaitUntilComplete();
			
			// GetPrimaryAssetObject возвращает уже загруженный ассет
			Asset = Manager.GetPrimaryAssetObject(AssetId);
		}
	}

	return Cast<USH_ItemPDA>(Asset);
}
