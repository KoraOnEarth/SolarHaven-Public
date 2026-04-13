// Copyright Lightline Studios. All rights reserved 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SH_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class SOLARHAVEN_API ASH_GameMode : public AGameModeBase
{
	GENERATED_BODY()

	ASH_GameMode();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
