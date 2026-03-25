// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"

AMyGameModeBase::AMyGameModeBase()
{
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_MyBasePlayerCharacter.BP_MyBasePlayerCharacter_C"));
    if(PlayerPawnBPClass.Class != nullptr) {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
}
