// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/Notify/AnimNotify_SetPlayRate_Montage.h"

void UAnimNotify_SetPlayRate_Montage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	auto AnimInstance = MeshComp->GetAnimInstance();
	AnimInstance->Montage_SetPlayRate(
		AnimInstance->	(), setPlayRate);
}
