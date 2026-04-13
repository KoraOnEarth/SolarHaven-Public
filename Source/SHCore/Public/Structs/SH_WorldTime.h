// Copyright Lightline Studios. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Enums/SH_Seasons.h"
#include "SH_WorldTime.generated.h"

struct FSH_TimeSpan;
enum class ESH_Seasons : uint8;

USTRUCT(BlueprintType)
struct SHCORE_API FSH_WorldTime
{
	GENERATED_BODY()

	FSH_WorldTime() :
		Years(0),
		Season(ESH_Seasons::None),
		Days(0),
		Hours(0),
		Minutes(0)
	{	}

	UPROPERTY(VisibleInstanceOnly) 
	int32 Years;
	UPROPERTY(VisibleInstanceOnly) 
	ESH_Seasons Season;
	UPROPERTY(VisibleInstanceOnly) 
	int32 Days;
	UPROPERTY(VisibleInstanceOnly) 
	int32 Hours;
	UPROPERTY(VisibleInstanceOnly) 
	int32 Minutes;

	void IncrementTime(int32 TimeMultiplier = 1);
	ESH_Seasons GetNextSeason(ESH_Seasons Current);
	FString GetSeasonName(ESH_Seasons Current) const;

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
