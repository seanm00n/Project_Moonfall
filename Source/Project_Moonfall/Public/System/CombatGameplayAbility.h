// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GSCGameplayAbility.h"
#include "CombatGameplayAbility.generated.h"

/**
 * 
 */

class UCombatSystemComponent;
UCLASS()
class PROJECT_MOONFALL_API UCombatGameplayAbility : public UGSCGameplayAbility
{
	GENERATED_BODY()
public:
	UCombatGameplayAbility();

protected:

	UPROPERTY()
	UCombatSystemComponent* CombatSystemComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Montage")
	UAnimMontage* Montages;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Effect")
	TSubclassOf<UGameplayEffect> AttackEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Effect")
	TSubclassOf<UGameplayAbility> ReactAbility;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Effect")
	TSubclassOf<UGameplayEffect> ReactEffect;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/*UFUNCTION(BlueprintCallable)
	void OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION(BlueprintCallable)
	void OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION(BlueprintCallable)
	void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData);*/

};
