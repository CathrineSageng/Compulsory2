
#include "Components/MyEnemyHealthComponent.h"

UMyEnemyHealthComponent::UMyEnemyHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	//Default health
	Health = 100.f;
}

//Applies damage to the enemy
void UMyEnemyHealthComponent::TakeDamage(float DamageAmount)
{
	Health -= DamageAmount;
}

//Checks if the enemy is dead 
bool UMyEnemyHealthComponent::IsDead() const
{
	return Health <= 0;
}

void UMyEnemyHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UMyEnemyHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

