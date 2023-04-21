// Fill out your copyright notice in the Description page of Project Settings.


#include "System/CombatSystemComponent.h"
#include "Abilities/GSCAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/GSCBlueprintFunctionLibrary.h"
#include "System/CombatSystemInterface.h"
#include "System/Combat/CombatSystemLibrary.h"

// Sets default values for this component's properties
UCombatSystemComponent::UCombatSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


void UCombatSystemComponent::RandomMonsterParrying()
{
	for (int i = 0; i < 3; i++) {
	auto rate = UCombatSystemLibrary::CalculateProbability_BinomialDistribution(3, ParryingSuccessProbability, i) * 100;

	UE_LOG(LogTemp, Warning, TEXT("SuccessRate : %f"), rate);
	}
	/*for (int i = 0; i < 3; i++) {
		auto rate = UCombatSystemLibrary::CalculateProbability_BinomialDistribution(1, ParryingSuccessProbability, i) * 100;

		UE_LOG(LogTemp, Warning, TEXT("SuccessRate : %f"), rate);
	}*/
	auto per = FMath::RandRange(0, 100);

	UE_LOG(LogTemp, Warning, TEXT("Per : %d"), per);
	UE_LOG(LogTemp, Warning, TEXT("CalculateProbability_BinomialDistribution : %f"), UCombatSystemLibrary::CalculateProbability_BinomialDistribution(3, ParryingSuccessProbability, ParryingNumSuccess)*100);
	if (per >= (UCombatSystemLibrary::CalculateProbability_BinomialDistribution(3,ParryingSuccessProbability,ParryingNumSuccess)*100)){
		ParryingNumSuccess++;
		//�и� ���� 
		UE_LOG(LogTemp, Warning, TEXT("Random Attack Parrying"));

		UE_LOG(LogTemp, Warning, TEXT("SuccessNum : %d"), ParryingNumSuccess);
	}
	else {
		ParryingNumSuccess = 0;
		ParryingSuccessProbability = ParryingSuccessProbability_base;
	}
}

// Called when the game starts
void UCombatSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	AbilitySystemComponent = Cast<UGSCAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()));

	OnTakeAttack.AddDynamic(this, &UCombatSystemComponent::BindTakeAttack);

	ParryingSuccessProbability_base = ParryingSuccessProbability;
}


// Called every frame
void UCombatSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCombatSystemComponent::TakeAttack(AActor* Target, TSubclassOf<UGameplayEffect> AttackEffect, TSubclassOf<UGameplayAbility> ReactAbility)
{
	auto context = AbilitySystemComponent->MakeEffectContext();
	auto TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (TargetAbilitySystemComponent) {
		if (AttackEffect)
			TargetAbilitySystemComponent->ApplyGameplayEffectToSelf(AttackEffect.GetDefaultObject(), 0.f, context);
		if (ReactAbility)
			TargetAbilitySystemComponent->GiveAbility(ReactAbility.GetDefaultObject());
	}
}

void UCombatSystemComponent::TakeAttackUseCurrent(AActor* Target)
{
	TakeAttack(Target, CurrentAttackEffect, CurrentReactAbility);
}

void UCombatSystemComponent::TakeAttackOnlyEffect(AActor* Target)
{
	TakeAttack(Target, CurrentAttackEffect, nullptr);
}



void UCombatSystemComponent::AttackInfo(const FAttackInfo& AttackInfo)
{
	CurrentAttackEffect = AttackInfo.Effect;
	CurrentReactAbility = AttackInfo.Ability;
}

void UCombatSystemComponent::AttackInfo(TSubclassOf<UGameplayEffect> AttackEffect, TSubclassOf<UGameplayAbility> ReactAbility)
{
	CurrentAttackEffect = AttackEffect;
	CurrentReactAbility = ReactAbility;
}

UCombatSystemComponent* UCombatSystemComponent::GetCombatSystemComponentFromActor(const AActor* Actor, bool LookForComponent)
{
	if (Actor == nullptr)
	{
		return nullptr;
	}

	const ICombatSystemInterface* CSI = Cast<ICombatSystemInterface>(Actor);
	if (CSI)
	{
		return CSI->GetCombatSystemComponent();
	}

	if (LookForComponent)
	{
		// Fall back to a component search to better support BP-only actors
		return Actor->FindComponentByClass<UCombatSystemComponent>();
	}
	return nullptr;
}

UCombatSystemComponent* UCombatSystemComponent::GetCombatSystemComponent(const AActor* Actor)
{
	return GetCombatSystemComponentFromActor(Actor);
}

void UCombatSystemComponent::BindTakeAttack()
{
	//TakeAttack(CurrentAttackEffect, CurrentReactAbility);
}

