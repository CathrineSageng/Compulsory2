// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/MyInteractionSystem.h"
#include "Character/MyEnemy.h"
#include "HUD/EnemyCounterWidget.h"
#include "Components/MyEnemyHealthComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

void UMyInteractionSystem::ProcessInteractions(AMyCharacter* PlayerCharacter)
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(PlayerCharacter->GetWorld(), 0);
    if (PlayerController == nullptr) return;

    if (PlayerController->WasInputKeyJustPressed(EKeys::X))
    {
        TArray<AActor*> FoundEnemies;
        UGameplayStatics::GetAllActorsOfClass(PlayerCharacter->GetWorld(), AMyEnemy::StaticClass(), FoundEnemies);

        for (AActor* Actor : FoundEnemies)
        {
            AMyEnemy* Enemy = Cast<AMyEnemy>(Actor);
            if (Enemy)
            {
                FVector PlayerLocation = PlayerCharacter->GetActorLocation();
                FVector EnemyLocation = Enemy->GetActorLocation();
                float DistanceToEnemy = FVector::Dist(PlayerLocation, EnemyLocation);
                float InteractionRadius = 200.f;

                if (DistanceToEnemy <= InteractionRadius)
                {
                    UMyEnemyHealthComponent* HealthComponent = Enemy->FindComponentByClass<UMyEnemyHealthComponent>();
                    if (HealthComponent)
                    {
                        HealthComponent->TakeDamage(100.f);

                        if (HealthComponent->IsDead())
                        {
                            Enemy->Destroy();
                            PlayerCharacter->EnemiesLeft--;  // Decrement enemy count
                            if (PlayerCharacter->EnemyCounterWidget)
                            {
                                PlayerCharacter->EnemyCounterWidget->UpdateEnemyCount(PlayerCharacter->EnemiesLeft);  // Update HUD
                            }
                        }
                    }
                }
            }
        }
    }
}
