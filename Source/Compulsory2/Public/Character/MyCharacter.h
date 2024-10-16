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
class UMyItemComponent;
class UMyItemSystem;
class AMyItemActor;
class UItemCounterWidget;
class UEnemyCounterWidget;
class UMyInteractionSystem;

UCLASS()
class COMPULSORY2_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyCharacter();

	virtual void Tick(float DeltaTime) override;

	void PickupItem();

	UPROPERTY()
	UEnemyCounterWidget* EnemyCounterWidget;

	// Number of enemies left in the game
	int32 EnemiesLeft;

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

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> ItemCounterWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> EnemyCounterWidgetClass;

	UPROPERTY()
	UMyInteractionSystem* InteractionSystem;

	// Reference to the destroy enemy action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* DestroyEnemyAction;

	// Function to handle X key press
	void DestroyEnemy();

private:
	int32 ItemCount;

	UPROPERTY()
	UMyItemSystem* ItemSystem;

	// Add a reference to track items if necessary
	TArray<AMyItemActor*> NearbyItems;

	UPROPERTY()
	UItemCounterWidget* ItemCounterWidget;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	// Bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
