// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnviromentQuerys/TDWEnvQueryContext_RandomCharacter.h"

#include "Character/TDWCharacter.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Kismet/GameplayStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TDWEnvQueryContext_RandomCharacter)

void UTDWEnvQueryContext_RandomCharacter::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();
	if (!QueryOwner)
	{
		return;
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATDWCharacter::StaticClass(), FoundActors);
	if (FoundActors.Num() <= 0)
	{
		return;
	}
	
	const int32 RandomIndex = FMath::RandRange(0, FoundActors.Num() - 1);
	AActor* CurrentTarget = FoundActors[RandomIndex];

	UEnvQueryItemType_Actor::SetContextHelper(ContextData, CurrentTarget);
}
