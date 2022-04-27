// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBasicEnemy.h"

// Sets default values
AMyBasicEnemy::AMyBasicEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyBasicEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyBasicEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyBasicEnemy::ChasePlayer(bool isChasing) {

}

