// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SetPlayRate_Montage.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MOONFALL_API UAnimNotify_SetPlayRate_Montage : public UAnimNotify
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Montage")
	float setPlayRate = 1.0f;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	
};
