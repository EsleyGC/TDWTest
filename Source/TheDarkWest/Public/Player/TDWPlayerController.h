// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TDWPlayerController.generated.h"

struct FOnAttributeChangeData;
class UTDWAbilityWidget;
class ATDWCharacter;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(TDWLogPlayerController, Log, All);

/**
 * 
 */
UCLASS()
class THEDARKWEST_API ATDWPlayerController : public APlayerController
{
	
	GENERATED_BODY()
	
public:
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void SetupInput() const;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupInputComponent() override;
	
	void UpdateTargetPosition();	
	
	UFUNCTION()
	void PrimaryActionInputPress(const FInputActionValue& InputActionValue);
	
	UFUNCTION()
	void SecondaryActionInputPress(const FInputActionValue& InputActionValue);
	
	UFUNCTION()
	void MoveInput(const FInputActionValue& InputActionValue);
	
	
	void OnHealthChanged(const FOnAttributeChangeData& Data);
	
protected:
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ATDWCharacter> TDWCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* MappingContext {nullptr};

	UPROPERTY(BlueprintReadOnly)
	FVector CurrentTargetPosition {FVector::ZeroVector};UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> PrimaryAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> SecondaryAction;
	
	UPROPERTY(EditAnywhere, Category="HUD")
	TSubclassOf<UTDWAbilityWidget> PlayerHUDClass;
	
	UPROPERTY(VisibleAnywhere, Category="HUD")
	TObjectPtr<UTDWAbilityWidget> AbilityWidget;
};
