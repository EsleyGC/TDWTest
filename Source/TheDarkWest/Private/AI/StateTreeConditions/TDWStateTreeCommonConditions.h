// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AITypes.h"
#include "StateTreeConditionBase.h"
#include "StateTreeAnyEnum.h"
#include "AI/TDWAIController.h"
#include "TDWStateTreeCommonConditions.generated.h"

class ATDWCharacter;
struct FStateTreeDataView;

USTRUCT()
struct FStateTreeCompareCharacterTagsConditionInstanceData
{
	GENERATED_BODY()
	
	/** Actor to check tags on */
	UPROPERTY(EditAnywhere, Category = Parameter)
	AActor* ActorToCheckTags = nullptr;

	/** Container to check against. */
	UPROPERTY(EditAnywhere, Category = Parameter)
	FGameplayTagContainer OtherContainer;
};
STATETREE_POD_INSTANCEDATA(FStateTreeCompareCharacterTagsConditionInstanceData);

/**
 * Condition comparing character's tags with another tag container.
 */
USTRUCT(DisplayName = "Compare Character Tags", Category = "TDW")
struct THEDARKWEST_API FStateTreeCompareCharacterTagsCondition : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeCompareCharacterTagsConditionInstanceData;

	FStateTreeCompareCharacterTagsCondition() = default;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;
#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
	virtual FName GetIconName() const override
	{
		return FName("StateTreeEditorStyle|Node.Tag");
	}
	virtual FColor GetIconColor() const override
	{
		return UE::StateTree::Colors::DarkGrey;
	}
#endif
	
	UPROPERTY(EditAnywhere, Category = Condition)
	EGameplayContainerMatchType MatchType = EGameplayContainerMatchType::Any;

	/** If true, the tag has to be exactly present, if false then TagContainer will include it's parent tags while matching */
	UPROPERTY(EditAnywhere, Category = Condition)
	bool bExactMatch = false;

	UPROPERTY(EditAnywhere, Category = Condition)
	bool bInvert = false;
};

USTRUCT()
struct FStateTreeCompareTDWDistanceConditionInstanceData
{
	GENERATED_BODY()
	
	/** Character to check distance from. */
	UPROPERTY(EditAnywhere, Category = "Input")
	ACharacter* Character = nullptr;

	/** Whether to compare distance to another Character instead of a location. */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bCompareToOtherCharacter = false;
	
	/** Target Character to check distance to if bCompareToOtherCharacter is true. */
	UPROPERTY(EditAnywhere, Category = "Parameter", meta = (EditCondition = "bCompareToOtherCharacter"))
	AActor* TargetCharacter = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Parameter", meta = (EditCondition = "!bCompareToOtherCharacter"))
	FVector TargetLocation = FVector(EForceInit::ForceInitToZero);

	UPROPERTY(EditAnywhere, Category = "Parameter")
	double Distance = 0.0;
};
STATETREE_POD_INSTANCEDATA(FStateTreeCompareTDWDistanceConditionInstanceData);

/**
 * Condition comparing distance between a Character and a Location.
 */
USTRUCT(DisplayName = "Character Distance Compare")
struct FStateTreeCompareTDWDistanceCondition : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeCompareTDWDistanceConditionInstanceData;

	FStateTreeCompareTDWDistanceCondition() = default;
	explicit FStateTreeCompareTDWDistanceCondition(const EGenericAICheck InOperator, const EStateTreeCompare InInverts = EStateTreeCompare::Default)
		: bInvert(InInverts == EStateTreeCompare::Invert)
		, Operator(InOperator)
	{}
	
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;
	
	UPROPERTY(EditAnywhere, Category = "Condition")
	bool bInvert = false;

	UPROPERTY(EditAnywhere, Category = "Condition", meta = (InvalidEnumValues = "IsTrue"))
	EGenericAICheck Operator = EGenericAICheck::Equal;
};

USTRUCT()
struct FStateTreeCompareHealthConditionInstanceData
{
	GENERATED_BODY()
	
	/** Character to check health from. */
	UPROPERTY(EditAnywhere, Category = "Input")
	ATDWCharacter* Character = nullptr;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bCompareToPercentage = true;

	UPROPERTY(EditAnywhere, Category = "Parameter", meta = (EditCondition = "bCompareToPercentage", Editconditionhides, ClampMin = "0.0", ClampMax = "100.0"))
	double HealthPercentage = 0.0;

	UPROPERTY(EditAnywhere, Category = "Parameter", meta = (EditCondition = "!bCompareToPercentage", Editconditionhides))
	double TotalHealth = 0.0;
};
STATETREE_POD_INSTANCEDATA(FStateTreeCompareHealthConditionInstanceData);

