// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Attributes/GSCAttributeSetBase.h"
#include "AbilitySystemComponent.h"
#include "EnemyAttributeSet.generated.h"

UCLASS()
class PROJECT_MOONFALL_API UEnemyAttributeSet : public UGSCAttributeSetBase
{
	GENERATED_BODY()

public:

	// Sets default values for this AttributeSet attributes
	UEnemyAttributeSet();

    // AttributeSet Overrides
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
        
    UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_AttackPoint)
    FGameplayAttributeData AttackPoint = 0.0;
    ATTRIBUTE_ACCESSORS(UEnemyAttributeSet, AttackPoint)    
    
    UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_AttackRange)
    FGameplayAttributeData AttackRange = 0.0;
    ATTRIBUTE_ACCESSORS(UEnemyAttributeSet, AttackRange)    
    
    UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_Stack)
    FGameplayAttributeData Stack = 0.0;
    ATTRIBUTE_ACCESSORS(UEnemyAttributeSet, Stack)    

protected:
    
    UFUNCTION()
    virtual void OnRep_AttackPoint(const FGameplayAttributeData& OldAttackPoint);

    UFUNCTION()
    virtual void OnRep_AttackRange(const FGameplayAttributeData& OldAttackRange);

    UFUNCTION()
    virtual void OnRep_Stack(const FGameplayAttributeData& OldStack);
	
	
};
