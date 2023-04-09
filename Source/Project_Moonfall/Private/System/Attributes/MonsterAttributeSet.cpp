// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Attributes/MonsterAttributeSet.h"

#include "Net/UnrealNetwork.h"

// Sets default values
UMonsterAttributeSet::UMonsterAttributeSet()
{
	// Default constructor
}

void UMonsterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
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

void UMonsterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
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

void UMonsterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
        
    DOREPLIFETIME_CONDITION_NOTIFY(UMonsterAttributeSet, HealthPoint, COND_None, REPNOTIFY_Always);
    
    DOREPLIFETIME_CONDITION_NOTIFY(UMonsterAttributeSet, AttackPoint, COND_None, REPNOTIFY_Always);
    
    DOREPLIFETIME_CONDITION_NOTIFY(UMonsterAttributeSet, Stack, COND_None, REPNOTIFY_Always);
    
    DOREPLIFETIME_CONDITION_NOTIFY(UMonsterAttributeSet, Phase, COND_None, REPNOTIFY_Always);
}

void UMonsterAttributeSet::OnRep_HealthPoint(const FGameplayAttributeData& OldHealthPoint)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UMonsterAttributeSet, HealthPoint, OldHealthPoint);
}

void UMonsterAttributeSet::OnRep_AttackPoint(const FGameplayAttributeData& OldAttackPoint)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UMonsterAttributeSet, AttackPoint, OldAttackPoint);
}

void UMonsterAttributeSet::OnRep_Stack(const FGameplayAttributeData& OldStack)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UMonsterAttributeSet, Stack, OldStack);
}

void UMonsterAttributeSet::OnRep_Phase(const FGameplayAttributeData& OldPhase)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UMonsterAttributeSet, Phase, OldPhase);
}
