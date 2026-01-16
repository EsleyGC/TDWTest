// Fill out your copyright notice in the Description page of Project Settings.


#include "TheDarkWest/Public/Character/TDWCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/TDWAttributeSet.h"
#include "Character/Components/TDWMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AISense_Damage.h"
#include "UI/TDWAbilityWidget.h"


ATDWCharacter::ATDWCharacter(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UTDWMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	BaseAttributeSet = CreateDefaultSubobject<UTDWAttributeSet>(TEXT("AttributeSet"));
	TDWMovementComponent = Cast<UTDWMovementComponent>(GetCharacterMovement());
	
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetRootComponent());
}

UAbilitySystemComponent* ATDWCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ATDWCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
	
	const FVector CurrentLocation = GetActorLocation();
	const FVector CurrentForward = GetActorForwardVector();
	const FVector LookAtLocation = CurrentLocation + (CurrentForward * 100.0f);
	SetLookAtLocation(LookAtLocation);

	TDWAbilityWidget = Cast<UTDWAbilityWidget>(WidgetComponent->GetUserWidgetObject());
	if (TDWAbilityWidget)
	{
		TDWAbilityWidget->InitASC(AbilitySystemComponent);
		const float MaxHealth = BaseAttributeSet->GetMaxHealth();

		TDWAbilityWidget->OnHealthChanged(MaxHealth, MaxHealth);
		
	}
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UTDWAttributeSet::GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
}

void ATDWCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UTDWAttributeSet::GetHealthAttribute()).RemoveAll(this);
}

void ATDWCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	RotateToTarget(DeltaTime);
}

void ATDWCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	AbilitySystemComponent->GetOwnedGameplayTags(TagContainer);
}

bool ATDWCharacter::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	return AbilitySystemComponent->HasMatchingGameplayTag(TagToCheck);
}

bool ATDWCharacter::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->HasAllMatchingGameplayTags(TagContainer);
}

bool ATDWCharacter::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->HasAnyMatchingGameplayTags(TagContainer);
}

FVector ATDWCharacter::GetCrowdAgentLocation() const
{
	return GetActorLocation();
}

FVector ATDWCharacter::GetCrowdAgentVelocity() const
{
	return TDWMovementComponent->Velocity;
}

void ATDWCharacter::GetCrowdAgentCollisions(float& CylinderRadius, float& CylinderHalfHeight) const
{
	CylinderRadius = AvoidanceCollisionRadius;
	CylinderHalfHeight = AvoidanceCollisionHalfHeight;
}

float ATDWCharacter::GetCrowdAgentMaxSpeed() const
{
	return TDWMovementComponent->GetMaxSpeed();
}

void ATDWCharacter::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	TeamId = TeamID;
}

ETeamAttitude::Type ATDWCharacter::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(&Other);
	return OtherTeamAgent ? FGenericTeamId::GetAttitude(GetGenericTeamId(), OtherTeamAgent->GetGenericTeamId())
		: ETeamAttitude::Neutral;
}

bool ATDWCharacter::IsDead()
{
	return bIsDead;
}

bool ATDWCharacter::CanBeDamaged(const AActor* DamageInstigator)
{
	if (!IsValid(DamageInstigator))
	{
		// Unknown instigator, allow damage by default
		return true;
	}
	
	// Prevent friendly fire based on team attitude
	const ETeamAttitude::Type Attitude = GetTeamAttitudeTowards(*DamageInstigator);
	return Attitude == ETeamAttitude::Hostile;
}

bool ATDWCharacter::HasAbilitySystemComponent()
{
	return true;
}

UAbilitySystemComponent* ATDWCharacter::GetAbilitySystemComponent()
{
	return AbilitySystemComponent;
}

void ATDWCharacter::NotifyDamageTaken(float DamageAmount, AActor* DamageInstigator, const FVector Origin)
{
	UAISense_Damage::ReportDamageEvent(GetWorld(), this, DamageInstigator, DamageAmount, Origin, GetActorLocation());
}

