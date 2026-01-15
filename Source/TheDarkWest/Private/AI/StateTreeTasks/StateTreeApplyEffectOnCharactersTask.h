
#pragma once

#include "StateTreePropertyRef.h"
#include "StateTreeTaskBase.h"
#include "StateTreeApplyEffectOnCharactersTask.generated.h"

class ATDWCharacter;
class UGameplayEffect;
enum class EStateTreeRunStatus : uint8;
struct FStateTreeTransitionResult;

USTRUCT()
struct THEDARKWEST_API FStateTreeApplyEffectOnCharactersTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Context)
	ATDWCharacter* Character = nullptr;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	TArray<AActor*> ActorsToApplyEffect;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	TObjectPtr<AActor> ActorToApplyEffect = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Parameter")
	TSubclassOf<UGameplayEffect> EffectToApply = nullptr;
	
	/** If true, the task will return Succeeded status upon execution, otherwise it will return Running. */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bShouldSucceed = false;
};

/**
 * Task that applies a gameplay effect to a list of actors or a single actor.
 */
USTRUCT(meta = (DisplayName = "Apply GameplayEffect On Actors Task", Category = "TDW"))
struct THEDARKWEST_API FStateTreeApplyEffectOnCharactersTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeApplyEffectOnCharactersTaskInstanceData;

	FStateTreeApplyEffectOnCharactersTask() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};
	