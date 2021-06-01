// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemInterface.h"
#include "FlipbookPawn.generated.h"


// Forward Declarations
class UCapsuleComponent;
class UPaperFlipbookComponent;
class UArrowComponent;
class UAbilitySystemComponent;
class UFlipbookRotatorComponent;
class UBaseAttributeSet;


UCLASS()
class HORADEUS_API AFlipbookPawn : public APawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFlipbookPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystem; };

	UFUNCTION(BlueprintPure, Category = "Abilities|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintPure, Category = "Abilities|Attributes")
	float GetMaxHealth() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UPaperFlipbookComponent* PaperFlipbook = nullptr;
	UPROPERTY()
	UArrowComponent* FacingArrow = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UFlipbookRotatorComponent* FlipbookRotator = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* AbilitySystem = nullptr;
	UPROPERTY()
	const UBaseAttributeSet* AttributeSet = nullptr;
};
