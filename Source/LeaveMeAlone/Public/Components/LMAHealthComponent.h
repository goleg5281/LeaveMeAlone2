// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LMAHealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEAVEMEALONE_API ULMAHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULMAHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	float GetHealth() const
	{
		return Health;
	}

private:
	float Health = 0.0f;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHealth = 100.0f;

	private:
	UFUNCTION()
	void OnTakeAnyDamage(
		AActor* DamagedActor, float Damage,
		const class UDamageType* DamageType,
		class AController* InstigatedBy, AActor* DamageCauser
	);
	
};
