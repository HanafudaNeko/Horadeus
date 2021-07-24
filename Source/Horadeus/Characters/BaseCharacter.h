// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "AbilitySystemInterface.h"
#include <GameplayEffectTypes.h>
#include "BaseCharacter.generated.h"


UCLASS()
class HORADEUS_API ABaseCharacter : public APaperCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ABaseCharacter();
	
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	// signed by JGS_Discord
	
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystem; };
	class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	class USpringArmComponent* GetSpringArmComponent() const { return SpringArmComponent; }

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetMaxHealth() const;
	
	// Initualize Set Max Health 
	UFUNCTION(BlueprintPure, Category = "Attributes")
	float SetMaxHealth() const;
	// Signed by JGS_Discord

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetStamina() const;

protected:
	void MoveRight(float Val);
	void MoveForward(float Val);
	void Turn(float Val);
	void LookUp(float Val);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paper2D", meta = (AllowPrivateAccess = "true"))
	class UFlipbookRotatorComponent* FlipbookRotator = nullptr;

	// Ability system
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystem = nullptr;
	UPROPERTY()
	class UBaseAttributeSet* AttributeSet = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class UBaseGameplayAbility>> DefaultAbilities;

	void GiveDefaultAbilities();
};
