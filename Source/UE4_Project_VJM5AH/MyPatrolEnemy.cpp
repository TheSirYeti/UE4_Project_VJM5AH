// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPatrolEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMyPatrolEnemy::AMyPatrolEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxColliderTurret"));
	BoxCollider->GetScaledBoxExtent();
	BoxCollider->SetupAttachment(RootComponent);

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AMyPatrolEnemy::OnTakeHit);

	TurretBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret_Body"));
	TurretBodyMesh->SetupAttachment(BoxCollider);

	TurretHeaderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret_Header"));
	TurretHeaderMesh->SetupAttachment(BoxCollider);

	BulletSpawnPosition = CreateDefaultSubobject<USceneComponent>(TEXT("BULLET_SPAWN_POS"));
	BulletSpawnPosition->SetupAttachment(TurretBodyMesh);

	pawnSensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PAWN_SENSOR"));
}

// Called when the game starts or when spawned
void AMyPatrolEnemy::BeginPlay()
{
	Super::BeginPlay();

	DynMaterial = UMaterialInstanceDynamic::Create(EngageMaterial, this);
	TurretBodyMesh->SetMaterial(5, DynMaterial);

	genericEnemy->OnMaterialReadyToChange(originalColor, DynMaterial);

	pawnSensor->OnSeePawn.AddDynamic(this, &AMyPatrolEnemy::OnSeePawn);
	
}

// Called every frame
void AMyPatrolEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPatrolEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyPatrolEnemy::OnTakeHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this) {
		hp--;

		if (genericEnemy->DoHitRegistry(hp, damageColor, DynMaterial))
		{
			UGameplayStatics::PlaySoundAtLocation(this, DamageSound, GetOwner()->GetActorLocation());
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT(" "));
			UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetOwner()->GetActorLocation());
			Destroy();
		}

		GetWorld()->GetTimerManager().SetTimer(handle, this, &AMyPatrolEnemy::OnTakeHitOver, 0.3f, false);
	}
}

void AMyPatrolEnemy::OnTakeHitOver()
{
	genericEnemy->OnMaterialReadyToChange(originalColor, DynMaterial);
}



void AMyPatrolEnemy::OnSeePawn(APawn* OtherPawn)
{
	FVector PlayerLoc = OtherPawn->GetActorLocation();
	FVector TurretLoc = this->GetActorLocation();
	FRotator FinalRot = UKismetMathLibrary::FindLookAtRotation(TurretLoc, PlayerLoc);
	this->SetActorRotation(FinalRot);

	FRotator Rot = this->GetActorRotation();
	FVector Loc = BulletSpawnPosition->GetComponentLocation();

	FActorSpawnParameters SpawnParams = FActorSpawnParameters();

	TSoftClassPtr<AActor> ActorBpClass = TSoftClassPtr<AActor>(FSoftObjectPath(TEXT("Blueprint'/Game/Blueprints/Player/FirstPersonProjectile.FirstPersonProjectile'")));
	UClass* LoadedBpAsset = ActorBpClass.LoadSynchronous();

	AActor* myBullet = GetWorld()->SpawnActor(bulletPrefab, &Loc, &Rot, SpawnParams);

	//if (!isShooting)
	//{
	//	isShooting = true;

	//	GetWorld()->GetTimerManager().SetTimer(handle, this, &AMyPatrolEnemy::DoBulletSpawning, fireRate, false);
	//}
}

void AMyPatrolEnemy::DoBulletSpawning()
{
	FRotator Rot = this->GetActorRotation();
	FVector Loc = BulletSpawnPosition->GetComponentLocation();

	FActorSpawnParameters SpawnParams = FActorSpawnParameters();

	TSoftClassPtr<AActor> ActorBpClass = TSoftClassPtr<AActor>(FSoftObjectPath(TEXT("Blueprint'/Game/Blueprints/Player/FirstPersonProjectile.FirstPersonProjectile'")));
	UClass* LoadedBpAsset = ActorBpClass.LoadSynchronous();

	AActor* myBullet = GetWorld()->SpawnActor(bulletPrefab, &Loc, &Rot, SpawnParams);

	isShooting = false;
}

