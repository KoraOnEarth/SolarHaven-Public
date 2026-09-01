// Copyright LightLine Studios. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "SH_Seasons.h"
#include "SH_TimeSpan.h"

#include "SH_WorldTime.generated.h"

USTRUCT(BlueprintType)
struct SHCORE_API FSH_WorldTime
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleInstanceOnly) 
	int32 Years = 0;
	UPROPERTY(VisibleInstanceOnly) 
	ESH_Seasons Season = ESH_Seasons::None;
	UPROPERTY(VisibleInstanceOnly) 
	int32 Days = 0;
	UPROPERTY(VisibleInstanceOnly) 
	int32 Hours = 0;
	UPROPERTY(VisibleInstanceOnly) 
	int32 Minutes = 0;

	void IncrementTime(int32 TimeMultiplier = 1);
	ESH_Seasons GetNextSeason(ESH_Seasons Current);
	FString GetSeasonName(ESH_Seasons Current) const;
	void ClearTime();

	// ----------------------
	// Overloads
	// ----------------------
	FSH_WorldTime operator+(const FSH_TimeSpan& Span) const;
	FSH_TimeSpan operator-(const FSH_WorldTime& Other) const;

	// ----------------------
	// Converters
	// ----------------------
	FString ToString() const;
	int32 ToTotalMinutes() const;
};
