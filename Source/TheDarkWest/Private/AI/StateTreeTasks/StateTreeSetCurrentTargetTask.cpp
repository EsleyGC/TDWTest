
#include "StateTreeSetCurrentTargetTask.h"
#include "StateTreeExecutionContext.h"
#include "AI/TDWAIController.h"

EStateTreeRunStatus FStateTreeSetCurrentTargetTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	ATDWAIController* AIController = InstanceData.AIController;
	if (!IsValid(AIController))
	{
		return EStateTreeRunStatus::Failed;
	}
	
	ATDWCharacter* TDWCharacter = Cast<ATDWCharacter>(InstanceData.ActorToSetAsTarget);
	if (!IsValid(TDWCharacter))
	{
		return EStateTreeRunStatus::Failed;
	}
	
	AIController->SetCurrentTarget(TDWCharacter);
	return InstanceData.bShouldSucceed ? EStateTreeRunStatus::Succeeded : EStateTreeRunStatus::Running;
}
