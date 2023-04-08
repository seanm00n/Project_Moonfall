// Fill out your copyright notice in the Description page of Project Settings.


#include "System/CombatGameplayAbility.h"
#include "Abilities/GSCBlueprintFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
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
	CombatSystemComponent->AttackInfo(AttackEffect, ReactAbility);

	OnActivateCombatAbility.Broadcast();

}

void UCombatGameplayAbility::OnCombatReceived(FGameplayEventData EventData)
{

	UE_LOG(LogTemp, Log, TEXT("OnCombatReceived 2"));
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	auto AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AvatarActor);
	if (AbilitySystemComponent) {
		if (ReactEffect) {
			auto context = AbilitySystemComponent->MakeEffectContext();
			AbilitySystemComponent->ApplyGameplayEffectToSelf(ReactEffect->GetDefaultObject<UGameplayEffect>(), 0.0, context);
		}
	}

}
