#include "Components/MyItemComponent.h"

UMyItemComponent::UMyItemComponent()
{
    PrimaryComponentTick.bCanEverTick = false; 
    bIsPickedUp = false;
    PickupRadius = 200.0f;
}


