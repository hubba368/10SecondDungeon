// Fill out your copyright notice in the Description page of Project Settings.

#include "FlyingEnemy.h"
#include "Engine/StaticMesh.h"
#include "EnemyBullet.h"

AFlyingEnemy::AFlyingEnemy()
{
	UStaticMesh *GunMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("StaticMesh'/Game/testing/Protag_Gun.Protag_Gun'")));

	EnemyWeaponComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
	EnemyWeaponComp->SetStaticMesh(GunMesh);
	EnemyWeaponComp->SetupAttachment(RootComponent);
	EnemyWeaponComp->SetRelativeRotation(FQuat(0.0f, 0.0f, 180.0f, 0.0f));
}

float AFlyingEnemy::TakeDamage(float DamageAmount, const FDamageEvent &DamageEvent, AController *EventInstigator,
	AActor *DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AFlyingEnemy::StateMoving()
{
	AttackTimer -= 2.0f;

	if (AttackTimer <= 0.0f)
	{
		StateAttack();
		AttackTimer = 200.0f;
	}

}

void AFlyingEnemy::StateAttack()
{
	//rotate/just fire at enemy

	if (FVector::Distance(GetActorLocation(), CurrentTarget->GetActorLocation()) <= 2000.0f)
	{
		FVector AttackRot = CurrentTarget->GetActorLocation() - this->GetActorLocation();

		FRotator Rotation = FRotationMatrix::MakeFromX(AttackRot).Rotator();
		SetActorRotation(Rotation);

		if (EnemyBulletClass != NULL)
		{
			UWorld *const World = GetWorld();

			if (World != NULL)
			{
				UE_LOG(LogTemp, Warning, TEXT("shooting boolet"));
				FRotator SpawnRotation = EnemyWeaponComp->GetSocketRotation("MuzzleSocket");
				FVector SpawnLocation = EnemyWeaponComp->GetSocketLocation("MuzzleSocket");

				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
				AEnemyBullet* const CurrentBullet = World->SpawnActor<AEnemyBullet>(EnemyBulletClass, SpawnLocation, SpawnRotation, SpawnParams);

				if (CurrentBullet)
				{
					const FVector ShootDirection = -SpawnRotation.Vector();
					CurrentBullet->InitialVelocity(ShootDirection, 1000.0f);
				}
			}
		}
	}
}
