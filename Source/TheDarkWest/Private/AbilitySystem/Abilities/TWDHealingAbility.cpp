// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TWDHealingAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/TDWCharacter.h"
#include "GameplayTags/TDWGameplayTags.h"

void UTWDHealingAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbility(Handle, ActorInfo, ActivationInfo);
	StartHealingAction();
}

void UTWDHealingAbility::StartHealingAction()
{
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, HealingMontage, 1.f, NAME_None, false);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &ThisClass::OnMontageEnded);
	PlayMontageTask->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, TDWGameplayTags::Events_Combat_Healing);
	WaitEventTask->EventReceived.AddDynamic(this, &ThisClass::OnHealingEventReceived);
	WaitEventTask->ReadyForActivation();
}

void UTWDHealingAbility::OnMontageEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UTWDHealingAbility::OnHealingEventReceived(FGameplayEventData Payload)
{
	ATDWCharacter* HealingCharacter = Cast<ATDWCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(HealingCharacter))
	{
		return;
	}
	
	HealingCharacter->ApplyEffectToSelf(HealingGameplayEffectClass);
	
	// Draw a sphere at the character location for debugging
	DrawDebugSphere(GetWorld(), HealingCharacter->GetActorLocation(), 50.f, 12, FColor::Green, false, 2.0f);
}
