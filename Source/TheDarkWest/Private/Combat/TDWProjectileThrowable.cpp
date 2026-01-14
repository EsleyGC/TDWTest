


#include "Combat/TDWProjectileThrowable.h"

#include "AbilitySystemComponent.h"
#include "Combat/TDWDamageableInterface.h"
#include "Engine/OverlapResult.h"
#include "GameplayTags/TDWGameplayTags.h"


ATDWProjectileThrowable::ATDWProjectileThrowable()
{
}

void ATDWProjectileThrowable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	InterpToTarget(DeltaTime);
}

void ATDWProjectileThrowable::InitializeProjectile(const FProjectilePayload& Payload)
{
	Super::InitializeProjectile(Payload);
	
	StartTime = GetWorld()->GetTimeSeconds();
	StartLocation = GetActorLocation();
	TargetLocation = Payload.TargetLocation;
	InitialDistance = FVector::Dist2D(StartLocation, TargetLocation);
	
	ElapsedTime = 0;

	const float Alpha = FMath::Clamp(InitialDistance / MaxDistance, 0.f, 1.f);

	ArcHeight = FMath::Lerp(MinMaxArcHeight.X, MinMaxArcHeight.Y, Alpha);
	DesiredSpeed = FMath::Lerp(MinMaxSpeed.X, MinMaxSpeed.Y, Alpha);
	TotalTravelTime = InitialDistance / DesiredSpeed;
}

void ATDWProjectileThrowable::InterpToTarget(const float DeltaTime)
{
	ElapsedTime += DeltaTime;

	const float Alpha = FMath::Clamp(ElapsedTime / TotalTravelTime, 0.f, 1.f);

	FVector HorizontalPos = FMath::Lerp(StartLocation, TargetLocation, Alpha);

	const float HeightFactor = 4.f * Alpha * (1.f - Alpha);
	HorizontalPos.Z += HeightFactor * ArcHeight;

	SetActorLocation(HorizontalPos);

	if (Alpha >= 1.f)
	{
		OnTargetReached();
	}
}

void ATDWProjectileThrowable::OnTargetReached()
{
	// Apply area damage/effects logic here
	TArray<FOverlapResult> OverlapResults;
	const FCollisionShape CollisionShape = FCollisionShape::MakeSphere(ImpactRadius.GetValue());
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(ProjectilePayload.InstigatorActor);
	bool bHasOverlaps = GetWorld()->OverlapMultiByChannel(OverlapResults, GetActorLocation(), FQuat::Identity, ECC_Pawn, CollisionShape, QueryParams);
	if (bHasOverlaps)
	{
		for (const FOverlapResult& Result : OverlapResults)
		{
			if (AActor* OverlappedActor = Result.GetActor(); IsValid(OverlappedActor))
			{
				if (ITDWDamageableInterface* DamageableInterface = Cast<ITDWDamageableInterface>(OverlappedActor))
				{
					const float DamageAmount = bOverrideDamage ? DefaultDamageAmount.GetValue() : ProjectilePayload.DamageAmount;
					ApplyImpactEffect(DamageableInterface, DamageAmount, DamageToApplyWhenNoAbilitySystem);
				}
			}
		}
	}
	
	//Implement any visual/sound effects on impact here
	DrawDebugSphere(GetWorld(), GetActorLocation(), ImpactRadius.GetValue(), 12, FColor::Red, false, 2.0f);
	
	DestroySelf();
}
