// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Combat/TDWDamageableInterface.h"
#include "TDWMeleeAbility.generated.h"

/**
 * 
 */
UCLASS()
class THEDARKWEST_API UTDWMeleeAbility : public UGameplayAbility
{
	GENERATED_BODY()
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:

	UFUNCTION()
	void StartAttack();
	
	UFUNCTION()
	void OnMontageEnded();
	
	UFUNCTION()
	void OnMeleeAttackEventReceived(FGameplayEventData Payload);
	void ApplyImpactEffect(ITDWDamageableInterface* DamageableInterface, float DamageAmount, float DamageOnNoAbilitySystem);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TDW|HitBox")
	FScalableFloat HitBoxRadius = 30.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TDW|Projectile")
	FName HitBoxSocketName = "hand_r";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TDW|Animation")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TDW|Damage")
	TSubclassOf<class UGameplayEffect> ImpactGameplayEffectClass;
};
