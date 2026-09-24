#include "BDFRPhysicsRouter.h"

#include "BDFRPhysicsCore.h"
#include "Engine/World.h"
#include "Misc/ScopeLock.h"

FBDFRPhysicsBackendRegistry& FBDFRPhysicsBackendRegistry::Get()
{
    static FBDFRPhysicsBackendRegistry Instance;
    return Instance;
}

bool FBDFRPhysicsBackendRegistry::RegisterBackend(
    EBDFRPhysicsBackendType Type,
    FBDFRPhysicsBackendFactory Factory)
{
    if (Type == EBDFRPhysicsBackendType::Auto || Type == EBDFRPhysicsBackendType::None || !Factory)
    {
        UE_LOG(LogBDFRPhysics, Warning, TEXT("Rejected invalid BDFR physics backend registration."));
        return false;
    }

    FScopeLock Lock(&RegistryMutex);

    if (Factories.Contains(Type))
    {
        UE_LOG(LogBDFRPhysics, Warning, TEXT("Physics backend %d is already registered."), static_cast<int32>(Type));
        return false;
    }

    Factories.Add(Type, MoveTemp(Factory));
    UE_LOG(LogBDFRPhysics, Log, TEXT("Registered physics backend %d."), static_cast<int32>(Type));
    return true;
}

void FBDFRPhysicsBackendRegistry::UnregisterBackend(EBDFRPhysicsBackendType Type)
{
    FScopeLock Lock(&RegistryMutex);
    Factories.Remove(Type);
}

bool FBDFRPhysicsBackendRegistry::IsRegistered(EBDFRPhysicsBackendType Type) const
{
    FScopeLock Lock(&RegistryMutex);
    return Factories.Contains(Type);
}

TUniquePtr<IBDFRPhysicsBackend> FBDFRPhysicsBackendRegistry::CreateBackend(EBDFRPhysicsBackendType Type) const
{
    FScopeLock Lock(&RegistryMutex);

    const FBDFRPhysicsBackendFactory* Factory = Factories.Find(Type);
    return Factory ? (*Factory)() : nullptr;
}

bool FBDFRPhysicsRouter::Initialize(UWorld* InWorld, EBDFRPhysicsBackendType RequestedBackend)
{
    Shutdown();

    if (!IsValid(InWorld))
    {
        UE_LOG(LogBDFRPhysics, Error, TEXT("Cannot initialize physics router without a valid UWorld."));
        return false;
    }

    World = InWorld;

    const EBDFRPhysicsBackendType ResolvedBackend = ResolveRequestedBackend(RequestedBackend);
    if (ResolvedBackend == EBDFRPhysicsBackendType::None)
    {
        UE_LOG(
            LogBDFRPhysics,
            Warning,
            TEXT("No BDFR physics backend is currently available for world '%s'."),
            *InWorld->GetName());
        return true;
    }

    ActiveBackend = FBDFRPhysicsBackendRegistry::Get().CreateBackend(ResolvedBackend);
    if (!ActiveBackend)
    {
        UE_LOG(LogBDFRPhysics, Error, TEXT("Failed to construct selected BDFR physics backend."));
        return false;
    }

    if (!ActiveBackend->Initialize(InWorld))
    {
        UE_LOG(
            LogBDFRPhysics,
            Error,
            TEXT("Backend '%s' failed to initialize."),
            *ActiveBackend->GetDebugName().ToString());
        ActiveBackend.Reset();
        return false;
    }

    UE_LOG(
        LogBDFRPhysics,
        Log,
        TEXT("World '%s' is using backend '%s'."),
        *InWorld->GetName(),
        *ActiveBackend->GetDebugName().ToString());

    return true;
}

void FBDFRPhysicsRouter::Shutdown()
{
    if (ActiveBackend)
    {
        ActiveBackend->Shutdown();
        ActiveBackend.Reset();
    }

    World.Reset();
}

void FBDFRPhysicsRouter::Tick(float DeltaTime)
{
    if (ActiveBackend && DeltaTime > 0.0f)
    {
        ActiveBackend->StepSimulation(DeltaTime);
    }
}

EBDFRPhysicsBackendType FBDFRPhysicsRouter::ResolveRequestedBackend(
    EBDFRPhysicsBackendType RequestedBackend) const
{
    const FBDFRPhysicsBackendRegistry& Registry = FBDFRPhysicsBackendRegistry::Get();

    if (RequestedBackend != EBDFRPhysicsBackendType::Auto)
    {
        if (RequestedBackend == EBDFRPhysicsBackendType::None || Registry.IsRegistered(RequestedBackend))
        {
            return RequestedBackend;
        }

        UE_LOG(
            LogBDFRPhysics,
            Warning,
            TEXT("Requested backend %d is unavailable. Falling back to automatic selection."),
            static_cast<int32>(RequestedBackend));
    }

    // Native Chaos is the safest automatic default once its backend module is registered.
    if (Registry.IsRegistered(EBDFRPhysicsBackendType::Chaos))
    {
        return EBDFRPhysicsBackendType::Chaos;
    }

    if (Registry.IsRegistered(EBDFRPhysicsBackendType::Bullet))
    {
        return EBDFRPhysicsBackendType::Bullet;
    }

    if (Registry.IsRegistered(EBDFRPhysicsBackendType::Hybrid))
    {
        return EBDFRPhysicsBackendType::Hybrid;
    }

    return EBDFRPhysicsBackendType::None;
}
