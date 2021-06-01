// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class HORADEUS_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:

	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	FGameplayAttributeData Health;
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Health)
	GAMEPLAYATTRIBUTE_VALUE_SETTER(Health)
	GAMEPLAYATTRIBUTE_VALUE_INITTER(Health)
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UBaseAttributeSet, Health)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	FGameplayAttributeData MaxHealth;
	GAMEPLAYATTRIBUTE_VALUE_GETTER(MaxHealth)
	GAMEPLAYATTRIBUTE_VALUE_SETTER(MaxHealth)
	GAMEPLAYATTRIBUTE_VALUE_INITTER(MaxHealth)
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UBaseAttributeSet, MaxHealth)
};
