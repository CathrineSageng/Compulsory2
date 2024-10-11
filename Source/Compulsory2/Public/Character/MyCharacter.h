#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MyCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

class UMyMovementComponent;
class UMyLookComponent;
class UMyMovementSystem;
class UMyLookSystem;

UCLASS()
class COMPULSORY2_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* SlashContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UMyMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UMyLookComponent* LookComponent;

	UPROPERTY()
	UMyMovementSystem* MovementSystem;

	UPROPERTY()
	UMyLookSystem* LookSystem;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StopMoving();
	void StopLooking();

private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;
};
