#include "GameplayTags/TDWGameplayTags.h"

namespace TDWGameplayTags
{
	/*
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Combat_PrimaryAttack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Combat_SecondaryAttack);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Combat_MeleeAttack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Combat_RangedAttack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Combat_RangedSpecialAttack);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Combat_Healling);
	*/
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Action_Combat_PrimaryAttack, "Action.Combat.PrimaryAttack", "Tag for primary combat attack action.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Action_Combat_SecondaryAttack, "Action.Combat.SecondaryAttack", "Tag for secondary combat attack action.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Action_Combat_MeleeAttack, "Action.Combat.MeleeAttack", "Tag for melee combat attack action.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Action_Combat_RangedAttack, "Action.Combat.RangedAttack", "Tag for ranged combat attack action.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Action_Combat_RangedSpecialAttack, "Action.Combat.RangedSpecialAttack", "Tag for ranged special combat attack action.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Action_Combat_Healing, "Action.Combat.Healing", "Tag for combat healing action.");
}