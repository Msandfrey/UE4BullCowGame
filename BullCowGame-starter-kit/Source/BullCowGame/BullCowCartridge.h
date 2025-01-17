// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

struct FBullCowCount
{
	int32 Bulls = 0;
	int32 Cows = 0;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& Input) override;
	void StartNewGame();
	void SetupGame();
	void PrintWelcomeMessage();
	void EndGame(const bool& IsWinner);
	void HandlePlayerGuess(const FString& Guess);
	bool IsGuessValid(const FString& Guess) const;
	static bool IsIsogram(const FString& Word);
	FBullCowCount GetBullsCows(const FString& Guess) const;
	FString ChooseRandomWord(const TArray<FString>& WordList) const;
	TArray<FString> GetValidWords(const TArray<FString>& SourceWords) const;

	// Your declarations go below!
	private:
	FString HiddenWord;
	int32 Lives;
	bool bIsGameOver;
	TArray<FString> Isograms;
	
};
