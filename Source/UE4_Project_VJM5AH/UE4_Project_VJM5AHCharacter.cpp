// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE4_Project_VJM5AHCharacter.h"
#include "UE4_Project_VJM5AHProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" 

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);


AUE4_Project_VJM5AHCharacter::AUE4_Project_VJM5AHCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	GetCharacterMovement()->AirControl = 1.0f;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxColliderPlayer"));
	BoxCollider->GetScaledBoxExtent();
	BoxCollider->SetupAttachment(RootComponent);


	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	DashForce = 2600.f;


	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); 
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);	
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->SetupAttachment(RootComponent);


	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);


	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(false);		
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));	


	PlayerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LuigiMesh"));
	PlayerMesh->SetupAttachment(FirstPersonCameraComponent);

	myGun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LuigiGun"));
	myGun->SetupAttachment(PlayerMesh, "LeftHandSocket");

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(myGun);
}

void AUE4_Project_VJM5AHCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}

	GetCharacterMovement()->JumpZVelocity *= 2;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUE4_Project_VJM5AHCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	hp = 5;
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	PlayerInputComponent->BindAction("SetRun", IE_Pressed, this, &AUE4_Project_VJM5AHCharacter::SetRunningMotion);
	PlayerInputComponent->BindAction("Boost", IE_Pressed, this, &AUE4_Project_VJM5AHCharacter::DoBoostJump);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AUE4_Project_VJM5AHCharacter::OnFire);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AUE4_Project_VJM5AHCharacter::OnResetVR);
	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AUE4_Project_VJM5AHCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUE4_Project_VJM5AHCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AUE4_Project_VJM5AHCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AUE4_Project_VJM5AHCharacter::LookUpAtRate);
}

void AUE4_Project_VJM5AHCharacter::OnFire()
{
	if (!hasShot) { //&& !isRunning && !isSideMovement
		// try and fire a projectile

		hasShot = true;

		if (ProjectileClass != nullptr)
		{
			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				if (bUsingMotionControllers)
				{
					const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
					const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();

					TSoftClassPtr<AActor> ActorBpClass = TSoftClassPtr<AActor>(FSoftObjectPath(TEXT("Blueprint'/Game/Blueprints/Player/FirstPersonProjectile.FirstPersonProjectile'")));

					UClass* LoadedBpAsset = ActorBpClass.LoadSynchronous();

					FVector Loc = SpawnLocation;
					FRotator Rot = SpawnRotation;
					FActorSpawnParameters SpawnParams = FActorSpawnParameters();
					GetWorld()->SpawnActor(LoadedBpAsset, &Loc, &Rot, SpawnParams);

					//static ConstructorHelpers::FObjectFinder<UBlueprint> BulletBP(TEXT("/Game/Blueprints/Player/FirstPersonProjectile"));
					//
					//MyBullet = GetWorld()->SpawnActor(BulletBP.Object->GetClass(), SpawnLocation, SpawnRotation);
					

					//World->SpawnActor<AUE4_Project_VJM5AHProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
				}
				else
				{
					const FRotator SpawnRotation = GetControlRotation();
					// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
					const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

					// spawn the projectile at the muzzle
					World->SpawnActor<AUE4_Project_VJM5AHProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
				}
			}
		}

		// try and play the sound if specified
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		// try and play a firing animation if specified
		if (FireAnimation != nullptr)
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
			if (AnimInstance != nullptr)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}

		GetWorld()->GetTimerManager().SetTimer(handle, this, &AUE4_Project_VJM5AHCharacter::DoShotDelay, 0.25f, false);

	}
}

void AUE4_Project_VJM5AHCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AUE4_Project_VJM5AHCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AUE4_Project_VJM5AHCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}


void AUE4_Project_VJM5AHCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
		isRunning = true;
	}
	else 
	{
		isRunning = false;
	}
}

void AUE4_Project_VJM5AHCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		isSideMovement = true;
		AddMovementInput(GetActorRightVector(), Value);
	}
	else 
	{
		isSideMovement = false;
	}
}

void AUE4_Project_VJM5AHCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AUE4_Project_VJM5AHCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AUE4_Project_VJM5AHCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AUE4_Project_VJM5AHCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AUE4_Project_VJM5AHCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AUE4_Project_VJM5AHCharacter::TouchUpdate);
		return true;
	}
	
	return false;
}

void AUE4_Project_VJM5AHCharacter::SetRunningMotion() {
	if (GetCharacterMovement()->IsMovingOnGround()) {

		float xValue = InputComponent->GetAxisValue("MoveForward");
		float yValue = InputComponent->GetAxisValue("MoveRight");
		FVector dashDirection = FVector(xValue, yValue, 0);
		
		FVector rotatedDash = GetActorRotation().RotateVector(dashDirection);
		LaunchCharacter(rotatedDash * DashForce, true, true);

		UGameplayStatics::PlaySound2D(this, DashSound);
	}
}

void AUE4_Project_VJM5AHCharacter::GenerateDamage() {

	hp--;
	UGameplayStatics::PlaySoundAtLocation(this, HurtSound, GetActorLocation());

	if (hp <= 0) {
		UGameplayStatics::OpenLevel(this, "LoseLevel", true, "");
	}
}

void AUE4_Project_VJM5AHCharacter::DoBoostJump() {
	if (GetCharacterMovement()->IsMovingOnGround()) {

		float xValue = InputComponent->GetAxisValue("MoveForward");
		float yValue = InputComponent->GetAxisValue("MoveRight");
		FVector dashDirection = FVector(xValue, yValue, 0);

		LaunchCharacter(GetViewRotation().Vector() * DashForce, true, true);

		UGameplayStatics::PlaySound2D(this, JumpSound);
	}
}

int AUE4_Project_VJM5AHCharacter::UpdateHPText() {
	return hp;
}

void AUE4_Project_VJM5AHCharacter::DoShotDelay() {
	hasShot = false;
}
