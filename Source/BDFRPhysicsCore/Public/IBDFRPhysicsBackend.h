#pragma once

#include "CoreMinimal.h"
#include "BDFRPhysicsTypes.h"

class UWorld;

/**
 * Common runtime contract implemented by every physics backend.
 *
 * Gameplay-facing code should communicate through this interface rather than
 * including Chaos or Bullet headers directly.
 */
class BDFRPHYSICSCORE_API IBDFRPhysicsBackend
{
public:
    virtual ~IBDFRPhysicsBackend() = default;

    virtual EBDFRPhysicsBackendType GetType() const = 0;
    virtual FName GetDebugName() const = 0;

    virtual bool Initialize(UWorld* World) = 0;
    virtual void Shutdown() = 0;
    virtual void StepSimulation(float DeltaTime) = 0;

    virtual FBDFRPhysicsBodyHandle CreateBody(const FBDFRPhysicsBodyDesc& Desc) = 0;
    virtual void DestroyBody(FBDFRPhysicsBodyHandle Body) = 0;

    virtual void AddForce(FBDFRPhysicsBodyHandle Body, const FVector& ForceNewtons) = 0;
    virtual void AddImpulse(FBDFRPhysicsBodyHandle Body, const FVector& ImpulseNewtonSeconds) = 0;

    virtual void SetBodyTransform(FBDFRPhysicsBodyHandle Body, const FTransform& Transform) = 0;
    virtual bool GetBodyTransform(FBDFRPhysicsBodyHandle Body, FTransform& OutTransform) const = 0;
};
