// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TDWFireAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "AbilitySystem/TDWAttributeSet.h"
#include "Character/TDWCharacter.h"
#include "Combat/TDWProjectile.h"
#include "Helpers/TDWCombatTypes.h"
#include "GameFramework/Character.h"

void UTDWFireAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	ScheduleFire();
}

void UTDWFireAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTDWFireAbility::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ScheduleFire();
}

void UTDWFireAbility::ScheduleFire()
{
	if (bHasScheduledFire)
	{
		return;
	}

	const float CurrentTime = GetWorld()->GetTimeSeconds();
	const float TimeSinceLastFire = CurrentTime - LastFireTime;
	const float DelayBetweenFires = ReExecutionDelay.GetValue();
	if (TimeSinceLastFire >= DelayBetweenFires)
	{
		ExecuteFire();
		return;
	}

	const float RemainingTime = DelayBetweenFires - TimeSinceLastFire;
	ScheduledFireTask = UAbilityTask_WaitDelay::WaitDelay(this, RemainingTime);
	ScheduledFireTask->OnFinish.AddDynamic(this, &ThisClass::ExecuteFire);
	ScheduledFireTask->ReadyForActivation();
	bHasScheduledFire = true;
}

void UTDWFireAbility::ExecuteFire()
{
	CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	
	bHasScheduledFire = false;
	LastFireTime = GetWorld()->GetTimeSeconds();

	UAbilityTask_PlayMontageAndWait* FireMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, FireMontage);
	FireMontageTask->OnBlendOut.AddDynamic(this, &ThisClass::OnMontageEnded);
	FireMontageTask->ReadyForActivation();

	UAbilityTask_WaitDelay* SpawnProjectileDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, FireStartDelay.GetValue());
	SpawnProjectileDelayTask->OnFinish.AddDynamic(this, &ThisClass::SpawnProjectile);
	SpawnProjectileDelayTask->ReadyForActivation();
}

void UTDWFireAbility::SpawnProjectile()
{
	if (!IsValid(ProjectileClass))
	{
		UE_LOG(LogTemp, Error, TEXT("ProjectileClass is not set in TDWFireAbility"));
		return;
	}

	ATDWCharacter* Character = Cast<ATDWCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(Character))
	{
		UE_LOG(LogTemp, Error, TEXT("AvatarActor is not a valid ATDWCharacter in TDWFireAbility"));
		return;
	}

	const USkeletalMeshComponent* MeshComp = Character->GetMesh();
	if (!IsValid(MeshComp))
	{
		UE_LOG(LogTemp, Error, TEXT("Character Mesh is not valid in TDWFireAbility"));
		return;
	}

	const FTransform SocketTransform = MeshComp->GetSocketTransform(SpawnSocketName, RTS_World);
	const FVector TransformedOffset = Character->GetTransform().TransformVectorNoScale(ProjectileSpawnOffset);
	const FVector SpawnLocation = SocketTransform.GetLocation() + TransformedOffset;
	const FVector CharacterLocation = Character->GetActorLocation();
	const FVector TargetLocation = Character->GetCurrentLookAtLocation();

	const float SpawnLocationDistance = FVector::Dist2D(SpawnLocation, CharacterLocation);
	const float TargetLocationDistance = FVector::Dist2D(TargetLocation, CharacterLocation);
	const float SpawnLocationZOffset = FMath::Abs(SpawnLocation.Z - CharacterLocation.Z);
	const float CloseMaxDistance = (SpawnLocationDistance + SpawnLocationZOffset) * 2; // We add a little offset to avoid missing the target when it's very close
	const float StartLocationLerpValue = FMath::Clamp(TargetLocationDistance / CloseMaxDistance, 0.0f, 1.0f);
	const FVector StartLocationForDirection = FMath::Lerp(CharacterLocation, SpawnLocation, StartLocationLerpValue);

	const FVector Direction = (TargetLocation - StartLocationForDirection).GetSafeNormal2D();
	const FRotator LookAtRotation = Direction.Rotation();
	const FRotator SpawnRotation = LookAtRotation;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Cast<AController>(Character->GetInstigatorController());
	SpawnParams.Instigator = Character;

	AActor* ProjectileActor = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (IsValid(ProjectileActor))
	{
		//Check if the spawned actor is an TDWProjectile
		//If so, initialize it with the payload data
		if (ATDWProjectile* Projectile = Cast<ATDWProjectile>(ProjectileActor); IsValid(Projectile))
		{
			const UTDWAttributeSet* AttributeSet = Character->GetAttributeSet();
			const float DamageAmount = AttributeSet ? AttributeSet->GetDamageAmount() : 0.0f;

			FProjectilePayload Payload(Character, SpawnLocation, TargetLocation, DamageAmount);
			Projectile->InitializeProjectile(Payload);
		}
	}
}

void UTDWFireAbility::OnMontageEnded()
{
	if (bHasScheduledFire)
	{
		return;
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
