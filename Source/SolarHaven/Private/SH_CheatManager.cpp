// Copyright Lightline Studios. All rights reserved 


#include "SH_CheatManager.h"

#include "SH_GameTimeSubsystem.h"
#include "SH_PlayerController.h"

void USH_CheatManager::AddItem(const FString& Args)
{
	TArray<FString> Tokens;
	Args.ParseIntoArray(Tokens, TEXT(" "), true);

	if (Tokens.Num() == 0) return;

	FName ItemID = FName(*Tokens[0]);
	int32 Quantity = 1;

	if (Tokens.Num() > 1)
	{
		LexTryParseString(Quantity, *Tokens[1]);
	}
	
	ASH_PlayerController* PC = Cast<ASH_PlayerController>(GetPlayerController());
	PC->AddItem(ItemID, Quantity);
}

void USH_CheatManager::ClearInventory()
{
	ASH_PlayerController* PC = Cast<ASH_PlayerController>(GetPlayerController());
	PC->ClearInventory();
}

void USH_CheatManager::ClearInventoryAtSlot(int32 TargetSlot)
{
	ASH_PlayerController* PC = Cast<ASH_PlayerController>(GetPlayerController());
	PC->ClearInventoryAtSlot(TargetSlot);
}

void USH_CheatManager::PrintInventoryData(const FString& Args)
{
	TArray<FString> Tokens;
	Args.ParseIntoArray(Tokens, TEXT(" "), true);
	
	int32 TargetSlot = -1;
	if (Tokens.Num() != 0)
	{
		LexTryParseString(TargetSlot, *Tokens[0]);
	}
	
	ASH_PlayerController* PC = Cast<ASH_PlayerController>(GetPlayerController());
	PC->PrintInventoryData(TargetSlot);
}

void USH_CheatManager::PrintCurrentTime()
{
	USH_GameTimeSubsystem* TimeSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USH_GameTimeSubsystem>();
	if (TimeSubsystem)
	{
		TimeSubsystem->PrintCurrentTime();
	}
}
