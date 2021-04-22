// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlipbookRotatorComponent.generated.h"


// Forward Declarations
class UCameraComponent;
class UPaperFlipbookComponent;


UENUM()
enum FlipbookRotatorMode
{
	BillboardWithEase	UMETA(DisplayName = "Billboard with Ease"),
	Billboard			UMETA(DisplayName = "Y Billboard"),
	Snap				UMETA(DisplayName = "Snap"),
	SnapWithEase		UMETA(DisplayName = "Snap with Ease"),
	NegatedBillboard	UMETA(DisplayName = "Negated Y Billboard"),
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORADEUS_API UFlipbookRotatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFlipbookRotatorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	static void SetActiveCameraComponent(UCameraComponent* NewActiveCameraComponent);

	UFUNCTION(BlueprintCallable)
	float GetDegreesBetweenActiveCameraAndFlipbook() const;

private:
	// The camera that all UFlipbookRotatorComponent will use to calculate rotation
	static UCameraComponent* ActiveCameraComponent;

	float DegreesBetweenActiveCameraAndFlipbook = 0.0f;
	UPaperFlipbookComponent* MyFlipbook = nullptr;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<FlipbookRotatorMode> RotatorMode = BillboardWithEase;

	float CalculateDegreesBetweenActiveCameraAndFlipbook() const;
	float TransformDegreesToRotatorModeSpace(float Degrees) const;
	void SetFlipbook();
};
