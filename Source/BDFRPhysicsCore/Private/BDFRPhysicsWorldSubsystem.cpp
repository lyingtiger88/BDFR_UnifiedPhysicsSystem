#include "BDFRPhysicsWorldSubsystem.h"

#include "BDFRPhysicsCore.h"
#include "BDFRPhysicsSettings.h"
#include "Stats/Stats.h"

void UBDFRPhysicsWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    const UBDFRPhysicsSettings* Settings = GetDefault<UBDFRPhysicsSettings>();

    Router = MakeUnique<FBDFRPhysicsRouter>();
    FixedStepAccumulator = 0.0;

    if (!Router->Initialize(GetWorld(), Settings->DefaultBackend))
    {
        UE_LOG(LogBDFRPhysics, Error, TEXT("BDFR physics router initialization failed."));
    }
}

void UBDFRPhysicsWorldSubsystem::Deinitialize()
{
    if (Router)
    {
        Router->Shutdown();
        Router.Reset();
    }

    FixedStepAccumulator = 0.0;
    Super::Deinitialize();
}

void UBDFRPhysicsWorldSubsystem::Tick(float DeltaTime)
{
    if (!Router || DeltaTime <= 0.0f)
    {
        return;
    }

    const UBDFRPhysicsSettings* Settings = GetDefault<UBDFRPhysicsSettings>();

    if (!Settings->bUseFixedTimestep)
    {
        Router->Tick(DeltaTime);
        return;
    }

    const double TickRate = FMath::Max(15.0, static_cast<double>(Settings->FixedTickRate));
    const double FixedStep = 1.0 / TickRate;
    const int32 MaxSubsteps = FMath::Max(1, Settings->MaxSubsteps);

    // Avoid an unbounded catch-up spiral after a hitch or breakpoint.
    FixedStepAccumulator += FMath::Min(static_cast<double>(DeltaTime), 0.25);

    int32 Steps = 0;
    while (FixedStepAccumulator >= FixedStep && Steps < MaxSubsteps)
    {
        Router->Tick(static_cast<float>(FixedStep));
        FixedStepAccumulator -= FixedStep;
        ++Steps;
    }

    if (Steps == MaxSubsteps && FixedStepAccumulator >= FixedStep)
    {
        FixedStepAccumulator = FMath::Fmod(FixedStepAccumulator, FixedStep);
    }
}

TStatId UBDFRPhysicsWorldSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UBDFRPhysicsWorldSubsystem, STATGROUP_Tickables);
}
