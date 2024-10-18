#include "Character/MyCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "HUD/ItemCounterWidget.h"
#include "HUD/EnemyCounterWidget.h"
#include "HUD/CharacterHealthWidget.h"
#include "HUD/GameOverWidget.h"
#include "HUD/YouWonWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"


#include "Components/MyMovementComponent.h"
#include "Components/MyLookComponent.h"
#include "Components/MyInputComponent.h"
#include "Components/MyItemComponent.h"
#include "Components/MyEnemyHealthComponent.h"
#include "Components/MyCharacterHealthComponent.h"
#include "Systems/MyLookSystem.h"
#include "Systems/MyMovementSystem.h"
#include "Systems/MyInputSystem.h"
#include "Systems/MyItemSystem.h"
#include "Systems/MyInteractionSystem.h"
#include "Character/MyItemActor.h"
#include "Character/MyEnemy.h"


// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	MovementComponent = CreateDefaultSubobject<UMyMovementComponent>(TEXT("MovementComponent"));
	LookComponent = CreateDefaultSubobject<UMyLookComponent>(TEXT("LookComponent"));
	MyInputComponentRef = CreateDefaultSubobject<UMyInputComponent>(TEXT("InputComponent"));

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//This is the default value for counting the items the character has collected
	ItemCount = 0;

	bCanTakeDamage = true; // Allow damage initially

	HealthComponent = CreateDefaultSubobject<UMyCharacterHealthComponent>(TEXT("HealthComponent"));
}



// Called when the game starts
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!InputSystem)
	{
		InputSystem = NewObject<UMyInputSystem>(this);
	}

	if (!MovementSystem)
	{
		MovementSystem = NewObject<UMyMovementSystem>(this);
	}

	if (!LookSystem)
	{
		LookSystem = NewObject<UMyLookSystem>(this);
	}

	if (!ItemSystem)
	{
		ItemSystem = NewObject<UMyItemSystem>(this);
	}

	if (HealthComponent)
	{
		HealthComponent->OnCharacterDeath.AddDynamic(this, &AMyCharacter::HandleDeath);
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (InputSystem && MyInputComponentRef && MovementComponent && LookComponent)
		{
			InputSystem->InitializeInput(PlayerController, MyInputComponentRef, MovementComponent, LookComponent);
		}
	}

	// Temporary array to hold all the found actors
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyItemActor::StaticClass(), FoundActors);

	// Loop through the found actors and cast them to AMyItemActor
	for (AActor* Actor : FoundActors)
	{
		if (AMyItemActor* Item = Cast<AMyItemActor>(Actor))
		{
			NearbyItems.Add(Item);
		}
	}

	//YouWon Widget
	MaxItemCount = NearbyItems.Num();

	if (ItemCounterWidgetClass)
	{
		ItemCounterWidget = CreateWidget<UItemCounterWidget>(GetWorld(), ItemCounterWidgetClass);
		if (ItemCounterWidget)
		{
			ItemCounterWidget->AddToViewport();

			// Verify ItemCountText is initialized //Ta bort denne if statement kanskje?
			if (ItemCounterWidget->ItemCountText == nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("ItemCountText is still null after widget creation!"));
			}
		}
	}

	// Initialize InteractionSystem
	InteractionSystem = NewObject<UMyInteractionSystem>(this);

	// Get all enemies in the world and count them
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyEnemy::StaticClass(), FoundEnemies);
	EnemiesLeft = FoundEnemies.Num();


	if (EnemyCounterWidgetClass)
	{
		EnemyCounterWidget = CreateWidget<UEnemyCounterWidget>(GetWorld(), EnemyCounterWidgetClass);
		if (EnemyCounterWidget)
		{
			EnemyCounterWidget->AddToViewport();
			EnemyCounterWidget->UpdateEnemyCount(EnemiesLeft);
		}
	}
	//// Bind to OnComponentHit to detect blocking hits
	//GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMyCharacter::OnEnemyHit);

	 // Bind overlap and hit events to the capsule component
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnEnemyOverlap);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMyCharacter::OnEnemyHit);

	
	if (HealthWidgetClass)
	{
		HealthWidget = CreateWidget<UCharacterHealthWidget>(GetWorld(), HealthWidgetClass);
		if (HealthWidget)
		{
			HealthWidget->AddToViewport();
			HealthWidget->UpdateHealth(HealthComponent->GetCurrentHealth(), HealthComponent->GetMaxHealth());
		}
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MovementSystem && MovementComponent)
	{
		MovementSystem->Execute(this, MovementComponent);

		// Reset movement input to zero after each update
		MovementComponent->MovementInput = FVector2D::ZeroVector;
	}

	if (LookSystem && LookComponent)
	{
		LookSystem->Execute(GetController(), LookComponent);
		LookComponent->LookInput = FVector2D::ZeroVector;
	}

	for (AMyItemActor* Item : NearbyItems)
	{
		if (Item && ItemSystem)
		{
			ItemSystem->CheckForPickup(this, Item->FindComponentByClass<UMyItemComponent>());
		}
	}

	// Clamp the character's position to stay within the plane boundaries
	FVector CurrentLocation = GetActorLocation();
	float HalfPlaneSize = 1000.0f; // Half of 20x20 plane dimensions

	CurrentLocation.X = FMath::Clamp(CurrentLocation.X, -HalfPlaneSize, HalfPlaneSize);
	CurrentLocation.Y = FMath::Clamp(CurrentLocation.Y, -HalfPlaneSize, HalfPlaneSize);

	// Apply the clamped position back to the character
	SetActorLocation(CurrentLocation);

	// Process interactions every tick
	if (InteractionSystem)
	{
		InteractionSystem->ProcessInteractions(this);
	}

	// Check if all items are collected and all enemies are killed
	if (ItemCount == MaxItemCount && EnemiesLeft == 0)
	{
		if (YouWonWidgetClass)
		{
			YouWonWidget = CreateWidget<UYouWonWidget>(GetWorld(), YouWonWidgetClass);
			if (YouWonWidget)
			{
				YouWonWidget->AddToViewport();
			}
		}
	}
}

