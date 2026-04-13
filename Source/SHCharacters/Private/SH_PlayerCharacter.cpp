// Copyright Lightline Studios. All rights reserved 

#include "SH_PlayerCharacter.h"

#include "BaseClasses/SH_ItemBaseBehavior.h"
#include "BaseClasses/SH_ItemPDA.h"
#include "Behaviors/Tools/SH_ToolBaseBehaviour.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SHComponents/Public/SH_InventoryComponent.h"
#include "SHItems/Public/BaseClasses/SH_ItemVisual.h"
#include "Structs/SH_ItemUseContext.h"


ASH_PlayerCharacter::ASH_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera->SetRelativeLocation(FVector(0, 0, 80));
	FirstPersonCamera->FieldOfView = 85.f;
	FirstPersonCamera->bUsePawnControlRotation = true;
	FirstPersonCamera->Activate();

	InventoryComponent = CreateDefaultSubobject<USH_InventoryComponent>(TEXT("InventoryComponent"));
	
	ItemInHandHolder = CreateDefaultSubobject<USceneComponent>(TEXT("ItemHolder"));
	ItemInHandHolder->SetupAttachment(FirstPersonCamera);
	ItemInHandHolder->SetRelativeLocation(FVector(50, 30, -45));

	CurrentActiveItemIndex = 0;
}

void ASH_PlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Не в конструкторе - потому что иначе изменения в Blueprint будут игнорироваться
	if (InventoryComponent)
	{
		InventoryComponent->Init(InventorySlotCount);
		InventoryComponent->OnInventoryChanged.AddUObject(this, &ASH_PlayerCharacter::InventoryComponentUpdated);
	}
}

void ASH_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	CreateActiveItemSpot();
}

void ASH_PlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (ItemInHandBehavior && ItemInHandBehavior->IsA(USH_ToolBaseBehaviour::StaticClass()))
	{
		FSH_ItemUseContext Context;
		Context.Instigator = this;
		Context.ViewLocation = FirstPersonCamera->GetComponentLocation();
		Context.ViewDirection = FirstPersonCamera->GetForwardVector();
		Context.InteractionDistance = InteractionDistance;
		
		ItemInHandBehavior->Tick(Context);
	}
}

void ASH_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASH_PlayerCharacter::Move(const FVector2D& InVector)
{
	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(ForwardDirection, InVector.Y);
		AddMovementInput(RightDirection, InVector.X);
	}
}

void ASH_PlayerCharacter::Look(const FVector2D& InVector)
{
	if (Controller)
	{
		AddControllerYawInput(InVector.X);
		AddControllerPitchInput(InVector.Y);
	}
}

void ASH_PlayerCharacter::Jump()
{
	Super::Jump();
}

void ASH_PlayerCharacter::Use(int32 SlotIndex)
{
	FInventorySlot* ActiveItem = InventoryComponent->GetSlot(SlotIndex);
	if (!ActiveItem || !ActiveItem->Item) return;
	if (ItemInHandBehavior)
	{
		FSH_ItemUseContext Context;
		Context.Instigator = this;
		Context.ViewLocation = FirstPersonCamera->GetComponentLocation();
		Context.ViewDirection = FirstPersonCamera->GetForwardVector();
		Context.InteractionDistance = InteractionDistance;
		
		ItemInHandBehavior->Use(Context);
	}
}

void ASH_PlayerCharacter::InventoryComponentUpdated(int32 UpdatedIndex)
{
	if (CurrentActiveItemIndex == UpdatedIndex)
	{
		ChangeActiveItem(UpdatedIndex);
	}
}

TSubclassOf<USH_ItemBaseBehavior> ASH_PlayerCharacter::GetActiveItemClass() const
{
	FInventorySlot* F = InventoryComponent->GetSlot(CurrentActiveItemIndex);
	if (!F || !F->Item) return nullptr;
	return F->Item->ItemBehaviorClass;
}

void ASH_PlayerCharacter::SetCurrentActiveItemIndex(int32 InNewIndex)
{
	CurrentActiveItemIndex = InNewIndex;
}

void ASH_PlayerCharacter::CreateActiveItemSpot()
{	
	FActorSpawnParameters Params;
	Params.Owner = this;

	ItemInHand = GetWorld()->SpawnActor<ASH_ItemVisual>(ASH_ItemVisual::StaticClass(), Params);
	if (ItemInHand)
	{
		ItemInHand->AttachToComponent(ItemInHandHolder, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}

	// Для мгновенного обновления, если уже что-то в слоте есть
	ChangeActiveItem(CurrentActiveItemIndex);
}

void ASH_PlayerCharacter::ChangeActiveItem(int32 NewActiveItemIndex)
{
	if (IsValid(ItemInHandBehavior))
	{
		ItemInHandBehavior->OnUnEquipped();
		ItemInHandBehavior = nullptr;
	}
	
	FInventorySlot* NewCurrentSlot = InventoryComponent->GetSlot(NewActiveItemIndex);

	// Обработка визуала
	UStaticMesh* NewMesh = nullptr;

	if (NewCurrentSlot && NewCurrentSlot->Item)
	{
		NewMesh = NewCurrentSlot->Item->StaticMesh;
	}

	if (ItemInHand)
	{
		ItemInHand->SetStaticMesh(NewMesh);
		ItemInHand->SetActorHiddenInGame(NewMesh == nullptr);
	}
	
	if (!NewCurrentSlot || !NewCurrentSlot->Item) return;


	// Behavior-часть
	if (!NewCurrentSlot->Item->ItemBehaviorClass) return;

	ItemInHandBehavior = NewObject<USH_ItemBaseBehavior>(
		this,
		NewCurrentSlot->Item->ItemBehaviorClass
	);
	
	ItemInHandBehavior->Init(this, NewCurrentSlot->Item);
	ItemInHandBehavior->OnEquipped();
}
