// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseBullet.h"
#include "PlayerBullet.generated.h"

/**
 * 
 */
UCLASS()
class DREAMMACHINE_API APlayerBullet : public ABaseBullet
{
	GENERATED_BODY()
	
private:
	class USphereComponent *CollisionComp;

protected:
	UFUNCTION()
	void OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor,
		UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit);



public:
	APlayerBullet();
	
	virtual void InitialVelocity(const FVector &ShootDirection, float Speed);

	void SetBulletTypes(DungeonBulletTypes types);
};
