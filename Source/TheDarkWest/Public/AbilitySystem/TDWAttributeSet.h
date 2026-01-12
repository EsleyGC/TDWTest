// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "TDWAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class ATDWCharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class THEDARKWEST_API UTDWAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	UCharacterMovementComponent* GetCharacterMovementComponent() const;
	ATDWCharacter* GetTDWCharacter() const;
	
	/*
	 * Vital Attributes
	 */
	
	UPROPERTY(BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UTDWAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UTDWAttributeSet, MaxHealth);
	
	/*
	 * Movement Attributes
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Movement Attributes")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UTDWAttributeSet, MoveSpeed);
};
