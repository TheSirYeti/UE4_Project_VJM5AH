// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBasicTurret.h"

// Sets default values
AMyBasicTurret::AMyBasicTurret()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxColliderTurret"));
	BoxCollider->GetScaledBoxExtent();
	BoxCollider->SetupAttachment(RootComponent);

	BoxCollider->OnComponentHit.AddDynamic(this, &AMyBasicTurret::OnTurretHit);
}

// Called when the game starts or when spawned
void AMyBasicTurret::BeginPlay()
{
	Super::BeginPlay();
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

void AMyBasicTurret::OnTurretHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	hp--;

	if (hp <= 0) {
		Destroy();
	}

	DynMaterial->SetVectorParameterValue("EngageColor", damageColor);

	GetWorld()->GetTimerManager().SetTimer(handle, this, &AMyBasicTurret::OnMaterialReadyToChange, 0.2f, false);
}

void AMyBasicTurret::OnMaterialReadyToChange() {
	DynMaterial->SetVectorParameterValue("EngageColor", originalColor);
}


