#include "Character/MyCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "HUD/ItemCounterWidget.h"
#include "Blueprint/UserWidget.h"


#include "Components/MyMovementComponent.h"
#include "Components/MyLookComponent.h"
#include "Components/MyInputComponent.h"
#include "Components/MyItemComponent.h"
#include "Systems/MyLookSystem.h"
#include "Systems/MyMovementSystem.h"
#include "Systems/MyInputSystem.h"
#include "Systems/MyItemSystem.h"
#include "Systems/MyInteractionSystem.h"
#include "Character/MyItemActor.h"


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

	// Set default item count
	ItemCount = 0;
}

// Called when the game starts or when spawned
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

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (InputSystem && MyInputComponentRef && MovementComponent && LookComponent)
		{
			InputSystem->InitializeInput(PlayerController, MyInputComponentRef, MovementComponent, LookComponent);
		}
	}

	// Temporary array to hold all found actors
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

	if (ItemCounterWidgetClass)
	{
		ItemCounterWidget = CreateWidget<UItemCounterWidget>(GetWorld(), ItemCounterWidgetClass);
		if (ItemCounterWidget)
		{
			ItemCounterWidget->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("ItemCounterWidget created successfully."));

			// Verify ItemCountText is initialized
			if (ItemCounterWidget->ItemCountText == nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("ItemCountText is still null after widget creation!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create ItemCounterWidget!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ItemCounterWidgetClass is null!"));
	}

	//Rebekka
// Initialize InteractionSystem
	InteractionSystem = NewObject<UMyInteractionSystem>(this);
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

			// Bind the "X" key action for destroying the enemy
			if (DestroyEnemyAction)
			{
				EnhancedInputComponent->BindAction(DestroyEnemyAction, ETriggerEvent::Triggered, this, &AMyCharacter::DestroyEnemy);
			}
		}
	}
}


