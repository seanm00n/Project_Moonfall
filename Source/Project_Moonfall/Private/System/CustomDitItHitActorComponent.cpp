// Copyright 2021, Project Zero. All Rights Reserved.


/*All the time you're saying to yourself,
'I could do that, but I won't,' ?
which is just another way of saying that you can't.*/

#include "System/CustomDitItHitActorComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "System/DitItHitCompInterface.h"
#include "System/Attributes/LotusAttributeSet.h"
#include "Kismet/KismetSystemLibrary.h"




// Sets default values for this component's properties
UCustomDitItHitActorComponent::UCustomDitItHitActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	// ...
}


// Called when the game starts
void UCustomDitItHitActorComponent::BeginPlay()
{
	Super::BeginPlay();
	MyWorldContextObject = GetOwner();
}

/*To every man is given the key to the gates of heaven.
The same key opens the gates of hell.
And so it is with science.*/

// Called every frame
void UCustomDitItHitActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CanTrace)
	{
		if (ModuloNumber % ModuloSkip == 0)
		{
			if (bHitSameSocketAtDifferentTimes)
			{
				HitSameSocketAtDifferentTimes();
			}

			if (bHitOtherSocketsAtSameTime)
			{
				HitOtherSocketsAtSameTime();
			}

			if (bHitOtherSocketsAtDifferentTime)
			{
				HitOtherSocketsAtDifferentTime();
			}
		}

		UpdateLastSocketLocation();
		ModuloNumber++;
	}
}

