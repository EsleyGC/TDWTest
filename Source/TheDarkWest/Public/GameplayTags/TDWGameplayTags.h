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
	
	
}