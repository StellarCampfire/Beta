#include "TemperatureManager.h"
#include "Kismet/GameplayStatics.h"
#include "Fireplace.h"
#include <cmath>
#include <numbers>

// Sets default values
ATemperatureManager::ATemperatureManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATemperatureManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATemperatureManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ATemperatureManager::GetTemperatureAtLocation(FVector Location)
{
    Location.Z = 0.0f; //Не учитываем высоту.

    float EnvironmentTemperature = BaseTemperature; // Начинаем с базовой температуры (минимальный приоритет)
    TArray<AActor*> TemperatureZones;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATemperatureZone::StaticClass(), TemperatureZones);

    // Проверяем все зоны температуры
    for (AActor* Actor : TemperatureZones)
    {
        ATemperatureZone* Zone = Cast<ATemperatureZone>(Actor);

        float Distance = FVector::Dist(Location, Zone->GetActorLocation());
        if (Distance <= Zone->Radius)
        {
            // Если зона горячая (температура выше текущей), повышаем приоритет
            if (Zone->Temperature > EnvironmentTemperature)
            {
                EnvironmentTemperature = Zone->Temperature; // Горячая зона перекрывает базовую
            }
            // Если зона холодная (температура ниже текущей), она имеет максимальный приоритет среди зон
            else if (Zone->Temperature < EnvironmentTemperature)
            {
                EnvironmentTemperature = Zone->Temperature; // Холодная зона перекрывает всё, кроме костров
            }
        }
    }

    // Проверяем влияние костров (наивысший приоритет)
    TArray<AActor*> Fireplaces;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFireplace::StaticClass(), Fireplaces);

    for (AActor* Actor : Fireplaces)
    {
        AFireplace* Fireplace = Cast<AFireplace>(Actor);
        if (Fireplace)
        {
            float DistanceToFire = FVector::Dist(Location, Fireplace->GetActorLocation());
            if (DistanceToFire <= Fireplace->HeatRadius)
            {
                float tempNearCampfire = GetTemperatureNearFire(Fireplace, DistanceToFire, EnvironmentTemperature);
                if (tempNearCampfire > EnvironmentTemperature) 
                {
                    EnvironmentTemperature = tempNearCampfire;
                }
                
                break; // Прерываем, так как костёр имеет максимальный приоритет
            }
        }
    }

    return EnvironmentTemperature; // Возвращаем целевую температуру
}

float ATemperatureManager::GetTemperatureNearFire(AFireplace* fireplace, float DistanceToFire, float EnvironmentTemp) {
    const float FlameTemp = fireplace->HeatAmount; // температура пламени (700°C)
    const float FlameRadius = 10.0f;               // радиус пламени
    const float FireRadius = fireplace->GetFireplaceRadius();  // радиус костра
    const float HeatRadius = fireplace->HeatRadius; // радиус распространения тепла (500)

    if (DistanceToFire <= FlameRadius)
    {
        // Внутри пламени — максимальная температура
        return FlameTemp;
    }
    else if (DistanceToFire <= FireRadius)
    {
        // Экспоненциальное спадание температуры от пламени до края костра.
        return FlameTemp * std::exp(-fireplace->AlphaCf * (DistanceToFire - FlameRadius));
    }
    else if (DistanceToFire <= HeatRadius)
    {
        // Температура на краю костра автоматически вычисляется как:
        float T_fireEdge = FlameTemp * std::exp(-fireplace->AlphaCf * (FireRadius - FlameRadius));
        // Затем выполняем плавную (косинусную) интерполяцию от T_fireEdge до температуры окружающей среды.
        float t = (DistanceToFire - FireRadius) / (HeatRadius - FireRadius);
        return EnvironmentTemp + (T_fireEdge - EnvironmentTemp) * 0.5f * (1.0f + std::cos(std::numbers::pi_v<float> * t));
    }
    else
    {
        // За пределами области тепла — температура окружающей среды.
        return EnvironmentTemp;
    }
}