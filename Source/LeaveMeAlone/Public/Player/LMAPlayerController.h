// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LMAPlayerController.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LEAVEMEALONE_API ALMAPlayerController : public APlayerController
{
	GENERATED_BODY()
	ALMAPlayerController();

public:
	virtual void BeginPlay() override;

protected:
	virtual void BeginSpectatingState() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	bool Sprint = false;

	float Stamina = 100.0f;

	UFUNCTION(BlueprintCallable)
	bool CanSprint() const
	{ 
		return Sprint;
	}
};
