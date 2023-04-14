// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Enemy/AIDarknessyController.h"
#include "BehaviorTree/BehaviorTree.h"

AAIDarknessyController::AAIDarknessyController()
	: ActivateRange(2000.0f)
	, LineOfSightTimer(4.0f)
	, StartPosition(0.0f, 0.0f, 0.0f)
	, IsInSight(TEXT("IsInSight"))
	, EnemyActor(TEXT("EnemyActor"))
	, BTPath_Darknessy(TEXT("/Content/Monster/Darknessy/AI"))
{
	APawn* controlledPawn = GetPawn();
	if (controlledPawn) StartPosition = controlledPawn->GetActorLocation();
	
	StartEnemyTimerDelegate.BindUObject(this, &AAIDarknessyController::StartEnemyTimer);
	BTAsset_Darknessy = Cast<UBehaviorTree>(StaticLoadObject(UBehaviorTree::StaticClass(), nullptr, *BTPath_Darknessy));
}

void AAIDarknessyController::OnPossess(APawn* Pawn)
{
	Super::OnPossess(Pawn);	
	if (BTAsset_Darknessy) RunBehaviorTree(BTAsset_Darknessy);
}//

void AAIDarknessyController::StartEnemyTimer()
{
	//
}