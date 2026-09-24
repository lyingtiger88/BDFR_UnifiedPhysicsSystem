#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "BDFRPhysicsRouter.h"
#include "BDFRPhysicsWorldSubsystem.generated.h"

/**
 * Creates one BDFR physics router per Unreal world and advances the selected
 * backend using either frame delta or a fixed simulation timestep.
 */
UCLASS()
class BDFRPHYSICSCORE_API UBDFRPhysicsWorldSubsystem : public UTickableWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    virtual void Tick(float DeltaTime) override;
    virtual TStatId GetStatId() const override;
    virtual bool IsTickableInEditor() const override { return false; }

    IBDFRPhysicsBackend* GetActiveBackend() const
    {
        return Router ? Router->GetActiveBackend() : nullptr;
    }

    UFUNCTION(BlueprintPure, Category = "BDFR Physics")
    EBDFRPhysicsBackendType GetActiveBackendType() const
    {
        return Router ? Router->GetActiveBackendType() : EBDFRPhysicsBackendType::None;
    }

private:
    TUniquePtr<FBDFRPhysicsRouter> Router;
    double FixedStepAccumulator = 0.0;
};
