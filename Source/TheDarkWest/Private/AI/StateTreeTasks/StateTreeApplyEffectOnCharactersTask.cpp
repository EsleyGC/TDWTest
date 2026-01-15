
#include "StateTreeApplyEffectOnCharactersTask.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "StateTreeExecutionContext.h"
#include "AI/TDWAIController.h"

EStateTreeRunStatus FStateTreeApplyEffectOnCharactersTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	const TSubclassOf<UGameplayEffect> Effect = InstanceData.EffectToApply;
	if (!IsValid(Effect) || Effect.GetDefaultObject() == nullptr)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	ATDWCharacter* Instigator = InstanceData.Character;
	if (!IsValid(Instigator))
	{
		return EStateTreeRunStatus::Failed;
	}
	
	TArray<AActor*> TargetActors = InstanceData.ActorsToApplyEffect;
	if (IsValid(InstanceData.ActorToApplyEffect))
	{
		TargetActors.Add(InstanceData.ActorToApplyEffect);
	}

	for (const AActor* TargetActor : TargetActors)
	{
		UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
		if (!IsValid(AbilitySystemComponent))
		{
			continue;
		}
		
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(Instigator);
		EffectContext.AddInstigator(Instigator, Instigator);
		EffectContext.AddOrigin(Instigator->GetActorLocation());
	
		AbilitySystemComponent->ApplyGameplayEffectToSelf(Effect.GetDefaultObject(), 1, EffectContext);
	}
	
	return InstanceData.bShouldSucceed ? EStateTreeRunStatus::Succeeded : EStateTreeRunStatus::Running;
}
