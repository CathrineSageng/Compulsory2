
#include "Systems/MyItemSystem.h"
#include "Character/MyCharacter.h"
#include "GameFramework/Character.h"
#include "Components/MyItemComponent.h"

//Checks if the character is close enough to pick up items 
void UMyItemSystem::CheckForPickup(ACharacter* Character, UMyItemComponent* ItemComponent)
{
    if (!Character || !ItemComponent || ItemComponent->bIsPickedUp)
    {
        return;
    }

    // Calculate the distance between the character and the item
    float Distance = FVector::Dist(Character->GetActorLocation(), ItemComponent->GetOwner()->GetActorLocation());

    if (Distance <= ItemComponent->PickupRadius)
    {
        ItemComponent->bIsPickedUp = true;

        //Removes the items from the game 
        ItemComponent->GetOwner()->SetActorHiddenInGame(true);
        ItemComponent->GetOwner()->SetActorEnableCollision(false);

        //Calls the pickup function 
        if (AMyCharacter* MyCharacter = Cast<AMyCharacter>(Character))
        {
            MyCharacter->PickupItem();
        }
    }
}