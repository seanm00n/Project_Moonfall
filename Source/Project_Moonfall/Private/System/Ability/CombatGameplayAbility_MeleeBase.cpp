// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Ability/CombatGameplayAbility_MeleeBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "System/CombatSystemComponent.h"


void UCombatGameplayAbility_MeleeBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	CombatSystemComponent = UCombatSystemComponent::GetCombatSystemComponent(AvatarActor);
	if (!CombatSystemComponent)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CombatSystemComponent->AttackInfo(AttackEffect, ReactAbility);
	/*FGameplayEventData eventData;
	eventData.Instigator=AvatarActor;
	UAbilityTask_WaitGameplayEvent* Task = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag(FName("Event.FinishAct")), nullptr, true);
	Task->EventReceived.AddDynamic(this, &UCombatGameplayAbility_MeleeBase::OnCombatReceived);
	
	UE_LOG(LogTemp, Log, TEXT("OnEventReceived_override 1 ActivateAbility"));*/
	//SendGameplayEvent(FGameplayTag::RequestGameplayTag(FName("Event.FinishAct")), eventData);
	//UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(AvatarActor, FGameplayTag::RequestGameplayTag(FName("Event.FinishAct")), eventData);
}

void UCombatGameplayAbility_MeleeBase::OnCombatReceived(FGameplayEventData EventData)
{

	UE_LOG(LogTemp, Log, TEXT("OnCombatReceived 2"));
	if (isEventReceived_Combat) {
		AActor* AvatarActor = GetAvatarActorFromActorInfo();
		auto AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AvatarActor);
		if (AbilitySystemComponent) {
			auto context = AbilitySystemComponent->MakeEffectContext();
			AbilitySystemComponent->ApplyGameplayEffectToSelf(ReactEffect->GetDefaultObject<UGameplayEffect>(), 0.0, context);
		}
	}
}