void UCustomDitItHitActorComponent::HitSameSocketAtDifferentTimes()
{
	if (UseKismet)
	{
		for (auto Socket : MySockets)
		{
			//Private Variables
			const FVector* Start = LastKnownSocketLocation.Find(Socket);
			const FVector End = MyPrimitive->GetSocketLocation(Socket);
			TArray<FHitResult> OutHits;

			if (TraceByChannelOrObjects)
			{
				switch (MyKismetTraceType)
				{
				case ECustomKismetTraceType::LineTrace:
				{
					UKismetSystemLibrary::LineTraceMulti(MyWorldContextObject, *Start, End, MyTraceChannel, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
					AddHitToHitArray(OutHits);  break;
				}

				case ECustomKismetTraceType::BoxTrace:
				{
					UKismetSystemLibrary::BoxTraceMulti(MyWorldContextObject, *Start, End, BoxHalfSize, BoxOrientation, MyTraceChannel, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
					break;
				}

				case ECustomKismetTraceType::CapsuleTrace:
				{
					UKismetSystemLibrary::CapsuleTraceMulti(MyWorldContextObject, *Start, End, CapsuleHalfHeight, CapsuleRadius, MyTraceChannel, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
					break;
				}

				case ECustomKismetTraceType::SphereTrace:
				{
					UKismetSystemLibrary::SphereTraceMulti(MyWorldContextObject, *Start, End, SphereRadius, MyTraceChannel, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
					break;
				}
				}
			}
			else if (!TraceByChannelOrObjects)
			{
				switch (MyKismetTraceType)
				{
				case ECustomKismetTraceType::LineTrace:
				{
					UKismetSystemLibrary::LineTraceMultiForObjects(MyWorldContextObject, *Start, End, MyObjectTypesToHit, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
					AddHitToHitArray(OutHits);  break;
				}

				case ECustomKismetTraceType::BoxTrace:
				{
					UKismetSystemLibrary::BoxTraceMultiForObjects(MyWorldContextObject, *Start, End, BoxHalfSize, BoxOrientation, MyObjectTypesToHit, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
					break;
				}

				case ECustomKismetTraceType::CapsuleTrace:
				{
					UKismetSystemLibrary::CapsuleTraceMultiForObjects(MyWorldContextObject, *Start, End, CapsuleHalfHeight, CapsuleRadius, MyObjectTypesToHit, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
					break;
				}

				case ECustomKismetTraceType::SphereTrace:
				{
					UKismetSystemLibrary::SphereTraceMultiForObjects(MyWorldContextObject, *Start, End, SphereRadius, MyObjectTypesToHit, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
					break;
				}
				}
			}
			AddHitToHitArray(OutHits);
		}
	}
	else if (!UseKismet)
	{
		//Unfinished - requires more tweaking to optimize
		//for (auto Socket : MySockets)
		//{
		//	TArray<FHitResult> OutHits;
		//	FCollisionQueryParams TraceParams;
		//	FVector* TraceStart = LastKnownSocketLocation.Find(Socket);
		//	FVector TraceEnd = MyPrimitive->GetSocketLocation(Socket);

		//	GetWorld()->LineTraceMultiByObjectType(OutHits, *TraceStart, TraceEnd, MyObjectTypesToHit, TraceParams);
		//	
		//	GetWorld()->SweepMultiByObjectType(OutHits, *TraceStart, TraceEnd, SweepQuaternion, MyObjectTypesToHit, MyCollisionShape, TraceParams);
		//	AddHitToHitArray(OutHits);
		//}
	}
}

void UCustomDitItHitActorComponent::HitOtherSocketsAtSameTime()
{
	//UE_LOG(LogTemp, Warning, TEXT("7"));
	if (UseKismet)
	{
		for (auto Socket1 : MySockets)
		{
			for (auto Socket2 : MySockets)
			{
				//Private Variables
				const FVector Start = MyPrimitive->GetSocketLocation(Socket1);
				const FVector End = MyPrimitive->GetSocketLocation(Socket2);
				TArray<FHitResult> OutHits;

				if (TraceByChannelOrObjects)
				{
					switch (MyKismetTraceType)
					{
					case ECustomKismetTraceType::LineTrace:
					{
						UKismetSystemLibrary::LineTraceMulti(MyWorldContextObject, Start, End, MyTraceChannel, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						AddHitToHitArray(OutHits); break;
					}

					case ECustomKismetTraceType::BoxTrace:
					{
						UKismetSystemLibrary::BoxTraceMulti(MyWorldContextObject, Start, End, BoxHalfSize, BoxOrientation, MyTraceChannel, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						break;
					}

					case ECustomKismetTraceType::CapsuleTrace:
					{
						UKismetSystemLibrary::CapsuleTraceMulti(MyWorldContextObject, Start, End, CapsuleHalfHeight, CapsuleRadius, MyTraceChannel, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						break;
					}

					case ECustomKismetTraceType::SphereTrace:
					{
						UKismetSystemLibrary::SphereTraceMulti(MyWorldContextObject, Start, End, SphereRadius, MyTraceChannel, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						break;
					}
					}
				}
				else if (!TraceByChannelOrObjects)
				{
					switch (MyKismetTraceType)
					{
					case ECustomKismetTraceType::LineTrace:
					{
						UKismetSystemLibrary::LineTraceMultiForObjects(MyWorldContextObject, Start, End, MyObjectTypesToHit, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						AddHitToHitArray(OutHits); break;
					}

					case ECustomKismetTraceType::BoxTrace:
					{
						UKismetSystemLibrary::BoxTraceMultiForObjects(MyWorldContextObject, Start, End, BoxHalfSize, BoxOrientation, MyObjectTypesToHit, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						break;
					}

					case ECustomKismetTraceType::CapsuleTrace:
					{
						UKismetSystemLibrary::CapsuleTraceMultiForObjects(MyWorldContextObject, Start, End, CapsuleHalfHeight, CapsuleRadius, MyObjectTypesToHit, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						break;
					}

					case ECustomKismetTraceType::SphereTrace:
					{
						UKismetSystemLibrary::SphereTraceMultiForObjects(MyWorldContextObject, Start, End, SphereRadius, MyObjectTypesToHit, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						break;
					}
					}
				}
				AddHitToHitArray(OutHits);
			}
		}
	}
	else if (!UseKismet) {
		for (auto Socket1 : MySockets)
		{
			for (auto Socket2 : MySockets)
			{

				//Not optimized:
				//TArray<FHitResult> OutHits;
				//FCollisionQueryParams TraceParams = MyTraceParams;
				//FVector TraceStart = MyPrimitive->GetSocketLocation(Socket1);
				//FVector TraceEnd = MyPrimitive->GetSocketLocation(Socket2);
				//TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToHit = MyObjectTypesToHit;

				//GetWorld()->LineTraceMultiByObjectType(OutHits, TraceStart, TraceEnd, ObjectTypesToHit, TraceParams);

				//AddHitToHitArray(OutHits);
			}
		}
	}
}

void UCustomDitItHitActorComponent::HitOtherSocketsAtDifferentTime()
{
	if (UseKismet)
	{
		for (auto Socket1 : MySockets)
		{
			for (auto Socket2 : MySockets)
			{
				TArray<FHitResult> OutHits1;
				FVector Start = MyPrimitive->GetSocketLocation(Socket1);
				FVector* End = LastKnownSocketLocation.Find(Socket2);

				if (TraceByChannelOrObjects)
				{
					switch (MyKismetTraceType)
					{
					case ECustomKismetTraceType::LineTrace:
					{
						UKismetSystemLibrary::LineTraceMulti(MyWorldContextObject, Start, *End, MyTraceChannel, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits1, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						AddHitToHitArray(OutHits1); break;
					}

					case ECustomKismetTraceType::BoxTrace:
					{
						UKismetSystemLibrary::BoxTraceMulti(MyWorldContextObject, Start, *End, BoxHalfSize, BoxOrientation, MyTraceChannel, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits1, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						break;
					}

					case ECustomKismetTraceType::CapsuleTrace:
					{
						UKismetSystemLibrary::CapsuleTraceMulti(MyWorldContextObject, Start, *End, CapsuleHalfHeight, CapsuleRadius, MyTraceChannel, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits1, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						break;
					}

					case ECustomKismetTraceType::SphereTrace:
					{
						UKismetSystemLibrary::SphereTraceMulti(MyWorldContextObject, Start, *End, SphereRadius, MyTraceChannel, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits1, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						break;
					}
					}
				}
				else if (!TraceByChannelOrObjects)
				{
					switch (MyKismetTraceType)
					{
					case ECustomKismetTraceType::LineTrace:
					{
						UKismetSystemLibrary::LineTraceMultiForObjects(MyWorldContextObject, Start, *End, MyObjectTypesToHit, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits1, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						AddHitToHitArray(OutHits1); break;
					}

					case ECustomKismetTraceType::BoxTrace:
					{
						UKismetSystemLibrary::BoxTraceMultiForObjects(MyWorldContextObject, Start, *End, BoxHalfSize, BoxOrientation, MyObjectTypesToHit, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits1, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						break;
					}

					case ECustomKismetTraceType::CapsuleTrace:
					{
						UKismetSystemLibrary::CapsuleTraceMultiForObjects(MyWorldContextObject, Start, *End, CapsuleHalfHeight, CapsuleRadius, MyObjectTypesToHit, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits1, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						break;
					}

					case ECustomKismetTraceType::SphereTrace:
					{
						UKismetSystemLibrary::SphereTraceMultiForObjects(MyWorldContextObject, Start, *End, SphereRadius, MyObjectTypesToHit, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits1, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						break;
					}
					}
				}

				AddHitToHitArray(OutHits1);
			}

			for (auto Socket3 : MySockets)
			{
				TArray<FHitResult> OutHits2;
				FVector* Start = LastKnownSocketLocation.Find(Socket3);
				FVector End = MyPrimitive->GetSocketLocation(Socket1);

				if (TraceByChannelOrObjects)
				{
					switch (MyKismetTraceType)
					{
					case ECustomKismetTraceType::LineTrace:
					{
						UKismetSystemLibrary::LineTraceMulti(MyWorldContextObject, *Start, End, MyTraceChannel, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits2, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						AddHitToHitArray(OutHits2); break;
					}

					case ECustomKismetTraceType::BoxTrace:
					{
						UKismetSystemLibrary::BoxTraceMulti(MyWorldContextObject, *Start, End, BoxHalfSize, BoxOrientation, MyTraceChannel, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits2, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						break;
					}

					case ECustomKismetTraceType::CapsuleTrace:
					{
						UKismetSystemLibrary::CapsuleTraceMulti(MyWorldContextObject, *Start, End, CapsuleHalfHeight, CapsuleRadius, MyTraceChannel, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits2, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						break;
					}

					case ECustomKismetTraceType::SphereTrace:
					{
						UKismetSystemLibrary::SphereTraceMulti(MyWorldContextObject, *Start, End, SphereRadius, MyTraceChannel, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits2, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						break;
					}
					}
				}
				else if (!TraceByChannelOrObjects)
				{
					switch (MyKismetTraceType)
					{
					case ECustomKismetTraceType::LineTrace:
					{
						UKismetSystemLibrary::LineTraceMultiForObjects(MyWorldContextObject, *Start, End, MyObjectTypesToHit, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits2, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						AddHitToHitArray(OutHits2); break;
					}

					case ECustomKismetTraceType::BoxTrace:
					{
						UKismetSystemLibrary::BoxTraceMultiForObjects(MyWorldContextObject, *Start, End, BoxHalfSize, BoxOrientation, MyObjectTypesToHit, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits2, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						break;
					}

					case ECustomKismetTraceType::CapsuleTrace:
					{
						UKismetSystemLibrary::CapsuleTraceMultiForObjects(MyWorldContextObject, *Start, End, CapsuleHalfHeight, CapsuleRadius, MyObjectTypesToHit, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits2, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						break;
					}

					case ECustomKismetTraceType::SphereTrace:
					{
						UKismetSystemLibrary::SphereTraceMultiForObjects(MyWorldContextObject, *Start, End, SphereRadius, MyObjectTypesToHit, ShouldTraceComplex, MyActorsToIgnore, MyDrawDebugType, OutHits2, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						break;
					}
					}
				}

				AddHitToHitArray(OutHits2);
			}
		}
	}
	else if (!UseKismet)
	{
		/*for (auto Socket1 : MySockets)
		{
			for (auto Socket2 : MySockets)
			{
				TArray<FHitResult> OutHits1;
				FCollisionQueryParams TraceParams;
				FVector TraceStart = MyPrimitive->GetSocketLocation(Socket1);
				FVector* TraceEnd = LastKnownSocketLocation.Find(Socket2);

				GetWorld()->LineTraceMultiByObjectType(OutHits1, TraceStart, *TraceEnd, MyObjectTypesToHit, TraceParams);

				AddHitToHitArray(OutHits1);
			}

			for (auto Socket3 : MySockets)
			{
				TArray<FHitResult> OutHits2;
				FCollisionQueryParams TraceParams;
				FVector* TraceStart = LastKnownSocketLocation.Find(Socket3);
				FVector TraceEnd = MyPrimitive->GetSocketLocation(Socket1);

				GetWorld()->LineTraceMultiByObjectType(OutHits2, *TraceStart, TraceEnd, MyObjectTypesToHit, TraceParams);

				AddHitToHitArray(OutHits2);
			}
		}*/
	}
}

void UCustomDitItHitActorComponent::GetPrimaryComponent(AActor* ActorTarget)
{
	if (MyStaticMeshComponent)
	{
		MyPrimitive = MyStaticMeshComponent;
		return;
	}

	auto taggedComponents = ActorTarget->GetComponentsByTag(UStaticMeshComponent::StaticClass(), "DidItHit?");
	if (taggedComponents.Num() > 0)
	{
		MyPrimitive = Cast<UPrimitiveComponent>(taggedComponents[0]);
		return;
	}

	auto meshComponent = ActorTarget->GetComponentByClass(UStaticMeshComponent::StaticClass());
	if (meshComponent)
	{
		MyPrimitive = Cast<UPrimitiveComponent>(meshComponent);
		return;
	}
}

void UCustomDitItHitActorComponent::SetupVariables(UPrimitiveComponent* MyPrimitiveTarget, AActor* MyActorTarget)
{
	MyPrimitive = MyPrimitiveTarget;
	MyActor = MyActorTarget;

	if (!MyActor)
	{
		MyActor = GetOwner();
	}

	GetSockets();
}

void UCustomDitItHitActorComponent::GetSockets()
{
	if (MyPrimitive)
	{
		MySockets = MyPrimitive->GetAllSocketNames();

		//remove sockets you don't want
		if (SkipStringFilter != "")
		{
			for (auto Socket : MySockets)
			{
				if (Socket.ToString().Contains(SkipStringFilter))
				{
					MySockets.Remove(Socket);
				}
			}
		}

		//remove sockets that aren't included
		if (InclusionStringFilter != "")
		{
			for (auto Socket : MySockets)
			{
				if (!Socket.ToString().Contains(InclusionStringFilter))
				{
					MySockets.Remove(Socket);
				}
			}
		}
	}
}

void UCustomDitItHitActorComponent::ToggleTraceCheck(bool bTrace)
{
	if (bTrace)
	{
		ClearHitArray();
		ClearSocketLocationMap();
		GetSocket_t0();
		ModuloNumber = 0; 
		for (AActor* IgnoreActor : MyActorsToIgnoreOnce) {
			MyActorsToIgnore.Remove(IgnoreActor);
		}
		MyActorsToIgnoreOnce.Empty();
		MyActorsToIgnoreEvade.Empty();
		MyActorsToIgnoreParrying.Empty();
	}
	
	CanTrace = bTrace;
}

void UCustomDitItHitActorComponent::UpdateLastSocketLocation()
{
	for (auto Socket : MySockets)
	{
		LastKnownSocketLocation.Add(Socket, MyPrimitive->GetSocketLocation(Socket));
	}
}

void UCustomDitItHitActorComponent::ClearHitArray()
{
	HitArray.Empty();
}

void UCustomDitItHitActorComponent::ClearSocketLocationMap()
{
	LastKnownSocketLocation.Empty();
}

void UCustomDitItHitActorComponent::GetSocket_t0()
{
	for (auto Socket : MySockets)
	{
		LastKnownSocketLocation.Add(Socket, MyPrimitive->GetSocketLocation(Socket));
	}
}

UCustomDitItHitActorComponent* UCustomDitItHitActorComponent::GetCustomDitItHitActorComponent(const AActor* Actor)
{
	return GetCustomDitItHitActorComponentFromActor(Actor);
}

UCustomDitItHitActorComponent* UCustomDitItHitActorComponent::GetCustomDitItHitActorComponentFromActor(const AActor* Actor, bool LookForComponent)
{
	if (Actor == nullptr)
	{
		return nullptr;
	}

	const IDitItHitCompInterface* CSI = Cast<IDitItHitCompInterface>(Actor);
	if (CSI)
	{
		return CSI->GetDitItHit();
	}

	if (LookForComponent)
	{
		// Fall back to a component search to better support BP-only actors
		return Actor->FindComponentByClass<UCustomDitItHitActorComponent>();
	}
	return nullptr;
}

void UCustomDitItHitActorComponent::AddHitToHitArray(TArray<FHitResult> HitArrayToAdd)
{
	for (const auto& Hit : HitArrayToAdd)
	{
		if (!HitArray.ContainsByPredicate([&](const FHitResult& Inner) {  return Inner.GetActor() == Hit.GetActor(); }))
		{

			auto AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Hit.GetActor());


			if (canEvade()) {
				if (isEvade(Hit.GetActor())) {
					if (!MyActorsToIgnoreOnce.Contains(Hit.GetActor())) {
						MyActorsToIgnoreOnce.AddUnique(Hit.GetActor());
						MyActorsToIgnoreEvade.AddUnique(Hit.GetActor());
					}
				}
			}
			if (canParrying()) {
				if (isPerfectParrying(Hit.GetActor())) {
					FGameplayEventData EventData;
					EventData.Instigator = GetOwner();
					UE_LOG(LogTemp, Warning, TEXT("State.Parrying.Perfect is Matched"));
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Hit.GetActor(),
						FGameplayTag::RequestGameplayTag(FName("State.Parrying.Perfect.Success")),
						EventData); 
					if (!MyActorsToIgnoreOnce.Contains(Hit.GetActor())) {
						MyActorsToIgnoreOnce.AddUnique(Hit.GetActor());
						MyActorsToIgnoreParrying.AddUnique(Hit.GetActor());
					}
				}
			}
			if (!MyActorsToIgnoreOnce.Contains(Hit.GetActor())) {
				HitArray.Add(Hit);
				SendTagHitEvent_Lotus();
				UE_LOG(LogTemp, Warning, TEXT("State.Evade.Perfect is not Matched"));
				OnItemAdded.Broadcast(Hit);/*
				AFightingCharacter* Character = Cast<AFightingCharacter*>(GetOwner());
				AbilitySystemComponent->ApplyGameplayEffectToSelf(,
					AbilitySystemComponent->MakeEffectContext())*/
			}
		}
	}
	for (AActor* IgnoreActor : MyActorsToIgnoreOnce) {
		MyActorsToIgnore.AddUnique(IgnoreActor);
	}
}

bool UCustomDitItHitActorComponent::isEvade(AActor* _target)
{
	auto AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(_target);
	if (AbilitySystemComponent&& AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Evade")))) {
		//UE_LOG(LogTemp, Log, TEXT("isEvade"));
		return true;
	}
	return false;
}

bool UCustomDitItHitActorComponent::canEvade()
{
	auto AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(this->GetOwner());
	if (AbilitySystemComponent&&
		AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Attack.Able.Evade")))) {
		//UE_LOG(LogTemp, Log, TEXT("CanEvade"));
		return true;
	}
	return false;
}

bool UCustomDitItHitActorComponent::isParrying(AActor* _target)
{
	auto AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(_target);
	if (AbilitySystemComponent &&
		AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Parrying")))) {
		//UE_LOG(LogTemp, Log, TEXT("isParrying"));
		return true;
	}
	return false;
}

bool UCustomDitItHitActorComponent::isPerfectParrying(AActor* _target)
{
	auto AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(_target);
	if (AbilitySystemComponent && 
		AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Parrying.Perfect")))) {
		UE_LOG(LogTemp, Warning, TEXT("isPerfectParrying"));
		return true;
	}
	return false;
}

bool UCustomDitItHitActorComponent::canParrying()
{
	auto AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(this->GetOwner());
	if (AbilitySystemComponent &&
		AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Attack.Able.Parrying")))) {
		//UE_LOG(LogTemp, Log, TEXT("CanParrying"));
		return true;
	}
	return false;
}

void UCustomDitItHitActorComponent::SendTagHitEvent_Lotus()
{
	FGameplayEventData EventData;
	EventData.Instigator = GetOwner();
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner()
		, FGameplayTag::RequestGameplayTag(FName("Event.HitEvent.Lotus"))
		, EventData);
}

//FHitResult OnHitAdded(FHitResult LastHit)
//{
//	return LastHit;
//}

/*
https://docs.unrealengine.com/4.26/en-US/API/Runtime/Engine/Kismet/UKismetSystemLibrary/LineTraceSingle/
*/

/*You can know the name of a bird in all the languages of the world,
but when you're finished,
you'll know absolutely nothing whatever about the bird...
So let's look at the bird and see what it's doing ?
that's what counts.
I learned very early the difference between knowing the name of something and knowing something.
*/
