// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LMAPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LEAVEMEALONE_API ALMAPlayerController : public APlayerController
{
	GENERATED_BODY()
	ALMAPlayerController();

public:
	virtual void BeginPlay() override;

protected:
	virtual void BeginSpectatingState() override;
};
