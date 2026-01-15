
#include "StateTreeExecuteActionTask.h"

#include "StateTreeExecutionContext.h"
#include "AI/TDWAIController.h"

EStateTreeRunStatus FStateTreeExecuteActionTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	const ATDWAIController* Controller = InstanceData.AIController.Get();
	if (!IsValid(Controller))
	{
		return EStateTreeRunStatus::Failed;
	}
	
	Controller->HandleAction(InstanceData.ActionTag);
	return InstanceData.bShouldSucceed ? EStateTreeRunStatus::Succeeded : EStateTreeRunStatus::Running;
}
