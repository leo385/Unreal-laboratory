// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerCharacter.h"


// Sets default values
ABasePlayerCharacter::ABasePlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Get the player controller for this character
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		
		// Get the enhanced input local player subsystem and add a new input mapping context to it
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(FirstPersonContext, 0);
		}
	}
}

// Called every frame
void ABasePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::MoveForward);
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::MoveRight);
	}
}

void ABasePlayerCharacter::MoveForward(const FInputActionValue &Value)
{
	float MovementValue = Value.Get<float>();

	bCheckControllerExists();
	if(MovementValue != 0.0f) {
		AddMovementInput(GetActorForwardVector(), MovementValue);
	}

}

void ABasePlayerCharacter::MoveRight(const FInputActionValue &Value)
{
	float MovementValue = Value.Get<float>();

	bCheckControllerExists();
	if(MovementValue != 0.0f) {
		AddMovementInput(GetActorRightVector(), MovementValue);
	}

}

bool ABasePlayerCharacter::bCheckControllerExists() const
{
	if(!Controller) {
		UE_LOG(LogTemp, Warning, TEXT("The controller is not assigned to character"));
		FPlatformMisc::RequestExit(false);
		return false;
	}
    return true;
}
