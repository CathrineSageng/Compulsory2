#include "Components/MyItemComponent.h"

// Sets default values for this component's properties
UMyItemComponent::UMyItemComponent()
{
    PrimaryComponentTick.bCanEverTick = false; // No ticking required for data storage component
    bIsPickedUp = false;
    PickupRadius = 200.0f; // Default pickup radius
}


