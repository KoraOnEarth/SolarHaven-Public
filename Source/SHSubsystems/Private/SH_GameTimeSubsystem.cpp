// Copyright Lightline Studios. All rights reserved 


#include "SH_GameTimeSubsystem.h"

void USH_GameTimeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USH_GameTimeSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void USH_GameTimeSubsystem::IncrementTime()
{
	if (!bIsRunning) return;

	const FSH_WorldTime OldTime = CurrentTime;

	// Если множитель будет, например, 1 000 000 - функция может стать дорогой
	CurrentTime.IncrementTime(TimeMultiplier);

	const FSH_WorldTime NewTime = CurrentTime;

	ProcessTimeEvents(OldTime, NewTime);
}

void USH_GameTimeSubsystem::ProcessTimeEvents(const FSH_WorldTime& OldTime, const FSH_WorldTime& NewTime)
{
	int32 OldTotal = OldTime.ToTotalMinutes();
	int32 NewTotal = NewTime.ToTotalMinutes();

	for (int32 t = OldTotal + 1; t <= NewTotal; t++)
	{
		int32 Minute = t % 60;
		int32 Hour = (t / 60) % 24;
		int32 Day = ((t / (60 * 24)) % 30) + 1;
		int32 SeasonIndex = (t / (60 * 24 * 30)) % 4;
		int32 Year = t / (60 * 24 * 30 * 4);

		// Минуты
		OnMinuteChanged.Broadcast(CurrentTime);

		// Часы
		if (Minute == 0)
		{
			OnHourChanged.Broadcast(CurrentTime);
		}

		// Дни
		if (Minute == 0 && Hour == 0)
		{
			OnDayChanged.Broadcast(CurrentTime);
		}

		// Сезоны
		if (Minute == 0 && Hour == 0 && Day == 1)
		{
			OnSeasonChanged.Broadcast(CurrentTime);
		}

		// Годы
		if (Minute == 0 && Hour == 0 && Day == 1 && SeasonIndex == 0)
		{
			OnYearChanged.Broadcast(CurrentTime);
		}
	}
}

void USH_GameTimeSubsystem::StartTime()
{
	bIsRunning = true;

	if (!GetWorld()) return;
	
	GetWorld()->GetTimerManager().SetTimer(
		Timer,
		this, &USH_GameTimeSubsystem::IncrementTime,
		1.f,
		true
	);
}

void USH_GameTimeSubsystem::StopTime()
{
	bIsRunning = false;
}

void USH_GameTimeSubsystem::InitDefaultTime()
{	
	CurrentTime.Years = 1;
	CurrentTime.Season = ESH_Seasons::Spring;
	CurrentTime.Days = 1;
	CurrentTime.Hours = 9;
	CurrentTime.Minutes = 0;
}

const FSH_WorldTime& USH_GameTimeSubsystem::GetCurrentTime()
{
	return CurrentTime;
}

void USH_GameTimeSubsystem::PrintCurrentTime()
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *CurrentTime.ToString());
}
