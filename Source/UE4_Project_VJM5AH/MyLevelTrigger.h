// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "MyLevelTrigger.generated.h"

UCLASS()
class UE4_PROJECT_VJM5AH_API AMyLevelTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyLevelTrigger();

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxTrigger;

	UPROPERTY(EditAnywhere)
	FString levelString;

	UPROPERTY(EditAnywhere)
	FName levelName;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void LevelLoadOnOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
