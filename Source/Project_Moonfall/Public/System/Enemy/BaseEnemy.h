// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "System/DitItHitCompInterface.h"
#include "System/CombatSystemInterface.h"

#include "BaseEnemy.generated.h"

UCLASS()
class PROJECT_MOONFALL_API ABaseEnemy : public ACharacter, public IAbilitySystemInterface, public IDitItHitCompInterface, public ICombatSystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostInitProperties() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "GAS Companion|Ability System")
	EGameplayEffectReplicationMode ReplicationMode = EGameplayEffectReplicationMode::Mixed;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UGSCAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCustomDitItHitActorComponent* CustomDitItHitActorComponent;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCombatSystemComponent* CombatSystemComponent;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual UCustomDitItHitActorComponent* GetDitItHit() const override;

	virtual UCombatSystemComponent* GetCombatSystemComponent() const override;
};
