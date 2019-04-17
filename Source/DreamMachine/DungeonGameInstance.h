// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DungeonGameInstance.generated.h"

/**
 *  Essentially acts as a Game Manager class
 */

UENUM(BlueprintType)
enum class EModifierType : uint8 { PlayerBuff, PlayerDebuff, BulletBuff, BulletDebuff };

USTRUCT(BlueprintType)
struct FModifiers
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifiers")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifiers")
		EModifierType Type;
};

UCLASS()
class DREAMMACHINE_API UDungeonGameInstance : public UGameInstance
{
	GENERATED_BODY()
	


public:
	UDungeonGameInstance(const FObjectInitializer& ObjectInitializer);

	/*
		Initialises the game when entering a new level.

		@param LevelNum: The current level the player is on.
		@param LevelTime: The current starting time.
		@param ModCount: The number of active modifiers allowed on the current level.
		@param WalkerCount: The number of walking ghost enemies on the level.
		@param TurretCount: The number of skull turret enemies on the level.
	*/
	UFUNCTION(BlueprintCallable)
		void InitGameStart(int LevelNum, float LevelTime, int ModCount, int WalkerCount, int TurretCount);

	/*
		Initialises a new level.
		@param LevelNum: The current level the player is on.
		@param LevelTime: The current level time that is carried on from previous level.
		@param WalkerCount: The number of walking ghost enemies on the level.
		@param TurretCount: The number of skull turret enemies on the level.
	*/
	UFUNCTION(BlueprintCallable)
		void InitNextLevel(int LevelNum, float LevelTime, int WalkerCount, int TurretCount);

	/*
		Actives the inputted modifier onto its intended target.
		@param CurrentMod: The modifier that is currently being applied.
	*/
	UFUNCTION(BlueprintCallable)
	void ApplyModifier(FModifiers CurrentMod);

	/*
		returns the list of active modifiers on the level.
		@return A TArray of type FModifiers (UE4 struct).
	*/
	UFUNCTION(BlueprintCallable)
	TArray<FModifiers> GetActiveModifierList();

	/*
		Chooses a new objective for the current level based on pseudorandom number selection.
	*/
	UFUNCTION(BlueprintCallable)
		void ChooseNextLevelObjective();

	/*
		Sets a pointer to the player class (called at game start).
	*/
	void SetCurrentPlayer(class APlayerCharacter *Player);

	UPROPERTY(BlueprintReadOnly)
		int MaxEnemyCount;										// The current number of walkers to be spawned.
	UPROPERTY(BlueprintReadOnly)
		int MaxTurretCount;										// The current number of turrets to be spawned.
	
	// Vars that are used by Level Instance class BP.
	UPROPERTY(BlueprintReadWrite)
		TArray<AActor*> CurrentEnemyList;						// List of enemies that are currently active in scene.

	UPROPERTY(BlueprintReadWrite)
		bool PlayingInitialLevel = true;						// Flag used by the Level Instance class to generate the 1st level game state.

	UPROPERTY(BlueprintReadWrite)
		bool SpawnStrongEnemies = false;						// Flag to tell level spawns to generate stronger enemies.

	UPROPERTY(BlueprintReadWrite)
		bool HasGameStarted = false;

	UPROPERTY(BlueprintReadWrite)
		bool KillAllEnemyMissionActive;

	UPROPERTY(BlueprintReadWrite)
		int CurrentEnemyKills;

	UPROPERTY(BlueprintReadWrite)
		bool PlayerIsDead = false;

	UPROPERTY(BlueprintReadWrite)
		float LevelMusicPreviousTime;
	//

protected:

	UFUNCTION(BlueprintCallable)
		int GetCurrentLevelNum();

	UFUNCTION(BlueprintCallable)
		void SetCurrentLevelNum(int LevelNum);

	UFUNCTION(BlueprintCallable)
		float GetCurrentLevelTime();

	UFUNCTION(BlueprintCallable)
		void SetCurrentLevelTime(float Time);

	UFUNCTION(BlueprintCallable)
		int GetMaxEnemyCount();

	UFUNCTION(BlueprintCallable)
		void SetMaxEnemyCount(int MaxCount);

	UFUNCTION(BlueprintCallable)
		int GetMaxTurretCount();

	UFUNCTION(BlueprintCallable)
		void SetMaxTurretCount(int MaxCount);


private:
	int CurrentLevelNum;										// The current level the player is on.
	int MaxActiveModifiersCount;								// The max amount of modifiers the player can have across an entire session.

	float CurrentLevelTime;										// The current time the player has left.

	TArray<FModifiers> GlobalModifiers;							// TArray of all modifiers in the game.
	TArray<FModifiers> CurrentActiveModifiers;					// TArray of all active modifiers.
	class APlayerCharacter *CurrentPlayer;

	/*
		Creates all modifier structs and adds them to the GlobalModifiers array.
	*/
	void InitialiseAllModifiers();


	void AddNewModifierToActiveList(FModifiers NewMod);


	void RemoveModifierFromGlobalList(FModifiers CurrentMod);


	void ResetBothModifierLists();




};
