// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDWProjectile.h"
#include "TDWProjectileBullet.generated.h"

UCLASS()
class THEDARKWEST_API ATDWProjectileBullet : public ATDWProjectile
{
	GENERATED_BODY()

public:
	ATDWProjectileBullet();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void InitializeProjectile(const FProjectilePayload& Payload) override;
	
	/** Called when projectile hits something */
	UFUNCTION()
	void OnContact(const FHitResult& ImpactResult);
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TDW|Components")
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;
};
