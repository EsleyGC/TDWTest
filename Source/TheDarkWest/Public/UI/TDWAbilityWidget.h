// TDW Test

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "TDWAbilityWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEDARKWEST_API UTDWAbilityWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "TDW|Ability System Component")
	void InitASC(UAbilitySystemComponent* AbilitySystemComponent);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "TDW|Ability System Component")
	void OnHealthChanged(float CurrentHealth, float MaxHealth);
};
