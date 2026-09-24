#include "BDFRPhysicsCore.h"

DEFINE_LOG_CATEGORY(LogBDFRPhysics);

void FBDFRPhysicsCoreModule::StartupModule()
{
    UE_LOG(LogBDFRPhysics, Log, TEXT("BDFR Unified Physics System core initialized."));
}

void FBDFRPhysicsCoreModule::ShutdownModule()
{
    UE_LOG(LogBDFRPhysics, Log, TEXT("BDFR Unified Physics System core shutdown."));
}

IMPLEMENT_MODULE(FBDFRPhysicsCoreModule, BDFRPhysicsCore)
