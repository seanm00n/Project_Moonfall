// Fill out your copyright notice in the Description page of Project Settings.


#include "UltimateAttributeSet.h"

#include "Net/UnrealNetwork.h"

// Sets default values
UUltimateAttributeSet::UUltimateAttributeSet()
{
	// Default constructor
}

void UUltimateAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
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

void UUltimateAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
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

void UUltimateAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
        
    DOREPLIFETIME_CONDITION_NOTIFY(UUltimateAttributeSet, Lotus, COND_None, REPNOTIFY_Always);
    
    DOREPLIFETIME_CONDITION_NOTIFY(UUltimateAttributeSet, MaxLotus, COND_None, REPNOTIFY_Always);
}

void UUltimateAttributeSet::OnRep_Lotus(const FGameplayAttributeData& OldLotus)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UUltimateAttributeSet, Lotus, OldLotus);
}

void UUltimateAttributeSet::OnRep_MaxLotus(const FGameplayAttributeData& OldMaxLotus)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UUltimateAttributeSet, MaxLotus, OldMaxLotus);
}