UAISense_Sight::EVisibilityResult ATDWCharacter::CanBeSeenFrom(const FCanBeSeenFromContext& Context, FVector& OutSeenLocation, int32& OutNumberOfLoSChecksPerformed, int32& OutNumberOfAsyncLosCheckRequested, float& OutSightStrength, int32* UserData, const FOnPendingVisibilityQueryProcessedDelegate* Delegate)
{
	if (bIsDead)
	{
		return UAISense_Sight::EVisibilityResult::NotVisible;
	}
	
	// Here, we replicate the Sync check from the Sight sensor
	const FVector TargetLocation = GetActorLocation();
	FHitResult HitResult;
	const FCollisionQueryParams QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(AILineOfSight), true, Context.IgnoreActor);
	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Context.ObserverLocation, TargetLocation, ECC_Visibility, QueryParams, FCollisionResponseParams::DefaultResponseParam);

	++OutNumberOfLoSChecksPerformed;

	if (IsTraceConsideredVisible(bHit ? &HitResult : nullptr, this))
	{
		OutSeenLocation = TargetLocation;
		return UAISense_Sight::EVisibilityResult::Visible;
	}
	else
	{
		return UAISense_Sight::EVisibilityResult::NotVisible;
	}
}

bool ATDWCharacter::IsTraceConsideredVisible(const FHitResult* HitResult, const AActor* TargetActor)
{
	if (HitResult == nullptr)
	{
		return true;
	}

	const AActor* HitResultActor = HitResult->HitObjectHandle.FetchActor();
	return (HitResultActor ? HitResultActor->IsOwnedBy(TargetActor) : false);
}

void ATDWCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeBaseGameplayEffects();
}

void ATDWCharacter::InitializeBaseGameplayEffects() const
{
	for (TSubclassOf<UGameplayEffect> GameplayEffectToApply : InitialGameplayEffects)
	{
		if (IsValid(GameplayEffectToApply))
		{
			ApplyEffectToSelf(GameplayEffectToApply);
		}
	}
}

FActiveGameplayEffectHandle ATDWCharacter::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect> GameplayEffectClass, const float Level) const
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	check(IsValid(ASC));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	return ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), ASC);
}

void ATDWCharacter::SetLookAtLocation(const FVector& NewTargetPosition)
{
	CurrentLookAtLocation = NewTargetPosition;
}

void ATDWCharacter::RotateToTarget(const float& DeltaTime)
{
	const FVector NormalizedTargetDirection = (CurrentLookAtLocation - GetActorLocation()).GetSafeNormal2D();
	const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(FVector::ZeroVector, NormalizedTargetDirection);
	const FRotator CurrentRotation = GetActorRotation();
	const float RotationSpeed = CurrentRotationSpeed;
	const FRotator Result = UKismetMathLibrary::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);

	SetActorRotation(Result);
}

void ATDWCharacter::Die()
{
	if (bIsDead)
	{
		return;
	}
	
	bIsDead = true;
	
	//Implement death logic such as playing animation, calling delegates to disable inputs and AI, etc.
	
	if (DeathGameplayEffectClass && DeathGameplayEffectClass.GetDefaultObject())
	{
		ApplyEffectToSelf(DeathGameplayEffectClass);
	}

	if (USkeletalMeshComponent* MeshComp = GetMesh(); IsValid(MeshComp))
	{
		MeshComp->SetSimulatePhysics(true);
	}

	if (UCapsuleComponent* CapsuleComp = GetCapsuleComponent(); IsValid(CapsuleComp))
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	if (TDWMovementComponent)
	{
		TDWMovementComponent->SetActive(false);
	}
}

void ATDWCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (TDWAbilityWidget)
	{
		const float CurrentHealth = bIsDead ? 0 : Data.NewValue;
		const float MaxHealth = bIsDead ? 0 : BaseAttributeSet->GetMaxHealth();

		TDWAbilityWidget->OnHealthChanged(CurrentHealth, MaxHealth);
	}
}
