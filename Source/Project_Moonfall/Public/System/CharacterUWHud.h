// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/GSCUWHud.h"
#include "GameplayEffectTypes.h"

#include "CharacterUWHud.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MOONFALL_API UCharacterUWHud : public UGSCUWHud
{
	GENERATED_BODY()
	
protected:
	//~ Begin UUserWidget interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//~ End UUserWidget interface

public:
	/** Init widget with attributes from owner character **/
	virtual void InitFromCharacter() override;

	/** Updates bound health widgets to reflect the new max health change */
	UFUNCTION(BlueprintCallable, Category = "GAS Companion|UI")
	virtual void SetMaxLotus(float MaxLotus);

	/** Updates bound health widgets to reflect the new health change */
	UFUNCTION(BlueprintCallable, Category = "GAS Companion|UI")
	virtual void SetLotus(float Lotus);

	/** Updates bound stamina progress bar with the new percent */
	UFUNCTION(BlueprintCallable, Category = "GAS Companion|UI")
	virtual void SetLotusPercentage(float LotusPercentage);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "GAS Companion|UI")
	UTextBlock* LotusText = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "GAS Companion|UI")
	UProgressBar* LotusProgressBar = nullptr;

	/** Updates bound widget whenever one of the attribute we care about is changed */
	virtual void HandleAttributeChange(FGameplayAttribute Attribute, float NewValue, float OldValue) override;


private:
	///** Array of active GE handle bound to delegates that will be fired when the count for the key tag changes to or away from zero */
	//TArray<FActiveGameplayEffectHandle> GameplayEffectAddedHandles;

	///** Array of tags bound to delegates that will be fired when the count for the key tag changes to or away from zero */
	//TArray<FGameplayTag> GameplayTagBoundToDelegates;

	static FString GetAttributeFormatString(float BaseValue, float MaxValue);

	/**
	 * Checks owner for a valid ASC and kick in initialization logic if it finds one
	 *
	 * @return Whether an ASC was found
	 */
	bool TryInitAbilitySystem();
};
