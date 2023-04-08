// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DitItHitCompInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDitItHitCompInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_MOONFALL_API IDitItHitCompInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual class UCustomDitItHitActorComponent* GetDitItHit() const = 0;
};
