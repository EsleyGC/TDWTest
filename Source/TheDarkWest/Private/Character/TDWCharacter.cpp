// Fill out your copyright notice in the Description page of Project Settings.


#include "TheDarkWest/Public/Character/TDWCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/TDWAttributeSet.h"
#include "Character/Components/TDWMovementComponent.h"
#include "Helpers/TDWMathLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


ATDWCharacter::ATDWCharacter(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UTDWMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	TDWMovementComponent = Cast<UTDWMovementComponent>(GetCharacterMovement());

	BaseAttributeSet = CreateDefaultSubobject<UTDWAttributeSet>("AttributeSet");
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
}

void ATDWCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	RotateToTarget(DeltaTime);
	UpdateLookAtPosition(DeltaTime);
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

void ATDWCharacter::UpdateLookAtPosition(const float& DeltaTime)
{
	const FVector TargetDirection = (CurrentLookAtLocation - GetActorLocation()).GetSafeNormal2D();
	const FVector ForwardDirection = GetMesh()->GetBoneTransform("root", RTS_World).GetUnitAxis(EAxis::X);
	constexpr float RootOffset = -90;

	const float NewAngle = UTDWMathLibrary::WrapToRange(FMath::RadiansToDegrees(FMath::Atan2(TargetDirection.Y, TargetDirection.X) - FMath::Atan2(ForwardDirection.Y, ForwardDirection.X)) + RootOffset, -180, 180);
	//CurrentAnimLookAtYawRotation = FMath::Clamp(NewAngle, -LookAtMaxYawRotation, LookAtMaxYawRotation);
	// TODO: Do we need that? Only use if we are planning to animate the head or upper body to look at target
}
