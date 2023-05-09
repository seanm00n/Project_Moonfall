// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/Notify/TrailOffNotify.h"

#include "System/Combat/CombatInterface.h"

void UTrailOffNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	auto owner = MeshComp->GetAnimInstance()->TryGetPawnOwner();
	auto CombatInterface = Cast<ICombatInterface>(owner);
	if (CombatInterface)
	{
		CombatInterface->Execute_TrailOff(owner);
	}
}
