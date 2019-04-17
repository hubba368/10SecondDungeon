// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonGameInstance.h"
#include "PlayerCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


UDungeonGameInstance::UDungeonGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

	// TODO
	// remember to reset both lists when needed to
	// pick a mod at random from active and global lists when needed
	// global list initial list initialisation of all modifiers
void UDungeonGameInstance::InitGameStart(int LevelNum, float LevelTime, int ModCount, int WalkerCount, int TurretCount)
{
	PlayerIsDead = false;
	KillAllEnemyMissionActive = false;
	CurrentLevelNum = LevelNum;
	CurrentLevelTime = LevelTime;
	MaxActiveModifiersCount = ModCount;
	MaxEnemyCount = WalkerCount;
	MaxTurretCount = TurretCount;

	ResetBothModifierLists();
	InitialiseAllModifiers();

	int RandNum = rand() % GlobalModifiers.Num();
	AddNewModifierToActiveList(GlobalModifiers[RandNum]);

	PlayingInitialLevel = false;
}

void UDungeonGameInstance::InitNextLevel(int LevelNum, float LevelTime, int WalkerCount, int TurretCount)
{
	PlayerIsDead = false;
	KillAllEnemyMissionActive = false; 
	CurrentLevelNum = LevelNum;
	CurrentLevelTime = LevelTime;
	MaxEnemyCount = WalkerCount;
	MaxTurretCount = TurretCount;
	ResetBothModifierLists();
	InitialiseAllModifiers();
	if (MaxActiveModifiersCount <= CurrentActiveModifiers.Num())
	{
		//show on UI that you have all modifiers that are available active.
	}
	else
	{
		int RandNum = rand() % GlobalModifiers.Num();
		AddNewModifierToActiveList(GlobalModifiers[RandNum]);
	}


	// choose objective for this level - kill minions for now
}

void UDungeonGameInstance::ChooseNextLevelObjective()
{
	int RandNum = rand() % 100;

	if (RandNum >= 50)
	{
		KillAllEnemyMissionActive = true;
	}
	if (RandNum < 50)
	{
		KillAllEnemyMissionActive = true;
	}
}

void UDungeonGameInstance::ApplyModifier(FModifiers CurrentMod)
{
	// maybe apply only modifiers that arent to do with bullets/enemies etc maybe
	if (CurrentMod.Name == "ExtraHealth") 
	{
		UE_LOG(LogTemp, Warning, TEXT("Extra health modifier"));
		CurrentPlayer->SetCurrentPlayerHealth(6);
	}

	if (CurrentMod.Name == "ExtraJump")
	{
		UE_LOG(LogTemp, Warning, TEXT("Extra Jump modifier"));
		CurrentPlayer->SetDoubleJumpFlag(true);
		CurrentPlayer->SetDoubleJumpCount(1);
	}
	if (CurrentMod.Name == "SlowMovement")
	{
		UE_LOG(LogTemp, Warning, TEXT("slow movement modifier"));
		CurrentPlayer->SetPlayerMovementSpeed(0.50f);
	}
	if (CurrentMod.Name == "FastMovement")
	{
		UE_LOG(LogTemp, Warning, TEXT("fast movement modifier"));
		CurrentPlayer->SetPlayerMovementSpeed(1.25f);
	}
	if (CurrentMod.Name == "DoubleGuns")
	{
		UE_LOG(LogTemp, Warning, TEXT("Dual wield modifier"));
		CurrentPlayer->SetDualWieldFlag(true);
	}
	if (CurrentMod.Name == "LessTime")
	{
		UE_LOG(LogTemp, Warning, TEXT("Less Time modifier"));
		CurrentLevelTime = CurrentLevelTime - 15.0f;
	}
	if (CurrentMod.Name == "StrongEnemies")
	{
		SpawnStrongEnemies = true;
	}

}

void UDungeonGameInstance::AddNewModifierToActiveList(FModifiers NewMod)
{
	CurrentActiveModifiers.Add(NewMod);
	RemoveModifierFromGlobalList(NewMod);
}

void UDungeonGameInstance::RemoveModifierFromGlobalList(FModifiers CurrentMod)
{
	for (int i = 0; i < GlobalModifiers.Num(); i++) 
	{
		if (CurrentMod.Name == GlobalModifiers[i].Name) 
		{
			GlobalModifiers.RemoveAt(i);
			return;
		}
	}

}

void UDungeonGameInstance::ResetBothModifierLists() 
{
	GlobalModifiers.Empty();
	CurrentActiveModifiers.Empty();
}

void UDungeonGameInstance::InitialiseAllModifiers() 
{
	FModifiers ExtraHealth = { "ExtraHealth", EModifierType::PlayerBuff };
	FModifiers LessTime = { "LessTime", EModifierType::PlayerDebuff };
	FModifiers SlowBullet = { "SlowBullet", EModifierType::BulletDebuff };
	FModifiers FastBullet = { "FastBullet", EModifierType::BulletBuff };
	FModifiers BouncingBullet = { "BouncingBullet", EModifierType::BulletBuff };
	FModifiers LessHealth = { "LessHealth", EModifierType::PlayerDebuff };
	FModifiers HeavyBullet = { "HeavyBullet", EModifierType::BulletDebuff };
	FModifiers ExtraJump = { "ExtraJump", EModifierType::PlayerBuff };
	FModifiers FastSpeed = { "FastMovement", EModifierType::PlayerBuff };
	FModifiers SlowSpeed = { "SlowMovement", EModifierType::PlayerDebuff };
	FModifiers StrongEnemies = { "StrongEnemies", EModifierType::PlayerDebuff };
	FModifiers DoubleGun = { "DoubleGuns", EModifierType::PlayerBuff };

	GlobalModifiers.Add(ExtraHealth);
	GlobalModifiers.Add(LessHealth);
	GlobalModifiers.Add(LessTime);
	GlobalModifiers.Add(SlowBullet);
	GlobalModifiers.Add(FastBullet);
	GlobalModifiers.Add(BouncingBullet);
	GlobalModifiers.Add(HeavyBullet);
	GlobalModifiers.Add(ExtraJump);
	GlobalModifiers.Add(FastSpeed);
	GlobalModifiers.Add(SlowSpeed);
	GlobalModifiers.Add(StrongEnemies);
	GlobalModifiers.Add(DoubleGun);
}


TArray<FModifiers> UDungeonGameInstance::GetActiveModifierList()
{
	return CurrentActiveModifiers;
}

void UDungeonGameInstance::SetCurrentPlayer(APlayerCharacter* Player) 
{
	CurrentPlayer = Player;
}

int UDungeonGameInstance::GetCurrentLevelNum()
{
	return CurrentLevelNum;
}

void UDungeonGameInstance::SetCurrentLevelNum(int LevelNum)
{
	CurrentLevelNum = LevelNum;
}

float UDungeonGameInstance::GetCurrentLevelTime()
{
	return CurrentLevelTime;
}

void UDungeonGameInstance::SetCurrentLevelTime(float Time)
{
	CurrentLevelTime = Time;
}

int UDungeonGameInstance::GetMaxEnemyCount()
{
	return MaxEnemyCount;
}

void UDungeonGameInstance::SetMaxEnemyCount(int MaxCount)
{
	MaxEnemyCount = MaxCount;
}

int UDungeonGameInstance::GetMaxTurretCount()
{
	return MaxTurretCount;
}

void UDungeonGameInstance::SetMaxTurretCount(int MaxCount)
{
	MaxTurretCount = MaxCount;
}



