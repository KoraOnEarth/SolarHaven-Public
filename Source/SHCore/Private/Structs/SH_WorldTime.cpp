// Copyright Lightline Studios. All rights reserved

#include "Structs/SH_WorldTime.h"

#include "Structs/SH_TimeSpan.h"

void FSH_WorldTime::IncrementTime(int32 TimeMultiplier)
{
	Minutes += TimeMultiplier;

	if (Minutes >= 60)
	{
		Hours += Minutes / 60;
		Minutes = Minutes % 60;
	}

	if (Hours >= 24)
	{
		Days += Hours / 24;
		Hours = Hours % 24;
	}

	if (Days > 30)
	{
		int32 PassedSeasons = (Days - 1) / 30;
		Days = ((Days - 1) % 30) + 1;

		for (int32 i = 0; i < PassedSeasons; i++)
		{
			if (Season == ESH_Seasons::Winter)
			{
				Years++;
			}

			Season = GetNextSeason(Season);
		}
	}
}
ESH_Seasons FSH_WorldTime::GetNextSeason(ESH_Seasons Current)
{
	int32 Next = static_cast<int32>(Current) + 1;
	if (Next > static_cast<int32>(ESH_Seasons::Winter))
	{
		return ESH_Seasons::Spring;
	}
	return static_cast<ESH_Seasons>(Next);
}
FString FSH_WorldTime::GetSeasonName(ESH_Seasons Current) const
{
	switch (Current)
	{
	case ESH_Seasons::Spring: return "Spring";
	case ESH_Seasons::Summer: return "Summer";
	case ESH_Seasons::Autumn: return "Autumn";
	case ESH_Seasons::Winter: return "Winter";

	default: return "";
	}
}

// ----------------------
// Overloads
// ----------------------
FSH_WorldTime FSH_WorldTime::operator+(const FSH_TimeSpan& Span) const
{
	int32 Total = ToTotalMinutes() + Span.GetTotalMinutes();

	FSH_WorldTime Result;

	Result.Years = Total / (60 * 24 * 30 * 4);
	Total %= (60 * 24 * 30 * 4);

	int32 SeasonIndex = Total / (60 * 24 * 30);
	Result.Season = static_cast<ESH_Seasons>(SeasonIndex);
	Total %= (60 * 24 * 30);

	Result.Days = (Total / (60 * 24)) + 1;
	Total %= (60 * 24);

	Result.Hours = Total / 60;
	Result.Minutes = Total % 60;

	return Result;
}
FSH_TimeSpan FSH_WorldTime::operator-(const FSH_WorldTime& Other) const
{
	int32 Diff = ToTotalMinutes() - Other.ToTotalMinutes();
	return FSH_TimeSpan(FMath::Max(0, Diff));
}

// ----------------------
// Converters
// ----------------------
FString FSH_WorldTime::ToString() const
{
	return FString::Printf(
		TEXT("Year: %d | Season: %s | Day: %d | %02d:%02d"),
		Years,
		*GetSeasonName(Season),
		Days,
		Hours,
		Minutes
	);
}
int32 FSH_WorldTime::ToTotalMinutes() const
{
	return Years * 4 * 30 * 24 * 60 +
		   static_cast<int32>(Season) * 30 * 24 * 60 +
		   (Days - 1) * 24 * 60 +
		   Hours * 60 +
		   Minutes;
}
