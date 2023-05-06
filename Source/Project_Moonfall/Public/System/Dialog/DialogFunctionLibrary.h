// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DialogFunctionLibrary.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FDialog {
	GENERATED_BODY();
	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	FString name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	FString texts;
};



UCLASS()
class PROJECT_MOONFALL_API UDialogFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
