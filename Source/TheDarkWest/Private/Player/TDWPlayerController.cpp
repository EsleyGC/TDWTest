// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TDWPlayerController.h"

#include "GameplayTags/TDWGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/TDWAttributeSet.h"
#include "Character/TDWCharacter.h"
#include "Navigation/CrowdManager.h"
#include "UI/TDWAbilityWidget.h"

DEFINE_LOG_CATEGORY(TDWLogPlayerController);

void ATDWPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetupInput();
	TDWCharacter = Cast<ATDWCharacter>(GetPawn());
	
	if (PlayerHUDClass)
	{
		AbilityWidget = CreateWidget<UTDWAbilityWidget>(this, PlayerHUDClass);
		if (AbilityWidget)
		{
			AbilityWidget->AddToViewport();
			AbilityWidget->InitASC(TDWCharacter ? TDWCharacter->GetAbilitySystemComponent() : nullptr);			
		}
	}
	
	TDWCharacter->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(UTDWAttributeSet::GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
}

void ATDWPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	TDWCharacter->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(UTDWAttributeSet::GetHealthAttribute()).RemoveAll(this);
}

void ATDWPlayerController::SetupInput() const
{
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!InputSubsystem)
	{
		return;
	}

	InputSubsystem->AddMappingContext(MappingContext, 0);
}

void ATDWPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	TDWCharacter = Cast<ATDWCharacter>(InPawn);
	if (!IsValid(TDWCharacter))
	{
		UE_LOG(TDWLogPlayerController, Error, TEXT("ATDWCharacter is NULL"));
		return;
	}

	if (UCrowdManager* CrowdManager = UCrowdManager::GetCurrent(this); IsValid(CrowdManager))
	{
		CrowdManager->RegisterAgent(TDWCharacter);
	}
}

void ATDWPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateTargetPosition();
}

void ATDWPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Triggered, this, &ThisClass::PrimaryActionInputPress);
	EnhancedInputComponent->BindAction(SecondaryAction, ETriggerEvent::Triggered, this, &ThisClass::SecondaryActionInputPress);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::MoveInput);
}

void ATDWPlayerController::UpdateTargetPosition()
{
	if (!IsValid(TDWCharacter))
	{
		return;
	}

	FVector MouseStartPosition;
	FVector MouseDirection;
	if (!DeprojectMousePositionToWorld(MouseStartPosition, MouseDirection))
	{
		return;
	}
	
	const float CharacterHalfHeight = TDWCharacter->GetDefaultHalfHeight();
	const FVector PlanePoint = TDWCharacter->GetActorLocation() - FVector(0.0f, 0.0f, CharacterHalfHeight);
	const FVector PlaneNormal = FVector::UpVector;

	const FVector RayEnd = MouseStartPosition + MouseDirection * 10000.0f;
	const FVector Intersection = FMath::LinePlaneIntersection(MouseStartPosition, RayEnd, PlanePoint, PlaneNormal);

	CurrentTargetPosition = Intersection;
	TDWCharacter->SetLookAtLocation(CurrentTargetPosition);
}

void ATDWPlayerController::PrimaryActionInputPress(const FInputActionValue& InputActionValue)
{
	if (!IsValid(TDWCharacter))
	{
		return;
	}

	UAbilitySystemComponent* TDWASC = Cast<UAbilitySystemComponent>(TDWCharacter->GetAbilitySystemComponent());
	if (!IsValid(TDWASC))
	{
		return;
	}

	const FGameplayTagContainer TagContainer = FGameplayTagContainer(TDWGameplayTags::Action_Combat_PrimaryAttack);	
	const bool bSuccess = TDWASC->TryActivateAbilitiesByTag(TagContainer);
	if (bSuccess)
	{
		return;
	}
	
	TArray<FGameplayAbilitySpec*> MatchingGameplayAbilities;
	TDWASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, MatchingGameplayAbilities);
	if (MatchingGameplayAbilities.Num() > 0)
	{
		for (FGameplayAbilitySpec* AbilitySpec : MatchingGameplayAbilities)
		{
			TDWASC->AbilitySpecInputPressed(*AbilitySpec);
		}
	}
}

void ATDWPlayerController::SecondaryActionInputPress(const FInputActionValue& InputActionValue)
{
	if (!IsValid(TDWCharacter))
	{
		return;
	}

	UAbilitySystemComponent* TDWASC = Cast<UAbilitySystemComponent>(TDWCharacter->GetAbilitySystemComponent());
	if (!IsValid(TDWASC))
	{
		return;
	}

	const FGameplayTagContainer TagContainer = FGameplayTagContainer(TDWGameplayTags::Action_Combat_SecondaryAttack);	
	const bool bSuccess = TDWASC->TryActivateAbilitiesByTag(TagContainer);
	if (bSuccess)
	{
		return;
	}
	
	TArray<FGameplayAbilitySpec*> MatchingGameplayAbilities;
	TDWASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, MatchingGameplayAbilities);
	if (MatchingGameplayAbilities.Num() > 0)
	{
		for (FGameplayAbilitySpec* AbilitySpec : MatchingGameplayAbilities)
		{
			TDWASC->AbilitySpecInputPressed(*AbilitySpec);
		}
	}
}

void ATDWPlayerController::MoveInput(const FInputActionValue& InputActionValue)
{
	if (!IsValid(TDWCharacter))
	{
		return;
	}

	//Simple north/east/south/west movement, since the camera is fixed we can just map X to Y and Y to X
	const FVector InputAxisVector = InputActionValue.Get<FVector>();
	return TDWCharacter->AddMovementInput(FVector(InputAxisVector.Y, InputAxisVector.X, 0));
}

void ATDWPlayerController::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (AbilityWidget)
	{
		const UTDWAttributeSet* BaseAttributeSet = TDWCharacter->GetAttributeSet();
		const float MaxHealth = BaseAttributeSet->GetMaxHealth();
		AbilityWidget->OnHealthChanged(Data.NewValue, MaxHealth);
	}
}
