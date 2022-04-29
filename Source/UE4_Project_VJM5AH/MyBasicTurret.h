// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/BoxComponent.h"
#include "MyBasicTurret.generated.h"

UCLASS()
class UE4_PROJECT_VJM5AH_API AMyBasicTurret : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyBasicTurret();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int hp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float fireRate;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollider;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnTurretHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


};
