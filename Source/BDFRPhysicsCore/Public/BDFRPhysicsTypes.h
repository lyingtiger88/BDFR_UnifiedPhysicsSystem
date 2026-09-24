#pragma once

#include "CoreMinimal.h"
#include "BDFRPhysicsTypes.generated.h"

UENUM(BlueprintType)
enum class EBDFRPhysicsBackendType : uint8
{
    Auto   UMETA(DisplayName = "Automatic"),
    Chaos  UMETA(DisplayName = "Unreal Chaos"),
    Bullet UMETA(DisplayName = "Bullet Physics"),
    Hybrid UMETA(DisplayName = "Chaos + Bullet Hybrid"),
    None   UMETA(DisplayName = "Disabled")
};

USTRUCT(BlueprintType)
struct BDFRPHYSICSCORE_API FBDFRPhysicsBodyHandle
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BDFR Physics")
    int64 Id = INDEX_NONE;

    bool IsValid() const
    {
        return Id != INDEX_NONE;
    }

    static FBDFRPhysicsBodyHandle Invalid()
    {
        return FBDFRPhysicsBodyHandle{};
    }
};

USTRUCT(BlueprintType)
struct BDFRPHYSICSCORE_API FBDFRPhysicsBodyDesc
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR Physics")
    FTransform InitialTransform = FTransform::Identity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR Physics", meta = (ClampMin = "0.0"))
    float MassKg = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR Physics")
    bool bKinematic = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR Physics", meta = (ClampMin = "0.0"))
    float LinearDamping = 0.01f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR Physics", meta = (ClampMin = "0.0"))
    float AngularDamping = 0.05f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR Physics")
    bool bEnableCCD = true;
};

/**
 * Backend-neutral collision payload. Deformation, damage, audio and gameplay
 * systems should consume this instead of depending directly on Chaos/Bullet events.
 */
USTRUCT(BlueprintType)
struct BDFRPHYSICSCORE_API FBDFRPhysicsImpactEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "BDFR Physics|Impact")
    FVector Location = FVector::ZeroVector;

    UPROPERTY(BlueprintReadOnly, Category = "BDFR Physics|Impact")
    FVector Normal = FVector::UpVector;

    UPROPERTY(BlueprintReadOnly, Category = "BDFR Physics|Impact")
    FVector Impulse = FVector::ZeroVector;

    UPROPERTY(BlueprintReadOnly, Category = "BDFR Physics|Impact")
    FVector RelativeVelocity = FVector::ZeroVector;

    UPROPERTY(BlueprintReadOnly, Category = "BDFR Physics|Impact")
    float NormalImpulse = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "BDFR Physics|Impact")
    float EstimatedEnergyJoules = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "BDFR Physics|Impact")
    FBDFRPhysicsBodyHandle BodyA;

    UPROPERTY(BlueprintReadOnly, Category = "BDFR Physics|Impact")
    FBDFRPhysicsBodyHandle BodyB;
};
