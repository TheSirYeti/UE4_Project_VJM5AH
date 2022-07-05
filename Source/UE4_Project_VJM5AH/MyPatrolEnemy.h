// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include <UE4_Project_VJM5AH/EnemyActorComponent.h>
#include "MyPatrolEnemy.generated.h"

UCLASS()
class UE4_PROJECT_VJM5AH_API AMyPatrolEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyPatrolEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HP")
		int hp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireRate")
		float fireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UEnemyActorComponent* genericEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* TurretBodyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* BulletSpawnPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInterface* EngageMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInstanceDynamic* DynMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OriginalColor")
		FLinearColor originalColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageColor")
		FLinearColor damageColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageSFX")
		USoundCue* DamageSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DeathSFX", meta = (UseComponentPicket, AllowedClasses = "SoundCue"))
		USoundCue* DeathSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PawnSensing")
		UPawnSensingComponent* pawnSensor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet_Blueprint")
		TSubclassOf<AActor> bulletPrefab;

	UPROPERTY()
		bool isShooting;

	FTimerHandle handle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void OnTakeHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnTakeHitOver();

	UFUNCTION()
		void OnSeePawn(APawn* OtherPawn);

	UFUNCTION()
		void DoBulletSpawning();

};
