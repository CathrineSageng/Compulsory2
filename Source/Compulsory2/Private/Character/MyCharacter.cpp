#include "Character/MyCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Components/MyMovementComponent.h"
#include "Components/MyLookComponent.h"
#include "Systems/MyLookSystem.h"
#include "Systems/MyMovementSystem.h"

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

	MovementSystem = NewObject<UMyMovementSystem>();
	LookSystem = NewObject<UMyLookSystem>();


	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SlashContext, 0);
		}
	}
}

void AMyCharacter::Move(const FInputActionValue& Value)
{
	//const FVector2D MovementVector = Value.Get<FVector2D>();

	//const FVector Forward = GetActorForwardVector();
	//AddMovementInput(Forward, MovementVector.Y);
	//const FVector Right = GetActorRightVector();
	//AddMovementInput(Right, MovementVector.X);

	MovementComponent->MovementInput = Value.Get<FVector2D>();

}

void AMyCharacter::Look(const FInputActionValue& Value)
{
	//const FVector2D LookAxisVector = Value.Get<FVector2D>();

	//AddControllerPitchInput(LookAxisVector.Y);
	//AddControllerYawInput(LookAxisVector.X);

	LookComponent->LookInput = Value.Get<FVector2D>();
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

	// Clamp the character's position to stay within the plane boundaries
	FVector CurrentLocation = GetActorLocation();
	float HalfPlaneSize = 1000.0f; // Half of 20x20 plane dimensions

	CurrentLocation.X = FMath::Clamp(CurrentLocation.X, -HalfPlaneSize, HalfPlaneSize);
	CurrentLocation.Y = FMath::Clamp(CurrentLocation.Y, -HalfPlaneSize, HalfPlaneSize);

	// Apply the clamped position back to the character
	SetActorLocation(CurrentLocation);
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{/*
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);*/

		// Bind movement input
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Completed, this, &AMyCharacter::StopMoving);

		// Bind look input
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Completed, this, &AMyCharacter::StopLooking);
	}
}

void AMyCharacter::StopMoving()
{
	MovementComponent->MovementInput = FVector2D::ZeroVector;
}

void AMyCharacter::StopLooking()
{
	LookComponent->LookInput = FVector2D::ZeroVector;
}


