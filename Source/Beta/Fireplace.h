#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Fireplace.generated.h"

UCLASS()
class BETA_API AFireplace : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireplace();

	// Radius of heat action
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fireplace")
	float HeatRadius = 500.0f;

	// Flame temperature
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fireplace")
	float HeatAmount = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fireplace")
	float AlphaCf = 0.07875f; // Damping coefficient

protected:
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float GetFireplaceRadius();

	// Component for visual display of a fire
	UPROPERTY(VisibleAnywhere, Category = "Fireplace")
	UStaticMeshComponent* FireMesh;

	// Component for heat zone
	UPROPERTY(VisibleAnywhere, Category = "Fireplace")
	USphereComponent* HeatSphere;
};
