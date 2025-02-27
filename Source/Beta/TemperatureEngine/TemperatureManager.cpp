#include "TemperatureManager.h"
#include "Kismet/GameplayStatics.h"
#include "Fireplace.h"
#include <cmath>
#include <numbers>

ATemperatureManager::ATemperatureManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATemperatureManager::BeginPlay()
{
	Super::BeginPlay();	
}

void ATemperatureManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ATemperatureManager::GetTemperatureAtLocation(FVector Location)
{
    //We do not take into account the height
    Location.Z = 0.0f;

    float EnvironmentTemperature = BaseTemperature;
    float TotalWeight = 0.0f;
    float WeightedTemperatureSum = 0.0f;

    TArray<AActor*> TemperatureZones;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATemperatureZone::StaticClass(), TemperatureZones);

    // Check all temperature zones
    for (AActor* Actor : TemperatureZones)
    {
        ATemperatureZone* Zone = Cast<ATemperatureZone>(Actor);
        if (!Zone)
        {
            continue;
        }

        float Distance = FVector::Dist(Location, Zone->GetActorLocation());
        if (Distance <= Zone->Radius)
        {
            float Weight = (Zone->Radius - Distance) / Zone->Radius;

            WeightedTemperatureSum += Zone->Temperature * Weight;
            TotalWeight += Weight;

            //// If the zone is hot (temperature is higher than current), increase the priority
            //if (Zone->Temperature > EnvironmentTemperature)
            //{
            //    EnvironmentTemperature = Zone->Temperature; // Hot zone overlaps base temp
            //}
            //// If the zone is cold (temperature is lower than the current one), it has the highest priority among the zones
            //else if (Zone->Temperature < EnvironmentTemperature)
            //{
            //    EnvironmentTemperature = Zone->Temperature; // Cold zone blocks everything except fires
            //}
        }
        if (TotalWeight > 0.0f)
        {
            EnvironmentTemperature = WeightedTemperatureSum / TotalWeight;
        }
    }

    // Check for fire influence (highest priority)
    TArray<AActor*> Fireplaces;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFireplace::StaticClass(), Fireplaces);

    float TotalFireWeight = 0.0f;
    float WeightedFireTempSum = 0.0f;

    for (AActor* Actor : Fireplaces)
    {
        AFireplace* Fireplace = Cast<AFireplace>(Actor);
        if (Fireplace)
        {
            float DistanceToFire = FVector::Dist(Location, Fireplace->GetActorLocation());
            if (DistanceToFire <= Fireplace->HeatRadius)
            {
                float Weight = (Fireplace->HeatRadius - DistanceToFire) / Fireplace->HeatRadius;
                float TempNearCampfire = GetTemperatureNearFire(Fireplace, DistanceToFire, EnvironmentTemperature);

                WeightedFireTempSum += TempNearCampfire * Weight;
                TotalFireWeight += Weight;
            }
        }
    }

    if (TotalFireWeight > 0.0f)
    {
        float MixedFireTemperature = WeightedFireTempSum / TotalFireWeight;
        EnvironmentTemperature = MixedFireTemperature;
    }

    return EnvironmentTemperature;
}

float ATemperatureManager::GetTemperatureNearFire(AFireplace* fireplace, float DistanceToFire, float EnvironmentTemp) {
    const float FlameTemp = fireplace->HeatAmount;
    const float FlameRadius = 10.0f;
    const float FireplaceRadius = fireplace->GetFireplaceRadius();
    const float HeatRadius = fireplace->HeatRadius;

    if (DistanceToFire <= FlameRadius)
    {
        // Inside the flame is the maximum temperature
        return FlameTemp;
    }
    else if (DistanceToFire <= FireplaceRadius)
    {
        // Exponential decrease in temperature from the flame to the edge of the fire.
        return FlameTemp * std::exp(-fireplace->AlphaCf * (DistanceToFire - FlameRadius));
    }
    else if (DistanceToFire <= HeatRadius)
    {
        // The temperature at the edge of the fire is automatically calculated as:
        float T_fireEdge = FlameTemp * std::exp(-fireplace->AlphaCf * (FireplaceRadius - FlameRadius));
        // Then we perform a smooth (cosine) interpolation from T_fireEdge to the ambient temperature.
        float t = (DistanceToFire - FireplaceRadius) / (HeatRadius - FireplaceRadius);
        return EnvironmentTemp + (T_fireEdge - EnvironmentTemp) * 0.5f * (1.0f + std::cos(std::numbers::pi_v<float> * t));
    }
    else
    {
        return EnvironmentTemp;
    }
}