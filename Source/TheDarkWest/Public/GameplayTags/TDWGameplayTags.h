#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace TDWGameplayTags
{
	/*
	 * Action Tags
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Combat_PrimaryAttack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Combat_SecondaryAttack);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Combat_MeleeAttack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Combat_RangedAttack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Combat_RangedSpecialAttack);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Combat_Healing);
	
	/*
	 * Status Tags
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Idle);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Combat);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Dead);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Inspired);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_HealCooldown);

	/**
	 * Set ByCaller Tags
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_DamageAmount);
	
	/** 
	 * Events Tags
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Events_Combat_AttackHitBoxCheck);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Events_Combat_Healing);

	/**
	 * GameplayCues
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Combat_HitReceive);
	
	/*
	 * AI Types
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CharacterType_Player);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CharacterType_AI_Melee);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CharacterType_AI_Ranged);
	
}