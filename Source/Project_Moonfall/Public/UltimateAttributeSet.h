// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Attributes/GSCAttributeSetBase.h"
#include "AbilitySystemComponent.h"
#include "UltimateAttributeSet.generated.h"

UCLASS()
class PROJECT_MOONFALL_API UUltimateAttributeSet : public UGSCAttributeSetBase
{
	GENERATED_BODY()

public:

	// Sets default values for this AttributeSet attributes
	UUltimateAttributeSet();

    // AttributeSet Overrides
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
        
    UPROPERTY(BlueprintReadOnly, Category = "Attribute|Lotus", ReplicatedUsing = OnRep_Lotus)
    FGameplayAttributeData Lotus = 0.0;
    ATTRIBUTE_ACCESSORS(UUltimateAttributeSet, Lotus)    
    
    UPROPERTY(BlueprintReadOnly, Category = "Attribute|Lotus", ReplicatedUsing = OnRep_MaxLotus)
    FGameplayAttributeData MaxLotus = 5.0;
    ATTRIBUTE_ACCESSORS(UUltimateAttributeSet, MaxLotus)    

protected:
    
    UFUNCTION()
    virtual void OnRep_Lotus(const FGameplayAttributeData& OldLotus);

    UFUNCTION()
    virtual void OnRep_MaxLotus(const FGameplayAttributeData& OldMaxLotus);
	
	
};
