﻿#include "TemperatureZone.h"
#include "TemperatureHelper.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"
#include "TemperatureHelper.h"


ATemperatureZone::ATemperatureZone()
{
	PrimaryActorTick.bCanEverTick = true;

	// Создаём сферу для зоны тепла
	HeatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HeatSphere"));
	RootComponent = HeatSphere;
	HeatSphere->SetSphereRadius(Radius);
}

void ATemperatureZone::BeginPlay()
{ 
	Super::BeginPlay();
}

void ATemperatureZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GEngine->IsEditor()) // Только в редакторе
	{
		DrawDebugSphere(
			GetWorld(),
			GetActorLocation(),
			Radius,
			32,
			TemperatureHelper::GetTemperatureColor(Temperature),
			false,
			0.0f,
			0,
			2.0f);
	}
}

#if WITH_EDITOR
void ATemperatureZone::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Check if the Radius property has changed
	FName PropertyName = PropertyChangedEvent.GetPropertyName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ATemperatureZone, Radius) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(ATemperatureZone, Temperature))
	{
		// Synchronize HeatSphere radius with Radius and Temperature
		if (HeatSphere)
		{
			HeatSphere->SetSphereRadius(Radius);
			HeatSphere->ShapeColor = TemperatureHelper::GetTemperatureColor(Temperature);
		}
	}
}
#endif

