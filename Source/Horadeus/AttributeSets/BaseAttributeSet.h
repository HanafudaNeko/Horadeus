// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
     GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
     GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
     GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
     GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class HORADEUS_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UBaseAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;


	// Attributes

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHealth)


	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Stamnia)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Stamina)


	// OnReps

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Health, OldHealth); };

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxHealth, OldMaxHealth); };

	UFUNCTION()
	virtual void OnRep_Stamnia(const FGameplayAttributeData& OldStamina)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Stamina, OldStamina); }
};
