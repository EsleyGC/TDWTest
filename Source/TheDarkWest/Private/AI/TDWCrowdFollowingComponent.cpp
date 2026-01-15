
#include "AI/TDWCrowdFollowingComponent.h"


UTDWCrowdFollowingComponent::UTDWCrowdFollowingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTDWCrowdFollowingComponent::BeginPlay()
{
	Super::BeginPlay();
	SetCrowdAvoidanceRangeMultiplier(AvoidanceRangeMultiplier);
}

void UTDWCrowdFollowingComponent::GetCrowdAgentCollisions(float& CylinderRadius, float& CylinderHalfHeight) const
{
	CylinderRadius = AvoidanceCollisionRadius;
	CylinderHalfHeight = AvoidanceCollisionHalfHeight;
}





