#pragma once

#include "CoreMinimal.h"
#include "SH_TimeSpan.generated.h"

/**
 * Структура для записи разницы времени.
 * Хранит время в минутах. Имеет перегрузки сложения и вычитания.
 * Позволяет по количеству минут получить дни, часы, минуты.
 */
USTRUCT(BlueprintType)
struct SHCORE_API FSH_TimeSpan
{
	GENERATED_BODY()

public:
	FSH_TimeSpan() :
		TotalMinutes(0)
	{	}

	explicit FSH_TimeSpan(int32 InTotalMinutes) : TotalMinutes(InTotalMinutes) {}

	float GetTotalMinutes() const { return static_cast<float>(TotalMinutes); }
	int32 GetDays() const { return TotalMinutes / (60 * 24); }
	int32 GetHours() const { return (TotalMinutes / 60) % 24; }
	int32 GetMinutes() const { return TotalMinutes % 60; }

	bool IsZero() const {return TotalMinutes == 0;}
	
	FSH_TimeSpan operator+(const FSH_TimeSpan& Other) const;
	FSH_TimeSpan operator-(const FSH_TimeSpan& Other) const;
	FSH_TimeSpan operator*(const FSH_TimeSpan& Other) const;
	
	FString ToString() const;

private:
	UPROPERTY()
	int32 TotalMinutes;
};

// ----------------------
// Overloads
// ----------------------
inline FSH_TimeSpan FSH_TimeSpan::operator+(const FSH_TimeSpan& Other) const
{
	return FSH_TimeSpan(TotalMinutes + Other.TotalMinutes);
}
inline FSH_TimeSpan FSH_TimeSpan::operator-(const FSH_TimeSpan& Other) const
{
	return FSH_TimeSpan(FMath::Max(0, TotalMinutes - Other.TotalMinutes));
}
inline FSH_TimeSpan FSH_TimeSpan::operator*(const FSH_TimeSpan& Other) const
{
	return FSH_TimeSpan(TotalMinutes * Other.TotalMinutes);
}

// ----------------------
// Converters
// ----------------------
inline FString FSH_TimeSpan::ToString() const
{
	return FString::Printf(
		TEXT("%d days, %02d:%02d"),
		GetDays(),
		GetHours(),
		GetMinutes()
	);
}
