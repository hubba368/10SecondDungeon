// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BaseBullet.h"
#include "PlayerBullet.h"
#include "Engine/StaticMesh.h"
#include "Engine/Scene.h"
#include "DungeonBulletTypes.h"
#include "DrawDebugHelpers.h"
#include "InteractableObject.h"
#include "ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "DungeonGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{

	PrimaryActorTick.bCanEverTick = true;

	// Initialise all player character components
	// camera component
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(RootComponent);
	CameraComp->SetRelativeLocation(FVector(20.0f, 0.0f, 50.0f));

	// possible TODO: Figure out how to set blendables without editor exposing
	//make weighted blendable with post process instance as input
	//make array of weighted blendable (weightedblendables) then add to Post process settings on camera comp
	/*ConstructorHelpers::FObjectFinder<UMaterialInstance> Material(TEXT("MaterialInstanceConstant'/Game/Materials/CameraPostProcess_Inst.CameraPostProcess_Inst'"));
	if (Material.Object != NULL) 
	{
		PostProcessInstance = Material.Object;
		
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("file lookup failed"));
	}
	
	CameraComp->AddOrUpdateBlendable(PostProcessInstance, 1.0f);
	CameraComp->SetPostProcessBlendWeight(1.0f);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *CameraComp->PostProcessSettings.WeightedBlendables.Array[0].Object->GetDetailedInfo());
	*/
	// collision component
	PlayerCollisionComp = this->GetCapsuleComponent();

	PlayerCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);
	PlayerCollisionComp->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEnd);
	PlayerCollisionComp->bGenerateOverlapEvents = true;
	PlayerCollisionComp->bApplyImpulseOnDamage = false;

	//initialise movement values
	MovementComp = this->GetCharacterMovement();
	MovementComp->MaxWalkSpeed = 10.0f;
	MovementComp->AirControl = 1.0f;

	//initialise character mesh
	UStaticMesh *PlayerMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("StaticMesh'/Game/testing/TestPlayerMesh.TestPlayerMesh'")));
	PlayerMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	PlayerMeshComp->SetStaticMesh(PlayerMesh);
	PlayerMeshComp->SetupAttachment(CameraComp);
	PlayerMeshComp->SetRelativeLocationAndRotation(FVector(-20.0f, 0.0f, -50.0f), FQuat(0.0f, 0.0f, 0.0f, 0.0f));

	// Weapon Meshes and placement points
	UStaticMesh *GunMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("StaticMesh'/Game/Meshes/righthand.righthand'")));
	UStaticMesh *LeftGunMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("StaticMesh'/Game/Meshes/lefthand.lefthand'")));
	GunMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
	GunMeshComp->SetStaticMesh(GunMesh);
	GunMeshComp->SetupAttachment(PlayerMeshComp, TEXT("MiddleSocket"));
	GunMeshComp->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));

	LeftGunMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftGun"));
	LeftGunMeshComp->SetStaticMesh(LeftGunMesh);
	LeftGunMeshComp->SetupAttachment(PlayerMeshComp, TEXT("LeftSocket"));
	LeftGunMeshComp->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));

	RightGunMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightGun"));
	RightGunMeshComp->SetStaticMesh(GunMesh);
	RightGunMeshComp->SetupAttachment(PlayerMeshComp, TEXT("RightSocket"));
	RightGunMeshComp->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));

	LeftGunMeshComp->bVisible = false;
	RightGunMeshComp->bVisible = false;

	InteractablePositionComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IntPoint"));
	InteractablePositionComp->SetupAttachment(PlayerMeshComp);
	InteractablePositionComp->SetRelativeLocation(FVector(100.0f, -40.0f, 28.0f));
}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerDualWieldingGuns == true)
	{
		GunMeshComp->bVisible = false;
		GunMeshComp->bHiddenInGame = true;
		LeftGunMeshComp->bVisible = true;
		RightGunMeshComp->bVisible = true;
	}

	PlayAttackAudio = false;
	InvulnTimer = 1.0f;

	IsCameraTiltingOnSideMovement = false;
	CurrentPlayerHealth = 3;
	UDungeonGameInstance *DungTest = Cast<UDungeonGameInstance>(GetGameInstance());

	if (DungTest)
	{
		DungTest->SetCurrentPlayer(this);
	}

	SetCurrentBulletTypes();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//check if guns change 
	if (PlayerDualWieldingGuns == true)
	{
		LeftGunMeshComp->bVisible = true;
		RightGunMeshComp->bVisible = true;
		GunMeshComp->bVisible = false;
	}
	
	// activate invuln time so player cannot get instantly killed by multiple on hit calls
	if (PlayerHasBeenDamaged)
	{
		UE_LOG(LogTemp, Warning, TEXT("invuln active"));
		InvulnTimer -= DeltaTime;
		if (InvulnTimer <= 0.0f)
		{
			InvulnTimer = 1.0f;
			PlayerHasBeenDamaged = false;
			UE_LOG(LogTemp, Warning, TEXT("invuln inactive"));
		}
	}

	if (IsCameraTiltingOnSideMovement) 
	{
		FRotator CurrentRotation = CameraComp->GetComponentRotation();
		FRotator TargetRotation = CurrentRotation;

		// Rotate player capsule collider (and therefore camera component) depending 
		// on strafe direction
		if (IsPlayerMovingLeft)
		{
			PlayerCollisionComp->SetRelativeRotation(FRotator(PlayerCollisionComp->RelativeRotation.Pitch,
				PlayerCollisionComp->RelativeRotation.Yaw, -10.0f));
		}
		else
		{
			PlayerCollisionComp->SetRelativeRotation(FRotator(PlayerCollisionComp->RelativeRotation.Pitch,
				PlayerCollisionComp->RelativeRotation.Yaw, FMath::Lerp(0.0f, 20.0f, 0.5f)));
		}
	}
	else if(CameraComp->GetComponentRotation().Roll != 0.0f || CameraComp->GetComponentRotation().Roll != -0.0f)
	{
		// reset the rotation
		PlayerCollisionComp->SetRelativeRotation(FRotator(PlayerCollisionComp->RelativeRotation.Pitch,
			PlayerCollisionComp->RelativeRotation.Yaw, FMath::Lerp(PlayerCollisionComp->RelativeRotation.Roll, 0.0f, 0.5f)));
	}
	IsCameraTiltingOnSideMovement = false;

	if (PlayerIsHoldingItem == false)
	{
		// check for interactable objects being moused over.
		FHitResult OutHit = FHitResult(ForceInit);
		FVector CameraVec = GunMeshComp->GetComponentLocation();
		FVector StartVec = CameraVec + (CameraComp->GetComponentRotation().Vector() * 40.0f);
		FVector EndVec = CameraVec + (CameraComp->GetComponentRotation().Vector() * 1000.0f);
		FCollisionQueryParams TraceParams(FName(TEXT("InteractTrace")), true, NULL);
		TraceParams.bTraceComplex = true;
		TraceParams.bReturnPhysicalMaterial = true;

		if (GetWorld()->LineTraceSingleByChannel(OutHit, StartVec, EndVec, ECC_GameTraceChannel3, TraceParams))
		{
			
			if (OutHit.Actor->ActorHasTag("Interactable"))
			{
				UE_LOG(LogTemp, Warning, TEXT("HIT INTERACTBLE"));
				AInteractableObject *Obj = Cast<AInteractableObject>(OutHit.Actor.Get());
				Cast<UStaticMeshComponent>(Obj->GetRootComponent()->GetChildComponent(0))->SetRenderCustomDepth(true);

			}
		}
	}

}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up player control bindings.
	PlayerInputComponent->BindAxis("PlayerForwardMovement", this, &APlayerCharacter::PlayerForwardMovement);
	PlayerInputComponent->BindAxis("PlayerSideMovement", this, &APlayerCharacter::PlayerSideMovement);
	PlayerInputComponent->BindAxis("PlayerHorizontalMouseMovement", this, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("PlayerVerticalMouseMovement", this, &APlayerCharacter::AddControllerPitchInput);
	
	PlayerInputComponent->BindAction("PlayerJump", EInputEvent::IE_Pressed, this, &APlayerCharacter::PlayerJump);
	PlayerInputComponent->BindAction("PlayerFireWeapon", EInputEvent::IE_Pressed, this, &APlayerCharacter::PlayerFireWeapon);
	//PlayerInputComponent->BindAction("TestModifierButton", EInputEvent::IE_Pressed, this, &APlayerCharacter::TestModifierButton);
	PlayerInputComponent->BindAction("PlayerPickUpItem", EInputEvent::IE_Pressed, this, &APlayerCharacter::PlayerPickUpItem);
	PlayerInputComponent->BindAction("PlayerThrowItem", EInputEvent::IE_Pressed, this, &APlayerCharacter::PlayerThrowItem);

}

