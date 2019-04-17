// Fill out your copyright notice in the Description page of Project Settings.

#include "WalkerEnemy.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMesh.h"


AWalkerEnemy::AWalkerEnemy() 
{
	// init enemy specific stuff
	/*ColliderComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SphereComp"));
	ColliderComp->BodyInstance.SetCollisionProfileName(TEXT("PlayerBullet"));
	ColliderComp->OnComponentHit.AddDynamic(this, &AWalkerEnemy::OnHit);
	ColliderComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.0f));
	ColliderComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = ColliderComp;

	UStaticMesh *EnemyMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("StaticMesh'/Game/Meshes/char01_body.char01_body'")));

	EnemyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	EnemyMeshComp->SetStaticMesh(EnemyMesh);
	EnemyMeshComp->SetupAttachment(RootComponent);
	EnemyMeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -20.0f));
	
	ColliderComp->InitBoxExtent(EnemyMeshComp->CalcBounds(EnemyMeshComp->GetComponentToWorld()).BoxExtent);
	*/
}


float AWalkerEnemy::TakeDamage(float DamageAmount, const FDamageEvent & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	//perform enemy specific damage reactions - e.g. an enemy teleporting away for example


	// return base class func call
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

