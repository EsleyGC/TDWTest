// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "DataProviders/AIDataProvider.h"
#include "TDWEnvQueryContext_RandomCharacter.generated.h"

/**
 * A test only random character getter. Used to test EQS setups.
 * 
 * DO NOT USE THAT ON PRODUCTION CODE!
 */
UCLASS(EditInlineNew, meta = (DisplayName = "Random Character (SETUP ONLY)"))
class THEDARKWEST_API UTDWEnvQueryContext_RandomCharacter : public UEnvQueryContext
{
	GENERATED_BODY()
public:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
