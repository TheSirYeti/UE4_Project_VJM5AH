// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "EnemyActorComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_PROJECT_VJM5AH_API UEnemyActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	bool DoHitRegistry(int hp, FLinearColor damageColor, UMaterialInstanceDynamic* DynMaterial);

	UFUNCTION()
	void OnMaterialReadyToChange(FLinearColor originalColor, UMaterialInstanceDynamic* DynMaterial);
};
