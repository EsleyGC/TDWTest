// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "Navigation/CrowdAgentInterface.h"
#include "AbilitySystemInterface.h"
#include "ActiveGameplayEffectHandle.h"
#include "TDWCharacter.generated.h"

class UGameplayEffect;
class UTDWAttributeSet;
class UTDWMovementComponent;

UCLASS()
class THEDARKWEST_API ATDWCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface, public ICrowdAgentInterface, public IGenericTeamAgentInterface
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
	
	virtual void InitAbilityActorInfo();
	
	/** Adds the initial gameplay effects containing the base attributes and abilities to the character */
	void InitializeBaseGameplayEffects() const;
	
	FActiveGameplayEffectHandle ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level = 1.0f) const;
	
	virtual void SetLookAtLocation(const FVector& NewTargetPosition);
	virtual FVector GetCurrentLookAtLocation() const { return CurrentLookAtLocation; }

	void RotateToTarget(const float& DeltaTime);
	void UpdateLookAtPosition(const float& DeltaTime);

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

	UPROPERTY()
	TObjectPtr<UTDWAttributeSet> BaseAttributeSet;

	UPROPERTY(EditAnywhere, Category="TDW|Team")
	FGenericTeamId TeamId;
	
	UPROPERTY(BlueprintReadOnly, Category = "TDW|Rotation")
	FVector CurrentLookAtLocation;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "TDW|Attributes")
	TArray<TSubclassOf<UGameplayEffect>> InitialGameplayEffects;
	
	UPROPERTY(EditAnywhere, Category = "TDW|Rotation")
	float CurrentRotationSpeed = 6.0f;
	
};
