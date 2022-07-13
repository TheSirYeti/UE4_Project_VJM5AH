// Fill out your copyright notice in the Description page of Project Settings.

#include "MyEnemyBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AMyEnemyBullet::AMyEnemyBullet()
{
    BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxColliderTurret"));
    BoxCollider->GetScaledBoxExtent();
    BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AMyEnemyBullet::OnBulletCollided);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	myMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet_Body"));
	myMesh->SetupAttachment(BoxCollider);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile_Movement"));
    ProjectileMovement->InitialSpeed = 2000.f;
    ProjectileMovement->MaxSpeed = 2000.f;
    ProjectileMovement->ProjectileGravityScale = 0.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;

    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMyEnemyBullet::BeginPlay()
{
	Super::BeginPlay();
	
    lerpMultiplier = 5;
    DynMaterial = UMaterialInstanceDynamic::Create(AltMaterial, this);
    myMesh->SetMaterial(0, DynMaterial);
}

// Called every frame
void AMyEnemyBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    //GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT(" a "));

    myColor.R = lerpingValue;
    myColor.G = lerpingValue;
    myColor.B = lerpingValue;

    DynMaterial->SetVectorParameterValue("LerpValue", myColor);

    if (lerpingValue >= 1 || lerpingValue <= 0) {
        lerpMultiplier *= -1;
    }

    lerpingValue += DeltaTime * lerpMultiplier;
}

void AMyEnemyBullet::OnBulletCollided(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
    AActor* FindClass = UGameplayStatics::GetActorOfClass(GetWorld(), characterClass);
    myCharacter = Cast<AUE4_Project_VJM5AHCharacter>(FindClass);

    if (myCharacter == nullptr)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("ENEMY BULLET COLLISION IS NULL"));
    }
    else
    {
        if (myCharacter == OtherActor) 
        {
            myCharacter->GenerateDamage();
            Destroy();
        }
    }
}


