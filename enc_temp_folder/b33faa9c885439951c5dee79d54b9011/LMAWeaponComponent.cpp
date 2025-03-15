// LeaveMeAlone Game by Netologiya. All Rights Reserved.


#include "Weapon/LMAWeaponComponent.h"
#include "Weapon/LMABaseWeapon.h"
#include "GameFramework/Character.h"
#include "Player/LMADefaultCharacter.h"
#include "Animations/LMAReloadFinishedAnimNotify.h"

// Sets default values for this component's properties
ULMAWeaponComponent::ULMAWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULMAWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
	InitAnimNotify();
}


// Called every frame
void ULMAWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULMAWeaponComponent::SpawnWeapon()
{
	Weapon = GetWorld()->SpawnActor<ALMABaseWeapon>(WeaponClass);
	if (Weapon)
	{
		const auto Character = Cast<ACharacter>(GetOwner());
		if (Character)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
			Weapon->AttachToComponent(Character->GetMesh(), AttachmentRules, "r_Weapon_Socket");

		}

	}
}

void ULMAWeaponComponent::Fire()
{
	UE_LOG(LogWeapon, Display, TEXT("Fire() AnimReloading=%i"), AnimReloading);																	//check
	if (Weapon && !AnimReloading)
	{
		Weapon->Fire();
	}
}

void ULMAWeaponComponent::InitAnimNotify()
{
	UE_LOG(LogWeapon, Display, TEXT("InitAnimNotify"));
	if (!ReloadMontage)
	{
		UE_LOG(LogWeapon, Display, TEXT("InitAnimNotify return"));																//check
		return;
	}


	const auto NotifiesEvents = ReloadMontage->Notifies;
	for (auto NotifyEvent : NotifiesEvents)
	{
		auto ReloadFinish = Cast<ULMAReloadFinishedAnimNotify>(NotifyEvent.Notify);//ULMAReloadFinishedAnimNotify*
		if (ReloadFinish)
		{
			ReloadFinish->OnNotifyReloadFinished.AddUObject(this, &ULMAWeaponComponent::OnNotifyReloadFinished);
			UE_LOG(LogWeapon, Display, TEXT("InitAnimNotify AddUObject() done"));												//check
			break;
		}
	}
}

void ULMAWeaponComponent::OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh)
{
	UE_LOG(LogWeapon, Display, TEXT("OnNotifyReloadFinished"));
	const auto Character = Cast<ACharacter>(GetOwner());
	if (Character->GetMesh() == SkeletalMesh)
	{
		AnimReloading = false;
		UE_LOG(LogWeapon, Display, TEXT("AnimReloading = false"));																//check
	}
}

bool ULMAWeaponComponent::CanReload() const
{
	return !AnimReloading;
}

void ULMAWeaponComponent::Reload()
{
	if (!CanReload())
	{
		UE_LOG(LogWeapon, Display, TEXT("Can't Reload"));
		return;
	}
		

	Weapon->ChangeClip();
	AnimReloading = true;
	UE_LOG(LogWeapon, Display, TEXT("AnimReloading = true"));
	auto Character = Cast<ACharacter>(GetOwner());
	UE_LOG(LogWeapon, Display, TEXT("Cast<ACharacter>"));
	Character->PlayAnimMontage(ReloadMontage);
	UE_LOG(LogWeapon, Display, TEXT("PlayAnimMontage"));																			//check
}




