// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlipbookRotator.generated.h"


// Forward Declarations
class UCameraComponent;
class ACameraActor;
class UPaperFlipbookComponent;


UENUM()
enum FlipbookRotationMode
{
	BillboardWithEase	UMETA(DisplayName = "Billboard with Ease"),
	Billboard			UMETA(DisplayName = "Y Billboard"),
	Snap				UMETA(DisplayName = "Snap"),
	SnapWithEase		UMETA(DisplayName = "Snap with Ease"),
	NegatedBillboard	UMETA(DisplayName = "Negated Y Billboard"),
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORADEUS_API UFlipbookRotator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFlipbookRotator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetActiveCameraComponent(ACameraActor* NewActiveCameraComponent);

		
private:
	// The camera that all SpriteRotator components will use to calculate rotation
	UPROPERTY(EditAnywhere)
		ACameraActor* ActiveCameraComponent;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<FlipbookRotationMode> RotationMode = BillboardWithEase;

	UPaperFlipbookComponent* MyFlipbook = nullptr;

	float TrueDegrees = 0.0f;


	float GetRotationDegrees();
	void AttemptToSetActiveCameraComponent();
};
