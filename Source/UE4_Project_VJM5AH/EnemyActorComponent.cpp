// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyActorComponent.h"

// Sets default values for this component's properties
UEnemyActorComponent::UEnemyActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UEnemyActorComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UEnemyActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UEnemyActorComponent::DoHitRegistry(int hp, FLinearColor damageColor, UMaterialInstanceDynamic* DynMaterial) 
{
	DynMaterial->SetVectorParameterValue("EngageColor", damageColor);

	if (hp <= 0) {
		return false;
	}
	return true;
}

void UEnemyActorComponent::OnMaterialReadyToChange(FLinearColor originalColor, UMaterialInstanceDynamic* DynMaterial) {

	DynMaterial->SetVectorParameterValue("EngageColor", originalColor);
}

