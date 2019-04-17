// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObject.generated.h"

UCLASS()
class DREAMMACHINE_API AInteractableObject : public AActor
{
	GENERATED_BODY()
	
private:

	UStaticMeshComponent *PlayerPosition;

	float TestTimer = 1.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
		bool PlayerHasSelectedObject = false;

	/*
		Attaches the interactable object to the player when clicked on.
	*/
	UFUNCTION(BlueprintCallable)
		void MoveObjectToPosition();

	// static mesh comp for the object - changed when needed.
	//UPROPERTY(BlueprintReadWrite)
		//class UStaticMeshComponent *ObjectMeshComp;

	//UPROPERTY(BlueprintReadOnly)
	//	TArray<UStaticMesh> ObjectMeshes;

public:	

	AInteractableObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UProjectileMovementComponent *ObjectMovementComp;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InteractWithObject(UStaticMeshComponent *PlayerPos);

	virtual void ThrowItem(const FVector &ShootDirection);

};
