// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "System/DitItHitCompInterface.h"
#include "System/CombatSystemInterface.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "FightingCharacter.generated.h"

class UGSCAbilitySystemComponent;
class UInputMappingContext;
class UInputAction;

USTRUCT(BlueprintType)
struct FCommand {
	GENERATED_BODY();
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
	FString name;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
	TArray<FString> inputs;
};


USTRUCT(BlueprintType)
struct FInputInfo {
	GENERATED_BODY();
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
	FString inputName;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
	float timeStamp;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMouseLeftEvent);

UCLASS(config=Game, Blueprintable)
class PROJECT_MOONFALL_API AFightingCharacter : public ACharacter, public IAbilitySystemInterface ,public IDitItHitCompInterface,public ICombatSystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;



	void StartAttack1();
	void StartAttack2();
	void StartAttack3();
	void StartAttack4();

public:
	AFightingCharacter(const FObjectInitializer& ObjectInitializer);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

	UPROPERTY(BlueprintAssignable)
	FOnMouseLeftEvent OnMouseLeftEvent;

	//Anim 블렌드 스페이스용 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	FVector2D MoveVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	bool isRun;

protected:


	void Move(const FInputActionValue& Value);
	void Run(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface



	UPROPERTY(EditAnyWhere,  Category = Input)
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnyWhere, Category = Input)
	UInputAction* MovementAction;
	UPROPERTY(EditAnyWhere, Category = Input)
	UInputAction* RunAction;
	UPROPERTY(EditAnyWhere, Category = Input)
	UInputAction* LookAction;
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }



	UPROPERTY(EditDefaultsOnly, Category = "GAS Companion|Ability System")
	EGameplayEffectReplicationMode ReplicationMode = EGameplayEffectReplicationMode::Mixed;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UGSCAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCustomDitItHitActorComponent* CustomDitItHitActorComponent;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCombatSystemComponent* CombatSystemComponent;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual UCustomDitItHitActorComponent* GetDitItHit() const override;

	virtual UCombatSystemComponent* GetCombatSystemComponent() const override;

	TSubclassOf<UGameplayEffect> CurrentDamageEffect;


	//~ Begin AActor Interface
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostInitProperties() override;
	//~ End AActor Interface
	UFUNCTION(BlueprintImplementableEvent)
	void Attack1(); 
	UFUNCTION(BlueprintImplementableEvent)
	void Attack2(); 
	UFUNCTION(BlueprintImplementableEvent)
	void Attack3();
	UFUNCTION(BlueprintImplementableEvent)
	void Attack4();


	//
	/*UFUNCTION(BlueprintImplementableEvent)
	UFUNCTION(BlueprintImplementableEvent)
	UFUNCTION(BlueprintImplementableEvent)
	UFUNCTION(BlueprintImplementableEvent)*/

	//Attack Buffer

	UFUNCTION(BlueprintCallable)
	void AddAttackBuffer(FKey _key);


	TQueue<FKey> AttackBuffers;

	UFUNCTION(BlueprintCallable)
	void CheckAttackBuffer();

	//inputBuffer
	UFUNCTION(BlueprintCallable)
	void AddInputToInputBuffer(FInputInfo _inputInfo);

	//UFUNCTION(BlueprintCallable)
	//void CheckInputBufferForCommand();

	//UFUNCTION(BlueprintCallable)
	//void StartCommand(FString _commandName);

	UFUNCTION(BlueprintCallable)
	void RemoveInputFromInputBuffer();

	FCommand tempCommand;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Input)
	TArray<FInputInfo> inputBuffer;

	

	FTimerHandle inputBufferTimerHandle;

	float removeInputFromBufferTime = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	bool hasUsedTempCommand;

};

