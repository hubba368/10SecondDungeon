// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseEnemy.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "DungeonGameInstance.h"
#include "Engine/World.h"


// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	CurrentActiveState = EEnemyState::INACTIVE;
	CurrentEnemyHealth = 100.0f;
	CurrentTarget = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	// get player target vector when supposed to (probs instansiation)
}

void ABaseEnemy::TickState()
{
	switch (CurrentActiveState) 
	{
		case EEnemyState::IDLE:
			StateIdle();
			break;
		case EEnemyState::MOVING:
			StateMoving();
			break;
		case EEnemyState::ATTACK:
			StateAttack();
			break;
		case EEnemyState::DEAD:
			StateDead();
			break;
		case EEnemyState::ABILITY:
			StateAbility();
			break;
		case EEnemyState::INACTIVE:
			StateInactive();
			break;
	}
}

void ABaseEnemy::StateIdle()
{
	//if target not in range do nothing
	//else move towards
	if (CurrentTarget) 
	{
		if (FVector::Distance(CurrentTarget->GetActorLocation(), this->GetActorLocation()) <= 1500.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("found target"));
			SetNewState(EEnemyState::MOVING);
		}
	}
}

void ABaseEnemy::StateMoving()
{
	// move towards target at set speed
	if (CurrentTarget) 
	{
		if (FVector::Distance(GetActorLocation(), CurrentTarget->GetActorLocation()) >= 1200.0f) 
		{
			//too far
		}
		else 
		{
			AAIController *Control = Cast<AAIController>(Controller);
			
			if (!Control->IsFollowingAPath())
			{
				Control->MoveToActor(CurrentTarget);
				//Control->MoveToLocation(FVector(0.0f, 0.0f, 0.0f));
			}
		}
	}
}

void ABaseEnemy::StateInactive()
{
	//UE_LOG(LogTemp, Warning, TEXT("enemy inactive"));
}

void ABaseEnemy::StateAttack()
{
	// attack target - shoot/melee

}

void ABaseEnemy::StateDead()
{
	// destroy character - FX etc
	UE_LOG(LogTemp, Warning, TEXT("Enemy Dead"));
	Cast<UDungeonGameInstance>(GetGameInstance())->CurrentEnemyKills++;
	Destroy();
}

void ABaseEnemy::StateAbility()
{
	// do enemy specific ability - TODO
	// Default: Move enemy actor backwards to stop constant calling of collision functions
	Timer -= 0.25f;

	if (Timer <= 0.0f)
	{
		SetNewState(EEnemyState::IDLE);
		Timer = 10.0f;
	}
}

void ABaseEnemy::SetNewState(EEnemyState NewState)
{
	if (CurrentActiveState != EEnemyState::DEAD)
	{
		CurrentActiveState = NewState;
	}
}

void ABaseEnemy::SetEnemyHealth(float Health)
{
	CurrentEnemyHealth = Health;
}

float ABaseEnemy::TakeDamage(float DamageAmount, const FDamageEvent & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("Taking dmg"));
	if (CurrentEnemyHealth <= 0.0f)
	{
		SetNewState(EEnemyState::DEAD);
	}

	if (CurrentEnemyHealth > 0.0f)
	{
		CurrentEnemyHealth -= ActualDamage;
	}

	return ActualDamage;
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickState();
	
}

void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

