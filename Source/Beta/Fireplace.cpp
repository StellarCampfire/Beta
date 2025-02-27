#include "Fireplace.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFireplace::AFireplace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Создаём корневой компонент
    FireMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FireMesh"));
    RootComponent = FireMesh;

    // Создаём сферу для зоны тепла
    HeatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HeatSphere"));
    HeatSphere->SetupAttachment(RootComponent);
    HeatSphere->SetSphereRadius(HeatRadius);
}

// Called when the game starts or when spawned
void AFireplace::BeginPlay()
{
	Super::BeginPlay();

    // Устанавливаем радиус сферы из переменной (на случай изменения в редакторе)
    HeatSphere->SetSphereRadius(HeatRadius);
}

// Called every frame
void AFireplace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (GEngine->IsEditor()) // Только в редакторе
    {
        DrawDebugSphere(GetWorld(), GetActorLocation(), HeatRadius, 32, FColor::Orange, false, 0.0f, 0, 2.0f);
    }
}

float AFireplace::GetFireplaceRadius() {
    if (!FireMesh || !FireMesh->GetStaticMesh())
    {
        return 50.0f; // Меш не задан применяем дефолтное значение
    }

    FVector MeshExtent = FireMesh->Bounds.BoxExtent;
    return MeshExtent.X > MeshExtent.Y ? MeshExtent.X : MeshExtent.Y;
}
