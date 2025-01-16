// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    StartNewGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    Input.ToLower();

    /*
        HANDLE GAME OVER -----------------------------
     */
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

    /*
        HANDLE PLAYER GUESS
     */
    HandlePlayerGuess(Input);
}

void UBullCowCartridge::StartNewGame()
{
    SetupGame();
    PrintWelcomeMessage();
}

void UBullCowCartridge::SetupGame()
{
    //set difficulty?
    //Set HiddenWord - TODO make it random (create a list of usable words)
    HiddenWord = ChooseHiddenWord();
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

void UBullCowCartridge::EndGame(bool IsWinner)
{
    bIsGameOver = true;
    if(IsWinner)
    {
        PrintLine(TEXT("Good job on getting them bulls rounded up."));
    }
    else
    {
        PrintLine(TEXT("Oof. No recovering from that. \nHave fun in the afterlife."));
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
    }
    PrintLine(TEXT("Would you like try again?(y/n)"));
}

void UBullCowCartridge::HandlePlayerGuess(FString Guess)
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
        PrintLine(TEXT("Try again."));  
        return;
    }

    //TODO - calculate bulls and cows
    //remove a life
    Lives--;
    //check if dead
    if(Lives <= 0)
    {
        EndGame(false);
        return;
    }
    //if not, PrintLine how many lives you have left
    else
    {
        PrintLine(TEXT("Nice try. You got trampled..."));
        PrintLine(TEXT("Let's try that again"));
        PrintLine(TEXT("You have %i lives left."), Lives);
    }
}

const bool UBullCowCartridge::IsGuessValid(FString Guess)
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

const bool UBullCowCartridge::IsIsogram(FString Word)
{
    //keep track of letters to make sure there are no repeats
    Word.ToLower();
    //My Way------------------(TODO - look up which is faster computation wise)
    /* FString Temp = TEXT("");
    for(int32 i = 0; i < Word.Len(); i++)
    {
        FString Letter = FString(1, &Word[i]);
        if(Temp.Contains(Letter))
        {
            return false;
        }
        Temp.Append(Letter);
    } */

    //Udemy Way
    for(int32 Index = 0; Index < Word.Len() - 1; Index++)
    {
        for(int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if(Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }

    return true;
}

FString UBullCowCartridge::ChooseHiddenWord()
{
    //TODO - choose a random word
    return TEXT("guess");
}