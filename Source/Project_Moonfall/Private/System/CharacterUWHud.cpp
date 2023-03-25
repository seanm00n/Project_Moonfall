// Fill out your copyright notice in the Description page of Project Settings.


#include "System/CharacterUWHud.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "System/Attributes/LotusAttributeSet.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "GSCLog.h"
void UCharacterUWHud::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCharacterUWHud::NativeDestruct()
{
	ResetAbilitySystem();

	Super::NativeDestruct();
}

void UCharacterUWHud::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Lazy init disabled, was either good from native construct or initialized once here
	if (!bLazyAbilitySystemInitialization)
	{
		return;
	}

	if (TryInitAbilitySystem())
	{
		// We now have proper ASC and initialized widget, prevent further tick run
		bLazyAbilitySystemInitialization = false;

		GSC_LOG(Warning, TEXT("UGSCUWHud::NativeTick reconciliated with ASC. We now have a reference to it: %s (%s)"), *GetNameSafe(AbilitySystemComponent), *GetNameSafe(OwnerActor))
	}
}

void UCharacterUWHud::InitFromCharacter()
{
	if (!AbilitySystemComponent)
	{
		// Prevent calls to GetAttributeValue if no ASC yet, which may happen for clients (and prevent warning logs during initialization)
		return;
	}
	Super::InitFromCharacter();
	SetLotus(GetAttributeValue(ULotusAttributeSet::GetLotusAttribute()));
}

void UCharacterUWHud::SetMaxLotus(float MaxLotus)
{
	const float Lotus = GetAttributeValue(ULotusAttributeSet::GetLotusAttribute());
	if (LotusText)
	{
		LotusText->SetText(FText::FromString(GetAttributeFormatString(Lotus, MaxLotus)));
	}

	if (MaxLotus != 0)
	{
		SetLotusPercentage(Lotus / MaxLotus);
	}
}

void UCharacterUWHud::SetLotus(float Lotus)
{
	const float MaxLotus = GetAttributeValue(ULotusAttributeSet::GetMaxLotusAttribute());
	if (LotusText)
	{
		LotusText->SetText(FText::FromString(GetAttributeFormatString(Lotus, MaxLotus)));
	}

	if (MaxLotus != 0)
	{
		SetLotusPercentage(Lotus / MaxLotus);
	}
}

void UCharacterUWHud::SetLotusPercentage(float LotusPercentage)
{
	if (LotusProgressBar)
	{
		LotusProgressBar->SetPercent(LotusPercentage);
	}
}

void UCharacterUWHud::HandleAttributeChange(FGameplayAttribute Attribute, float NewValue, float OldValue)
{
	if (Attribute == ULotusAttributeSet::GetLotusAttribute())
	{
		SetLotus(NewValue);
	}
	else if (Attribute == ULotusAttributeSet::GetMaxLotusAttribute())
	{
		SetMaxLotus(NewValue);
	}
	Super::HandleAttributeChange(Attribute, NewValue, OldValue);
}

FString UCharacterUWHud::GetAttributeFormatString(float BaseValue, float MaxValue)
{
	return FString::Printf(TEXT("%d / %d"), FMath::FloorToInt(BaseValue), FMath::FloorToInt(MaxValue));
}

bool UCharacterUWHud::TryInitAbilitySystem()
{
	GSC_LOG(Verbose, TEXT("UGSCUWHud::TryInitAbilitySystem check for ASC: %s (%s)"), *GetNameSafe(AbilitySystemComponent), *GetNameSafe(OwnerActor))

		if (!AbilitySystemComponent)
		{
			AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerActor);
		}

	if (AbilitySystemComponent)
	{
		bLazyAbilitySystemInitialization = false;

		GSC_LOG(Verbose, TEXT("UGSCUWHud::TryInitAbilitySystem check found a new ASC: %s (%s)"), *GetNameSafe(AbilitySystemComponent), *GetNameSafe(OwnerActor))

			InitializeWithAbilitySystem(AbilitySystemComponent);

		// Init Stats
		InitFromCharacter();
	}

	return AbilitySystemComponent != nullptr;
}
