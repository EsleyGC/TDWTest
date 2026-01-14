// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifies/TDWGenericEventAnimNotify.h"

#include "AbilitySystemComponent.h"
#include "Character/TDWCharacter.h"

void UTDWGenericEventAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (!IsValid(MeshComp))
	{
		return;
	}
	
	ATDWCharacter* Character = Cast<ATDWCharacter>(MeshComp->GetOwner());
	if (!IsValid(Character))
	{
		return;
	}
	
	UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
	if (!IsValid(ASC))
	{
		return;
	}
	
	FGameplayEventData* EventData = new FGameplayEventData();
	EventData->Target = Character;
	ASC->HandleGameplayEvent(EventTag, EventData);
}
