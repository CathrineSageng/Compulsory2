
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyEnemy.generated.h"

// Forward declaration
class UMyEnemyHealthComponent;

UCLASS()
class COMPULSORY2_API AMyEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AMyEnemy();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// This is the radius where the enemy will follow the character
	UPROPERTY(EditAnywhere, Category = "AI")
	float FollowRadius = 500.0f; 

	// Referencing the character the enemy is following
	AActor* TargetActor; // The character the enemy will follow

protected:
	virtual void BeginPlay() override;

	// Health component to the enemy, used for managing health and damage 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMyEnemyHealthComponent* HealthComponent;
	

};
