#include "GameplayTags/TDWGameplayTags.h"

namespace TDWGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Action_Combat_PrimaryAttack, "Action.Combat.PrimaryAttack", "Tag for primary combat attack action.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Action_Combat_SecondaryAttack, "Action.Combat.SecondaryAttack", "Tag for secondary combat attack action.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Action_Combat_MeleeAttack, "Action.Combat.MeleeAttack", "Tag for melee combat attack action.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Action_Combat_RangedAttack, "Action.Combat.RangedAttack", "Tag for ranged combat attack action.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Action_Combat_RangedSpecialAttack, "Action.Combat.RangedSpecialAttack", "Tag for ranged special combat attack action.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Action_Combat_Healing, "Action.Combat.Healing", "Tag for combat healing action.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Dead, "Status.Dead", "Tag indicating that the character is dead.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_DamageAmount, "Damage.DamageAmount", "Tag representing the amount of damage dealt.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Events_Combat_AttackHitBoxCheck, "Events.Combat.AttackHitBoxCheck", "Tag for combat attack hitbox check event.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Events_Combat_Healing, "Events.Combat.Healing", "Tag for combat healing event.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Combat_HitReceived, "GameplayCue.Combat.HitReceived", "Gameplay cue for when a character receives a hit in combat.");
	
}