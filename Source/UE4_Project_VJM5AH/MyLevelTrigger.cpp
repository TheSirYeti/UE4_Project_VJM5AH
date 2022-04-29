// Fill out your copyright notice in the Description page of Project Settings.


#include "MyLevelTrigger.h"

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
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, levelString);
	UGameplayStatics::OpenLevel(OtherActor, levelName, true, "");
}

