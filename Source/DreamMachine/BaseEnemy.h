// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	IDLE,
	MOVING,
	ATTACK,
	DEAD,
	ABILITY,
	INACTIVE
};

UCLASS()
class DREAMMACHINE_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// change to protected
	UPROPERTY(BlueprintReadOnly)
	EEnemyState CurrentActiveState;

	virtual float TakeDamage(float DamageAmount, const FDamageEvent &DamageEvent, AController *EventInstigator,
		AActor *DamageCauser);

	UFUNCTION(BlueprintCallable)
	void SetEnemyHealth(float Health);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
		Base: Checks which state needs to be executed every frame.
	*/
	virtual void TickState();

	/*
		Base: Checks if current target is within a certain distance.
	*/
	virtual void StateIdle();

	/*
		Base: Do Nothing (should only be used for initial level loading and level ending).
	*/
	virtual void StateInactive();

	/*
		Base: Move towards the current target along a navmesh path.
	*/
	virtual void StateMoving();

	/*
		Base: Do Nothing (TODO)
	*/
	virtual void StateAttack();

	/*
		Base: Destroy Actor
		TODO - tell ai manager to increment killcount if necessary
		TODO - player particle effect on death
	*/
	virtual void StateDead();

	/*
		Base: Reset the current state to Idle after a set amount of time
	*/
	virtual void StateAbility();

	UFUNCTION(BlueprintCallable)
	void SetNewState(EEnemyState NewState);

	AActor *CurrentTarget;		// Pointer to the current target in scene (usually the player)

private:
	float CurrentEnemyHealth;	
	float Timer = 10.0f;		// Timer that controls how long the enemy will wait until reverting back to an idle state.
								// used witihn BP - need to change
};