/**
 * Condition comparing health of a Character.
 */
USTRUCT(DisplayName = "Character Health Compare")
struct FStateTreeCompareHealthCondition : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeCompareHealthConditionInstanceData;

	FStateTreeCompareHealthCondition() = default;
	explicit FStateTreeCompareHealthCondition(const EGenericAICheck InOperator, const EStateTreeCompare InInverts = EStateTreeCompare::Default)
		: bInvert(InInverts == EStateTreeCompare::Invert)
		, Operator(InOperator)
	{}
	
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;
	
	UPROPERTY(EditAnywhere, Category = "Condition")
	bool bInvert = false;

	UPROPERTY(EditAnywhere, Category = "Condition", meta = (InvalidEnumValues = "IsTrue"))
	EGenericAICheck Operator = EGenericAICheck::Equal;
};

USTRUCT()
struct FStateTreeCompareNumberOfTargetsOnSightConditionInstanceData
{
	GENERATED_BODY()
	
	/** AI Controller to check targets on sight from. */
	UPROPERTY(EditAnywhere, Category = "Context")
	ATDWAIController* AIController = nullptr;

	/** Weather to limit the count to specific affiliation. */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bShouldLimitToAffiliation = false;

	/** Affiliation of targets to count. */
	UPROPERTY(EditAnywhere, Category = "Parameter", meta = (EditCondition = "bShouldLimitToAffiliation", Editconditionhides))
	TEnumAsByte<ETeamAttitude::Type> AttitudeToMatch;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	int32 NumberOfTargets = 0;
};
STATETREE_POD_INSTANCEDATA(FStateTreeCompareNumberOfTargetsOnSightConditionInstanceData);

/**
 * Condition comparing number of targets on sight of an AI Controller.
 */
USTRUCT(DisplayName = "Number of Targets On Sight Compare")
struct FStateTreeCompareNumberOfTargetsOnSightCondition : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeCompareNumberOfTargetsOnSightConditionInstanceData;

	FStateTreeCompareNumberOfTargetsOnSightCondition() = default;
	explicit FStateTreeCompareNumberOfTargetsOnSightCondition(const EGenericAICheck InOperator, const EStateTreeCompare InInverts = EStateTreeCompare::Default)
		: bInvert(InInverts == EStateTreeCompare::Invert)
		, Operator(InOperator)
	{}
	
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;
	
	UPROPERTY(EditAnywhere, Category = "Condition")
	bool bInvert = false;

	UPROPERTY(EditAnywhere, Category = "Condition", meta = (InvalidEnumValues = "IsTrue"))
	EGenericAICheck Operator = EGenericAICheck::Equal;
};

USTRUCT()
struct FStateTreeCompareDotConditionInstanceData
{
	GENERATED_BODY()
	
	/** Actor to check forward vector from. */
	UPROPERTY(EditAnywhere, Category = "Context")
	AActor* ActorToCompareForward = nullptr;

	/** Weather to use location as target instead of target actor. */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bUseLocationAsTarget = false;

	/** Target location to compare to if bUseLocationAsTarget is true. */
	UPROPERTY(EditAnywhere, Category = "Parameter", meta = (EditCondition = "bUseLocationAsTarget", Editconditionhides))
	FVector TargetLocation = FVector::ZeroVector;

	/** Target actor to compare to if bUseLocationAsTarget is false. */
	UPROPERTY(EditAnywhere, Category = "Parameter", meta = (EditCondition = "!bUseLocationAsTarget", Editconditionhides))
	AActor* TargetActor = nullptr;
	
	/** Dot product value to compare against. */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float DotProductValue = 0.0f;
};
STATETREE_POD_INSTANCEDATA(FStateTreeCompareDotConditionInstanceData);

/**
 * Condition comparing dot product between actor forward and direction to target.
 */
USTRUCT(DisplayName = "Dot Product Compare")
struct FStateTreeCompareDotCondition : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeCompareDotConditionInstanceData;

	FStateTreeCompareDotCondition() = default;
	explicit FStateTreeCompareDotCondition(const EGenericAICheck InOperator, const EStateTreeCompare InInverts = EStateTreeCompare::Default)
		: bInvert(InInverts == EStateTreeCompare::Invert)
		, Operator(InOperator)
	{}
	
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;
	
	UPROPERTY(EditAnywhere, Category = "Condition")
	bool bInvert = false;

	UPROPERTY(EditAnywhere, Category = "Condition", meta = (InvalidEnumValues = "IsTrue"))
	EGenericAICheck Operator = EGenericAICheck::Equal;
};




