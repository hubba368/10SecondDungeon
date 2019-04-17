// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseBullet.h"
#include "EnemyBullet.generated.h"

/**
 * 
 */
UCLASS()
class DREAMMACHINE_API AEnemyBullet : public ABaseBullet
{
	GENERATED_BODY()
	
public:
	AEnemyBullet();

	virtual void InitialVelocity(const FVector &ShootDirection, float Speed);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


protected:

	virtual void DamageTarget(AActor * OtherActor);

private:
	
	class USphereComponent *CollisionComp;
};
