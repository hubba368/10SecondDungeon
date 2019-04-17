// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DreamMachineGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DREAMMACHINE_API ADreamMachineGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

		virtual void StartPlay() override;
	
public:

private:



protected:
};
