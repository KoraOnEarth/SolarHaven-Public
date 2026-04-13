// Copyright Lightline Studios. All rights reserved 


#include "BaseClasses/SH_ItemPDA.h"

FPrimaryAssetId USH_ItemPDA::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("Item", ItemID);
}
