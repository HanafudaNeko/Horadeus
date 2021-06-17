// Fill out your copyright notice in the Description page of Project Settings.


#include "FlipbookPawn.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/ArrowComponent.h"
#include "Horadeus/Components/HoradeusAbilitySystemComponent.h"
#include "Horadeus/AttributeSets/BaseAttributeSet.h"
#include "Horadeus/Abilities/BaseGameplayAbility.h"
#include "Horadeus/Components/FlipbookRotatorComponent.h"


// Sets default values
AFlipbookPawn::AFlipbookPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent")); 
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	CapsuleComponent->InitCapsuleSize(60.0f, 100.0f);
	RootComponent = CapsuleComponent;

	PaperFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("PaperFlipbook"));
	PaperFlipbook->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	PaperFlipbook->SetRelativeScale3D(FVector(7.0f));
	PaperFlipbook->SetCastShadow(true);
	PaperFlipbook->SetupAttachment(RootComponent);

	FacingArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("FacingArrow"));
	FacingArrow->SetArrowColor(FColor::Purple);
	FacingArrow->SetupAttachment(RootComponent);

	AbilitySystem = CreateDefaultSubobject<UHoradeusAbilitySystemComponent>(TEXT("AbilitySystem"));
	AbilitySystem->SetIsReplicated(true);
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("Attributes"));

	FlipbookRotator = CreateDefaultSubobject<UFlipbookRotatorComponent>(TEXT("FlipbookRotator"));
}

// Called every frame
void AFlipbookPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AFlipbookPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (AbilitySystem && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EAbilityInoutID", static_cast<int32>(EAbilityInputID::Confirm), static_cast<int32>(EAbilityInputID::Cancel));
		AbilitySystem->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

void AFlipbookPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Server GAS init
	AbilitySystem->InitAbilityActorInfo(this, this);
	GiveDefaultAbilities();
}

void AFlipbookPawn::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Client GAS init
	AbilitySystem->InitAbilityActorInfo(this, this);
	if (AbilitySystem && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EAbilityInoutID", static_cast<int32>(EAbilityInputID::Confirm), static_cast<int32>(EAbilityInputID::Cancel));
		AbilitySystem->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

UAbilitySystemComponent* AFlipbookPawn::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

float AFlipbookPawn::GetHealth() const
{
	return IsValid(AttributeSet) ? AttributeSet->GetHealth() : 0.0f;
}

float AFlipbookPawn::GetMaxHealth() const
{
	return IsValid(AttributeSet) ? AttributeSet->GetMaxHealth() : 0.0f;
}

float AFlipbookPawn::GetStamina() const
{
	return IsValid(AttributeSet) ? AttributeSet->GetStamina() : 0.0f;
}

// Called when the game starts or when spawned
void AFlipbookPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AFlipbookPawn::GiveDefaultAbilities()
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
