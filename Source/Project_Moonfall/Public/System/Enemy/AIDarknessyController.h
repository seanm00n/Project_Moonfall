// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIDarknessyController.generated.h"

/**
 * 
 */
DECLARE_DELEGATE(Del_StartEnemyTimer);

UCLASS()
class PROJECT_MOONFALL_API AAIDarknessyController : public AAIController
{
	GENERATED_BODY()

public:
	AAIDarknessyController();

private:
	virtual void OnPossess(APawn* InPawn) override;
	
	Del_StartEnemyTimer Event_StartEnemyTimer;

private:
	UPROPERTY()
	float LineOfSightTimer;
	UPROPERTY()
	FTimerHandle EnemyTimer;
	UPROPERTY()
	FName IsInSight;
	UPROPERTY()
	FName EnemyActor;
	UPROPERTY(EditAnywhere)
	float ActivateRange;
	UPROPERTY()
	FVector StartPosition;

};