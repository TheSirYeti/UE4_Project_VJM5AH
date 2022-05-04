// Fill out your copyright notice in the Description page of Project Settings.


#include "MyLevelTrigger.h"
#include <UE4_Project_VJM5AH/UE4_Project_VJM5AHCharacter.h>

// Sets default values
AMyLevelTrigger::AMyLevelTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTriggerLevel"));
	BoxTrigger->GetScaledBoxExtent();
	BoxTrigger->SetupAttachment(RootComponent);
	BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &AMyLevelTrigger::LevelLoadOnOverlap);
}

// Called when the game starts or when spawned
void AMyLevelTrigger::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyLevelTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyLevelTrigger::LevelLoadOnOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
	
	TSubclassOf<AUE4_Project_VJM5AHCharacter> classToFind;
	classToFind = AUE4_Project_VJM5AHCharacter::StaticClass();
	TArray<AActor*> playerArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, playerArray);

	actorReference = playerArray[0]->GetName();

	if (OtherActor->GetName() == actorReference) {
		UGameplayStatics::OpenLevel(OtherActor, levelName, true, "");
	}
}

