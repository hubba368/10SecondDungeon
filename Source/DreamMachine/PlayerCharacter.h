// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DungeonBulletTypes.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class DREAMMACHINE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		class UCapsuleComponent *PlayerCollisionComp;				// the capsule collider and root component of the player.
	UPROPERTY(EditAnywhere)
		class UCharacterMovementComponent *MovementComp;			// movement component that handles all movement related functions
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent *GunMeshComp;					// unused
	UPROPERTY(EditAnywhere)	
		class UStaticMeshComponent *LeftGunMeshComp;				// MeshComponent of the player's 'left hand'
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent *RightGunMeshComp;				// MeshComponent of the player's 'right hand'
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent *InteractablePositionComp;		// MeshComponent of the position where interactable objects are attached to.
	UPROPERTY(EditAnywhere)
		class UCameraComponent *CameraComp;							// CameraComponent of the player.
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent *PlayerMeshComp;

	class UMaterialInstance *PostProcessInstance;					// pointer to post process material that will be blended on the camera component(unused)

	float CameraTiltFactor;											// the amount of tilt applied to the camera when strafing.
	float InvulnTimer;												// player invincibility timer
	int CurrentPlayerHealth;
	bool IsCameraTiltingOnSideMovement;								// bool to check if the camera should be tilted every frame
	bool IsPlayerMovingLeft;										// bool to check whether the player is strafing (for camera rotations)
	bool PlayerDualWieldingGuns = false;	
	bool CanPlayerDoubleJump = false;
	bool PlayerHasBeenDamaged = false;

	DungeonBulletTypes ActiveBulletTypes;							// struct of type DungeonBulletTypes of the currently active bullet types on the player.

	/*
		Traces a line from the view of the camera, and checks if it overlaps an Interactable 
		Object.

		@return A pointer to the object we are hovering the mouse over.
	*/
	class AInteractableObject *CanPlayerPickUpInteractableObj();

protected:
	virtual void BeginPlay() override;

	/*
		Player movement functions.
	*/
	UFUNCTION()
		void PlayerForwardMovement(float Value);
	UFUNCTION()
		void PlayerSideMovement(float Value);
	UFUNCTION()
		void PlayerJump();

	// Collision Overlap Triggers(unusued)
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent *OverlappedComp, class AActor *OtherActor, class UPrimitiveComponent *OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent *OverlappedComp, class AActor *OtherActor, class UPrimitiveComponent *OtherComp,
			int32 OtherBodyIndex);

	/*
		Attempts to pick up an item that player is looking at.
	*/
	void PlayerPickUpItem();

	/*
		Attempts to throw the item that player is currently holding.
	*/
	void PlayerThrowItem();

	/*
		Fires an instantiated copy of the PlayerBullet BP on click.
	*/
	void PlayerFireWeapon();

public:	
	APlayerCharacter();

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// change these 2 to private with accessors where needed
	UPROPERTY(BlueprintReadWrite)
		bool PlayAttackAudio = false;	

	UPROPERTY(BlueprintReadOnly)
		bool PlayerIsHoldingItem = false;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class APlayerBullet> BaseBulletClass;		// PlayerBullet BP reference var.


	UFUNCTION(BlueprintCallable)
	int GetCurrentPlayerHealth();

	void SetCurrentPlayerHealth(int Value);

	void SetPlayerMovementSpeed(float Value);

	UFUNCTION(BlueprintCallable)
	void SetCurrentBulletTypes();

	void SetDualWieldFlag(bool Flag);

	void SetDoubleJumpCount(int MaxCount);

	void SetDoubleJumpFlag(bool Flag);

	/*
		Calls the TakeDamage function on the base Actor class, whilst changing game-specific things within this overloaded function.
	*/
	virtual float TakeDamage(float DamageAmount, const FDamageEvent &DamageEvent, AController *EventInstigator,
		AActor *DamageCauser);

	//debug func
	void TestModifierButton();
};
