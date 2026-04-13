// Copyright Lightline Studios. All rights reserved 


#include "PrimaryDataAssets/SH_SeedItemPDA.h"

TSoftObjectPtr<UStaticMesh> FSH_GrowVisualStages::GetMeshAtStage(int Stage) const
{
	return Meshes[Stage];
}
