// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBasicTurret.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include <UE4_Project_VJM5AH/UE4_Project_VJM5AHProjectile.h>

// Sets default values
AMyBasicTurret::AMyBasicTurret()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxColliderTurret"));
	BoxCollider->GetScaledBoxExtent();
	BoxCollider->SetupAttachment(RootComponent);

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AMyBasicTurret::OnTakeHit);

	TurretBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret_Body"));
	TurretBodyMesh->SetupAttachment(BoxCollider);

	BulletSpawnPosition = CreateDefaultSubobject<USceneComponent>(TEXT("BULLET_SPAWN_POS"));
	BulletSpawnPosition->SetupAttachment(TurretBodyMesh);

	pawnSensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PAWN_SENSOR"));
}

// Called when the game starts or when spawned
void AMyBasicTurret::BeginPlay()
{
	Super::BeginPlay();

	DynMaterial = UMaterialInstanceDynamic::Create(EngageMaterial, this);
	TurretBodyMesh->SetMaterial(5, DynMaterial);

	pawnSensor->OnSeePawn.AddDynamic(this, &AMyBasicTurret::OnSeePawn);
}

// Called every frame
void AMyBasicTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyBasicTurret::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyBasicTurret::OnTakeHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("BASIC TURRET | LLAMO A GENERIC ACTOR FUNC"));

	hp--;

	if (genericEnemy->DoHitRegistry(hp, damageColor, DynMaterial))
	{
		UGameplayStatics::PlaySoundAtLocation(this, DamageSound, GetOwner()->GetActorLocation());
	}
	else 
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("DEAD"));
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetOwner()->GetActorLocation());
		Destroy();
	}

	GetWorld()->GetTimerManager().SetTimer(handle, this, &AMyBasicTurret::OnTakeHitOver, 0.3f, false);
}

void AMyBasicTurret::OnTakeHitOver() 
{
	genericEnemy->OnMaterialReadyToChange(originalColor, DynMaterial);
}


void AMyBasicTurret::OnSeePawn(APawn* OtherPawn)
{
	if (!isShooting) 
	{
		isShooting = true;
		FString message = TEXT("Saw Actor ") + OtherPawn->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);

		FVector PlayerLoc = OtherPawn->GetActorLocation();
		FVector TurretLoc = this->GetActorLocation();
		FRotator FinalRot = UKismetMathLibrary::FindLookAtRotation(TurretLoc, PlayerLoc);
		this->SetActorRotation(FinalRot);

		GetWorld()->GetTimerManager().SetTimer(handle, this, &AMyBasicTurret::DoBulletSpawning, fireRate, false);
	}
}

void AMyBasicTurret::DoBulletSpawning() 
{
	FString message = TEXT("BANG!");
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);
	
	FRotator Rot = this->GetActorRotation();
	FVector Loc = BulletSpawnPosition->GetComponentLocation();

	message = TEXT("ACTOR LOCATION: " + this->GetActorLocation().ToString());
	GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, message);

	message = TEXT("DETERMINED COMPONENT LOCATION: " + Loc.ToString());
	GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, message);

	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	
	TSoftClassPtr<AActor> ActorBpClass = TSoftClassPtr<AActor>(FSoftObjectPath(TEXT("Blueprint'/Game/Blueprints/Player/FirstPersonProjectile.FirstPersonProjectile'")));
	UClass* LoadedBpAsset = ActorBpClass.LoadSynchronous();

	AActor* myBullet = GetWorld()->SpawnActor(bulletPrefab, &Loc, &Rot, SpawnParams);

	if (myBullet == nullptr) {
		message = TEXT("ES NULO: ");
		GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, message);
	}
	else {
		message = TEXT("NO ES NULO: ");
		GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, message);
	}

	message = TEXT("FINAL COMPONENT LOCATION: " + myBullet->GetActorLocation().ToString());
	GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, message);

	isShooting = false;
}



