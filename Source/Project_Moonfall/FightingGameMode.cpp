// Copyright Epic Games, Inc. All Rights Reserved.

#include "FightingGameMode.h"
#include "FightingCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFightingGameMode::AFightingGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Blueprint'/Game/FightingGame/Characters/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
