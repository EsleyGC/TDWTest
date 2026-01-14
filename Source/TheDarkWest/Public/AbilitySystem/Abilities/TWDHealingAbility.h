// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TWDHealingAbility.generated.h"

/**
 * 
 */
UCLASS()
class THEDARKWEST_API UTWDHealingAbility : public UGameplayAbility
{
	GENERATED_BODY()
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:

	UFUNCTION()
	void StartHealingAction();
	
	UFUNCTION()
	void OnMontageEnded();
	
	UFUNCTION()
	void OnHealingEventReceived(FGameplayEventData Payload);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TDW|Animation")
	TObjectPtr<UAnimMontage> HealingMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TDW|Damage")
	TSubclassOf<class UGameplayEffect> HealingGameplayEffectClass;
};
