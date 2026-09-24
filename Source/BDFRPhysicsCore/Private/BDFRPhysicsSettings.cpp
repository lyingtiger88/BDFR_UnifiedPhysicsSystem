#include "BDFRPhysicsSettings.h"

UBDFRPhysicsSettings::UBDFRPhysicsSettings()
{
    CategoryName = TEXT("Plugins");
    SectionName = TEXT("BDFR Unified Physics System");
}

FName UBDFRPhysicsSettings::GetCategoryName() const
{
    return TEXT("Plugins");
}
