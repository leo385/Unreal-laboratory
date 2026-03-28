// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BPCAnimInstance.generated.h"

/**
 * BP_BasePlayerCharacter reads character's properties from this class,
 * to assign correctly animations to input basis on this class values.
 */
UCLASS()
class FIGHTGAME_API UBPCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed{0.0f};

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsFalling{false};
};
