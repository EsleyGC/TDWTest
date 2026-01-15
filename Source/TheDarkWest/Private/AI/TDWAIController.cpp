
#include "AI/TDWAIController.h"

#include "AbilitySystemComponent.h"
#include "AI/TDWCrowdFollowingComponent.h"
#include "Components/SplineComponent.h"
#include "Components/StateTreeAIComponent.h"
#include "AI/TDWCrowdFollowingComponent.h"
#include "GameplayTags/TDWGameplayTags.h"
#include "Perception/AIPerceptionComponent.h"

ATDWAIController::ATDWAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UTDWCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	PrimaryActorTick.bCanEverTick = true;
	StateTreeComponent = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTreeComponent"));
	SplinePathComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplinePathComponent"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
}

// Called when the game starts or when spawned
void ATDWAIController::BeginPlay()
{
	Super::BeginPlay();

	TDWCharacter = Cast<ATDWCharacter>(GetPawn());
	check(TDWCharacter);
}

void ATDWAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ATDWAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATDWAIController::AddInputDirection(const FVector& InputDirection) const
{
	TDWCharacter->AddMovementInput(InputDirection);
}

void ATDWAIController::SetLookAtLocation(const FVector& NewLookAtTargetPosition)
{
	LastLookAtLocation = NewLookAtTargetPosition;
	TDWCharacter->SetLookAtLocation(NewLookAtTargetPosition);
}

FVector ATDWAIController::GetLookAtPosition() const
{
	if (!IsValid(TDWCharacter))
	{
		const APawn* CurrentPawn = GetPawn();
		if (!IsValid(CurrentPawn))
		{
			return FVector::ZeroVector;
		}

		return CurrentPawn->GetActorForwardVector();
	}
	
	return TDWCharacter->GetCurrentLookAtLocation();
}

void ATDWAIController::HandleAction(const FGameplayTag& ActionTag) const
{
	if (!IsValid(TDWCharacter))
	{
		return;
	}

	UAbilitySystemComponent* TDWASC = Cast<UAbilitySystemComponent>(TDWCharacter->GetAbilitySystemComponent());
	if (!IsValid(TDWASC))
	{
		return;
	}

	const FGameplayTagContainer TagContainer = FGameplayTagContainer(ActionTag);	
	const bool bSuccess = TDWASC->TryActivateAbilitiesByTag(TagContainer);
	if (bSuccess)
	{
		return;
	}
	
	TArray<FGameplayAbilitySpec*> MatchingGameplayAbilities;
	TDWASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, MatchingGameplayAbilities);
	if (MatchingGameplayAbilities.Num() > 0)
	{
		for (FGameplayAbilitySpec* AbilitySpec : MatchingGameplayAbilities)
		{
			TDWASC->AbilitySpecInputPressed(*AbilitySpec);
		}
	}
}

void ATDWAIController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (IsValid(TDWCharacter))
	{
		TDWCharacter->SetGenericTeamId(NewTeamID);
	}
}

FGenericTeamId ATDWAIController::GetGenericTeamId() const
{
	if (IsValid(TDWCharacter))
	{
		return TDWCharacter->GetGenericTeamId();
	}

	return Super::GetGenericTeamId();
}

ETeamAttitude::Type ATDWAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (IsValid(TDWCharacter))
	{
		return TDWCharacter->GetTeamAttitudeTowards(Other);
	}

	return Super::GetTeamAttitudeTowards(Other);
}
