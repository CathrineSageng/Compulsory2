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

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	//Initializes the components for movement, input and look. 
	MovementComponent = CreateDefaultSubobject<UMyMovementComponent>(TEXT("MovementComponent"));
	LookComponent = CreateDefaultSubobject<UMyLookComponent>(TEXT("LookComponent"));
	MyInputComponentRef = CreateDefaultSubobject<UMyInputComponent>(TEXT("InputComponent"));

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//This is the starting value for how many items the character has collected 
	ItemCount = 0;

	//The character is able to take damage 
	bCanTakeDamage = true; 

	// Initializes the Health Component
	HealthComponent = CreateDefaultSubobject<UMyCharacterHealthComponent>(TEXT("HealthComponent"));
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Initializes the different systems, input, movement, look and item. 
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

	// Here the character's death function gets bind to the helath component on character death event
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

	// All of the items around in the world is stored in the nearby NearbyItems array
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

	// Stores the total numbers of items in the world to win the game 
	MaxItemCount = NearbyItems.Num();

	// Item counter widget is created and desplayed on the screen 
	if (ItemCounterWidgetClass)
	{
		ItemCounterWidget = CreateWidget<UItemCounterWidget>(GetWorld(), ItemCounterWidgetClass);
		if (ItemCounterWidget)
		{
			ItemCounterWidget->AddToViewport();
		}
	}

	// Initialize InteractionSystem to kill enemies 
	InteractionSystem = NewObject<UMyInteractionSystem>(this);

	// Get all enemies in the world and count them
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyEnemy::StaticClass(), FoundEnemies);
	EnemiesLeft = FoundEnemies.Num();

	// Enemy counter widget is created and displayed on the screen 
	if (EnemyCounterWidgetClass)
	{
		EnemyCounterWidget = CreateWidget<UEnemyCounterWidget>(GetWorld(), EnemyCounterWidgetClass);
		if (EnemyCounterWidget)
		{
			EnemyCounterWidget->AddToViewport();
			EnemyCounterWidget->UpdateEnemyCount(EnemiesLeft);
		}
	}

	 // Bind overlap and hit to find out when the character is interacting with the enemies
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnEnemyOverlap);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMyCharacter::OnEnemyHit);

	// Health widget is created and displayed to see the character's helath 
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

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Movement input
	if (MovementSystem && MovementComponent)
	{
		MovementSystem->Execute(this, MovementComponent);
		MovementComponent->MovementInput = FVector2D::ZeroVector;
	}

	// Look input
	if (LookSystem && LookComponent)
	{
		LookSystem->Execute(GetController(), LookComponent);
		LookComponent->LookInput = FVector2D::ZeroVector;
	}

	// Checks if there are any items to pick up 
	for (AMyItemActor* Item : NearbyItems)
	{
		if (Item && ItemSystem)
		{
			ItemSystem->CheckForPickup(this, Item->FindComponentByClass<UMyItemComponent>());
		}
	}

	// Clamp the character's position to stay within the plane
	FVector CurrentLocation = GetActorLocation();
	// Size of the plane 
	float HalfPlaneSize = 1000.0f; 

	CurrentLocation.X = FMath::Clamp(CurrentLocation.X, -HalfPlaneSize, HalfPlaneSize);
	CurrentLocation.Y = FMath::Clamp(CurrentLocation.Y, -HalfPlaneSize, HalfPlaneSize);

	SetActorLocation(CurrentLocation);

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

// Called when an character is hit by an enemy 
void AMyCharacter::OnEnemyHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AMyEnemy* Enemy = Cast<AMyEnemy>(OtherActor);
	if (Enemy && bCanTakeDamage)
	{
		bCanTakeDamage = false;
		// Applies damage to character's health
		HealthComponent->ApplyDamage(1);  
		// Updates the health widget
		if (HealthWidget)
		{
			HealthWidget->UpdateHealth(HealthComponent->GetCurrentHealth(), HealthComponent->GetMaxHealth());
		}
		GetWorld()->GetTimerManager().SetTimer(DamageCooldownTimer, this, &AMyCharacter::ResetDamageCooldown, 1.0f, false);  // Damage cooldown
	}
}

// This is called when an character overlaps with an enemy 
void AMyCharacter::OnEnemyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyEnemy* Enemy = Cast<AMyEnemy>(OtherActor);
	if (Enemy && bCanTakeDamage)
	{
		bCanTakeDamage = false;
		// Applies damage to character's health
		HealthComponent->ApplyDamage(1); 
		if (HealthWidget)
		{
			HealthWidget->UpdateHealth(HealthComponent->GetCurrentHealth(), HealthComponent->GetMaxHealth());
		}
		GetWorld()->GetTimerManager().SetTimer(DamageCooldownTimer, this, &AMyCharacter::ResetDamageCooldown, 1.0f, false);  // Damage cooldown
	}
}

// The character can take damage again after a cooldown period
void AMyCharacter::ResetDamageCooldown()
{
	bCanTakeDamage = true; 
}

// Increment the item count for the Character
void AMyCharacter::PickupItem()
{ 
	ItemCount++;

	// Updates the UI
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

// Destroys an enemy when spacebar is pressed 
void AMyCharacter::DestroyEnemy()
{
	if (InteractionSystem)
	{
		InteractionSystem->ProcessInteractions(this);
	}
}

// The character's death is handled, the 'Game over widget' is shown
void AMyCharacter::HandleDeath()
{
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

// Binds the input components and actions 
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


