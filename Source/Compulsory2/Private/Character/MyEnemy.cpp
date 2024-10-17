// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyEnemy.h"
#include "Components/MyEnemyHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "GameFramework/Actor.h"

// Sets default values
AMyEnemy::AMyEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Attach the HealthComponent
	HealthComponent = CreateDefaultSubobject<UMyEnemyHealthComponent>(TEXT("HealthComponent"));

	// For the mesh
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// Find the player character as the target to follow
	TargetActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

// Called every frame
void AMyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (TargetActor)
    {
        float DistanceToPlayer = FVector::Dist(GetActorLocation(), TargetActor->GetActorLocation());

        if (DistanceToPlayer <= FollowRadius)
        {
            // Get the AI controller for the enemy
            AAIController* AIController = Cast<AAIController>(GetController());

            if (AIController)
            {
                // Move the enemy toward the player
                AIController->MoveToActor(TargetActor);
            }
        }
        else
        {
            // Stop the movement if the player is out of range
            AAIController* AIController = Cast<AAIController>(GetController());
            if (AIController)
            {
                AIController->StopMovement();
            }
        }
    }
}

// Called to bind functionality to input
void AMyEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

