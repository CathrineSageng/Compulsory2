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
class UMyCharacterHealthComponent;


UCLASS()
class COMPULSORY2_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	//The constuctor and tick function
	AMyCharacter();
	virtual void Tick(float DeltaTime) override;

	//Function that handles pick up 
	void PickupItem();

	// Number of enemies left in the game
	int32 EnemiesLeft;

	//References the enemy counter Widget for the HUD
	UPROPERTY()
	UEnemyCounterWidget* EnemyCounterWidget;

	int32 MaxItemCount;
protected:

	virtual void BeginPlay() override;

	//Functions for the movement and look input
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StopMoving();
	void StopLooking();

	// Function to handle collision with enemy
	UFUNCTION()
	void OnEnemyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	//Function to handle hits from enemies
	UFUNCTION()
	void OnEnemyHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit);

	// Function to handle X key press and destroying an enemy
	void DestroyEnemy();

	// Function to handle character death
	UFUNCTION()
	void HandleDeath();

	//Components
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UMyMovementComponent* MovementComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UMyLookComponent* LookComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UMyInputComponent* MyInputComponentRef;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UMyCharacterHealthComponent* HealthComponent;

	//Systems
	UPROPERTY()
	UMyMovementSystem* MovementSystem;
	UPROPERTY()
	UMyLookSystem* LookSystem;
	UPROPERTY()
	UMyInputSystem* InputSystem;
	UPROPERTY()
	UMyInteractionSystem* InteractionSystem;

	// Input action for destroying an enemy 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* DestroyEnemyAction;

	//UI elements 
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> ItemCounterWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> EnemyCounterWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UCharacterHealthWidget> HealthWidgetClass;
	UCharacterHealthWidget* HealthWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UGameOverWidget> GameOverWidgetClass;
	UGameOverWidget* GameOverWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UYouWonWidget> YouWonWidgetClass;
	UYouWonWidget* YouWonWidget;
private:
	//Variables for handling item collection
	int32 ItemCount;
	TArray<AMyItemActor*> NearbyItems;
	//Item system reference 
	UPROPERTY()
	UMyItemSystem* ItemSystem;
	
	//References the Item Widget 
	UPROPERTY()
	UItemCounterWidget* ItemCounterWidget;

	//Track if the character can take damage, this is so we prevents duplicate the hits 
	//Letter 'b' in the name shows that the variable is an boolean and is common prectice in UE
	bool bCanTakeDamage;
	FTimerHandle DamageCooldownTimer;

	//Resets for character taking damage 
	void ResetDamageCooldown();

	//Camera properties
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	// Bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};

