// Fill out your copyright notice in the Description page of Project Settings.

#include "BPCAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBPCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    ACharacter* Character = Cast<ACharacter>(TryGetPawnOwner());
    if(Character) {
        GroundSpeed = Character->GetVelocity().Size();
        bIsFalling = Character->GetCharacterMovement()->IsFalling();
    }
}
