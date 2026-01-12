// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TDWFireAbility.generated.h"

class UAbilityTask_WaitDelay;
/**
 * 
 */
UCLASS()
class THEDARKWEST_API UTDWFireAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	
protected:
	void ScheduleFire();

	UFUNCTION()
	void SpawnProjectile();
	
	UFUNCTION()
	void ExecuteFire();
	
	UFUNCTION()
	void OnMontageEnded();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TDW|Delays")
	FScalableFloat FireStartDelay = 0.2f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TDW|Delays")
	FScalableFloat ReExecutionDelay = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TDW|Animation")
	TObjectPtr<UAnimMontage> FireMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TDW|Projectile")
	TSubclassOf<AActor> ProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TDW|Projectile")
	FName SpawnSockedName = "hand_r";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TDW|Projectile")
	FVector ProjectileSpawnOffset = FVector::ZeroVector;
	
private:
	float LastFireTime = 0.0f;
	
	bool bHasScheduledFire = false;
	
	UPROPERTY()
	UAbilityTask_WaitDelay* ScheduledFireTask = nullptr;
};
