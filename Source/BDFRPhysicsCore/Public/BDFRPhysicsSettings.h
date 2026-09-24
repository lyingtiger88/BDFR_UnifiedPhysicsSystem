#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "BDFRPhysicsTypes.h"
#include "BDFRPhysicsSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "BDFR Unified Physics System"))
class BDFRPHYSICSCORE_API UBDFRPhysicsSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UBDFRPhysicsSettings();

    virtual FName GetCategoryName() const override;

    UPROPERTY(Config, EditAnywhere, Category = "Backend")
    EBDFRPhysicsBackendType DefaultBackend = EBDFRPhysicsBackendType::Auto;

    UPROPERTY(Config, EditAnywhere, Category = "Simulation")
    bool bUseFixedTimestep = true;

    UPROPERTY(Config, EditAnywhere, Category = "Simulation", meta = (ClampMin = "15.0", ClampMax = "480.0", UIMin = "30.0", UIMax = "240.0"))
    float FixedTickRate = 120.0f;

    UPROPERTY(Config, EditAnywhere, Category = "Simulation", meta = (ClampMin = "1", ClampMax = "32"))
    int32 MaxSubsteps = 8;

    UPROPERTY(Config, EditAnywhere, Category = "Simulation")
    bool bEnableCCD = true;

    UPROPERTY(Config, EditAnywhere, Category = "Simulation")
    bool bEnableSleeping = true;

    UPROPERTY(Config, EditAnywhere, Category = "Simulation")
    bool bEnableAsyncSimulation = false;
};
