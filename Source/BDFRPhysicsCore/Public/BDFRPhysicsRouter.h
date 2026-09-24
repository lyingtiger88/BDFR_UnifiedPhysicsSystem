#pragma once

#include "CoreMinimal.h"
#include "IBDFRPhysicsBackend.h"

class UWorld;

using FBDFRPhysicsBackendFactory = TFunction<TUniquePtr<IBDFRPhysicsBackend>()>;

/**
 * Global backend factory registry. Backend modules register themselves here
 * during StartupModule and unregister during ShutdownModule.
 */
class BDFRPHYSICSCORE_API FBDFRPhysicsBackendRegistry
{
public:
    static FBDFRPhysicsBackendRegistry& Get();

    bool RegisterBackend(EBDFRPhysicsBackendType Type, FBDFRPhysicsBackendFactory Factory);
    void UnregisterBackend(EBDFRPhysicsBackendType Type);

    bool IsRegistered(EBDFRPhysicsBackendType Type) const;
    TUniquePtr<IBDFRPhysicsBackend> CreateBackend(EBDFRPhysicsBackendType Type) const;

private:
    mutable FCriticalSection RegistryMutex;
    TMap<EBDFRPhysicsBackendType, FBDFRPhysicsBackendFactory> Factories;
};

/**
 * Per-world router responsible for selecting and owning the active backend.
 */
class BDFRPHYSICSCORE_API FBDFRPhysicsRouter
{
public:
    bool Initialize(UWorld* InWorld, EBDFRPhysicsBackendType RequestedBackend);
    void Shutdown();
    void Tick(float DeltaTime);

    IBDFRPhysicsBackend* GetActiveBackend() const
    {
        return ActiveBackend.Get();
    }

    EBDFRPhysicsBackendType GetActiveBackendType() const
    {
        return ActiveBackend ? ActiveBackend->GetType() : EBDFRPhysicsBackendType::None;
    }

private:
    EBDFRPhysicsBackendType ResolveRequestedBackend(EBDFRPhysicsBackendType RequestedBackend) const;

    TWeakObjectPtr<UWorld> World;
    TUniquePtr<IBDFRPhysicsBackend> ActiveBackend;
};
