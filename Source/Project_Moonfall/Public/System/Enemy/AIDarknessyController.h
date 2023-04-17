// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIDarknessyController.generated.h"
#include <Perception/AIPerceptionTypes.h>



/**
 * 
 */
DECLARE_DELEGATE(FStartEnemyTimerDelegate);

UCLASS()
class PROJECT_MOONFALL_API AAIDarknessyController : public AAIController
{
	GENERATED_BODY()

public:
	AAIDarknessyController();

private:
	UFUNCTION()
	virtual void OnPossess(APawn* Pawn) override;

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*> UpdatedActors, FAIStimulus Stimulus);

	UFUNCTION()
	void StartEnemyTimer();

private:
	UBehaviorTree* BTAsset_Darknessy;
	FStartEnemyTimerDelegate StartEnemyTimerDelegate;
	const FString BTPath_Darknessy;
	const float ActivateRange;
	APawn* controlledPawn;

	UPROPERTY()
	float LineOfSightTimer;

	UPROPERTY()
	FTimerHandle EnemyTimer;

	UPROPERTY()
	FName IsInSight;

	UPROPERTY()
	FName EnemyActor;

	UPROPERTY()
	FVector StartPosition;

	UPROPERTY()
	UBehaviorTreeComponent* BehaviorComp;

	UPROPERTY()
	UBlackboardComponent* BlackboardComp;
};