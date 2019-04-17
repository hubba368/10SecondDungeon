// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "WalkerEnemy.generated.h"

/**
 * 
 */
UCLASS()
class DREAMMACHINE_API AWalkerEnemy : public ABaseEnemy
{
	GENERATED_BODY()
	
public:
	AWalkerEnemy();


protected:

	virtual float TakeDamage(float DamageAmount, const FDamageEvent &DamageEvent, AController *EventInstigator,
		AActor *DamageCauser);

private:
	
	class UStaticMeshComponent *EnemyMeshComp;
	class UBoxComponent *ColliderComp;


};
