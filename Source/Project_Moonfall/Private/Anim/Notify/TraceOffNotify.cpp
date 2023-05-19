// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/Notify/TraceOffNotify.h"

#include "System/DitItHitCompInterface.h"
#include "System/CustomDitItHitActorComponent.h"
void UTraceOffNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	auto owner = MeshComp->GetAnimInstance()->TryGetPawnOwner();
	auto DitItHitInterface = Cast<IDitItHitCompInterface>(owner);
	if (DitItHitInterface) {
		auto TraceComp = DitItHitInterface->GetDitItHit();
		TraceComp->ToggleTraceCheck(false);
	}

	
}
