// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyEnemy.generated.h"

class UMyEnemyHealthComponent;

UCLASS()
class COMPULSORY2_API AMyEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "AI")
	float FollowRadius = 500.0f; // Radius within which the enemy will follow the character

	AActor* TargetActor; // The character the enemy will follow

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMyEnemyHealthComponent* HealthComponent;
	

};
