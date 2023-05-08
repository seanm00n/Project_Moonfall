// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/Notify/TrailOnNotify.h"

#include "System/Combat/CombatInterface.h"

void UTrailOnNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	auto owner = MeshComp->GetAnimInstance()->TryGetPawnOwner();
	auto CombatInterface = Cast<ICombatInterface>(owner);
	if (CombatInterface)
	{
		CombatInterface->Execute_TrailOn(owner);
	}
}