void APlayerCharacter::TestModifierButton() 
{
	UDungeonGameInstance *DungTest = Cast<UDungeonGameInstance>(GetGameInstance());
	
	if (DungTest) 
	{
		UE_LOG(LogTemp, Warning, TEXT("%d"), DungTest->GetActiveModifierList().Num());
		for (int i = 0; i < DungTest->GetActiveModifierList().Num(); i++)
		{
			DungTest->ApplyModifier(DungTest->GetActiveModifierList()[i]);
		}
	}
}

void APlayerCharacter::PlayerForwardMovement(float Value)
{
	// TODO: fix lack of accell when looking downwards
	// probs have to multiply forward vector
	if (Value != 0.0f) 
	{
		FVector MoveDirection = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
		AddMovementInput(MoveDirection, Value);
	}
}

void APlayerCharacter::PlayerSideMovement(float Value)
{
	if (Value != 0.0f)
	{
		FVector MoveDirection = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
		IsCameraTiltingOnSideMovement = true;
		
		// check if moving left or right
		if (Value == -1.0f) 
		{
			IsPlayerMovingLeft = true;
		}
		else 
		{
			IsPlayerMovingLeft = false;
		}

		AddMovementInput(MoveDirection, Value);
	}
}

void APlayerCharacter::PlayerJump()
{
	if (MovementComp && !MovementComp->IsFalling()) 
	{
		MovementComp->DoJump(false);
		UE_LOG(LogTemp, Warning, TEXT("jumping"));
	}
	else if(MovementComp->IsFalling() && CanPlayerDoubleJump == true)
	{
		if (JumpCurrentCount < JumpMaxCount)
		{
			JumpCurrentCount++;
			// taken from here: https://wiki.unrealengine.com/Authoritative_Networked_Character_Movement#Double_Jump
			//Calculate lateral speed to use in adjusting trajectory in midair
			FVector LateralVelocity = MovementComp->Velocity;
			LateralVelocity.Z = 0.0f;//Don't care about vertical velocity
			float LateralSpeed = LateralVelocity.Size();

			//Average the actual velocity with the target velocity
			FVector NewVelocity = GetLastMovementInputVector()*LateralSpeed;
			NewVelocity.Z = 0.0f;
			NewVelocity += LateralVelocity;
			NewVelocity *= 0.5f;

			MovementComp->Velocity = NewVelocity;
			MovementComp->Velocity.Z = 500.0f;
			UE_LOG(LogTemp, Warning, TEXT("double jumping"));			
		}
	}
}

