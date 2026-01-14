
#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "TDWProjectile.h"
#include "GameFramework/Actor.h"
#include "Helpers/TDWCombatTypes.h"
#include "TDWProjectileThrowable.generated.h"

class ITDWDamageableInterface;

UCLASS()
class THEDARKWEST_API ATDWProjectileThrowable : public ATDWProjectile
{
	GENERATED_BODY()


public:
	ATDWProjectileThrowable();

protected:
	virtual void Tick(float DeltaTime) override;

public:
	
	virtual void InitializeProjectile(const FProjectilePayload& Payload) override;
	
	void InterpToTarget(const float DeltaTime);
	
	/** Called when projectile ends its trajectory */
	void OnTargetReached();
	
protected:
	
	/** Maximum height the projectile reaches at the apex of its trajectory */
	UPROPERTY(EditAnywhere, Category="TDW|Projectile")
	float MaxHeight = 300.0f;
	
	/** Min and Max speed of the projectile. Based on distance to target and max distance property */
	UPROPERTY(EditAnywhere, Category="TDW|Projectile")
	FVector2D MinMaxSpeed = FVector2D(600.0f, 1200.0f);
	
	/** Min and Max arc height based on distance to target and max distance property */
	UPROPERTY(EditAnywhere, Category="TDW|Projectile")
	FVector2D MinMaxArcHeight = FVector2D(100.0f, 300.0f);
	
	/** Maximum distance the projectile can travel. Used to scale speed and arc height. It does not limit actual distance. */
	UPROPERTY(EditAnywhere, Category="TDW|Projectile")
	float MaxDistance = 2000.0f;
	
	/** Radius around the impact point to apply damage/effects */
	UPROPERTY(EditAnywhere, Category="TDW|Damage")
	FScalableFloat ImpactRadius = 200.0f;
	
	/** Whether to override the payload damage amount */
	UPROPERTY(EditAnywhere, Category="TDW|Damage")
	bool bOverrideDamage = false;
	
	/** Damage amount to apply on impact if bOverrideDamage is true */
	UPROPERTY(EditAnywhere, Category="TDW|Damage", meta=(EditCondition="bOverrideDamage"))
	FScalableFloat DefaultDamageAmount = 20.0f;
	
private:
	
	float StartTime = 0.0f;
	
	FVector StartLocation = FVector::ZeroVector;	
	FVector TargetLocation = FVector::ZeroVector;
	float ElapsedTime = 0.0f;
	float TotalTravelTime = 0.0f;
	
	float InitialDistance = 0.0f;
	
	float ArcHeight = 0.0f;
	float DesiredSpeed = 0.0f;
};
