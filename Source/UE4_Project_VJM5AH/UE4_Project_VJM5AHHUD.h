// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UE4_Project_VJM5AHHUD.generated.h"

UCLASS()
class AUE4_Project_VJM5AHHUD : public AHUD
{
	GENERATED_BODY()

public:
	AUE4_Project_VJM5AHHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

