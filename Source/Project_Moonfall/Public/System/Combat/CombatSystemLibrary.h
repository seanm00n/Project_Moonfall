// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CombatSystemLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MOONFALL_API UCombatSystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	static float CalculateProbability_Bernoulli(int NumSuccesses, int NumTrials, float SuccessRate);

	UFUNCTION()
	static float CalculateProbability_EWMA(float Weight, float SuccessRate);
    UFUNCTION()
    static float CalculateProbability_BinomialDistribution(int32 NumTrials, float Probability, int32 NumSuccesses);
};
