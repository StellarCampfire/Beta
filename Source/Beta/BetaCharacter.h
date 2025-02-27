#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TemperatureManager.h"
#include "BetaCharacter.generated.h"

UCLASS(Blueprintable)
class ABetaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABetaCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	// Called when the game starts or when spawned
    virtual void BeginPlay() override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetHunger() const { return Hunger; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetThirst() const { return Thirst; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetSanity() const { return Sanity; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetPerceivedTemperature() const { return PerceivedTemperature; }

	// Переменные для статистики
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float Hunger = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float Thirst = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float Sanity = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "-50.0", ClampMax = "50.0"))
	float PerceivedTemperature = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Temperature")
	ATemperatureManager* TemperatureManager;

	// Переменные для интерфейса
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> StatsHUDClass; // Класс виджета

	UPROPERTY(Transient)
	class UUserWidget* StatsHUDInstance; // Экземпляр виджета

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
};

