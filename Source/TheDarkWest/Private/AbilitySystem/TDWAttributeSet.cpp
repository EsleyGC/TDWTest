// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TDWAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Character/TDWCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UTDWAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
}

void UTDWAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
}

void UTDWAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	if (Attribute == GetMoveSpeedAttribute())
	{
		if (UCharacterMovementComponent* MovementComponent = GetCharacterMovementComponent(); IsValid(MovementComponent))
		{
			MovementComponent->MaxWalkSpeed = NewValue * 10;
		}
	}
}

UCharacterMovementComponent* UTDWAttributeSet::GetCharacterMovementComponent() const
{
	if (const FGameplayAbilityActorInfo* ActorInfo = GetActorInfo())
	{
		return Cast<UCharacterMovementComponent>(ActorInfo->MovementComponent);
	}

	if (const AActor* Owner = GetOwningActor())
	{
		return Cast<UCharacterMovementComponent>(Owner->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	}

	return nullptr;
}

ATDWCharacter* UTDWAttributeSet::GetTDWCharacter() const
{
	if (const FGameplayAbilityActorInfo* ActorInfo = GetActorInfo())
	{
		return Cast<ATDWCharacter>(ActorInfo->OwnerActor);
	}

	if (AActor* Owner = GetOwningActor())
	{
		return Cast<ATDWCharacter>(Owner);
	}

	return nullptr;
}
