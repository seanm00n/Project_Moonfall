// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Enemy/AIDarknessyController.h"
#include "BehaviorTree/BehaviorTree.h"
AAIDarknessyController::AAIDarknessyController() : 
	LineOfSightTimer(4.0f), IsInSight(TEXT("IsInSight")), EnemyActor(TEXT("EnemyActor")), 
	ActivateRange(2000.0f), StartPosition(0.0f, 0.0f, 0.0f)
{
	APawn* controlledPawn = GetPawn();
	if (controlledPawn) {
		StartPosition = controlledPawn->GetActorLocation();
	}
}

void AAIDarknessyController::OnPossess(APawn* InPawn)
{
	const FString BTPath_Darknessy = "/Content/Monster/Darknessy/AI";
	UBehaviorTree* BTAsset_Darknessy = Cast<UBehaviorTree>(StaticLoadObject(UBehaviorTree::StaticClass(), nullptr, *BTPath_Darknessy));
	if (BTAsset_Darknessy) {
		RunBehaviorTree(BTAsset_Darknessy);
	}

}
