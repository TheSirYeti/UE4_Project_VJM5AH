// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include <UE4_Project_VJM5AH/UE4_Project_VJM5AHCharacter.h>
#include "GameFramework/Actor.h"
#include "MyEnemyBullet.generated.h"

UCLASS()
class UE4_PROJECT_VJM5AH_API AMyEnemyBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyEnemyBullet();

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AUE4_Project_VJM5AHCharacter* myCharacter;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
