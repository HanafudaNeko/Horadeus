// Fill out your copyright notice in the Description page of Project Settings.


#include "Horadeus/Characters/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Horadeus/Components/HoradeusAbilitySystemComponent.h"
#include "Horadeus/AttributeSets/BaseAttributeSet.h"
#include "Horadeus/Abilities/BaseGameplayAbility.h"
#include "Horadeus/Components/FlipbookRotatorComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PaperFlipbookComponent.h"

ABaseCharacter::ABaseCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(60.0f, 100.0f);

	// Create a camera boom attached to the root (capsule)
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetUsingAbsoluteRotation(false); // Rotation of the character should not affect rotation of boom
	SpringArmComponent->bDoCollisionTest = true;
	SpringArmComponent->TargetArmLength = 500.0f;
	SpringArmComponent->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	SpringArmComponent->bUsePawnControlRotation = true;

	// Create a camera and attach to boom
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	GetSprite()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetSprite()->SetRelativeScale3D(FVector(7.0f));
	GetSprite()->SetCastShadow(true);

	AbilitySystem = CreateDefaultSubobject<UHoradeusAbilitySystemComponent>(TEXT("AbilitySystem"));
	AbilitySystem->SetIsReplicated(true);
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("Attributes"));

	FlipbookRotator = CreateDefaultSubobject<UFlipbookRotatorComponent>(TEXT("FlipbookRotator"));
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Turn", this, &ABaseCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ABaseCharacter::LookUp);

	if (IsValid(AbilitySystem) && IsValid(InputComponent))
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EAbilityInputID", static_cast<int32>(EAbilityInputID::Confirm), static_cast<int32>(EAbilityInputID::Cancel));
		AbilitySystem->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Server GAS init
	AbilitySystem->InitAbilityActorInfo(this, this);
	GiveDefaultAbilities();

	// If first player, set the tracked component to this pawns camera
	if (GetWorld()->GetFirstPlayerController() == NewController)
	{
		UFlipbookRotatorComponent::SetTrackedComponent(CameraComponent);
	}
}

void ABaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Client GAS init
	AbilitySystem->InitAbilityActorInfo(this, this);
	if (IsValid(AbilitySystem) && IsValid(InputComponent))
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EAbilityInoutID", static_cast<int32>(EAbilityInputID::Confirm), static_cast<int32>(EAbilityInputID::Cancel));
		AbilitySystem->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

float ABaseCharacter::GetHealth() const
{
	return IsValid(AttributeSet) ? AttributeSet->GetHealth() : 0.0f;
}

float ABaseCharacter::GetMaxHealth() const
{
	return IsValid(AttributeSet) ? AttributeSet->GetMaxHealth() : 0.0f;
}

float ABaseCharacter::GetStamina() const
{
	return IsValid(AttributeSet) ? AttributeSet->GetStamina() : 0.0f;
}

void ABaseCharacter::MoveRight(float Val)
{
	FVector Direction = FVector::RightVector.RotateAngleAxis(GetControlRotation().Yaw, FVector::UpVector);
	AddMovementInput(Direction, Val);
}

void ABaseCharacter::MoveForward(float Val)
{
	FVector Direction = FVector::ForwardVector.RotateAngleAxis(GetControlRotation().Yaw, FVector::UpVector);
	AddMovementInput(Direction, Val);
}

void ABaseCharacter::Turn(float Val)
{
	AddControllerYawInput(Val);
}

void ABaseCharacter::LookUp(float Val)
{
	AddControllerPitchInput(Val);
}

void ABaseCharacter::GiveDefaultAbilities()
{
	if (HasAuthority() && IsValid(AbilitySystem))
	{
		for (auto Ability : DefaultAbilities)
		{
			AbilitySystem->GiveAbility(FGameplayAbilitySpec(
				Ability,
				1,
				static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID),
				this
			));
		}
	}
}
