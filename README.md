# BDFR Unified Physics System

**BDFR Unified Physics System** is a modular, multi-backend physics framework for Unreal Engine.

The project is designed around a unified physics API so gameplay systems can use different physics backends without being tightly coupled to a single solver.

## Goals

- Support **Unreal Chaos** as a native backend.
- Integrate **Bullet Physics** as an alternative runtime backend.
- Support **Hybrid** simulations that route different systems to different backends.
- Keep gameplay-facing APIs backend-agnostic.
- Allow future physics backends to be added without rewriting gameplay systems.
- Provide scalable simulation quality for PC, console, and mobile targets.

## Planned Architecture

```text
Unreal Engine
    |
    +-- BDFR Unified Physics System
            |
            +-- Physics Router
            |
            +-- Unified Physics API
            |
            +-- Backends
            |     +-- Chaos
            |     +-- Bullet
            |     +-- Hybrid
            |     +-- Future custom backends
            |
            +-- Runtime Systems
                  +-- Rigid Bodies
                  +-- Collision
                  +-- Constraints
                  +-- Vehicles
                  +-- Ragdolls
                  +-- Soft Bodies
                  +-- Live Mesh Deformation
                  +-- Structural Damage / Fracture
```

## Backend Selection

The framework is intended to support backend selection at multiple levels:

```text
Project Default
    -> World Override
        -> Actor / Component Override
```

Initial backend modes:

- `Chaos`
- `Bullet`
- `Hybrid`
- `Auto`

## Live Mesh Deformation

A dedicated deformation layer is planned to consume normalized impact data from any physics backend.

Planned deformation modes:

- Elastic deformation
- Plastic deformation
- Persistent dents
- Explosion-driven deformation
- Local collision updates
- Material-dependent response
- Stress accumulation
- Fracture / structural failure
- CPU and GPU deformation paths

The deformation system will remain independent from Bullet or Chaos by consuming a common impact/event format.

## Planned Plugin Modules

```text
BDFR_UnifiedPhysicsSystem/
|
+-- Source/
|   +-- BDFRPhysicsCore/
|   +-- BDFRBulletBackend/
|   +-- BDFRChaosBackend/
|   +-- BDFRHybridBackend/
|   +-- BDFRDeformation/
|   +-- BDFRFracture/
|   +-- BDFRPhysicsEditor/
|
+-- ThirdParty/
|   +-- Bullet/
|
+-- Config/
+-- Content/
+-- Resources/
+-- Docs/
+-- Examples/
+-- Tests/
```

## Development Roadmap

### Phase 1 — Foundation
- Plugin descriptor
- Core runtime module
- `IPhysicsBackend` abstraction
- Physics Router
- Project settings
- Backend lifecycle

### Phase 2 — Bullet Integration
- Bullet third-party build integration
- Bullet world management
- Rigid bodies
- Collision shapes
- Forces and impulses
- Fixed timestep and substepping
- Debug rendering

### Phase 3 — Constraints and Interaction
- Fixed, hinge, slider, cone-twist, and 6DOF constraints
- Collision/contact events
- Queries and traces
- Sleeping / activation controls
- Continuous collision detection

### Phase 4 — Hybrid Backend
- Chaos/Bullet routing
- Kinematic proxy bridge
- Cross-backend event synchronization
- Per-system backend assignment

### Phase 5 — Deformation and Damage
- Impact resolver
- Material response
- Elastic/plastic deformation
- Explosion deformation
- Adaptive collision rebuilding
- Structural damage and fracture

### Phase 6 — Advanced Simulation
- Vehicle physics
- Ragdolls
- Soft bodies
- Performance tiers
- Multithreaded / async simulation
- Deterministic-mode investigation

## Status

Early development / architecture foundation.

## License

License selection is pending.
