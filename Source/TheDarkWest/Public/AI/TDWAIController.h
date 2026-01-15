
#pragma once

#include "CoreMinimal.h"
#include "Character/TDWCharacter.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "TDWAIController.generated.h"

class UStateTree;
class FTDWTypes;
class USplineComponent;
class UStateTreeAIComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnForgetActor, AActor* /*ForgottenActor*/);

UCLASS()
class THEDARKWEST_API ATDWAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATDWAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddInputDirection(const FVector& InputDirection) const;

	void SetLookAtLocation(const FVector& NewLookAtTargetPosition);
	FVector GetLookAtPosition() const;
	
	ATDWCharacter* GetTDWCharacter() const { return TDWCharacter; }
	
	void SetCurrentTarget(ATDWCharacter* NewTarget) { CurrentTarget = NewTarget; }
	ATDWCharacter* GetCurrentTarget() const {return CurrentTarget;}
	
	USplineComponent* GetSplinePathComponent() const { return SplinePathComponent; }
	
	/** Handles a generic action based on the provided action tag */
	void HandleAction(const FGameplayTag& ActionTag) const;

	// Generic Team Interface
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	// End Generic Team Interface

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	UStateTreeAIComponent* StateTreeComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<ATDWCharacter> CurrentTarget;
	
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	FVector LastLookAtLocation;

	/** Map of gameplay tags to select State Tree assets 
	 * 
	 * Currently not working, needs more development to support loading State Trees at runtime
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Character")
	TMap<FGameplayTag, TSoftObjectPtr<UStateTree>> StateTreeByTag;

protected:

	UPROPERTY(BlueprintReadOnly, Category= "Character")
	TObjectPtr<ATDWCharacter> TDWCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Character")
	TObjectPtr<USplineComponent> SplinePathComponent;
};