void APlayerCharacter::PlayerPickUpItem()
{
	//check if we are attempting to pickup an interactable object
	if (!PlayerIsHoldingItem)
	{
		AInteractableObject *Obj = CanPlayerPickUpInteractableObj();
		if (Obj != nullptr)
		{
			Obj->InteractWithObject(InteractablePositionComp);
		}
	}
}

void APlayerCharacter::PlayerThrowItem()
{
	if (PlayerIsHoldingItem)
	{
		AActor *ChildObj = InteractablePositionComp->GetChildComponent(0)->GetOwner();
		if (ChildObj)
		{
			AInteractableObject *Objj = Cast<AInteractableObject>(ChildObj);
			if (Objj != NULL && Objj->ActorHasTag("Interactable"))
			{
				//turn collider back on and change velocity to straight ahead
				//depending on players direction
				FDetachmentTransformRules Rules{ EDetachmentRule::KeepWorld,EDetachmentRule::KeepWorld,EDetachmentRule::KeepWorld,false };
				Objj->DetachFromActor(Rules);
				FVector SpawnDirection = LeftGunMeshComp->GetSocketRotation("MuzzleSocket").Vector();
				//Objj->SetActorEnableCollision(true);
				Objj->ThrowItem(SpawnDirection);
				UE_LOG(LogTemp, Warning, TEXT("Throw the Item"));
				PlayerIsHoldingItem = false;
			}
		}
	}
}

void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// POTENTIAL TODO: add item/weapon pickup

	// check tags for item that is going to be picked up
	/*for (int i = 0; i < OtherComp->ComponentTags.Num(); i++) 
	{
		if (OtherComp->ComponentTags[i] == "Weapon") 
		{
			// pickup weapon
			UE_LOG(LogTemp, Warning, TEXT("overlapping weapon"));

			TArray<UStaticMeshComponent*>WeaponPlacementMesh;
			GetComponents<UStaticMeshComponent>(WeaponPlacementMesh);
			//PlayerPickUpItem();
		}
	}*/
}

