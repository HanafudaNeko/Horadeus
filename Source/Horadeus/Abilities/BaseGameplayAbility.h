// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Horadeus/Horadeus.h"
#include "Abilities/GameplayAbility.h"
#include "BaseGameplayAbility.generated.h"


UCLASS()
class HORADEUS_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	// Abilities with this set will automatically activate when input is pressed
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EAbilityInputID AbilityInputID = EAbilityInputID::None;
	
};
