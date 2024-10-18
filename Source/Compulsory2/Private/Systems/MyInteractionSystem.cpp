
#include "Systems/MyInteractionSystem.h"
#include "Character/MyEnemy.h"
#include "HUD/EnemyCounterWidget.h"
#include "Components/MyEnemyHealthComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Components/MyCharacterHealthComponent.h"

// Handles inteaction between the player (character) and the nearby enemies. 

void UMyInteractionSystem::ProcessInteractions(AMyCharacter* PlayerCharacter)
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(PlayerCharacter->GetWorld(), 0);
    if (PlayerController == nullptr) return;

    if (PlayerController->WasInputKeyJustPressed(EKeys::SpaceBar))
    {
        // Finds all the enemies in the world
        TArray<AActor*> FoundEnemies;
        UGameplayStatics::GetAllActorsOfClass(PlayerCharacter->GetWorld(), AMyEnemy::StaticClass(), FoundEnemies);

        // Loops through the found enemies in the world and checks for any interactions between enemies and character
        for (AActor* Actor : FoundEnemies)
        {
            AMyEnemy* Enemy = Cast<AMyEnemy>(Actor);
            if (Enemy)
            {
                // Gets the location 
                FVector PlayerLocation = PlayerCharacter->GetActorLocation();
                FVector EnemyLocation = Enemy->GetActorLocation();
                // Calculates the distance between the character and enemies
                float DistanceToEnemy = FVector::Dist(PlayerLocation, EnemyLocation);
                float InteractionRadius = 200.f;

                if (DistanceToEnemy <= InteractionRadius)
                {
                    // Character gets damage
                    UMyCharacterHealthComponent* HealthComponent = PlayerCharacter->FindComponentByClass<UMyCharacterHealthComponent>();
                    if (HealthComponent)
                    {
                        HealthComponent->ApplyDamage(1);
                    }

                    // Damage to the enemy
                    UMyEnemyHealthComponent* EnemyHealthComponent = Enemy->FindComponentByClass<UMyEnemyHealthComponent>();
                    if (EnemyHealthComponent)
                    {
                        EnemyHealthComponent->TakeDamage(100.f);
                        if (EnemyHealthComponent->IsDead())
                        {
                            Enemy->Destroy();
                            PlayerCharacter->EnemiesLeft--; 
                            if (PlayerCharacter->EnemyCounterWidget)
                            {
                                PlayerCharacter->EnemyCounterWidget->UpdateEnemyCount(PlayerCharacter->EnemiesLeft);
                            }
                        }
                    }
                }
            }
        }
    }
}
