// Fill out your copyright notice in the Description page of Project Settings.


#include "FlipbookRotator.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include "PaperFlipbookComponent.h"


#define FLIPBOOK_Z_ROTATION_OFFSET -90.0f;


// Sets default values for this component's properties
UFlipbookRotator::UFlipbookRotator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFlipbookRotator::BeginPlay()
{
	UActorComponent::BeginPlay();

	if (ActiveCameraComponent == nullptr)
	{
		AttemptToSetActiveCameraComponent();
	}

	MyFlipbook = GetOwner()->FindComponentByClass<UPaperFlipbookComponent>();

	if (MyFlipbook == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("FlipbookRotator needs a Flipbook to function properly"));
	}
	
}


// Called every frame
void UFlipbookRotator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	UActorComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FRotator NewRotation = FRotator(0.0f, GetRotationDegrees() - 90.0f, 0.0f);
	MyFlipbook->SetRelativeRotation(NewRotation);
}


void UFlipbookRotator::SetActiveCameraComponent(ACameraActor* NewActiveCameraComponent)
{
	UFlipbookRotator::ActiveCameraComponent = NewActiveCameraComponent;
}


float UFlipbookRotator::GetRotationDegrees()
{
	if (ActiveCameraComponent == nullptr)
	{
		return 0.0f;
	}

	FVector CameraWorldPosition = ActiveCameraComponent->GetActorLocation();
	//FVector CameraWorldPosition = ActiveCameraComponent->GetOwner()->GetActorLocation();
	FVector OwnerWorldPosition = GetOwner()->GetActorLocation();

	FVector CameraToOwnerLocal = GetOwner()->GetActorTransform().InverseTransformVector(CameraWorldPosition - OwnerWorldPosition);
	CameraToOwnerLocal.Z = 0.0f;
	CameraToOwnerLocal.Normalize();

	TrueDegrees = FMath::Acos(FVector::DotProduct(FVector::ForwardVector, CameraToOwnerLocal)) * (CameraToOwnerLocal.Y < 0.0f ? -1.0f : 1.0f);
	TrueDegrees = FMath::RadiansToDegrees(TrueDegrees);

	switch (RotationMode)
	{
	case BillboardWithEase:
	{
		float StartEaseAt = 30.0f;
		float ClampedAngle90 = 90.0f * FMath::FloorToFloat((TrueDegrees + 45.0f) / 90.0f);
		float DegreesFromClampedValue = ClampedAngle90 - TrueDegrees;
		float EaseStrength = (DegreesFromClampedValue / (45.0f - StartEaseAt)) - (StartEaseAt / (45.0f - StartEaseAt)) * (DegreesFromClampedValue >= 0.0f ? 1.0f : -1.0f);
		return FMath::Abs(DegreesFromClampedValue) <= StartEaseAt ? ClampedAngle90 : ClampedAngle90 - (45.0f * EaseStrength);
	}
	case Billboard:
		return TrueDegrees;
	case Snap:
		return 90.0f * FMath::FloorToFloat((TrueDegrees + 45.0f) / 90.0f);
	case SnapWithEase:
		return (TrueDegrees + (90.0f * FMath::FloorToFloat((TrueDegrees + 45.0f) / 90.0f))) / 2.0f;
	case NegatedBillboard:
		return -TrueDegrees;
	default:
		return TrueDegrees;
	}
}


void UFlipbookRotator::AttemptToSetActiveCameraComponent()
{
	if (ActiveCameraComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("SpriteRotator cannot find a camera to use."))
	}
}
