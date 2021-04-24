// Fill out your copyright notice in the Description page of Project Settings.


#include "FlipbookRotatorComponent.h"
#include "PaperFlipbookComponent.h"

#define FLIPBOOK_Z_ROTATION_OFFSET -90.0f


UPrimitiveComponent* UFlipbookRotatorComponent::TrackedComponent = nullptr;


// Sets default values for this component's properties
UFlipbookRotatorComponent::UFlipbookRotatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called every frame
void UFlipbookRotatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	DegreesBetweenActiveCameraAndFlipbook = CalculateDegreesBetweenActiveCameraAndFlipbook();
	MyFlipbook->SetRelativeRotation(FRotator(0.0f, TransformDegreesToRotatorModeSpace(DegreesBetweenActiveCameraAndFlipbook) + FLIPBOOK_Z_ROTATION_OFFSET, 0.0f));
}

void UFlipbookRotatorComponent::SetTrackedComponent(UPrimitiveComponent* NewTrackedComponent)
{
	UFlipbookRotatorComponent::TrackedComponent = NewTrackedComponent;
}

float UFlipbookRotatorComponent::GetDegreesBetweenActiveCameraAndFlipbook() const
{
	return DegreesBetweenActiveCameraAndFlipbook;
}

// Called when the game starts
void UFlipbookRotatorComponent::BeginPlay()
{
	Super::BeginPlay();
	SetFlipbook();
}

float UFlipbookRotatorComponent::CalculateDegreesBetweenActiveCameraAndFlipbook() const
{
	if (TrackedComponent == nullptr)
	{
		return 0.0f;
	}

	FVector CameraWorldPosition = TrackedComponent->GetOwner()->GetActorLocation();
	FVector OwnerWorldPosition = GetOwner()->GetActorLocation();

	FVector CameraToOwnerLocal = GetOwner()->GetActorTransform().InverseTransformVector(CameraWorldPosition - OwnerWorldPosition);
	CameraToOwnerLocal.Z = 0.0f;
	CameraToOwnerLocal.Normalize();

	float Degrees = FMath::Acos(FVector::DotProduct(FVector::ForwardVector, CameraToOwnerLocal)) * (CameraToOwnerLocal.Y < 0.0f ? -1.0f : 1.0f);
	return FMath::RadiansToDegrees(Degrees);
}

float UFlipbookRotatorComponent::TransformDegreesToRotatorModeSpace(float Degrees) const
{
	switch (RotatorMode)
	{
	case BillboardWithEase:
	{
		float StartEaseAt = 30.0f;
		float ClampedAngle90 = 90.0f * FMath::FloorToFloat((Degrees + 45.0f) / 90.0f);
		float DegreesFromClampedValue = ClampedAngle90 - Degrees;
		float EaseStrength = (DegreesFromClampedValue / (45.0f - StartEaseAt)) - (StartEaseAt / (45.0f - StartEaseAt)) * (DegreesFromClampedValue >= 0.0f ? 1.0f : -1.0f);
		return FMath::Abs(DegreesFromClampedValue) <= StartEaseAt ? ClampedAngle90 : ClampedAngle90 - (45.0f * EaseStrength);
	}
	case Billboard:
		return Degrees;
	case Snap:
		return 90.0f * FMath::FloorToFloat((Degrees + 45.0f) / 90.0f);
	case SnapWithEase:
		return (Degrees + (90.0f * FMath::FloorToFloat((Degrees + 45.0f) / 90.0f))) / 2.0f;
	case NegatedBillboard:
		return -Degrees;
	default:
		return Degrees;
	}
}

void UFlipbookRotatorComponent::SetFlipbook()
{
	MyFlipbook = GetOwner()->FindComponentByClass<UPaperFlipbookComponent>();

	if (MyFlipbook == nullptr)
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("%s has no PaperFlipbookComponent; attached FlipbookRotatorComponent needs a Flipbook to function properly."),
			*GetOwner()->GetName()
		);
	}
}