void APlayerCharacter::OnOverlapEnd(UPrimitiveComponent * OVerlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{

}

void APlayerCharacter::PlayerFireWeapon() 
{
	if (BaseBulletClass != NULL) 
	{
		UWorld *const World = GetWorld();
		
		if (World != NULL) 
		{
			PlayAttackAudio = true;
			// get the location and rotation of the socket on the player's weapon mesh
			if (PlayerDualWieldingGuns == true)
			{
				if (PlayerIsHoldingItem == true)
				{
					// dual wield and holding modifier is active
					// only shoot the right gun
					// pls god refactor this to single function
					FRotator SpawnRotation = LeftGunMeshComp->GetSocketRotation("MuzzleSocket");
					FVector RightSpawnLocation = RightGunMeshComp->GetSocketLocation("MuzzleSocket");

					FActorSpawnParameters SpawnParams;
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
					APlayerBullet* const RightBullet = World->SpawnActor<APlayerBullet>(BaseBulletClass, RightSpawnLocation, SpawnRotation, SpawnParams);

					// set the bullet type depending on player's modifiers 
					RightBullet->SetBulletTypes(ActiveBulletTypes);

					UE_LOG(LogTemp, Warning, TEXT("player health:%d"), CurrentPlayerHealth);
					if (RightBullet)
					{
						const FVector ShootDirection = SpawnRotation.Vector();
						RightBullet->InitialVelocity(ShootDirection, RightBullet->GetInitialSpeed());
					}
				}
				else
				{
					// dual wield modifier is active
					FRotator SpawnRotation = LeftGunMeshComp->GetSocketRotation("MuzzleSocket");
					FVector LeftSpawnLocation = LeftGunMeshComp->GetSocketLocation("MuzzleSocket");
					FVector RightSpawnLocation = RightGunMeshComp->GetSocketLocation("MuzzleSocket");

					FActorSpawnParameters SpawnParams;
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
					APlayerBullet* const LeftBullet = World->SpawnActor<APlayerBullet>(BaseBulletClass, LeftSpawnLocation, SpawnRotation, SpawnParams);
					APlayerBullet* const RightBullet = World->SpawnActor<APlayerBullet>(BaseBulletClass, RightSpawnLocation, SpawnRotation, SpawnParams);

					// set the bullet type depending on player's modifiers 
					LeftBullet->SetBulletTypes(ActiveBulletTypes);
					RightBullet->SetBulletTypes(ActiveBulletTypes);

					UE_LOG(LogTemp, Warning, TEXT("player health:%d"), CurrentPlayerHealth);
					if (LeftBullet && RightBullet)
					{
						const FVector ShootDirection = SpawnRotation.Vector();
						LeftBullet->InitialVelocity(ShootDirection, LeftBullet->GetInitialSpeed());
						RightBullet->InitialVelocity(ShootDirection, RightBullet->GetInitialSpeed());
					}
				}
			}
			else 
			{
				FRotator SpawnRotation = GunMeshComp->GetSocketRotation("MuzzleSocket");
				FVector SpawnLocation = GunMeshComp->GetSocketLocation("MuzzleSocket");

				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
				APlayerBullet* const CurrentBullet = World->SpawnActor<APlayerBullet>(BaseBulletClass, SpawnLocation, SpawnRotation, SpawnParams);

				// set the bullet type depending on player's modifiers 
				CurrentBullet->SetBulletTypes(ActiveBulletTypes);

				UE_LOG(LogTemp, Warning, TEXT("player health:%d"), CurrentPlayerHealth);
				if (CurrentBullet)
				{
					const FVector ShootDirection = SpawnRotation.Vector();
					CurrentBullet->InitialVelocity(ShootDirection, CurrentBullet->GetInitialSpeed());
				}
			}
		}
	}
	else 
	{
		//throw exception
		UE_LOG(LogTemp, Warning, TEXT("somethin broke"));
	}
}

AInteractableObject *APlayerCharacter::CanPlayerPickUpInteractableObj()
{
	//trace a line from the players view to see if they are in range of an interactable object
	// offset the start vector so it doesn't detect the players own character.
	FHitResult OutHit = FHitResult(ForceInit);
	FVector CameraVec = GunMeshComp->GetComponentLocation();
	FVector StartVec = CameraVec + (CameraComp->GetComponentRotation().Vector() * 40.0f);
	FVector EndVec = CameraVec + (CameraComp->GetComponentRotation().Vector() * 1000.0f);
	FCollisionQueryParams TraceParams(FName(TEXT("InteractTrace")), true, NULL);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = true;

	if (GetWorld()->LineTraceSingleByChannel(OutHit, StartVec, EndVec, ECC_GameTraceChannel3, TraceParams))
	{
		//DrawDebugLine(GetWorld(), StartVec, EndVec, FColor::Green, false, 5.f, ECC_WorldStatic, 1.f);

		if (OutHit.Actor->ActorHasTag("Interactable"))
		{
			UE_LOG(LogTemp, Warning, TEXT("HIT INTERACTBLE"));
			AInteractableObject *Obj = Cast<AInteractableObject>(OutHit.Actor.Get());
			Cast<UStaticMeshComponent>(Obj->GetRootComponent()->GetChildComponent(0))->SetRenderCustomDepth(true);
			return Obj;
		}
		else 
		{
			return nullptr;
		}
	}
	else
	{
		//DrawDebugLine(GetWorld(), StartVec, EndVec, FColor::Purple, false, 5.f, ECC_WorldStatic, 1.f);
		return nullptr;
	}
}

float APlayerCharacter::TakeDamage(float DamageAmount, const FDamageEvent &DamageEvent, AController *EventInstigator,
	AActor *DamageCauser)
{
	if (PlayerHasBeenDamaged == false)
	{
		float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
		UE_LOG(LogTemp, Warning, TEXT("player hit"));
		PlayerHasBeenDamaged = true;
		if (CurrentPlayerHealth <= 0.0f)
		{
			Cast<UDungeonGameInstance>(GetGameInstance())->PlayerIsDead = true;
			Destroy();
		}

		if (CurrentPlayerHealth > 0.0f)
		{
			CurrentPlayerHealth -= ActualDamage;
		}

		return ActualDamage;
	}
	else
	{
		return 0.0f;
	}

}


// setters/getters
int APlayerCharacter::GetCurrentPlayerHealth() 
{
	return CurrentPlayerHealth;
}

void APlayerCharacter::SetCurrentPlayerHealth(int Value) 
{
	CurrentPlayerHealth = Value;
}

void APlayerCharacter::SetCurrentBulletTypes() 
{
	UDungeonGameInstance *DungTest = Cast<UDungeonGameInstance>(GetGameInstance());

	if (DungTest)
	{
		for (int i = 0; i < DungTest->GetActiveModifierList().Num(); i++) 
		{
			if (DungTest->GetActiveModifierList()[i].Name == "BouncingBullet") 
			{
				UE_LOG(LogTemp, Warning, TEXT("bouncy"));
				ActiveBulletTypes.BulletBounce = true;
			}
			if (DungTest->GetActiveModifierList()[i].Name == "HomingBullet")
			{
				UE_LOG(LogTemp, Warning, TEXT("homing"));
				ActiveBulletTypes.BulletHoming = true;
			}
			if (DungTest->GetActiveModifierList()[i].Name == "SlowBullet")
			{
				UE_LOG(LogTemp, Warning, TEXT("slow"));
				ActiveBulletTypes.BulletSlow = true;
			}
			if (DungTest->GetActiveModifierList()[i].Name == "FastBullet")
			{
				UE_LOG(LogTemp, Warning, TEXT("fast"));
				ActiveBulletTypes.BulletFast = true;
			}
			if (DungTest->GetActiveModifierList()[i].Name == "WideBullet")
			{
				UE_LOG(LogTemp, Warning, TEXT("wide"));
				ActiveBulletTypes.BulletWide= true;
			}
			if (DungTest->GetActiveModifierList()[i].Name == "HeavyBullet")
			{
				UE_LOG(LogTemp, Warning, TEXT("heavy"));
				ActiveBulletTypes.BulletHeavy = true;
			}
		}

	}
}

void APlayerCharacter::SetPlayerMovementSpeed(float Value)
{
	MovementComp->MaxWalkSpeed = MovementComp->MaxWalkSpeed / Value;
	MovementComp->AirControl = MovementComp->AirControl / Value;
}

void APlayerCharacter::SetDualWieldFlag(bool Flag)
{
	PlayerDualWieldingGuns = Flag;
}

void APlayerCharacter::SetDoubleJumpCount(int MaxCount)
{
	JumpMaxCount = MaxCount;
}

void APlayerCharacter::SetDoubleJumpFlag(bool Flag)
{
	CanPlayerDoubleJump = Flag;
}
