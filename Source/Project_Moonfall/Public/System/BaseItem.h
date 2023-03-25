// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inspection/InteractionInterface.h"
#include "BaseItem.generated.h"

UCLASS()
class PROJECT_MOONFALL_API ABaseItem : public AActor ,public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// IInteractionInterface을(를) 통해 상속됨
	void LookAt_Implementation(ACharacter* Player) override;

	void InteractWith_Implementation(ACharacter* Player) override;

};