void AMyCharacter::OnEnemyHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AMyEnemy* Enemy = Cast<AMyEnemy>(OtherActor);
	if (Enemy && bCanTakeDamage)
	{
		bCanTakeDamage = false;
		HealthComponent->ApplyDamage(1);  // Apply damage
		// Update the health widget
		if (HealthWidget)
		{
			HealthWidget->UpdateHealth(HealthComponent->GetCurrentHealth(), HealthComponent->GetMaxHealth());
		}
		GetWorld()->GetTimerManager().SetTimer(DamageCooldownTimer, this, &AMyCharacter::ResetDamageCooldown, 1.0f, false);  // Damage cooldown
	}
}

void AMyCharacter::OnEnemyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyEnemy* Enemy = Cast<AMyEnemy>(OtherActor);
	if (Enemy && bCanTakeDamage)
	{
		bCanTakeDamage = false;
		HealthComponent->ApplyDamage(1);  // Apply damage
		if (HealthWidget)
		{
			HealthWidget->UpdateHealth(HealthComponent->GetCurrentHealth(), HealthComponent->GetMaxHealth());
		}
		GetWorld()->GetTimerManager().SetTimer(DamageCooldownTimer, this, &AMyCharacter::ResetDamageCooldown, 1.0f, false);  // Damage cooldown
	}
}

void AMyCharacter::ResetDamageCooldown()
{
	bCanTakeDamage = true; // Allow damage again after the cooldown period
}

void AMyCharacter::PickupItem()
{
	// Increment the item count
	ItemCount++;

	// Update the UI
	if (ItemCounterWidget)
	{
		ItemCounterWidget->UpdateItemCount(ItemCount);
	}
}

void AMyCharacter::Move(const FInputActionValue& Value)
{
	MovementComponent->MovementInput = Value.Get<FVector2D>();
}

void AMyCharacter::Look(const FInputActionValue& Value)
{
	LookComponent->LookInput = Value.Get<FVector2D>();
}

void AMyCharacter::StopMoving()
{
	MovementComponent->MovementInput = FVector2D::ZeroVector;
}

void AMyCharacter::StopLooking()
{
	LookComponent->LookInput = FVector2D::ZeroVector;
}

void AMyCharacter::DestroyEnemy()
{
	// Call the interaction system logic for destroying an enemy when "X" is pressed
	if (InteractionSystem)
	{
		InteractionSystem->ProcessInteractions(this);
	}
}

void AMyCharacter::HandleDeath()
{
	// Show the Game Over Widget
	if (GameOverWidgetClass)
	{
		GameOverWidget = CreateWidget<UGameOverWidget>(GetWorld(), GameOverWidgetClass);
		if (GameOverWidget)
		{
			GameOverWidget->AddToViewport();
		}
	}

	Destroy();
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MyInputComponentRef)
		{
			// Bind movement action
			EnhancedInputComponent->BindAction(MyInputComponentRef->MovementAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
			EnhancedInputComponent->BindAction(MyInputComponentRef->MovementAction, ETriggerEvent::Completed, this, &AMyCharacter::StopMoving);

			// Bind look action
			EnhancedInputComponent->BindAction(MyInputComponentRef->LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
			EnhancedInputComponent->BindAction(MyInputComponentRef->LookAction, ETriggerEvent::Completed, this, &AMyCharacter::StopLooking);

			// Bind destroying enemy action
			if (DestroyEnemyAction)
			{
				EnhancedInputComponent->BindAction(DestroyEnemyAction, ETriggerEvent::Triggered, this, &AMyCharacter::DestroyEnemy);
			}
		}
	}
}


