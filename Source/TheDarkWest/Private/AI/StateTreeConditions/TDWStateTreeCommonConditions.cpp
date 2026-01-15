#include "TDWStateTreeCommonConditions.h"

#include "GameplayTagAssetInterface.h"
#include "StateTreeExecutionContext.h"
#include "UObject/EnumProperty.h"
#include "StateTreeNodeDescriptionHelpers.h"
#include "AbilitySystem/TDWAttributeSet.h"
#include "Character/TDWCharacter.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TDWStateTreeCommonConditions)

#define LOCTEXT_NAMESPACE "StateTree"

namespace TDW::StateTree::Conditions
{
	template <typename T>
	bool CompareNumbers(const T Left, const T Right, const EGenericAICheck Operator)
	{
		switch (Operator)
		{
		case EGenericAICheck::Equal:
			return Left == Right;
			break;
		case EGenericAICheck::NotEqual:
			return Left != Right;
			break;
		case EGenericAICheck::Less:
			return Left < Right;
			break;
		case EGenericAICheck::LessOrEqual:
			return Left <= Right;
			break;
		case EGenericAICheck::Greater:
			return Left > Right;
			break;
		case EGenericAICheck::GreaterOrEqual:
			return Left >= Right;
			break;
		default:
			ensureMsgf(false, TEXT("Unhandled operator %d"), Operator);
			return false;
			break;
		}
	}
} // TDW::StateTree::Conditions


//----------------------------------------------------------------------//
//  FStateTreeCompareCharacterTagsCondition
//----------------------------------------------------------------------//

bool FStateTreeCompareCharacterTagsCondition::TestCondition(FStateTreeExecutionContext& Context) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (!IsValid(InstanceData.ActorToCheckTags))
	{
		return false ^ bInvert;
	}

	IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(InstanceData.ActorToCheckTags);
	if (!TagInterface)
	{
		return false ^ bInvert;
	}

	FGameplayTagContainer TagContainer;
	TagInterface->GetOwnedGameplayTags(TagContainer);
	bool bResult = false;
	switch (MatchType)
	{
	case EGameplayContainerMatchType::Any:
		bResult = bExactMatch ? TagContainer.HasAnyExact(InstanceData.OtherContainer) : TagContainer.HasAny(InstanceData.OtherContainer);
		break;
	case EGameplayContainerMatchType::All:
		bResult = bExactMatch ? TagContainer.HasAllExact(InstanceData.OtherContainer) : TagContainer.HasAll(InstanceData.OtherContainer);
		break;
	default:
		ensureMsgf(false, TEXT("Unhandled match type %s."), *UEnum::GetValueAsString(MatchType));
	}

	return bResult ^ bInvert;
}

#if WITH_EDITOR
FText FStateTreeCompareCharacterTagsCondition::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	const FInstanceDataType* InstanceData = InstanceDataView.GetPtr<FInstanceDataType>();
	check(InstanceData);

	FText ActorName = BindingLookup.GetBindingSourceDisplayName(FStateTreePropertyPath(ID, GET_MEMBER_NAME_CHECKED(FInstanceDataType, ActorToCheckTags)), Formatting);
	if (ActorName.IsEmpty())
	{
		ActorName = InstanceData->ActorToCheckTags ? FText::FromString(InstanceData->ActorToCheckTags->GetName()) : LOCTEXT("NullActor", "None");
	}

	FText OtherContainerValue = BindingLookup.GetBindingSourceDisplayName(FStateTreePropertyPath(ID, GET_MEMBER_NAME_CHECKED(FInstanceDataType, OtherContainer)), Formatting);
	if (OtherContainerValue.IsEmpty())
	{
		OtherContainerValue = UE::StateTree::DescHelpers::GetGameplayTagContainerAsText(InstanceData->OtherContainer);
	}

	const FText InvertText = UE::StateTree::DescHelpers::GetInvertText(bInvert, Formatting);
	const FText ExactMatchText = UE::StateTree::DescHelpers::GetExactMatchText(bExactMatch, Formatting);
	const FText MatchTypeText = UEnum::GetDisplayValueAsText(MatchType);

	const FText Format = (Formatting == EStateTreeNodeFormatting::RichText)
		                     ? LOCTEXT("GameplayTagContainerMatchRich", "{EmptyOrNot}{TagContainer} <s>contains {AnyOrAll}</> {EmptyOrExactly}{OtherTagContainer}")
		                     : LOCTEXT("GameplayTagContainerMatch", "{EmptyOrNot}{TagContainer} contains {AnyOrAll} {EmptyOrExactly}{OtherTagContainer}");

	return FText::FormatNamed(Format,
	                          TEXT("EmptyOrNot"), InvertText,
	                          TEXT("TagContainer"), ActorName,
	                          TEXT("AnyOrAll"), MatchTypeText,
	                          TEXT("EmptyOrExactly"), ExactMatchText,
	                          TEXT("OtherTagContainer"), OtherContainerValue);
}

