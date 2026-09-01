// LightLine Studios. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "SHCore/Public/SH_WorldTime.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "SH_GameTimeSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMinuteChanged, const FSH_WorldTime&  /* Current Time */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHourChanged,   const FSH_WorldTime&	/* Current Time */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDayChanged,    const FSH_WorldTime&  /* Current Time */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSeasonChanged, const FSH_WorldTime&	/* Current Time */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnYearChanged,   const FSH_WorldTime&	/* Current Time */);

/**
 * 
 */
UCLASS()
class SHSUBSYSTEMS_API USH_GameTimeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	USH_GameTimeSubsystem();
	
	void StartTime();
	void StopTime();
	void InitDefaultTime();
	
	const FSH_WorldTime& GetCurrentTime();
	void PrintCurrentTime();
	
	FOnMinuteChanged	OnMinuteChanged;
	FOnHourChanged		OnHourChanged;
	FOnDayChanged		OnDayChanged;
	FOnSeasonChanged	OnSeasonChanged;
	FOnYearChanged		OnYearChanged;
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
private:
	void IncrementTime();
	void ProcessTimeEvents(const FSH_WorldTime& OldTime, const FSH_WorldTime& NewTime);
	
	FTimerHandle Timer;
	FSH_WorldTime CurrentTime;
	bool bIsRunning = false;
	
	UPROPERTY()
	int32 TimeMultiplier = 1;
};
