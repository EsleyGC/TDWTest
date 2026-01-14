// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/TDWProjectileBullet.h"

#include "Combat/TDWDamageableInterface.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


ATDWProjectileBullet::ATDWProjectileBullet()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 3000.f;
	ProjectileMovementComponent->MaxSpeed = 3000.f;
	ProjectileMovementComponent->bShouldBounce = false;
}

void ATDWProjectileBullet::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovementComponent->OnProjectileStop.AddDynamic(this, &ThisClass::OnContact);	
}

void ATDWProjectileBullet::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	ProjectileMovementComponent->OnProjectileStop.RemoveDynamic(this, &ThisClass::OnContact);
}

void ATDWProjectileBullet::InitializeProjectile(const FProjectilePayload& Payload)
{
	Super::InitializeProjectile(Payload);
	SphereComponent->IgnoreActorWhenMoving(Payload.InstigatorActor, true);
}

void ATDWProjectileBullet::OnContact(const FHitResult& ImpactResult)
{
	if (ImpactResult.bBlockingHit)
	{
		if (AActor* HitActor = ImpactResult.GetActor(); IsValid(HitActor))
		{
			//Check if HitActor has IDamageable interface
			if (ITDWDamageableInterface* DamageableInterface = Cast<ITDWDamageableInterface>(HitActor))
			{
				ApplyImpactEffect(DamageableInterface, ProjectilePayload.DamageAmount, DamageToApplyWhenNoAbilitySystem);
			}
		}
	}

	DestroySelf();
}

