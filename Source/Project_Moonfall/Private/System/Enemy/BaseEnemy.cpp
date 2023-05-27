// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Enemy/BaseEnemy.h"

#include "Components/GSCCoreComponent.h"
#include "Abilities/GSCAbilitySystemComponent.h"
#include "System/CustomDitItHitActorComponent.h"
#include "System/CombatSystemComponent.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GSCCoreComponent = CreateDefaultSubobject<UGSCCoreComponent>(TEXT("GSCCoreComponent"));

	AbilitySystemComponent = CreateDefaultSubobject<UGSCAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	CustomDitItHitActorComponent = CreateDefaultSubobject<UCustomDitItHitActorComponent>(TEXT("CustomDitItHitComponent"));

	CombatSystemComponent = CreateDefaultSubobject<UCombatSystemComponent>(TEXT("CombatSystemComponent"));

	Mesh
}

void ABaseEnemy::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	/*if (HealthWidgetClass) {
		healthWidget = CreateWidget<>(GetWorld(), HealthWidgetClass);
		healthWidget->AddToViewport();
	}*/
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* ABaseEnemy::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UCustomDitItHitActorComponent* ABaseEnemy::GetDitItHit() const
{
	return CustomDitItHitActorComponent;
}

UCombatSystemComponent* ABaseEnemy::GetCombatSystemComponent() const
{
	return CombatSystemComponent;
}

void ABaseEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ABaseEnemy::PostInitProperties()
{
	Super::PostInitProperties();
	if (AbilitySystemComponent)
	{
		// GSC_LOG(Verbose, TEXT("AGSCModularCharacter::PostInitProperties for %s - Setting up ASC Replication Mode to: %d"), *GetName(), ReplicationMode)
		AbilitySystemComponent->SetReplicationMode(ReplicationMode);
	}
}
