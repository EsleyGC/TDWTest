// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnviromentQuerys/TDWEnvQueryContext_CurrentTarget.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

#include "AI/TDWAIController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TDWEnvQueryContext_CurrentTarget)

void UTDWEnvQueryContext_CurrentTarget::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();
	if (!QueryOwner)
	{
		return;
	}

	const ATDWAIController* TDWAIController;
	if (const APawn* Pawn = Cast<APawn>(QueryOwner))
	{
		TDWAIController = Cast<ATDWAIController>(Pawn->GetController());
	}
	else
	{
		TDWAIController = Cast<ATDWAIController>(QueryOwner);
	}

	if (!TDWAIController)
	{
		return;
	}

	AActor* CurrentTarget = TDWAIController->GetCurrentTarget();
	if (!CurrentTarget)
	{
		return;
	}

	UEnvQueryItemType_Actor::SetContextHelper(ContextData, CurrentTarget);
}
