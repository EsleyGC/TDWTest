// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/TDWProjectile.h"

#include "AbilitySystemComponent.h"
#include "Combat/TDWDamageableInterface.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameplayTags/TDWGameplayTags.h"


// Sets default values
ATDWProjectile::ATDWProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;
	
	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	ProjectileMeshComponent->SetupAttachment(RootComponent);
}

void ATDWProjectile::InitializeProjectile(const FProjectilePayload& Payload)
{
	ProjectilePayload = Payload;
}

void ATDWProjectile::ApplyImpactEffect(ITDWDamageableInterface* DamageableInterface, float DamageAmount, float DamageOnNoAbilitySystem)
{
	const bool HasAbilitySystem = DamageableInterface->HasAbilitySystemComponent();
	if (!HasAbilitySystem)
	{
		DamageableInterface->ApplyDamageToNonAbilitySystemActor(DamageOnNoAbilitySystem);
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

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	EffectContext.AddInstigator(ProjectilePayload.InstigatorActor, this /*This could be the weapon*/);
	EffectContext.AddOrigin(ProjectilePayload.Origin);

	FGameplayEffectSpec Spec(ImpactGameplayEffectClass->GetDefaultObject<UGameplayEffect>(), EffectContext, 1.0f);
	Spec.SetSetByCallerMagnitude(TDWGameplayTags::Damage_DamageAmount, -DamageAmount); //Apply the damage amount from the payload
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(Spec);
}

void ATDWProjectile::DestroySelf()
{
	// Handle any pre-destroy logic here if needed and then recycle to the pool
	// We are destroying the actor for now for simplicity
	Destroy();
}
