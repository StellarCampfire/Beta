#include "BetaCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


ABetaCharacter::ABetaCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ABetaCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!TemperatureManager)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATemperatureManager::StaticClass(), FoundActors);
		if (FoundActors.Num() > 0)
		{
			TemperatureManager = Cast<ATemperatureManager>(FoundActors[0]);
			UE_LOG(LogTemp, Log, TEXT("Automatically found TemperatureManager: %s"), *TemperatureManager->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No TemperatureManager found on level!"));
		}
	}

	// Create and add a widget to the screen
	if (StatsHUDClass && IsLocallyControlled())
	{
		StatsHUDInstance = CreateWidget<UUserWidget>(GetWorld(), StatsHUDClass);
		if (StatsHUDInstance)
		{
			StatsHUDInstance->AddToViewport();
		}
	}
}

void ABetaCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Player stats
	Hunger -= 0.1f * DeltaSeconds;
	Thirst -= 0.15f * DeltaSeconds;
	Sanity -= 0.05f * DeltaSeconds;

	float EnvironmentalTemperature;
	// Get the environment temperature
	if (TemperatureManager)
	{
		EnvironmentalTemperature = TemperatureManager->GetTemperatureAtLocation(GetActorLocation());
		// Smoothly change Perceived Temperature to ambient temperature
		PerceivedTemperature = FMath::FInterpTo(PerceivedTemperature, EnvironmentalTemperature, DeltaSeconds,0.02f);

		// Limit the Perceived Temperature so that it does not go beyond the Environmental Temperature
		if (EnvironmentalTemperature > PerceivedTemperature)
		{
			PerceivedTemperature = FMath::Min(PerceivedTemperature, EnvironmentalTemperature);
		}
		else
		{
			PerceivedTemperature = FMath::Max(PerceivedTemperature, EnvironmentalTemperature);
		}
	}
	else
	{
		// If the temperature manager is not set, the temperature drops, but not below -50
		PerceivedTemperature -= 0.5f * DeltaSeconds;
		PerceivedTemperature = FMath::Max(PerceivedTemperature, -50.0f);
	}

	// Debug message
	GEngine->AddOnScreenDebugMessage(4, 0.0f, FColor::Yellow, FString::Printf(TEXT("EnviropmentTemperature: %.1f"), EnvironmentalTemperature));
}
