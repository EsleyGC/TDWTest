// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Helpers/TDWCombatTypes.h"
#include "TDWProjectile.generated.h"

class ITDWDamageableInterface;

UCLASS()
class THEDARKWEST_API ATDWProjectile : public AActor
{
	GENERATED_BODY()

public:
	ATDWProjectile();
	
	virtual void InitializeProjectile(const FProjectilePayload& Payload);
	
	/** Applies the impact gameplay effect to the hit actor */
	virtual void ApplyImpactEffect(ITDWDamageableInterface* DamageableInterface, float DamageAmount, float DamageOnNoAbilitySystem);
	
	/** Destroys the projectile actor */
	virtual void DestroySelf();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TDW|Components")
	TObjectPtr<class USphereComponent> SphereComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TDW|Components")
	TObjectPtr<class UStaticMeshComponent> ProjectileMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TDW|Damage")
	TSubclassOf<class UGameplayEffect> ImpactGameplayEffectClass;
	
	/** Damage to apply if the hit actor does not have an ability system component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TDW|Damage")
	float DamageToApplyWhenNoAbilitySystem = 10.0f;
	
	FProjectilePayload ProjectilePayload;
};
