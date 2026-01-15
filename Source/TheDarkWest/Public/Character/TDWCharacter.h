// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "Navigation/CrowdAgentInterface.h"
#include "AbilitySystemInterface.h"
#include "ActiveGameplayEffectHandle.h"
#include "Combat/TDWDamageableInterface.h"
#include "GameplayTags/TDWGameplayTags.h"
#include "Perception/AISightTargetInterface.h"
#include "TDWCharacter.generated.h"

class UGameplayEffect;
class UTDWAttributeSet;
class UTDWMovementComponent;


UCLASS()
class THEDARKWEST_API ATDWCharacter : public ACharacter, 
public IAbilitySystemInterface, public IGameplayTagAssetInterface, public ICrowdAgentInterface, public IGenericTeamAgentInterface, public ITDWDamageableInterface, public IAISightTargetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATDWCharacter(const FObjectInitializer& ObjectInitializer);
	
	// ----- IAbilitySystemInterface Methods ------
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// -----------------------------------------------
	
	// ----- GameplayTagAssetInterface Methods ------
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	// -----------------------------------------------

	// ----- ICrowdAgentInterface Methods ------
	virtual FVector GetCrowdAgentLocation() const override;
	virtual FVector GetCrowdAgentVelocity() const override;
	virtual void GetCrowdAgentCollisions(float& CylinderRadius, float& CylinderHalfHeight) const override;
	virtual float GetCrowdAgentMaxSpeed() const override;
	// -----------------------------------------------

	// ----- IGenericTeamAgentInterface Methods ------
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	// -----------------------------------------------
	
	// ----- ITDWDamageableInterface Methods ------
	virtual bool IsDead() override;
	virtual bool CanBeDamaged(const AActor* DamageInstigator) override;
	virtual bool HasAbilitySystemComponent() override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() override;
	virtual void NotifyDamageTaken(float DamageAmount, AActor* DamageInstigator, const FVector Origin) override;
	// -----------------------------------------------
	
	// ----- IAISightTargetInterface Methods --------
	virtual UAISense_Sight::EVisibilityResult CanBeSeenFrom(const FCanBeSeenFromContext& Context, FVector& OutSeenLocation, int32& OutNumberOfLoSChecksPerformed, int32& OutNumberOfAsyncLosCheckRequested, float& OutSightStrength, int32* UserData = nullptr, const FOnPendingVisibilityQueryProcessedDelegate* Delegate = nullptr) override;
	// -----------------------------------------------
	
	bool IsTraceConsideredVisible(const FHitResult* HitResult, const AActor* TargetActor);

	virtual void InitAbilityActorInfo();
	
	/** Adds the initial gameplay effects containing the base attributes and abilities to the character */
	void InitializeBaseGameplayEffects() const;
	
	FActiveGameplayEffectHandle ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level = 1.0f) const;
	
	UTDWAttributeSet* GetAttributeSet() const { return BaseAttributeSet; }
	
	virtual void SetLookAtLocation(const FVector& NewTargetPosition);
	virtual FVector GetCurrentLookAtLocation() const { return CurrentLookAtLocation; }
	
	FGameplayTag GetCharacterTypeTag() const { return CharacterTypeTag; }

	/** Rotates the character to face the CurrentLookAtLocation */
	void RotateToTarget(const float& DeltaTime);
	
	/** Character's death logic */
	void Die();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
protected:
	
	//----------------------------------------------------------------------------
	// Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TDW|Components")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TDW|Components")
	TObjectPtr<UTDWMovementComponent> TDWMovementComponent;
	
	//----------------------------------------------------------------------------
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TDW|Crowd Following")
	float AvoidanceCollisionRadius = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TDW|Crowd Following")
	float AvoidanceCollisionHalfHeight = 90.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TDW|Type")
	FGameplayTag CharacterTypeTag = TDWGameplayTags::CharacterType_Player;

	UPROPERTY()
	UTDWAttributeSet* BaseAttributeSet;

	/** Defines the team ID for this character 
	 * This will be used for team-based logic, such as AI behavior and damage application
	 */
	UPROPERTY(EditAnywhere, Category="TDW|Team")
	FGenericTeamId TeamId;
	
	/** Current location the character is looking at 
	 * Either set by AI or player input (mouse cursor location in the world)
	 */
	UPROPERTY(BlueprintReadOnly, Category = "TDW|Rotation")
	FVector CurrentLookAtLocation;
	
	/** Initial gameplay effects to apply to the character on BeginPlay */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "TDW|Attributes")
	TArray<TSubclassOf<UGameplayEffect>> InitialGameplayEffects;
	
	/** Gameplay effect to apply when the character dies */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "TDW|GameplayEffects")
	TSubclassOf<UGameplayEffect> DeathGameplayEffectClass;
	
	/** How fast the character rotates to face the look at location */
	UPROPERTY(EditAnywhere, Category = "TDW|Rotation")
	float CurrentRotationSpeed = 6.0f;
	
private:
	
	/** Flag indicating if the character is dead */
	bool bIsDead;
	
};
