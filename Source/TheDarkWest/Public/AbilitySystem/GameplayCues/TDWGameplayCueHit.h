// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "Character/TDWBaseEnums.h"
#include "StructUtils/InstancedStruct.h"
#include "TDWGameplayCueHit.generated.h"

/** Struct that defines camera shake info for gameplay cue notifies 
 * Move that to a CombatTypes.h file on a better system
 */
USTRUCT(BlueprintType)
struct FTDWGameplayCue_CameraShakeInfo
{
	GENERATED_BODY()

	bool PlayCameraShake(AActor* MyTarget, const FGameplayCueParameters& Parameters) const;

	// Camera shake to play.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayCueNotify)
	TSubclassOf<UCameraShakeBase> CameraShake { nullptr};
	
	// Scale applied to the camera shake.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayCueNotify)
	float ShakeScale {0.0f};

	// What coordinate space to play the camera shake relative to.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayCueNotify)
	ECameraShakePlaySpace PlaySpace {ECameraShakePlaySpace::CameraLocal};;

	// If PlaySpace is UserDefined, this is the rotation to use.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayCueNotify)
	FRotator UserPlaySpaceRot {FRotator::ZeroRotator};

	// If enabled, the camera shake will be played in the world and affect all players.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayCueNotify)
	uint32 bPlayInWorld : 1 {0};

	// Players inside this radius get the full intensity camera shake.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayCueNotify, Meta = (EditCondition = "bPlayInWorld", UIMin = "0.0", ClampMin = "0.0"))
	float WorldInnerRadius {0.0f};

	// Players outside this radius do not get the camera shake applied.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayCueNotify, Meta = (EditCondition = "bPlayInWorld", UIMin = "0.0", ClampMin = "0.0"))
	float WorldOuterRadius {0.0f};

	// Exponent that describes the shake intensity falloff curve between the inner and outer radii.  (1.0 is linear)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayCueNotify, Meta = (EditCondition = "bPlayInWorld", UIMin = "0.0", ClampMin = "0.0"))
	float WorldFalloffExponent {0.0f};
};

/** 
 * Basically a copy of the default GameplayCueNotify_Hit but with customizable play montage hit reactions
 */
UCLASS()
class THEDARKWEST_API UTDWGameplayCueHit : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UTDWGameplayCueHit();
	
	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;
	void PlayHitReactions(AActor* TargetActor, const FGameplayCueParameters& Parameters) const;
	UAnimMontage* GetDirectionalHitReactionMontage(const ECardinalDirection& LocalDirection) const;

protected:

	// Camera shake to be played on gameplay cue execution.  This should never use a looping effect!
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayCueNotify)
	FTDWGameplayCue_CameraShakeInfo CameraShakeInfo;
	
	UPROPERTY(EditAnywhere, Meta = (ExcludeBaseStruct, BaseStruct = "/Script/Chooser.ObjectChooserBase"), Category = "TDW|Animations|Combat")
	FInstancedStruct HitReactionsChooser;
};
