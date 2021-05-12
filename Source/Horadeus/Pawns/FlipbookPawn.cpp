// Fill out your copyright notice in the Description page of Project Settings.


#include "FlipbookPawn.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/ArrowComponent.h"
#include "../Components/FlipbookRotatorComponent.h"


// Sets default values
AFlipbookPawn::AFlipbookPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent")); 
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	CapsuleComponent->SetCapsuleHalfHeight(100.0f);
	CapsuleComponent->SetCapsuleRadius(60.0f);
	RootComponent = CapsuleComponent;

	PaperFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("PaperFlipbook"));
	PaperFlipbook->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	PaperFlipbook->SetRelativeScale3D(FVector(7.0f));
	PaperFlipbook->SetCastShadow(true);
	PaperFlipbook->SetupAttachment(RootComponent);

	FacingArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("FacingArrow"));
	FacingArrow->SetArrowColor(FColor::Purple);
	FacingArrow->SetupAttachment(RootComponent);

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

}

// Called when the game starts or when spawned
void AFlipbookPawn::BeginPlay()
{
	Super::BeginPlay();

}