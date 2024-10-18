
#include "Character/MyEnemy.h"
#include "Components/MyEnemyHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "GameFramework/Actor.h"

AMyEnemy::AMyEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UMyEnemyHealthComponent>(TEXT("HealthComponent"));

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();
	
    //Stores a reference to the character that the enemy can follow 
	TargetActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

void AMyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    //Updates the enemies behavior
    if (TargetActor)
    {
        float DistanceToPlayer = FVector::Dist(GetActorLocation(), TargetActor->GetActorLocation());

        if (DistanceToPlayer <= FollowRadius)
        {
            // Get the AI controller for the enemy
            AAIController* AIController = Cast<AAIController>(GetController());

            if (AIController)
            {
                // Moves the enemy toward the character
                AIController->MoveToActor(TargetActor);
            }
        }
        else
        {
            //Stops the movement on the enemy if the character is our if range. 
            AAIController* AIController = Cast<AAIController>(GetController());
            if (AIController)
            {
                AIController->StopMovement();
            }
        }
    }
}

void AMyEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

