
#include "Systems/MyMovementSystem.h"
#include "Components/MyMovementComponent.h"
#include "Character/MyCharacter.h"

void UMyMovementSystem::Execute(AMyCharacter* Character, UMyMovementComponent* MovementComponent)
{
    if (MovementComponent)
    {
        const FVector Forward = Character->GetActorForwardVector();
        Character->AddMovementInput(Forward, MovementComponent->MovementInput.Y);
        const FVector Right = Character->GetActorRightVector();
        Character->AddMovementInput(Right, MovementComponent->MovementInput.X);
    }
}
