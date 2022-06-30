// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyActorComponent.h"

// Sets default values for this component's properties
UEnemyActorComponent::UEnemyActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	hp = 3;
	fireRate = 2.0f;


	//BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxColliderTurret"));
	//BoxCollider->GetScaledBoxExtent();

	//BoxCollider->OnComponentHit.AddDynamic(this, &UEnemyActorComponent::OnTurretHit);
}


// Called when the game starts
void UEnemyActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEnemyActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEnemyActorComponent::OnTurretHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {

	hp--;
	
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("TOME DANIO AUCH"));

	if (hp <= 0) {
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetOwner()->GetActorLocation());
		GetOwner()->Destroy();
	}

	UGameplayStatics::PlaySoundAtLocation(this, DamageSound, GetOwner()->GetActorLocation());
	DynMaterial->SetVectorParameterValue("EngageColor", damageColor);

	GetWorld()->GetTimerManager().SetTimer(handle, this, &UEnemyActorComponent::OnMaterialReadyToChange, 0.35f, false);
}

void UEnemyActorComponent::OnGenericHit() {

	hp--;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("TOME DANIO AUCH"));

	if (hp <= 0) {
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetOwner()->GetActorLocation());
		GetOwner()->Destroy();
	}

	UGameplayStatics::PlaySoundAtLocation(this, DamageSound, GetOwner()->GetActorLocation());
	DynMaterial->SetVectorParameterValue("EngageColor", damageColor);

	GetWorld()->GetTimerManager().SetTimer(handle, this, &UEnemyActorComponent::OnMaterialReadyToChange, 0.35f, false);

}

void UEnemyActorComponent::OnMaterialReadyToChange() {
	DynMaterial->SetVectorParameterValue("EngageColor", originalColor);
}

