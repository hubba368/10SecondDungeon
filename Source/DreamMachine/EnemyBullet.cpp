// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBullet.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "PlayerCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"


AEnemyBullet::AEnemyBullet()
{
	// initialise collision defs
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName(TEXT("PlayerBullet"));
	CollisionComp->OnComponentHit.AddDynamic(this, &AEnemyBullet::OnHit);
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.0f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	BulletMovementComp->UpdatedComponent = CollisionComp;
}

void AEnemyBullet::InitialVelocity(const FVector &ShootDirection, float Speed)
{
	Super::InitialVelocity(ShootDirection, 300.0f);
}

void AEnemyBullet::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		UE_LOG(LogTemp, Warning, TEXT("enemy bulelt hit"));

		if (OtherActor == Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
		{
			UE_LOG(LogTemp, Warning, TEXT("non bouncy enemy Bullet Collision"));
			DamageTarget(OtherActor);
		}
		// apply damage onto enemy if enemy is hit
		if (OtherActor->ActorHasTag(TEXT("Enemy")))
		{		
			UE_LOG(LogTemp, Warning, TEXT("enemy bullet hit enemy"));
			Destroy();
		}
		else
		{
			Destroy();
		}
		
		Destroy();
	}
}

void AEnemyBullet::DamageTarget(AActor * OtherActor)
{
	TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
	FDamageEvent DamageEvent(ValidDamageTypeClass);

	float DamageAmount = 1.0f;
	OtherActor->TakeDamage(DamageAmount, DamageEvent, UGameplayStatics::GetPlayerController(GetWorld(), 0), this);

}
