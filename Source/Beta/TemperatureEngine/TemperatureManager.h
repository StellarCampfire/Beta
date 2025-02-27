﻿#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fireplace.h"
#include "TemperatureZone.h"
#include "TemperatureManager.generated.h"

UCLASS()
class BETA_API ATemperatureManager : public AActor
{
    GENERATED_BODY()

public:
    ATemperatureManager();

    // World base temperature
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Temperature")
    float BaseTemperature = -10.0f;

    // Get temperature at a given point
    UFUNCTION(BlueprintCallable, Category = "Temperature")
    float GetTemperatureAtLocation(FVector Location);

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual float GetTemperatureNearFire(AFireplace* fireplace, float DistanceToFire, float EnvironmentTemp);
};