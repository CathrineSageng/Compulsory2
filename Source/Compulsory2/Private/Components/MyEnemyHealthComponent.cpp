// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MyEnemyHealthComponent.h"

// Sets default values for this component's properties
UMyEnemyHealthComponent::UMyEnemyHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Health = 100.f; // Default health
}

void UMyEnemyHealthComponent::TakeDamage(float DamageAmount)
{
	Health -= DamageAmount;
}

bool UMyEnemyHealthComponent::IsDead() const
{
	return Health <= 0;
}

// Called when the game starts
void UMyEnemyHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UMyEnemyHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

