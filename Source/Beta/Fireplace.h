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

	// Радиус действия тепла
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fireplace")
	float HeatRadius = 500.0f;

	// Температура пламени
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fireplace")
	float HeatAmount = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fireplace")
	float AlphaCf = 0.07875f; // Коэффициент затухания 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float GetFireplaceRadius();

	// Компонент для визуального отображения костра
	UPROPERTY(VisibleAnywhere, Category = "Fireplace")
	UStaticMeshComponent* FireMesh;

	// Компонент для зоны тепла
	UPROPERTY(VisibleAnywhere, Category = "Fireplace")
	USphereComponent* HeatSphere;
};
