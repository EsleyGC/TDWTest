
#include "StateTreeIncreaseCountTask.h"

#include "StateTreeExecutionContext.h"

EStateTreeRunStatus FStateTreeIncreaseCountTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (InstanceData.bShouldIncreaseOnEnter)
	{
		int32* OutIntValue = InstanceData.OutIntValueRef.GetMutablePtr<int32>(Context);
		if (OutIntValue)
		{
			if (InstanceData.bOverrideValue)
			{
				*OutIntValue = InstanceData.OverrideValue;
				return EStateTreeRunStatus::Succeeded;
			}
			
			*OutIntValue += InstanceData.IncreaseBy;
		}
		
		return EStateTreeRunStatus::Succeeded;
	}
	
	return EStateTreeRunStatus::Running;
}

void FStateTreeIncreaseCountTask::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.bShouldIncreaseOnEnter)
	{
		int32* OutIntValue = InstanceData.OutIntValueRef.GetMutablePtr<int32>(Context);
		if (OutIntValue)
		{
			if (InstanceData.bOverrideValue)
			{
				*OutIntValue = InstanceData.OverrideValue;
				return;
			}
			
			*OutIntValue += InstanceData.IncreaseBy;
		}
	}
}
