
#pragma once

#include "StateTreePropertyRef.h"
#include "StateTreeTaskBase.h"
#include "StateTreeInitializeDataTask.generated.h"

class ATDWCharacter;
class ATDWAIController;
enum class EStateTreeRunStatus : uint8;
struct FStateTreeTransitionResult;

USTRUCT()
struct THEDARKWEST_API FStateTreeInitializeDataTaskInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<ATDWCharacter> Character {nullptr};

	UPROPERTY(EditAnywhere, Category = "Parameter", meta = (RefType = "/Script/THEDARKWEST.TDWAIController"))
	FStateTreePropertyRef OutAiControllerRef;
};

/**
 * Task that initializes data for TDW AI characters.
 */
USTRUCT(meta = (DisplayName = "Initialize Data Task", Category = "TDW"))
struct THEDARKWEST_API FStateTreeInitializeDataTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeInitializeDataTaskInstanceData;

	FStateTreeInitializeDataTask() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};
	