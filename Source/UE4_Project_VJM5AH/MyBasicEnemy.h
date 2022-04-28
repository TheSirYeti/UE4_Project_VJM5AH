// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <UE4_Project_VJM5AH/UE4_Project_VJM5AHCharacter.h>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyBasicEnemy.generated.h"

UCLASS()
class UE4_PROJECT_VJM5AH_API AMyBasicEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyBasicEnemy();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float hp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float fireRate;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void ChasePlayer(bool isChasing);
};
