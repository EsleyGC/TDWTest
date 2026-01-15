// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TDWMeleeAbility.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/TDWAttributeSet.h"
#include "Character/TDWCharacter.h"
#include "Combat/TDWDamageableInterface.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Character.h"
#include "GameplayTags/TDWGameplayTags.h"

void UTDWMeleeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbility(Handle, ActorInfo, ActivationInfo);
	StartAttack();
}

void UTDWMeleeAbility::StartAttack()
{
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, AttackMontage, 1.f, NAME_None, false);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &UTDWMeleeAbility::OnMontageEnded);
	PlayMontageTask->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, TDWGameplayTags::Events_Combat_AttackHitBoxCheck);
	WaitEventTask->EventReceived.AddDynamic(this, &UTDWMeleeAbility::OnMeleeAttackEventReceived);
	WaitEventTask->ReadyForActivation();
}

void UTDWMeleeAbility::OnMontageEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UTDWMeleeAbility::OnMeleeAttackEventReceived(const FGameplayEventData Payload)
{
	// For now, we will check the hitbox only once per attack event
	// In a more complete implementation, we would use a NotifyState that, using an AttackData system, would bake the hitbox frames
	// and check for all collisions during those frames.
	
	ATDWCharacter* MeleeCharacter = Cast<ATDWCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(MeleeCharacter))
	{
		return;
	}
	
	USkeletalMeshComponent* MeshComp = MeleeCharacter->GetMesh();
	if (!IsValid(MeshComp))
	{
		return;
	}
	
	const FVector HitBoxLocation = MeshComp->GetSocketLocation(HitBoxSocketName);
	
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MeleeCharacter);
	const FCollisionShape CollisionShape = FCollisionShape::MakeSphere(HitBoxRadius.GetValue());
	const bool bHasOverlaps = GetWorld()->OverlapMultiByChannel(OverlapResults, HitBoxLocation, FQuat::Identity, ECC_Pawn, CollisionShape, QueryParams);
	if (bHasOverlaps)
	{
		const UTDWAttributeSet* AttributeSet = MeleeCharacter->GetAttributeSet();
		const float DamageAmount = AttributeSet->GetDamageAmount();
		
		for (const FOverlapResult& Result : OverlapResults)
		{
			if (!Result.bBlockingHit)
			{
				continue;
			}
			
			AActor* HitActor = Result.GetActor();
			if (IsValid(HitActor))
			{
				//Check if HitActor has IDamageable interface
				if (ITDWDamageableInterface* DamageableInterface = Cast<ITDWDamageableInterface>(HitActor))
				{
					ApplyImpactEffect(DamageableInterface, DamageAmount, DamageAmount);
				}
			}
		}
	}	
}

void UTDWMeleeAbility::ApplyImpactEffect(ITDWDamageableInterface* DamageableInterface, float DamageAmount, float DamageOnNoAbilitySystem)
{
	const bool HasAbilitySystem = DamageableInterface->HasAbilitySystemComponent();
	if (!HasAbilitySystem)
	{
		DamageableInterface->ApplyDamageToNonAbilitySystemActor(DamageOnNoAbilitySystem);
		return;
	}
	
	if (!DamageableInterface)
	{
		return;
	}
	
	if (DamageableInterface->IsDead() || !DamageableInterface->CanBeDamaged(GetAvatarActorFromActorInfo()))
	{
		return;
	}

	UAbilitySystemComponent* AbilitySystemComponent = DamageableInterface->GetAbilitySystemComponent();
	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}

	if (!IsValid(ImpactGameplayEffectClass))
	{
		return;
	}
	
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	const FVector Origin = AvatarActor->GetActorLocation();
	DamageableInterface->NotifyDamageTaken(DamageAmount, AvatarActor, Origin);
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(AvatarActor);
	EffectContext.AddInstigator(AvatarActor, AvatarActor);
	EffectContext.AddOrigin(Origin);

	FGameplayEffectSpec Spec(ImpactGameplayEffectClass->GetDefaultObject<UGameplayEffect>(), EffectContext, 1.0f);
	Spec.SetSetByCallerMagnitude(TDWGameplayTags::Damage_DamageAmount, -DamageAmount); //Apply the damage amount from the payload
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(Spec);
}
