// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemComponent.h"
#include "CombatSystemComponent.generated.h"


class UGSCAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FAttackInfo {
GENERATED_BODY();
	public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
	TSubclassOf<UGameplayEffect> Effect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
	TSubclassOf<UGameplayAbility> Ability;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTakeAttack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCombatStart, AActor*, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCombatEnd, AActor*, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPostAttack, AActor*, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPreAttack, AActor*, Instigator);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECT_MOONFALL_API UCombatSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCombatSystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(BlueprintAssignable, Category = "CombatSystem")
		FTakeAttack OnTakeAttack;

	UPROPERTY(BlueprintAssignable, Category = "CombatSystem")
	FCombatStart OnCombatStart;
	UPROPERTY(BlueprintAssignable, Category = "CombatSystem")
	FCombatEnd OnCombatEnd;
	UPROPERTY(BlueprintAssignable, Category = "CombatSystem")
	FPostAttack OnPostTakeAttack;
	UPROPERTY(BlueprintAssignable, Category = "CombatSystem")
	FPreAttack OnPreTakeAttack;

	UFUNCTION(BlueprintCallable)
	void TakeAttack(AActor* Target,TSubclassOf<UGameplayEffect> AttackEffect, TSubclassOf<UGameplayAbility> ReactAbility);
	UFUNCTION(BlueprintCallable)
	void TakeAttackUseCurrent(AActor* Target);
	UFUNCTION(BlueprintCallable)
	void TakeAttackOnlyEffect(AActor* Target);

	UFUNCTION(BlueprintCallable)
	void AttackInfo(const FAttackInfo& AttackInfo);
	void AttackInfo(TSubclassOf<UGameplayEffect> AttackEffect, TSubclassOf<UGameplayAbility> ReactAbility);

	UFUNCTION(BlueprintPure, Category = Combat)
		static UCombatSystemComponent* GetCombatSystemComponent(const AActor* Actor);
	UFUNCTION()
		void BindTakeAttack();
private:
	UPROPERTY(Category = Combat, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UGSCAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(Category = Combat, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> CurrentAttackEffect;

	UPROPERTY(Category = Combat, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayAbility> CurrentReactAbility;

	static UCombatSystemComponent* GetCombatSystemComponentFromActor(const AActor* Actor, bool LookForComponent = true);
};
