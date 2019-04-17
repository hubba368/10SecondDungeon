// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonBulletTypes.h"
#include "BaseBullet.generated.h"

UCLASS()
class DREAMMACHINE_API ABaseBullet : public AActor
{
	GENERATED_BODY()
	

public:	
	// Sets default values for this actor's properties
	ABaseBullet();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay();

	/*
		Base: Fires a bullet at a set velocity and direction.

		@param &ShootDirection: the direction of which to move the bullet.
		@param Speed: The new speed of the bullet.
	*/
	virtual void InitialVelocity(const FVector &ShootDirection, float Speed);

	/*
		Sets the bullet types of the current bullet.

		@param types: struct of DungeonBulletTypes to be applied to the current bullet.
	*/
	void SetBulletTypes(DungeonBulletTypes types);

	float GetInitialSpeed();

protected:
	/*
		Base: Damages the collided actor.

		@param *OtherActor: The actor that is to be damaged.
	*/
	virtual void DamageTarget(AActor * OtherActor);

	class UProjectileMovementComponent *BulletMovementComp;		// ProjectileMovementComponent that handles movement of the bullet.

	DungeonBulletTypes CurrentActiveBulletTypes;				// struct of the active bullet modifiers.

	int CurrentBounceCount = 0;

private:

};
