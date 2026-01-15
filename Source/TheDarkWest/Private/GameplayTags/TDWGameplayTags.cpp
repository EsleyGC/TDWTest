#include "GameplayTags/TDWGameplayTags.h"

namespace TDWGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Action_Combat_PrimaryAttack, "Action.Combat.PrimaryAttack", "Tag for primary combat attack action.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Action_Combat_SecondaryAttack, "Action.Combat.SecondaryAttack", "Tag for secondary combat attack action.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Action_Combat_MeleeAttack, "Action.Combat.MeleeAttack", "Tag for melee combat attack action.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Action_Combat_RangedAttack, "Action.Combat.RangedAttack", "Tag for ranged combat attack action.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Action_Combat_RangedSpecialAttack, "Action.Combat.RangedSpecialAttack", "Tag for ranged special combat attack action.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Action_Combat_Healing, "Action.Combat.Healing", "Tag for combat healing action.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Idle, "Status.Idle", "Tag indicating that the character is relaxed.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Combat, "Status.Combat", "Tag indicating that the character is in combat mode, searching for a valid target on engaged.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Dead, "Status.Dead", "Tag indicating that the character is dead.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Inspired, "Status.Inspired", "Tag indicating that the character is inspired.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_HealCooldown, "Status.HealCooldown", "Tag indicating that the character is on healing cooldown.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_DamageAmount, "Damage.DamageAmount", "Tag representing the amount of damage dealt.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Events_Combat_AttackHitBoxCheck, "Events.Combat.AttackHitBoxCheck", "Tag for combat attack hitbox check event.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Events_Combat_Healing, "Events.Combat.Healing", "Tag for combat healing event.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Combat_HitReceived, "GameplayCue.Combat.HitReceived", "Gameplay cue for when a character receives a hit in combat.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(CharacterType_Player, "CharacterType.Player", "Tag for player characters.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(CharacterType_AI_Melee, "CharacterType.AI.Melee", "Tag for melee type AI characters.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(CharacterType_AI_Ranged, "CharacterType.AI.Ranged", "Tag for ranged type AI characters.");
	
}