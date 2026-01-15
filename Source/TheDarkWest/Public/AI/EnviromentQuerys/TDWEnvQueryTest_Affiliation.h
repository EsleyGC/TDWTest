// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Templates/SubclassOf.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "GenericTeamAgentInterface.h"
#include "TDWEnvQueryTest_Affiliation.generated.h"

UCLASS()
class THEDARKWEST_API UTDWEnvQueryTest_Affiliation : public UEnvQueryTest
{
	GENERATED_UCLASS_BODY()

	/** Attitude to match */
	UPROPERTY(EditDefaultsOnly, Category=Distance)
	TEnumAsByte<ETeamAttitude::Type> AttitudeToMatch;

	/** context */
	UPROPERTY(EditDefaultsOnly, Category=Distance)
	TSubclassOf<UEnvQueryContext> FirstTarget;

	/** context */
	UPROPERTY(EditDefaultsOnly, Category=Distance)
	TSubclassOf<UEnvQueryContext> SecondTarget;

	/** Desired boolean value of the test for scoring to occur or filtering test to pass. */
	UPROPERTY(EditDefaultsOnly, Category=Filter)
	FAIDataProviderBoolValue AffiliationBoolValue;

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;
};
