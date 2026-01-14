// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TDWDamageableInterface.generated.h"

UINTERFACE()
class UTDWDamageableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class THEDARKWEST_API ITDWDamageableInterface
{
	GENERATED_BODY()

public:
	/** Returns true if the actor is dead.
	 * If the actor is dead, it should not be able to receive any more damage or effects.
	 */
	virtual bool IsDead() { return false; }
	
	/** Returns true if the actor has an ability system component. 
	 * Useful to check before calling GetAbilitySystemComponent().
	 */
	virtual bool HasAbilitySystemComponent() { return false; }
	
	/** Returns the actor's ability system component. 
	 * Returns nullptr if the actor does not have an ability system component.
	 */
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() { return nullptr; }
	
	/** Apply damage to actors that do not have an ability system component.
	 * This function is called when applying damage to an actor that does not implement an ability system component.
	 * Override this function to implement custom damage logic for non-ability system actors.
	 */
	virtual void ApplyDamageToNonAbilitySystemActor(float DamageAmount) {  }
};
