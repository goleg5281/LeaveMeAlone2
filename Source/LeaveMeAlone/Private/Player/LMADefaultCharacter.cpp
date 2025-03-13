// LeaveMeAlone Game by Netologiya. All Rights Reserved.


#include "Player/LMADefaultCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/InputComponent.h"
#include "Components/DecalComponent.h"
#include "Components/LMAHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ALMADefaultCharacter::ALMADefaultCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	this->SpringArmComponent->SetupAttachment(GetRootComponent());

	this->CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	this->CameraComponent->SetupAttachment(SpringArmComponent);

	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->TargetArmLength = ArmLength;
	SpringArmComponent->SetRelativeRotation(FRotator(YRotation, 0.0f, 0.0f));
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bEnableCameraLag = true;

	CameraComponent->SetFieldOfView(FOV);
	CameraComponent->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	HealthComponent = CreateDefaultSubobject<ULMAHealthComponent>("HealthComponent");
}

// Called when the game starts or when spawned
void ALMADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (CursorMaterial)
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}
	HealthComponent->OnDeath.AddUObject(this, &ALMADefaultCharacter::OnDeath);

}

// Called every frame
void ALMADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		FHitResult ResultHit;
		PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);
		float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
		SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f)));
		if (CurrentCursor)
		{
			CurrentCursor->SetWorldLocation(ResultHit.Location);
		}

	}

	SprintBool ? 
		  Stamina -= 5.0f 
		: Stamina >= 100 ? Stamina += 0.0f : Stamina += 0.5f;
	UE_LOG(LogTemp, Display, TEXT("Stamina: %f"), Stamina);
}

// Called to bind functionality to input
void ALMADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ALMADefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALMADefaultCharacter::MoveRight);

	PlayerInputComponent->BindAction("ZoomIn", EInputEvent::IE_Pressed, this, &ALMADefaultCharacter::ZoomIn);
	PlayerInputComponent->BindAction("ZoomOut", EInputEvent::IE_Pressed, this, &ALMADefaultCharacter::ZoomOut);
	PlayerInputComponent->BindAxis("Zoom", this, &ALMADefaultCharacter::ZoomGamePad);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ALMADefaultCharacter::Sprint);
}

void ALMADefaultCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void ALMADefaultCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void ALMADefaultCharacter::ZoomGamePad(float Value)
{
	ArmLength += Value;
	SpringArmComponent->TargetArmLength = ArmLength;
	//UE_LOG(LogTemp, Display, TEXT("ArmLength: %f"), ArmLength);
}


void ALMADefaultCharacter::ZoomIn()
{
	SpringArmComponent->TargetArmLength = ArmLength -= 10;
}

void ALMADefaultCharacter::ZoomOut()
{
	SpringArmComponent->TargetArmLength = ArmLength += 10;
}

void ALMADefaultCharacter::Sprint()
{
	SprintBool = SprintBool == false ? true : false;
	//TODO//SetDefault();

}

void ALMADefaultCharacter::OnDeath()
{
	CurrentCursor->DestroyRenderState_Concurrent();
	PlayAnimMontage(DeathMontage);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.0f);

	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}
