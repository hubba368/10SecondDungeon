// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerBullet.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "PlayerCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

APlayerBullet::APlayerBullet()
{
	// initialise collision defs
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName(TEXT("PlayerBullet"));
	CollisionComp->OnComponentHit.AddDynamic(this, &APlayerBullet::OnHit);
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.0f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	BulletMovementComp->UpdatedComponent = CollisionComp;
	CurrentActiveBulletTypes = { true, false, false, false, false, false, false };
	CurrentBounceCount = 0;
}

void APlayerBullet::InitialVelocity(const FVector &ShootDirection, float Speed)
{
	Super::InitialVelocity(ShootDirection, Speed);
}

void APlayerBullet::SetBulletTypes(DungeonBulletTypes types)
{
	Super::SetBulletTypes(types);
}

void APlayerBullet::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor,
	UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		// do bouncy bullet collision checks, otherwise regular collision.
		if (CurrentActiveBulletTypes.BulletBounce == true)
		{
			
			if (Cast<APlayerCharacter>(OtherActor)== Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
			{
				UE_LOG(LogTemp, Warning, TEXT("bullet hit player actor"));
			}
			if (OtherActor->ActorHasTag(TEXT("Enemy")))
			{
				DamageTarget(OtherActor);
			}
			UE_LOG(LogTemp, Warning, TEXT("bouncy Collision"));
			if (CurrentBounceCount < 2)
			{
				UE_LOG(LogTemp, Warning, TEXT("bouncing"));
				UE_LOG(LogTemp, Warning, TEXT("count: %d"), CurrentBounceCount);
				CurrentBounceCount++;
			}
			else if (CurrentBounceCount >= 2)
			{
				CurrentBounceCount = 0;
				Destroy();
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("non bouncy Bullet Collision"));

			if (OtherActor->ActorHasTag(TEXT("Player")))
			{

			}
			if (OtherActor->ActorHasTag(TEXT("Enemy")))
			{
				DamageTarget(OtherActor);
				Destroy();
			}
			else
			{
				Destroy();
			}
		}
		Destroy();
	}
}