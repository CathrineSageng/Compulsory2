// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/MyInteractionSystem.h"
#include "Character/MyEnemy.h"

#include "Components/MyEnemyHealthComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

void UMyInteractionSystem::ProcessInteractions(AMyCharacter* PlayerCharacter)
{
    // Get the player controller to detect the X key input
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(PlayerCharacter->GetWorld(), 0);
    if (PlayerController == nullptr) return;

    // Check if the X key was pressed
    if (PlayerController->WasInputKeyJustPressed(EKeys::X))
    {
        // Get all nearby enemies (or you could filter based on proximity or specific targeting)
        TArray<AActor*> FoundEnemies;
        UGameplayStatics::GetAllActorsOfClass(PlayerCharacter->GetWorld(), AMyEnemy::StaticClass(), FoundEnemies);

        for (AActor* Actor : FoundEnemies)
        {
            AMyEnemy* Enemy = Cast<AMyEnemy>(Actor);
            if (Enemy)
            {
                // Get the player and enemy locations
                FVector PlayerLocation = PlayerCharacter->GetActorLocation();
                FVector EnemyLocation = Enemy->GetActorLocation();

                // Calculate the distance between the player and the enemy
                float DistanceToEnemy = FVector::Dist(PlayerLocation, EnemyLocation);
                float InteractionRadius = 200.f;  // Define your interaction radius here (e.g., 200 units)

                // Check if the player is within the interaction radius
                if (DistanceToEnemy <= InteractionRadius)
                {
                    UMyEnemyHealthComponent* HealthComponent = Enemy->FindComponentByClass<UMyEnemyHealthComponent>();
                    if (HealthComponent)
                    {
                        // Destroy the enemy if the X key was pressed and within range
                        HealthComponent->TakeDamage(100.f);  // Adjust the damage value as needed

                        // Check if the enemy is dead
                        if (HealthComponent->IsDead())
                        {
                            Enemy->Destroy();  // Destroy the enemy if health is 0
                        }
                    }
                }
                else
                {
                    // Optional: Provide feedback if player is too far away
                    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("You are too far away!"));
                }
            }
        }
    }
}
