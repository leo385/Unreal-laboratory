// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerCharacter.h"


// Sets default values
ABasePlayerCharacter::ABasePlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FirstPersonCameraComponent != nullptr);

	FirstPersonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FirstPersonMeshComponent != nullptr);

	// Attach the first-person mesh to the parent character mesh.
	FirstPersonMeshComponent->SetupAttachment(GetMesh());
	FirstPersonMeshComponent->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	
	// Set the first person mesh to not collide with other objects.
	FirstPersonMeshComponent->SetCollisionProfileName(FName("NoCollision"));

	/*
		This primitive type tells the first-person rendering pass to treat the third-person mesh as normal world geometry.
		The mesh uses the standard world FOV and world-space rendering rules even when hidden from the owning player.
	*/
	// Your first person camera parameters should not apply to the 3rd person mesh.
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	// Attach Camera Component to the First Person Mesh to the Socket Head.
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMeshComponent, FName("head"));
	// Position the camera slightly above the eyes and rotate it behind the player's head.
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FirstPersonCameraOffset, FRotator(-89.0f, 105.0f, 90.0f));
	// Enable First Person Camera rotation to be controlled by the pawn.
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	// Enable first-person rendering and set default FOV and scale values.
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = FirstPersonFieldOfView;
	FirstPersonCameraComponent->FirstPersonScale = FirstPersonScale;

}

// Called when the game starts or when spawned
void ABasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(GEngine != nullptr);

	// Find your character's blueprint animation class and attach to its character blueprint.
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBPClass(TEXT("/Game/Blueprints/Anim/ABP_BasePlayerCharacterAnimation.ABP_BasePlayerCharacterAnimation_C"));
	if(AnimBPClass.Succeeded()){
		FirstPersonMeshComponent->SetAnimInstanceClass(AnimBPClass.Class);
		GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);
	}

	if(FirstPersonDefaultAnim != nullptr){
		// Set the animations on the first person mesh.
		FirstPersonMeshComponent->SetAnimInstanceClass(FirstPersonDefaultAnim->GeneratedClass);
		// Set the animations on the 3rd person mesh (for multiplayer settings purpose).
		GetMesh()->SetAnimInstanceClass(FirstPersonDefaultAnim->GeneratedClass);
		UE_LOG(LogTemp, Warning, TEXT("FirstPersonDefaultAnim class has been generated"));
	}

	// Only owning player can see first person mesh. (Invisible for the other player).
	FirstPersonMeshComponent->SetOnlyOwnerSee(true);
	// Hide 3rd mesh to owner.
	GetMesh()->SetOwnerNoSee(true);

	// Make 3rd-person mesh cast a shadow
	GetMesh()->CastShadow = true;
	GetMesh()->bCastHiddenShadow = true;

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
		// 2D bind movement
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::MoveForward);
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::MoveRight);

		// bind jump inheritance from parent class
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// bind look along axis
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::Look);
	}
}

void ABasePlayerCharacter::MoveForward(const FInputActionValue &Value)
{
	const float MovementValue = Value.Get<float>();

	bCheckControllerExists();
	if(MovementValue != 0.0f) {
		AddMovementInput(GetActorForwardVector(), MovementValue);
	}

}

void ABasePlayerCharacter::MoveRight(const FInputActionValue &Value)
{
	const float MovementValue = Value.Get<float>();

	bCheckControllerExists();
	if(MovementValue != 0.0f) {
		AddMovementInput(GetActorRightVector(), MovementValue);
	}

}

void ABasePlayerCharacter::Look(const FInputActionValue &Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	if(Controller) {
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
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
