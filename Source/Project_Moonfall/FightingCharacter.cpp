// Copyright Epic Games, Inc. All Rights Reserved.

#include "FightingCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Abilities/GSCAbilitySystemComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

//////////////////////////////////////////////////////////////////////////
// AFightingCharacter


AFightingCharacter::AFightingCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
{

	AbilitySystemComponent = CreateDefaultSubobject<UGSCAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	tempCommand.name = "Temp Command";
	tempCommand.inputs.Add("A");
	tempCommand.inputs.Add("B");
	tempCommand.inputs.Add("C");
	hasUsedTempCommand = false;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFightingCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{


	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AFightingCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFightingCharacter::Look);
	}

	// Set up gameplay key bindings
	check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
//	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	/*PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AFightingCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AFightingCharacter::MoveRight);*/

	//Attack

	PlayerInputComponent->BindAction("Attack1", IE_Pressed, this, &AFightingCharacter::StartAttack1);

	PlayerInputComponent->BindAction("Attack2", IE_Pressed, this, &AFightingCharacter::StartAttack2);

	PlayerInputComponent->BindAction("Attack3", IE_Pressed, this, &AFightingCharacter::StartAttack3);

	PlayerInputComponent->BindAction("Attack4", IE_Pressed, this, &AFightingCharacter::StartAttack4);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	/*PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AFightingCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AFightingCharacter::LookUpAtRate);*/

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AFightingCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AFightingCharacter::TouchStopped);
}

UAbilitySystemComponent* AFightingCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AFightingCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void AFightingCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
}

void AFightingCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AFightingCharacter::PostInitProperties()
{
	Super::PostInitProperties();
	if (AbilitySystemComponent)
	{
		// GSC_LOG(Verbose, TEXT("AGSCModularCharacter::PostInitProperties for %s - Setting up ASC Replication Mode to: %d"), *GetName(), ReplicationMode)
		AbilitySystemComponent->SetReplicationMode(ReplicationMode);
	}
}

void AFightingCharacter::AddAttackBuffer(FKey _key)
{
	AttackBuffers.Enqueue(_key);
}

void AFightingCharacter::CheckAttackBuffer()
{

	while (!AttackBuffers.IsEmpty()) {
		FKey InputKey;
		AttackBuffers.Dequeue(InputKey);
		
		UE_LOG(LogTemp, Warning, TEXT("AttackBuffer Check is %s"), *InputKey.ToString());
		if (InputKey == EKeys::LeftMouseButton) {
			UE_LOG(LogTemp, Warning, TEXT("AttackBuffer Check is LeftMouseButton success"));
			auto animInst =
				GetMesh()->GetAnimInstance();
			auto MontagePosition = animInst->Montage_GetPosition(animInst->GetCurrentActiveMontage());
			if (MontagePosition > 0.5f) {
				OnMouseLeftEvent.Broadcast();
			}
		}
	}
}
void AFightingCharacter::AddInputToInputBuffer(FInputInfo _inputInfo)
{
	inputBuffer.Add(_inputInfo);
	CheckInputBufferForCommand();
}

void AFightingCharacter::CheckInputBufferForCommand()
{
	int correnctSequenceCounter = 0;
	for (auto skillCommand : skillCommands) {
		for (int commandInput = 0; commandInput < skillCommand.ketInputs.Num(); ++commandInput) {
			for (int input = 0; input < inputBuffer.Num(); ++input) {
				if (input + correnctSequenceCounter < inputBuffer.Num()) {
					if (inputBuffer[input + correnctSequenceCounter].inputName.Compare(skillCommand.ketInputs[commandInput].ToString()) == 0) {
						UE_LOG(LogTemp, Warning, TEXT("The Player added another input to the command sequence"));
						++correnctSequenceCounter;
						if (correnctSequenceCounter == skillCommand.ketInputs.Num()) {
							StartCommand(skillCommand.name);
						}
						break;
					}
					else {
						UE_LOG(LogTemp, Warning, TEXT("The Player broke the command sequence."));
						correnctSequenceCounter = 0;
					}
				}
				else {
					UE_LOG(LogTemp, Warning, TEXT("The Player is not yet finished with the Command sequence"));
					correnctSequenceCounter = 0;
				}
			}
		}
	}
}

void AFightingCharacter::StartCommand(FString _commandName)
{
	for (int currentCommand = 0; currentCommand < skillCommands.Num(); ++currentCommand) {
		if (_commandName.Compare(skillCommands[currentCommand].name) == 0) {
			UE_LOG(LogTemp, Warning, TEXT("The Character is using the command : %s"), *_commandName);
			hasUsedTempCommand = true;
		}
	}
}

void AFightingCharacter::RemoveInputFromInputBuffer()
{
}

void AFightingCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AFightingCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AFightingCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AFightingCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AFightingCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);

	// add movement in that direction
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);

	
}

void AFightingCharacter::Look(const FInputActionValue& Value)
{

	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void AFightingCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AFightingCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AFightingCharacter::StartAttack1()
{
	UE_LOG(LogTemp, Warning, TEXT("AFightingCharacter::StartAttack1"));
	Attack1();
}

void AFightingCharacter::StartAttack2()
{
	UE_LOG(LogTemp, Warning, TEXT("AFightingCharacter::StartAttack2"));
	Attack2();
}

void AFightingCharacter::StartAttack3()
{
	UE_LOG(LogTemp, Warning, TEXT("AFightingCharacter::StartAttack3"));
	Attack3();
}

void AFightingCharacter::StartAttack4()
{
	UE_LOG(LogTemp, Warning, TEXT("AFightingCharacter::StartAttack4"));
	Attack4();
}

