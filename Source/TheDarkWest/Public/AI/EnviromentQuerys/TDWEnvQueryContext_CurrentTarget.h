// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "DataProviders/AIDataProvider.h"
#include "TDWEnvQueryContext_CurrentTarget.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, meta = (DisplayName = "Data Provider Actor"))
class THEDARKWEST_API UTDWEnvQueryContext_CurrentTarget : public UEnvQueryContext
{
	GENERATED_BODY()
public:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
