
#include "Components/MyEnemyHealthComponent.h"

UMyEnemyHealthComponent::UMyEnemyHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Health = 100.f;
}

void UMyEnemyHealthComponent::TakeDamage(float DamageAmount)
{
	Health -= DamageAmount;
}

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

