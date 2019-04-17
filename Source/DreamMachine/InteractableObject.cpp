// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableObject.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Engine/StaticMesh.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AInteractableObject::AInteractableObject()
{


	// initialise movement defs
	ObjectMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ObjectMoveComp"));
	ObjectMovementComp->InitialSpeed = 2000.0f;
	ObjectMovementComp->MaxSpeed = 3000.0f;
	ObjectMovementComp->ProjectileGravityScale = 0.0f;
	ObjectMovementComp->bRotationFollowsVelocity = true;
	ObjectMovementComp->bShouldBounce = false;

	// have a list of file paths to various, small static meshes
	//MeshFilePaths.Add("StaticMesh'/Game/Meshes/Chalice.Chalice'");
	//MeshFilePaths.Add("StaticMesh'/Game/Meshes/stool.stool'");
	//UStaticMesh *ObjMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, *MeshFilePaths[rand() % MeshFilePaths.Num()]));

	//ObjectMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	//ObjectMeshComp->SetStaticMesh(ObjMesh);
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TestTimer -= DeltaTime;
	if (TestTimer <= 0.0f)
	{
		Cast<UStaticMeshComponent>(this->GetRootComponent()->GetChildComponent(0))->SetRenderCustomDepth(false);
		TestTimer = 1.0f;
	}

	if (PlayerHasSelectedObject == true)
	{
		MoveObjectToPosition();
	}
}

void AInteractableObject::MoveObjectToPosition()
{
	// attach to player whilst keeping its world dimensions
	this->SetActorLocation(PlayerPosition->GetComponentLocation());
	this->AttachToComponent(PlayerPosition, FAttachmentTransformRules::KeepWorldTransform);
	PlayerHasSelectedObject = false;
	Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->PlayerIsHoldingItem = true;

}

void AInteractableObject::InteractWithObject(UStaticMeshComponent* PlayerPos)
{
	PlayerPosition = PlayerPos;
	PlayerHasSelectedObject = true;
}


void AInteractableObject::ThrowItem(const FVector & ShootDirection)
{	
	ObjectMovementComp->InitialSpeed = 2000.0f;
	ObjectMovementComp->ProjectileGravityScale = 1.0f;
	ObjectMovementComp->Velocity = ShootDirection * ObjectMovementComp->InitialSpeed;
}


