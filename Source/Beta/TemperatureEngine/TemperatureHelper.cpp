#include "TemperatureHelper.h"
#include "CoreMinimal.h"
#include "Math/UnrealMathUtility.h"

FColor TemperatureHelper::GetTemperatureColor(float Temp)
{
    // Limit the temperature to a range from -100 to 100
    Temp = FMath::Clamp(Temp, -100.f, 100.f);

    // Normalize temperature: -100 -> 0.0, 100 -> 1.0
    float t = (Temp + 100.f) / 200.f;

    // Interpolate between blue (0,0,255) and red (255,0,0)
    uint8 Red = FMath::Lerp(0, 255, t);
    uint8 Green = 0;
    uint8 Blue = FMath::Lerp(255, 0, t);

    return FColor(Red, Green, Blue, 255);
}
