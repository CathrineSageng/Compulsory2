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
class UMyInputComponent;
class UMyMovementSystem;
class UMyLookSystem;
class UMyInputSystem;

UCLASS()
class COMPULSORY2_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyCharacter();

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	// Input handling methods for movement and look actions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StopMoving();
	void StopLooking();

	//Components
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UMyMovementComponent* MovementComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UMyLookComponent* LookComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UMyInputComponent* MyInputComponentRef;

	//Systems
	UPROPERTY()
	UMyMovementSystem* MovementSystem;
	UPROPERTY()
	UMyLookSystem* LookSystem;
	UPROPERTY()
	UMyInputSystem* InputSystem;


private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	// Bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
