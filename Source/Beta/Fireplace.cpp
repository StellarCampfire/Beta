#include "Fireplace.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFireplace::AFireplace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create the root component
    FireMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FireMesh"));
    RootComponent = FireMesh;

    // Create a sphere for the heat zone
    HeatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HeatSphere"));
    HeatSphere->SetupAttachment(RootComponent);
    HeatSphere->SetSphereRadius(HeatRadius);
}

void AFireplace::BeginPlay()
{
	Super::BeginPlay();
    HeatSphere->SetSphereRadius(HeatRadius);
}

void AFireplace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // Display the heat sphere for ease of development
    if (GEngine->IsEditor())
    {
        DrawDebugSphere(GetWorld(), GetActorLocation(), HeatRadius, 32, FColor::Orange, false, 0.0f, 0, 2.0f);
    }
}

float AFireplace::GetFireplaceRadius() {
    if (!FireMesh || !FireMesh->GetStaticMesh())
    {
        return 50.0f;
    }

    FVector MeshExtent = FireMesh->Bounds.BoxExtent;
    return MeshExtent.X > MeshExtent.Y ? MeshExtent.X : MeshExtent.Y;
}
