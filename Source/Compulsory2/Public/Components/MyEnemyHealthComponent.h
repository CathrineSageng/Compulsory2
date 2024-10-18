
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyEnemyHealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMPULSORY2_API UMyEnemyHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMyEnemyHealthComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;

	void TakeDamage(float DamageAmount);

	bool IsDead() const;

protected:
	virtual void BeginPlay() override;
};
