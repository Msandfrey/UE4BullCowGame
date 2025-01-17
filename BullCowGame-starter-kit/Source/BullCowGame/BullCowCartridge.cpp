// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

//for using text file
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    // Words from text file. Not used now
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate(Isograms, *WordListPath, [](const FString& Word)
    {
        return Word.Len() >= 4 && Word.Len() <= 6 && IsIsogram(Word);
    });

    // Setup List of words that can used. Only needs to be ONCE. Uses the header
    //Isograms = GetValidWords(RandomWords);

    StartNewGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    Input.ToLower();
    
    // HANDLE GAME OVER -----------------------------
    if(bIsGameOver)
    {
        if(Input == "y")
        {
            // Restart game
            ClearScreen();
            StartNewGame();
            return;
        }
        else
        {
            //TODO - quit the game
        }
        return;
    }
    //-------------------------------------------------

    // HANDLE PLAYER GUESS-----------------------------
    HandlePlayerGuess(Input);
}

void UBullCowCartridge::StartNewGame()
{
    SetupGame();
    PrintWelcomeMessage();
}

void UBullCowCartridge::SetupGame()
{
    //TODO - set difficulty?
    HiddenWord = ChooseRandomWord(Isograms);
    Lives = HiddenWord.Len();
    bIsGameOver = false;
}

void UBullCowCartridge::PrintWelcomeMessage()
{
    //DEBUG
    PrintLine(TEXT("The Hidden word is: %s"), *HiddenWord);
    
    PrintLine(TEXT("Welcome to the magical world of \nBulls and Cows!"));
    PrintLine(TEXT("Let's get started with the guessing, \nyou've got %i lives."), Lives);
    PrintLine(TEXT("Try any %i letter word and press ENTER..."), HiddenWord.Len());
}

void UBullCowCartridge::EndGame(const bool& IsWinner)
{
    bIsGameOver = true;
    if(IsWinner)
    {
        PrintLine(TEXT("Good job on getting them bulls rounded up."));
    }
    else
    {
        PrintLine(TEXT("Oof. No recovering from that."));
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
    }
    PrintLine(TEXT("Would you like try again?(y/n)"));
}

void UBullCowCartridge::HandlePlayerGuess(const FString& Guess)
{
    //Check player's guess
    if(Guess.Equals(HiddenWord))
    {
        //Call the winning function and stop taking guesses - TODO add more levels/guesses
        EndGame(true);
        return;
    }

    //Check if input is valid
    if(!IsGuessValid(Guess))
    {
        PrintLine(TEXT("Try again.\n"));  
        return;
    }

    //TODO - calculate bulls and cows
    //remove a life
    Lives--;
    if(Lives <= 0)
    {
        EndGame(false);
        return;
    }
    
    FBullCowCount Count;
    Count = GetBullsCows(Guess);

    PrintLine(TEXT("Let's see how many you rounded up."));
    PrintLine(TEXT("Bulls: %i, Cows: %i"), Count.Bulls, Count.Cows);
    PrintLine(TEXT("You have %i lives left.\n"), Lives);
}

FBullCowCount UBullCowCartridge::GetBullsCows(const FString& Guess) const
{
    FBullCowCount BCCount;
    for(int32 I = 0; I < Guess.Len(); I++)
    {
        if(Guess[I] == HiddenWord[I])
        {
            BCCount.Bulls++;
            continue;
        }
        
        if(HiddenWord.Contains(FString(1, &Guess[I])))
        {
            BCCount.Cows++;
            continue;
        }

        //Udemy way
        /* for(int32 J = 0; J < HiddenWord.Len(); J++)
        {
            if(Guess[I] == HiddenWord[J])
            {
                BCCount.Cows++;
                break;
            }
        } */
    }
    return BCCount;
}

bool UBullCowCartridge::IsGuessValid(const FString& Guess) const
{
    int32 HiddenLength = HiddenWord.Len();
    if(Guess.Len() != HiddenLength)
    {
        PrintLine(TEXT("Does that look like %i letters to you?"), HiddenLength);
        return false;
    }
    else if(!IsIsogram(Guess))
    {
        PrintLine(TEXT("An isogram has no repeating letters."));
        return false;
    }
    return true;
}

bool UBullCowCartridge::IsIsogram(const FString& Word) 
{
    //keep track of letters to make sure there are no repeats
    Word.ToLower();
    //My Way------------------(TODO - look up which is faster computation wise)
    FString Temp = TEXT("");
    for(int32 I = 0; I < Word.Len(); I++)
    {
        if(Temp.Contains(FString(1, &Word[I])))
        {
            return false;
        }
        Temp.AppendChar(Word[I]);
    }

    //Udemy Way
    /* for(int32 Index = 0; Index < Word.Len() - 1; Index++)
    {
        for(int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if(Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    } */

    return true;
}

FString UBullCowCartridge::ChooseRandomWord(const TArray<FString>& WordList) const
{
    int32 RandomIndex = FMath::RandRange(0, WordList.Num() - 1);
    return WordList[RandomIndex];
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& SourceWords) const
{
    TArray<FString> Isograms;
    for(FString Word : SourceWords)
    {
        if(Word.Len() >= 4 && Word.Len() <= 8)//TODO - magic numbers MIN_WORD_LEN and MAX_WORD_LEN
        {
            if(IsIsogram(Word))
            {
                Isograms.Emplace(Word);
            }
        }
    }
    return Isograms;
}