// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "EnemyActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_PROJECT_VJM5AH_API UEnemyActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyActorComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HP")
	int hp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireRate")
	float fireRate;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* EngageMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstanceDynamic* DynMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OriginalColor")
	FLinearColor originalColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageColor")
	FLinearColor damageColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageSFX")
	USoundCue* DamageSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category =  "DeathSFX", meta = (UseComponentPicket, AllowedClasses = "SoundCue"))
	USoundCue* DeathSound;

	FTimerHandle handle;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void OnTurretHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnMaterialReadyToChange();

	UFUNCTION(BlueprintCallable)
	void OnGenericHit();
};
