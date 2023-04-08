// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/Notify/TraceOnNotify.h"
#include "System/DitItHitCompInterface.h"
#include "System/CustomDitItHitActorComponent.h"
void UTraceOnNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	auto DitItHitInterface = Cast<IDitItHitCompInterface>(MeshComp->GetAnimInstance()->TryGetPawnOwner());
	if (DitItHitInterface) {
		auto TraceComp = DitItHitInterface->GetDitItHit();
		TraceComp->ToggleTraceCheck(true);
	}
}
