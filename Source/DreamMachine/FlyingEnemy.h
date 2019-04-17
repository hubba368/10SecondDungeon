// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "FlyingEnemy.generated.h"

/**
 * 
 */
UCLASS()
class DREAMMACHINE_API AFlyingEnemy : public ABaseEnemy
{
	GENERATED_BODY()
	

public:
	AFlyingEnemy();

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AEnemyBullet> EnemyBulletClass;

protected:
	virtual float TakeDamage(float DamageAmount, const FDamageEvent &DamageEvent, AController *EventInstigator,
		AActor *DamageCauser);

	void StateMoving();

	void StateAttack();

private:
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent *EnemyWeaponComp;

	float AttackTimer = 200.0f;
	
};
