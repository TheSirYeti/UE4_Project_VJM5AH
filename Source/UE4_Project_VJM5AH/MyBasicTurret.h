// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/PawnSensingComponent.h"
#include <UE4_Project_VJM5AH/EnemyActorComponent.h>
#include "MyBasicTurret.generated.h"

UCLASS()
class UE4_PROJECT_VJM5AH_API AMyBasicTurret : public APawn
{
	GENERATED_BODY()

public:
	AMyBasicTurret();

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet_Blueprint")
		TSubclassOf<AActor> bulletPrefab;

	FTimerHandle handle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PawnSensing")
		UPawnSensingComponent* pawnSensor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isShooting;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

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