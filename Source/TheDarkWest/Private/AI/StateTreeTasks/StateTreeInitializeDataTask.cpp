
#include "StateTreeInitializeDataTask.h"

#include "StateTreeExecutionContext.h"
#include "AI/TDWAIController.h"

EStateTreeRunStatus FStateTreeInitializeDataTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	ATDWCharacter* TDWCharacter = InstanceData.Character;
	if (!IsValid(TDWCharacter))
	{
		return EStateTreeRunStatus::Failed;
	}
	
	ATDWAIController* TDWAIController = Cast<ATDWAIController>(TDWCharacter->GetController());
	if (!IsValid(TDWAIController))
	{
		return EStateTreeRunStatus::Failed;
	}
	
	ATDWAIController** AIControllerRef = InstanceData.OutAiControllerRef.GetMutablePtr<ATDWAIController*>(Context);
	if (!AIControllerRef)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	*AIControllerRef = TDWAIController;	
	return EStateTreeRunStatus::Running;
}