#endif

bool FStateTreeCompareTDWDistanceCondition::TestCondition(FStateTreeExecutionContext& Context) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.Character)
	{
		return false ^ bInvert;
	}

	const FVector SourceLocation = InstanceData.Character->GetActorLocation();
	const FVector TargetLocation = InstanceData.bCompareToOtherCharacter && InstanceData.TargetCharacter
		                               ? InstanceData.TargetCharacter->GetActorLocation()
		                               : InstanceData.TargetLocation;

	const FVector::FReal Left = FVector::DistSquared2D(SourceLocation, TargetLocation);
	const FVector::FReal Right = FMath::Square(InstanceData.Distance);
	const bool bResult = TDW::StateTree::Conditions::CompareNumbers<FVector::FReal>(Left, Right, Operator);

	return bResult ^ bInvert;
}

bool FStateTreeCompareHealthCondition::TestCondition(FStateTreeExecutionContext& Context) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.Character)
	{
		return false ^ bInvert;
	}

	const UTDWAttributeSet* AttributeSet = InstanceData.Character->GetAttributeSet();
	if (!AttributeSet)
	{
		return false ^ bInvert;
	}

	const float CurrentHealth = AttributeSet->GetHealth();
	float ValueToCompare = CurrentHealth;
	if (InstanceData.bCompareToPercentage)
	{
		const float MaxHealth = AttributeSet->GetMaxHealth();
		const float HealthPercent = (MaxHealth > 0.f) ? (CurrentHealth / MaxHealth) * 100.f : 0.f;
		ValueToCompare = HealthPercent;
	}

	const FVector::FReal Left = ValueToCompare;
	const FVector::FReal Right = InstanceData.bCompareToPercentage ? InstanceData.HealthPercentage : InstanceData.TotalHealth;
	const bool bResult = TDW::StateTree::Conditions::CompareNumbers<FVector::FReal>(Left, Right, Operator);

	return bResult ^ bInvert;
}

bool FStateTreeCompareNumberOfTargetsOnSightCondition::TestCondition(FStateTreeExecutionContext& Context) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	const ATDWAIController* AIController = InstanceData.AIController;
	if (!AIController)
	{
		return false ^ bInvert;
	}

	int32 NumberOfTargets = 0;
	if (InstanceData.bShouldLimitToAffiliation)
	{
		if (InstanceData.AttitudeToMatch == ETeamAttitude::Hostile)
		{
			TArray<AActor*> PerceivedActors;
			AIController->AIPerceptionComponent->GetPerceivedHostileActors(PerceivedActors);
			NumberOfTargets = PerceivedActors.Num();
		}
		else
		{
			TArray<AActor*> PerceivedActors;
			AIController->AIPerceptionComponent->GetCurrentlyPerceivedActors(nullptr, PerceivedActors);
			for (const AActor* Actor : PerceivedActors)
			{
				const ETeamAttitude::Type Attitude = AIController->GetTeamAttitudeTowards(*Actor);
				if (Attitude == InstanceData.AttitudeToMatch)
				{
					NumberOfTargets++;
				}
			}
		}
	}
	else
	{
		TArray<AActor*> PerceivedActors;
		AIController->AIPerceptionComponent->GetCurrentlyPerceivedActors(nullptr, PerceivedActors);
		NumberOfTargets = PerceivedActors.Num();
	}

	const FVector::FReal Left = NumberOfTargets;
	const FVector::FReal Right = InstanceData.NumberOfTargets;
	const bool bResult = TDW::StateTree::Conditions::CompareNumbers<FVector::FReal>(Left, Right, Operator);

	return bResult ^ bInvert;
}

bool FStateTreeCompareDotCondition::TestCondition(FStateTreeExecutionContext& Context) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	const AActor* ActorToCompareForward = InstanceData.ActorToCompareForward;
	if (!ActorToCompareForward)
	{
		return false ^ bInvert;
	}

	const FVector TargetLocation = !InstanceData.bUseLocationAsTarget && IsValid(InstanceData.TargetActor)  ? 
	InstanceData.TargetActor->GetActorLocation() : 
	InstanceData.TargetLocation;
	
	const FVector ActorLocation = ActorToCompareForward->GetActorLocation();
	const FVector ToTarget = (TargetLocation - ActorLocation).GetSafeNormal();
	const FVector ForwardVector = ActorToCompareForward->GetActorForwardVector().GetSafeNormal();
	const float DotValue = FVector::DotProduct(ForwardVector, ToTarget);
	
	const FVector::FReal Left = DotValue;
	const FVector::FReal Right = InstanceData.DotProductValue;
	const bool bResult = TDW::StateTree::Conditions::CompareNumbers<FVector::FReal>(Left, Right, Operator);

	return bResult ^ bInvert;
}

#undef LOCTEXT_NAMESPACE
