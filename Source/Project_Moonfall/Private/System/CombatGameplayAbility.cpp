// Fill out your copyright notice in the Description page of Project Settings.


#include "System/CombatGameplayAbility.h"
#include "Abilities/GSCBlueprintFunctionLibrary.h"
#include "Abilities/Tasks/GSCTask_PlayMontageWaitForEvent.h"
#include "System/CombatSystemComponent.h"

UCombatGameplayAbility::UCombatGameplayAbility()
{
}

void UCombatGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	CombatSystemComponent = UCombatSystemComponent::GetCombatSystemComponent(AvatarActor);
	if (!CombatSystemComponent)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

}

