#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "TemperatureZone.generated.h"

UCLASS()
class BETA_API ATemperatureZone : public AActor
{
	GENERATED_BODY()
	
public:	
	ATemperatureZone();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TemperatureZone")
	float Radius = 1000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TemperatureZone")
	float Temperature = -10.0f;

	// Компонент для зоны тепла
	UPROPERTY(VisibleAnywhere, Category = "TermperatureZone")
	USphereComponent* HeatSphere;

protected:
	virtual void BeginPlay() override;

// The function is needed for visual display of the temperature zone during development
#if WITH_EDITOR
	// Called when properties are changed in the editor
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};


