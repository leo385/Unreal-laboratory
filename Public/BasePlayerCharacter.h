// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
// enhanced input headers
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "BasePlayerCharacter.generated.h"

// forwards declaration
class UAnimBlueprint;
class UInputMappingContext;
class UInputAction;
class UInputComponent;

UCLASS()
class FIGHTGAME_API ABasePlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasePlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> FirstPersonContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveForwardAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveRightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

public:
	UFUNCTION()
	void MoveForward(const FInputActionValue& Value);
	UFUNCTION()
	void MoveRight(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);

	// Camera
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FVector FirstPersonCameraOffset = FVector(0.0f, -31.940952f, 33.296291f);

	UPROPERTY(EditAnywhere, Category = "Camera")
	float FirstPersonFieldOfView = 70.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float FirstPersonScale = 0.6f;

	// Mesh
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> FirstPersonMeshComponent;

	// Animation
	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimBlueprint> FirstPersonDefaultAnim;

private:
	bool bCheckControllerExists() const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
