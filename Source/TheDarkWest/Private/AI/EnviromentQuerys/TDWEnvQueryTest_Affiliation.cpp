#include "AI/EnviromentQuerys/TDWEnvQueryTest_Affiliation.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"

UTDWEnvQueryTest_Affiliation::UTDWEnvQueryTest_Affiliation(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ValidItemType = UEnvQueryItemType_Actor::StaticClass();
	TestPurpose = EEnvTestPurpose::Filter;
	FilterType = EEnvTestFilterType::Match;
	Cost = EEnvTestCost::Low;
	AttitudeToMatch = ETeamAttitude::Hostile;

	FirstTarget = UEnvQueryContext_Querier::StaticClass();
}

void UTDWEnvQueryTest_Affiliation::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();
	if (!QueryOwner)
	{
		return;
	}

	TArray<AActor*> FirstActors;
	TArray<AActor*> SecondActors;

	AffiliationBoolValue.BindData(QueryOwner, QueryInstance.QueryID);
	bool bWantsValid = AffiliationBoolValue.GetValue();
	
	QueryInstance.PrepareContext(FirstTarget, FirstActors);
	QueryInstance.PrepareContext(SecondTarget, SecondActors);

	const bool bUseItemAsSecond = (SecondActors.Num() == 0);

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		AActor* First = (FirstActors.Num() > 0) ? FirstActors[0] : nullptr;
		AActor* Second = nullptr;

		if (bUseItemAsSecond)
		{
			Second = GetItemActor(QueryInstance, It.GetIndex());
		}
		else if (SecondActors.Num() > 0)
		{
			Second = SecondActors[0];
		}

		if (!First || !Second)
		{
			It.ForceItemState(EEnvItemStatus::Failed);
			continue;
		}

		// Consulta o TeamAttitude
		ETeamAttitude::Type Attitude = ETeamAttitude::Neutral;

		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(First))
		{
			Attitude = TeamAgent->GetTeamAttitudeTowards(*Second);
		}
		
		const bool bMatches = (Attitude == AttitudeToMatch);
		It.SetScore(TestPurpose, FilterType, bMatches, bWantsValid);
	}
}

FText UTDWEnvQueryTest_Affiliation::GetDescriptionTitle() const
{
	return FText::FromString(FString::Printf(TEXT("Check if TeamAttitude == %s"), *UEnum::GetValueAsString(AttitudeToMatch.GetValue())));
}

FText UTDWEnvQueryTest_Affiliation::GetDescriptionDetails() const
{
	FString First = FirstTarget ? FirstTarget->GetName() : TEXT("None");
	FString Second = SecondTarget ? SecondTarget->GetName() : TEXT("Item");

	return FText::FromString(FString::Printf(TEXT("Compares attitude between %s and %s"), *First, *Second));
}