// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Attributes/GSCAttributeSetBase.h"
#include "AbilitySystemComponent.h"
#include "MonsterAttributeSet.generated.h"

UCLASS()
class PROJECT_MOONFALL_API UMonsterAttributeSet : public UGSCAttributeSetBase
{
	GENERATED_BODY()

public:

	// Sets default values for this AttributeSet attributes
	UMonsterAttributeSet();

    // AttributeSet Overrides
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
        
    UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_HealthPoint)
    FGameplayAttributeData HealthPoint = 0.0;
    ATTRIBUTE_ACCESSORS(UMonsterAttributeSet, HealthPoint)    
    
    UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_AttackPoint)
    FGameplayAttributeData AttackPoint = 0.0;
    ATTRIBUTE_ACCESSORS(UMonsterAttributeSet, AttackPoint)    
    
    UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_Stack)
    FGameplayAttributeData Stack = 0.0;
    ATTRIBUTE_ACCESSORS(UMonsterAttributeSet, Stack)    
    
    UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_Phase)
    FGameplayAttributeData Phase = 0.0;
    ATTRIBUTE_ACCESSORS(UMonsterAttributeSet, Phase)    

protected:
    
    UFUNCTION()
    virtual void OnRep_HealthPoint(const FGameplayAttributeData& OldHealthPoint);

    UFUNCTION()
    virtual void OnRep_AttackPoint(const FGameplayAttributeData& OldAttackPoint);

    UFUNCTION()
    virtual void OnRep_Stack(const FGameplayAttributeData& OldStack);

    UFUNCTION()
    virtual void OnRep_Phase(const FGameplayAttributeData& OldPhase);
	
	
};
