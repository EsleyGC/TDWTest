#pragma once

#include "StateTreeTaskBase.h"
#include "StateTreeLookAroundTask.generated.h"

class ATDWAIController;
enum class EStateTreeRunStatus : uint8;
struct FStateTreeTransitionResult;

USTRUCT()
struct THEDARKWEST_API FStateTreeLookAroundTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<ATDWAIController> AIController {nullptr};

	/** Whether to use the character starting direction as the base direction for the look around behavior.
	 * This will make the character look around relative to its initial facing direction when the task starts.
	 * If false, the character will look around without a specific base direction, resulting in a more random look around pattern. */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bUseStartingDirectionAsBase {true};

	/** Whether to look back to the starting direction when the task finishes.
	 * This is only relevant if bUseStartingDirectionAsBase is true.
	 * If true, the character will return to its initial facing direction when the task completes.
	 * If false, the character will remain looking in the last direction of the look around behavior. */
	UPROPERTY(EditAnywhere, Category = "Parameter", meta = (EditCondition = "bUseStartingDirectionAsBase", EditConditionHides))
	bool bLookBackToStartingDirectionOnFinish {false};

	/** Duration of the look back to starting direction behavior when the task finishes.
	 * This defines how long it takes for the character to smoothly rotate back to its starting direction.
	 * Only relevant if bLookBackToStartingDirectionOnFinish is true. */
	UPROPERTY(EditAnywhere, Category = "Parameter", meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition = "bLookBackToStartingDirectionOnFinish && bUseStartingDirectionAsBase", EditConditionHides))
	float LookBackToStartingDirectionDuration {1.0f};

	/** Smoothing time for the initial look at the target direction when the task starts.
	 * This defines how quickly the character will rotate to face the target direction at the beginning of the task.
	 * A lower value results in a faster rotation, while a higher value produces a smoother, slower turn. */
	UPROPERTY(EditAnywhere, Category = "Parameter", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float FirstLookAtSmoothingTime {0.5f};

	/** Duration of the look around behavior, the task will succeed after this time has elapsed.
	 * If set to 0, the task will run indefinitely until interrupted. */
	UPROPERTY(EditAnywhere, Category = "Parameter", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float LookAroundDuration {3.0f};

	/** Deviation time added/subtracted to the LookAroundDuration to introduce variability.
	 * The actual duration will be randomly chosen within the range [LookAroundDuration - LookAroundDurationDeviation, LookAroundDuration + LookAroundDurationDeviation]. */
	UPROPERTY(EditAnywhere, Category = "Parameter", meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition = "LookAroundDuration > 0.0"))
	float LookAroundDurationDeviation {2.0f};

	/** Optional angle offset override in degrees to adjust the base look around direction.
	 * This allows for fine-tuning of the look around behavior by shifting the entire oscillation pattern by a specified angle.
	 * The value is clamped between -180 and 180 degrees to ensure valid rotation adjustments. */
	UPROPERTY(EditAnywhere, Category = "Parameter", meta = (ClampMin = "-180.0", UIMin = "-180.0", ClampMax = "810.0", UIMax = "180.0"))
	float AngleOffSet {0.0f};

	/** Amount of randomness to apply to the look around angle oscillation.
	 * A value of 0 results in a smooth, predictable oscillation, while a value of 1 produces highly erratic movements.
	 * 0 = Full Sine use, 1 = Full Perlin Noise use.
	 * Values in between blend between these extremes, allowing for controlled variability in the look around behavior. */
	UPROPERTY(EditAnywhere, Category = "Parameter", meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "1.0", UIMax = "1.0"))
	float RandomnessAmount {0.5f};

	/** Frequency of the look around angle oscillation.
	 * Higher values result in faster oscillations, while lower values produce slower movements. */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float LookAroundAngleFrequency {1.0f};

	/** Size of the look around angle in degrees.
	 * This defines the maximum angle from the base direction that the character will look towards during the oscillation. */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float LookAroundAngleSize {180.0f};

	/** Deviation in degrees added/subtracted to the LookAroundAngleSize to introduce variability.
	 * The actual angle size will be randomly chosen within the range [LookAroundAngleSize - LookAroundAngleSizeDeviation, LookAroundAngleSize + LookAroundAngleSizeDeviation]. */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float LookAroundAngleSizeDeviation {60.0f};
	
	FVector StartingDirection {FVector::ForwardVector};
	float LastSetOffSet {0.0f};
	float TaskDuration {0.0f};
	float AngleSize {0.0f};
	
	float StartingTime {0.0f};
	float ElapsedTime {0.0f};
};

/**
 * Look Around Task for AI characters. Makes the character look around in a smooth oscillating pattern.
 * The look around behavior can be customized with parameters such as duration, angle size, and frequency.
 * The task will succeed after the specified duration has elapsed, or can run indefinitely if duration is set to 0.
 */
USTRUCT(meta = (DisplayName = "Look Around Task", Category = "TDW"))
struct THEDARKWEST_API FStateTreeLookAroundTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeLookAroundTaskInstanceData;

	FStateTreeLookAroundTask() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};
