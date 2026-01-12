// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TDWFireAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
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

	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(Character))
	{
		UE_LOG(LogTemp, Error, TEXT("AvatarActor is not a valid ACharacter in TDWFireAbility"));
		return;
	}
		
	const USkeletalMeshComponent* MeshComp = Character->GetMesh();
	if (!IsValid(MeshComp))
	{
		UE_LOG(LogTemp, Error, TEXT("Character Mesh is not valid in TDWFireAbility"));
		return;
	}
	
	const FTransform SocketTransform = MeshComp->GetSocketTransform(SpawnSockedName, RTS_World);
	const FVector TransformedOffset = Character->GetTransform().TransformVectorNoScale(ProjectileSpawnOffset);
	const FVector SpawnLocation = SocketTransform.GetLocation() + TransformedOffset;
	const FRotator SpawnRotation = Character->GetActorRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Cast<AController>(Character->GetInstigatorController());
	SpawnParams.Instigator = Character;
	
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
}

void UTDWFireAbility::OnMontageEnded()
{
	if (bHasScheduledFire)
	{
		return;
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
