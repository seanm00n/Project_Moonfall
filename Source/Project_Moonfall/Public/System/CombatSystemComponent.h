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



	UFUNCTION()
	bool RandomMonsterParrying();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
private:
	//Function
	void Success_Parrying();
	void Failure_Parrying();
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:


	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="CombatSystem|Montage")
	TMap<FString, UAnimMontage*> CombatMontage;

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
	//Probability
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatSystem|Probability", meta = (AllowPrivateAccess = "true"))
	float ParryingSuccessProbability = 0.2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatSystem|Probability", meta = (AllowPrivateAccess = "true"))
	float ParryingSuccessWeight = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatSystem|Probability", meta = (AllowPrivateAccess = "true"))
	int MaxParryingSuccess = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatSystem|Probability", meta = (AllowPrivateAccess = "true"))
	bool bUseSuccessWeight = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatSystem|Probability", meta = (AllowPrivateAccess = "true"))
	bool bUseAutoParry = false;

	float ParryingSuccessProbability_base = ParryingSuccessProbability;

	int ParryingNumSuccess = 0;
	
	//Montage|CombatAI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatSystem|Montage|CombatAI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UGameplayAbility> ParryingAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatSystem|Montage|CombatAI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UGameplayAbility> HitReactAbility;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatSystem|Montage|CombatAI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UGameplayAbility> GroggyAbility;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatSystem|Montage|CombatAI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UGameplayAbility> CriticalHitReactAbility;


	UPROPERTY(Category = Combat, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UGSCAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(Category = Combat, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> CurrentAttackEffect;

	UPROPERTY(Category = Combat, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayAbility> CurrentReactAbility;

	static UCombatSystemComponent* GetCombatSystemComponentFromActor(const AActor* Actor, bool LookForComponent = true);
};
