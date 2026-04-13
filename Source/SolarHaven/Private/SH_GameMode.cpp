// Copyright Lightline Studios. All rights reserved 


#include "SolarHaven/Public/SH_GameMode.h"

#include "SH_GameTimeSubsystem.h"
#include "SH_PlayerCharacter.h"
#include "SH_PlayerController.h"

ASH_GameMode::ASH_GameMode()
{
	DefaultPawnClass = ASH_PlayerCharacter::StaticClass();
	PlayerControllerClass = ASH_PlayerController::StaticClass();
}

void ASH_GameMode::BeginPlay()
{
	Super::BeginPlay();

	USH_GameTimeSubsystem* TimeSystem = GetGameInstance()->GetSubsystem<USH_GameTimeSubsystem>();
	if (TimeSystem)
	{
		TimeSystem->InitDefaultTime();
		TimeSystem->StartTime();
	}
}

void ASH_GameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	USH_GameTimeSubsystem* TimeSystem = GetGameInstance()->GetSubsystem<USH_GameTimeSubsystem>();
	if (TimeSystem)
	{
		TimeSystem->StopTime();
	}
	
	Super::EndPlay(EndPlayReason);
}
