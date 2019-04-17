// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseBullet.h"
#include "DungeonBulletTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ABaseBullet::ABaseBullet()
{
	// initialise movement defs
	BulletMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveComp"));
	BulletMovementComp->InitialSpeed = 3000.0f;
	BulletMovementComp->MaxSpeed = 3000.0f;
	BulletMovementComp->ProjectileGravityScale = 0.0f;
	BulletMovementComp->bRotationFollowsVelocity = true;
	BulletMovementComp->bShouldBounce = false;
}

void ABaseBullet::BeginPlay()
{
	Super::BeginPlay();

	InitialLifeSpan = 3.0f;
	//PrimaryActorTick.bCanEverTick = true;
}



void ABaseBullet::InitialVelocity(const FVector &ShootDirection, float Speed) 
{
	//refactor this
	if (CurrentActiveBulletTypes.BulletBase)
	{
		
	}
	if (CurrentActiveBulletTypes.BulletBounce)
	{ 
		BulletMovementComp->bShouldBounce = true;
		BulletMovementComp->Bounciness = 1.0f;
	}
	if (CurrentActiveBulletTypes.BulletHoming)
	{
		// get closest enemy vector and subtly move towards it
	}
	if (CurrentActiveBulletTypes.BulletSlow)
	{// slow bullet - will probs need to make this a percent increase instead of const val
		Speed = 1000.0f;
		BulletMovementComp->MaxSpeed = 1000.0f;
	}
	if (CurrentActiveBulletTypes.BulletFast)
	{
		Speed = 5000.0f;
		BulletMovementComp->MaxSpeed = 5000.0f;
	}
	if (CurrentActiveBulletTypes.BulletWide)
	{

	}
	if (CurrentActiveBulletTypes.BulletHeavy)
	{
		BulletMovementComp->ProjectileGravityScale = 1.0f;
	}

	BulletMovementComp->Velocity = ShootDirection * Speed;
}

void ABaseBullet::DamageTarget(AActor * OtherActor)
{
	TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
	FDamageEvent DamageEvent(ValidDamageTypeClass);

	float DamageAmount = 50.0f;
	OtherActor->TakeDamage(DamageAmount, DamageEvent, UGameplayStatics::GetPlayerController(GetWorld(), 0), this);
}


void ABaseBullet::SetBulletTypes(DungeonBulletTypes types)
{
	CurrentActiveBulletTypes.BulletBounce = types.BulletBounce;
	CurrentActiveBulletTypes.BulletFast = types.BulletFast;
	CurrentActiveBulletTypes.BulletHeavy = types.BulletHeavy;
	CurrentActiveBulletTypes.BulletHoming = types.BulletHoming;
	CurrentActiveBulletTypes.BulletSlow = types.BulletSlow;
	CurrentActiveBulletTypes.BulletWide = types.BulletWide;
}


float ABaseBullet::GetInitialSpeed() 
{
	return BulletMovementComp->InitialSpeed;
}

void ABaseBullet::Tick(float DeltaTime) 
{

	Super::Tick(DeltaTime);
	//FVector BulletLocation = this->GetActorLocation();
	//UE_LOG(LogTemp, Warning, TEXT("Bullet Location: %f, %f, %f"), BulletLocation.X, BulletLocation.Y, BulletLocation.Z);
}
