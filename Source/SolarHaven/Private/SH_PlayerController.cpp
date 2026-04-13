// Copyright Lightline Studios. All rights reserved 

#include "SH_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SH_CheatManager.h"
#include "SH_ItemLibrary.h"
#include "SH_PlayerCharacter.h"
#include "SHComponents/Public/SH_InventoryComponent.h"

ASH_PlayerController::ASH_PlayerController()
{
	CheatClass = USH_CheatManager::StaticClass();
	CurrentActiveItemIndex = 0;
}

void ASH_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->AddMappingContext(MainMappingContext, 0);
		}
	}

	if (GetWorld() && GetWorld()->IsPlayInEditor())
	{
		AddItem(FName("hoe"), 1);
		AddItem(FName("corn_seed"), 1);
	}
}
void ASH_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(InputComponent);
	EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASH_PlayerController::Move);
	EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASH_PlayerController::Look);
	EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &ASH_PlayerController::Jump);
	EIC->BindAction(UseAction, ETriggerEvent::Started, this, &ASH_PlayerController::Use);
	EIC->BindAction(ChangeActiveItemAction, ETriggerEvent::Started, this, &ASH_PlayerController::ChangeActiveItem);

	PlayerCameraManager->ViewPitchMin = -70.f;
	PlayerCameraManager->ViewPitchMax = 80.f;
}
void ASH_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//
	SwitchUIState(ESH_GameState::Game);
}
void ASH_PlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	//
}

// ------------------------
// Inventory Functions
// ------------------------
void ASH_PlayerController::AddItem(const FName& ItemID, int32 Quantity)
{
	USH_ItemPDA* Item = USH_ItemLibrary::GetItemByID(ItemID);
	if (!Item)
	{
		UE_LOG(LogTemp, Warning, TEXT("No item found with ID: %s"), *ItemID.ToString());
		return;
	}
	
	ASH_PlayerCharacter* PC = Cast<ASH_PlayerCharacter>(GetPawn());
	if (!PC) return;
	
	USH_InventoryComponent* PlayerInventoryComponent = PC->GetInventoryComponent();
	if (!PlayerInventoryComponent) return;

	PlayerInventoryComponent->AddItem(Item, Quantity);
}
void ASH_PlayerController::ClearInventory()
{
	ASH_PlayerCharacter* PC = Cast<ASH_PlayerCharacter>(GetPawn());
	if (!PC) return;
	
	USH_InventoryComponent* PlayerInventoryComponent = PC->GetInventoryComponent();
	if (!PlayerInventoryComponent) return;

	PlayerInventoryComponent->ClearInventory();
}
void ASH_PlayerController::ClearInventoryAtSlot(int32 TargetSlot)
{
	ASH_PlayerCharacter* PC = Cast<ASH_PlayerCharacter>(GetPawn());
	if (!PC) return;
	
	USH_InventoryComponent* PlayerInventoryComponent = PC->GetInventoryComponent();
	if (!PlayerInventoryComponent) return;

	PlayerInventoryComponent->ClearInventoryAtSlot(TargetSlot);
}
void ASH_PlayerController::PrintInventoryData(int32 TargetSlot)
{
	ASH_PlayerCharacter* PC = Cast<ASH_PlayerCharacter>(GetPawn());
	if (!PC) return;
	
	USH_InventoryComponent* PlayerInventoryComponent = PC->GetInventoryComponent();
	if (!PlayerInventoryComponent) return;

	PlayerInventoryComponent->PrintInventoryData(TargetSlot);
}

// ------------------------
// UI Control
// ------------------------
void ASH_PlayerController::SwitchUIState(ESH_GameState InState)
{
	CurrentGameState = InState;

	switch (CurrentGameState)
	{
	case ESH_GameState::Game:
		{
			bShowMouseCursor = false;
			FInputModeGameOnly InputMode;
			SetInputMode(InputMode);

			break;
		}
		
	case ESH_GameState::Widget:
		{
			bShowMouseCursor = true;
			FInputModeUIOnly InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);

			SetInputMode(InputMode);

			break;
		}

	default:
		{
			UE_LOG(LogTemp, Warning, TEXT("GameState: None"));
		}
	}
}

// ------------------------
// Input Functions
// ------------------------
void ASH_PlayerController::Move(const FInputActionValue& Value)
{
	if (ASH_PlayerCharacter* PlayerCharacter = Cast<ASH_PlayerCharacter>(GetPawn()))
	{
		const FVector2D& Vector = Value.Get<FVector2D>();
		PlayerCharacter->Move(Vector);
	}
}
void ASH_PlayerController::Look(const FInputActionValue& Value)
{
	if (ASH_PlayerCharacter* PlayerCharacter = Cast<ASH_PlayerCharacter>(GetPawn()))
	{
		const FVector2D& Vector = Value.Get<FVector2D>();
		PlayerCharacter->Look(Vector);
	}
}
void ASH_PlayerController::Jump()
{
	if (ASH_PlayerCharacter* PlayerCharacter = Cast<ASH_PlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->Jump();
	}
}
void ASH_PlayerController::Use()
{
	if (ASH_PlayerCharacter* PlayerCharacter = Cast<ASH_PlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->Use(CurrentActiveItemIndex);
	}
}
void ASH_PlayerController::ChangeActiveItem(const FInputActionValue& Value)
{
	float Axis = Value.Get<float>();
	int32 Delta = Axis > 0 ? 1 : -1;
	
	if (ASH_PlayerCharacter* PlayerCharacter = Cast<ASH_PlayerCharacter>(GetPawn()))
	{
		int32 InvSlotCount = PlayerCharacter->GetInventorySlotCount();
		CurrentActiveItemIndex = (CurrentActiveItemIndex + Delta + InvSlotCount) % InvSlotCount;
		
		PlayerCharacter->ChangeActiveItem(CurrentActiveItemIndex);
		UE_LOG(LogTemp, Log, TEXT("New CurrentActiveItemIndex = %d"), CurrentActiveItemIndex);
	}
	else
	{
		CurrentActiveItemIndex = 0;
		
		PlayerCharacter->ChangeActiveItem(CurrentActiveItemIndex);
		UE_LOG(LogTemp, Error, TEXT("Player character not found! New CurrentActiveItemIndex = %d"), CurrentActiveItemIndex);
	}
}