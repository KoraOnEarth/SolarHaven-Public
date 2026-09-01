// LightLine Studios. All rights reserved.


#include "SH_PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <SH_InventoryComponent.h>

#include "Interfaces/SH_PlantableInterface.h"

#include "Items/SH_ItemInHandVisual.h"
#include "Items/Behaviors/SH_BuildCertificateBehavior.h"
#include "Items/PDAs/SH_CropItemPDA.h"
#include "Items/PDAs/SH_ItemPDA.h"
#include "Items/PDAs/SH_SeedItemPDA.h"
#include "Items/Structs/SH_ItemUseContext.h"


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
	
	InventoryComponent = CreateDefaultSubobject<USH_InventoryComponent>("InventoryComponent");
	
	ItemInHandSceneComponent = CreateDefaultSubobject<USceneComponent>("ItemInHandSceneComponent");
	ItemInHandSceneComponent->SetupAttachment(FirstPersonCamera);
	ItemInHandSceneComponent->SetRelativeLocation(FVector(50, 30, -45));
}

void ASH_PlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	InventoryComponent->Init(InventorySlotsCount);
}
void ASH_PlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (ItemInHandBehavior)
	{
		FSH_ItemUseContext Context;
		Context.Instigator = this;
		Context.ViewLocation = FirstPersonCamera->GetComponentLocation();
		Context.ViewDirection = FirstPersonCamera->GetForwardVector();
		Context.InteractionDistance = InteractionDistance;
		
		ItemInHandBehavior->Tick(Context);
	}
}

void ASH_PlayerCharacter::EquipItem(USH_ItemPDA* ItemPDA)
{
	ItemInHandBehavior->Init(this, ItemPDA);
	ItemInHandBehavior->OnEquipped();
}
void ASH_PlayerCharacter::UnEquipItem()
{
	if (IsValid(ItemInHandBehavior))
	{
		ItemInHandBehavior->OnUnEquipped();
		ItemInHandBehavior = nullptr;
	}
	if (IsValid(ItemInHand))
	{
		ItemInHand->SetMesh(nullptr);
		ItemInHand->SetActorHiddenInGame(true);
	}
}
void ASH_PlayerCharacter::UpdateInHandVisuals(const FInventorySlot* NewCurrentSlot)
{
	UStaticMesh* StaticMesh = NewCurrentSlot->Item->StaticMesh;
	if (ItemInHand)
	{
		ItemInHand->SetMesh(StaticMesh);
		ItemInHand->SetActorHiddenInGame(StaticMesh == nullptr);
	}
}

void ASH_PlayerCharacter::CreateActiveItemSpot()
{
	FActorSpawnParameters Params;
	Params.Owner = this;
	
	if (!GetWorld()) return;
	
	ItemInHand = GetWorld()->SpawnActor<ASH_ItemInHandVisual>(ASH_ItemInHandVisual::StaticClass(), Params);
	if (ItemInHand) { ItemInHand->AttachToComponent(ItemInHandSceneComponent, FAttachmentTransformRules::SnapToTargetIncludingScale); }
}
void ASH_PlayerCharacter::ChangeActiveItem(int32 NewActiveItemIndex)
{
	FInventorySlot* NewCurrentSlot = InventoryComponent->GetSlot(NewActiveItemIndex);
	if (!NewCurrentSlot) return;
	if (NewCurrentSlot->Item && NewCurrentSlot->Item->ItemBehaviorClass)
	{
		// If there is no behavior class at all.
		if (!IsValid(ItemInHandBehavior))
		{
			ItemInHandBehavior = NewObject<USH_BaseItemBehavior>(this, NewCurrentSlot->Item->ItemBehaviorClass);
			EquipItem(NewCurrentSlot->Item);
		}
		// If behavior classes are not equal
		else if (ItemInHandBehavior->GetClass() != NewCurrentSlot->Item->ItemBehaviorClass)
		{
			UnEquipItem();
			ItemInHandBehavior = NewObject<USH_BaseItemBehavior>(this, NewCurrentSlot->Item->ItemBehaviorClass);
			EquipItem(NewCurrentSlot->Item);
		}
		// If behavior classes are equal (useful for seeds of building certificates
		else if (ItemInHandBehavior->GetItemData() != NewCurrentSlot->Item)
		{
			ItemInHandBehavior->OnUnEquipped();
			ItemInHandBehavior->UpdateItemData(this, NewCurrentSlot->Item);
			ItemInHandBehavior->OnEquipped();
			ItemInHand->SetMesh(NewCurrentSlot->Item->StaticMesh);
		}
		
		UpdateInHandVisuals(NewCurrentSlot);
		return;
	}
	
	// If new slot does not contain any data about Item
	UnEquipItem();
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
void ASH_PlayerCharacter::Use(int32 CurrentItemIndex)
{
	if (!ItemInHandBehavior) return;
	
	FSH_ItemUseContext Context;
	Context.Instigator = this;
	Context.ViewLocation = FirstPersonCamera->GetComponentLocation();
	Context.ViewDirection = FirstPersonCamera->GetForwardVector();
	Context.InteractionDistance = InteractionDistance;
	const bool UseResult = ItemInHandBehavior->Use(Context);
	// If the player is holding a seed, remove it when the seed is planted.
	if (UseResult && ItemInHandBehavior->IsA<USH_BaseSeedBehavior>()) InventoryComponent->RemoveItemAtSlot(CurrentItemIndex);
}
void ASH_PlayerCharacter::Interact()
{
	UWorld* World = GetWorld();
	if (!World) return;
	
	FVector Start = FirstPersonCamera->GetComponentLocation();
	FVector End = Start + FirstPersonCamera->GetForwardVector() * InteractionDistance;
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	// GameTraceChannel2 == TraceCrop
	const bool bHit = World->LineTraceSingleByChannel(Hit, Start, End, ECC_GameTraceChannel2, Params);
	if (!bHit) return;
	
	if (ISH_PlantableInterface* PlantableInterface = Cast<ISH_PlantableInterface>(Hit.GetActor()))
	{
		USH_ItemPDA* PlantedSeedPDA = PlantableInterface->Crop();
		if (!PlantedSeedPDA) return;
		USH_SeedItemPDA* PlantedSeed = CastChecked<USH_SeedItemPDA>(PlantedSeedPDA);
		InventoryComponent->AddItem(PlantedSeed->CropItemPDA, PlantedSeed->CountOfCrops);
	}
}
void ASH_PlayerCharacter::RotateBuildPreview(int32 RotateDirection)
{
	ISH_RotatablePreview* RotatablePreview = Cast<ISH_RotatablePreview>(ItemInHandBehavior);
	if (!RotatablePreview) return;
	RotatablePreview->RotatePreview(RotateDirection);
}
