// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Attributes/EnemyAttributeSet.h"

#include "Net/UnrealNetwork.h"

// Sets default values
UEnemyAttributeSet::UEnemyAttributeSet()
{
	// Default constructor
}

void UEnemyAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    // This is called whenever attributes change, so for max attributes we want to scale the current totals to match
    Super::PreAttributeChange(Attribute, NewValue);

    // Set adjust code here
    //
    // Example:
    //
    // If a Max value changes, adjust current to keep Current % of Current to Max
    //
    // if (Attribute == GetMaxHealthAttribute())
    // {
    //     AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
    // }
}

void UEnemyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    FGSCAttributeSetExecutionData ExecutionData;
    GetExecutionDataFromMod(Data, ExecutionData);

    // Set clamping or handling or "meta" attributes here (like damages)

    // Example 1: Using helpers to handle each attribute in their own methods (See GSCAttributeSet.cpp)

    // if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    // {
    //     HandleHealthAttribute(ExecutionData);
    // }

    // Example 2: Basic example to clamp the value of an Health Attribute between 0 and another MaxHealth Attribute

    // if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    // {
    //     SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
    // }
}

void UEnemyAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
        
    DOREPLIFETIME_CONDITION_NOTIFY(UEnemyAttributeSet, AttackPoint, COND_None, REPNOTIFY_Always);
    
    DOREPLIFETIME_CONDITION_NOTIFY(UEnemyAttributeSet, AttackRange, COND_None, REPNOTIFY_Always);
    
    DOREPLIFETIME_CONDITION_NOTIFY(UEnemyAttributeSet, Stack, COND_None, REPNOTIFY_Always);
}

void UEnemyAttributeSet::OnRep_AttackPoint(const FGameplayAttributeData& OldAttackPoint)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UEnemyAttributeSet, AttackPoint, OldAttackPoint);
}

void UEnemyAttributeSet::OnRep_AttackRange(const FGameplayAttributeData& OldAttackRange)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UEnemyAttributeSet, AttackRange, OldAttackRange);
}

void UEnemyAttributeSet::OnRep_Stack(const FGameplayAttributeData& OldStack)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UEnemyAttributeSet, Stack, OldStack);
}
