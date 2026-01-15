
#pragma once

#include "CoreMinimal.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "TDWCrowdFollowingComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEDARKWEST_API UTDWCrowdFollowingComponent : public UCrowdFollowingComponent
{
	GENERATED_BODY()

public:
	UTDWCrowdFollowingComponent();

	virtual void BeginPlay() override;
	
	virtual void GetCrowdAgentCollisions(float& CylinderRadius, float& CylinderHalfHeight) const override;

protected:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TDW|Crowd Following")
	float AvoidanceRangeMultiplier = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TDW|Crowd Following")
	float AvoidanceCollisionRadius = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TDW|Crowd Following")
	float AvoidanceCollisionHalfHeight = 90.0f;
};
