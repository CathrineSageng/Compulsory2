// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/MyItemSystem.h"
#include "Character/MyCharacter.h"
#include "GameFramework/Character.h"
#include "Components/MyItemComponent.h"

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

        // Optionally hide or destroy the item
        ItemComponent->GetOwner()->SetActorHiddenInGame(true);
        ItemComponent->GetOwner()->SetActorEnableCollision(false);

        // Cast to AMyCharacter to call PickupItem
        if (AMyCharacter* MyCharacter = Cast<AMyCharacter>(Character))
        {
            MyCharacter->PickupItem();
        }

        UE_LOG(LogTemp, Warning, TEXT("Item picked up! Effect: %s"), *ItemComponent->ItemEffect);
    }
}