// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayCues/TDWGameplayCueHit.h"

#include "ChooserFunctionLibrary.h"
#include "IObjectChooser.h"
#include "Character/TDWBaseEnums.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"

static APlayerController* FindPlayerControllerFromActor(AActor* Actor)
{
	if (!Actor)
	{
		return nullptr;
	}

	// Check the actual actor first.
	APlayerController* PC = Cast<APlayerController>(Actor);
	if (PC)
	{
		return PC;
	}

	// Check using pawn next.
	APawn* Pawn = Cast<APawn>(Actor);
	if (!Pawn)
	{
		Pawn = Actor->GetInstigator<APawn>();
	}

	PC = (Pawn ? Cast<APlayerController>(Pawn->Controller) : nullptr);
	if (PC)
	{
		return PC;
	}

	// Check using player state.
	APlayerState* PS = Cast<APlayerState>(Actor);

	PC = (PS ? Cast<APlayerController>(PS->GetOwner()) : nullptr);
	if (PC)
	{
		return PC;
	}

	return nullptr;
}

static float CalculateFalloffIntensity(const FVector& SourceLocation, const FVector& TargetLocation, float InnerRadius, float OuterRadius, float FalloffExponent)
{
	if ((InnerRadius <= 0.0f) && (OuterRadius <= 0.0f))
	{
		return 1.0f;
	}

	const float DistanceSqr = FVector::DistSquared(SourceLocation, TargetLocation);

	if (InnerRadius < OuterRadius)
	{
		const float Distance = FMath::Sqrt(DistanceSqr);
		const float Percent = 1.0f - FMath::Clamp(((Distance - InnerRadius) / (OuterRadius - InnerRadius)), 0.0f, 1.0f);

		return FMath::Pow(Percent, FalloffExponent);
	}

	// Ignore the outer radius and do a cliff falloff at the inner radius.
	return (DistanceSqr < FMath::Square(InnerRadius)) ? 1.0f : 0.0f;
}

bool FTDWGameplayCue_CameraShakeInfo::PlayCameraShake(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	if (!CameraShake || ShakeScale <= 0.0f)
	{
		return false;
	}

	if (bPlayInWorld)
	{
		const FVector SpawnLocation = MyTarget->GetActorLocation();
		const UWorld* World = MyTarget->GetWorld();
		if (!IsValid(World))
		{
			return false;
		}

		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			const APlayerController* PC = Iterator->Get();
			if (!PC || !PC->PlayerCameraManager || !PC->IsLocalController())
			{
				continue;
			}

			const APawn* Pawn = PC->GetPawn();
			if (!Pawn)
			{
				continue;
			}

			//TODO: Check if we need to use the Pawn's location or the PlayerCameraManager's location.
			const FVector PawnLocation = Pawn->GetActorLocation();
			const float FalloffIntensity = CalculateFalloffIntensity(SpawnLocation, PawnLocation, WorldInnerRadius, WorldOuterRadius, WorldFalloffExponent);

			if (FalloffIntensity <= 0.0f)
			{
				continue;
			}

			UCameraShakeBase* CameraShakeInstance = PC->PlayerCameraManager->StartCameraShake(CameraShake, (ShakeScale * FalloffIntensity), PlaySpace, UserPlaySpaceRot);
		}
	}
	else
	{
		// Play the camera shake directly on the target player controller.  No intensity falloff is applied when playing directly on the target.
		const APlayerController* TargetPC = FindPlayerControllerFromActor(MyTarget);;

		if (TargetPC && TargetPC->IsLocalController() && TargetPC->PlayerCameraManager)
		{
			UCameraShakeBase* CameraShakeInstance = TargetPC->PlayerCameraManager->StartCameraShake(CameraShake, ShakeScale, PlaySpace, UserPlaySpaceRot);
		}
	}

	return true;
}

UTDWGameplayCueHit::UTDWGameplayCueHit()
{
}

bool UTDWGameplayCueHit::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{	
	CameraShakeInfo.PlayCameraShake(Target, Parameters);
	PlayHitReactions(Target, Parameters);
	return true;
}

void UTDWGameplayCueHit::PlayHitReactions(AActor* TargetActor, const FGameplayCueParameters& Parameters) const
{
	const FVector Origin = Parameters.EffectContext.GetOrigin();
	const FVector TargetLocation = TargetActor->GetActorLocation();
	const FVector ImpactDirection = (TargetLocation - Origin).GetSafeNormal2D();
	const FVector TargetForward = TargetActor->GetActorForwardVector().GetSafeNormal2D();
	const float ForwardDot = FVector::DotProduct(TargetForward, ImpactDirection);
	const float RightDot = FVector::DotProduct(FVector::CrossProduct(FVector::UpVector, TargetForward), ImpactDirection);
	
	ECardinalDirection HitDirection;
	if (FMath::Abs(ForwardDot) >= FMath::Abs(RightDot))
	{
		HitDirection = ForwardDot >= 0 ? ECardinalDirection::Forward : ECardinalDirection::Backward;
	}
	else
	{
		HitDirection = RightDot >= 0 ? ECardinalDirection::Right : ECardinalDirection::Left;
	}
	
	UAnimMontage* HitReactionMontage = GetDirectionalHitReactionMontage(HitDirection);
	if (!IsValid(HitReactionMontage))
	{
		return;
	}

	const ACharacter* TargetCharacter = Cast<ACharacter>(TargetActor);
	if (!IsValid(TargetCharacter))
	{
		return;
	}

	const USkeletalMeshComponent* MeshComp = TargetCharacter->GetMesh();
	if (!IsValid(MeshComp))
	{
		return;
	}
	
	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();	
	AnimInstance->Montage_Play(HitReactionMontage);
}

UAnimMontage* UTDWGameplayCueHit::GetDirectionalHitReactionMontage(const ECardinalDirection& LocalDirection) const
{
	if (!HitReactionsChooser.IsValid())
	{
		return nullptr;
	}

	FChooserEvaluationContext HitReactionContext = UChooserFunctionLibrary::MakeChooserEvaluationContext();
	FTDWHitReactionData HitReactionData(LocalDirection);
	HitReactionContext.AddStructParam(HitReactionData);
	
	const FObjectChooserBase& ChooserBase = HitReactionsChooser.Get<FObjectChooserBase>();
	UObject* ChooserResult = ChooserBase.ChooseObject(HitReactionContext);
	
	UAnimMontage* HitReactionMontage = Cast<UAnimMontage>(ChooserResult);
	if (!IsValid(HitReactionMontage))
	{
		return nullptr;
	}
	
	return HitReactionMontage;
}
