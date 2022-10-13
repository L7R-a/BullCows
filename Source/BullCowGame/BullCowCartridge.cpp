// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() 
{
    Super::BeginPlay();

    Isograms = GetValidWords(Words);

    SetupGame();

}

void UBullCowCartridge::OnInput(const FString& PlayerInput) 
{
    if(bGameOver == true)
    {
        ClearScreen();
        SetupGame();
    }
    else //Check PlayerGuess
    {
        ProcessGuess(PlayerInput);
        }        
}

        

void UBullCowCartridge::SetupGame()
{
    HiddenWord = Isograms[FMath::RandRange(0,Isograms.Num() - 1)]; 
    Lives = HiddenWord.Len()*2;
    bGameOver = false;

    PrintLine(TEXT("Hi there! Welcome to Bull Cows.\nBe ready to find the words!"));
    PrintLine(TEXT("Guess the %i letters word! "), HiddenWord.Len()); 
    PrintLine(TEXT("You have %i lives"), Lives);
    // PrintLine(TEXT("The HiddenWord is: %s."), *HiddenWord); Debug Line 
    PrintLine(TEXT("Type your guess and \nPress enter to continue"));
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true; 
    PrintLine(TEXT("\nPress enter to play again"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if(HiddenWord == Guess)
    {
        PrintLine(TEXT("You have won!"));
        EndGame();
        return;
    }

    if(HiddenWord.Len()!=Guess.Len())
    {
        PrintLine(TEXT("The length of your word is not %i."), HiddenWord.Len());
        PrintLine(TEXT("Try guessing again, you still have %i lives left"), Lives);
        return;
    }

         if(!IsIsogram(Guess))
     {
        PrintLine(TEXT("No repeating letters, try again."));   
        return;
     }

        --Lives;
        PrintLine(TEXT("You have lost a live"));
        PrintLine(TEXT("You have %i lives now"),Lives);

    if(Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no more lives"));
        PrintLine(TEXT("The hidden word was %s"), *HiddenWord);
        EndGame();
        return;
    }
    
    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);

    PrintLine(TEXT("Guess again"));
}

bool UBullCowCartridge :: IsIsogram(const FString& Word) const
{
    for(int32 i = 0; i < Word.Len(); i++)
        {
            for(int32 Comparison = i + 1 ; Comparison < Word.Len(); Comparison++)
                {
                    if(Word[i] == Word[Comparison])
                    {
                        return false;
                    }
                }
         }
    return true;
}

TArray<FString> UBullCowCartridge :: GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for(const FString& Word : WordList)
    {
        if(Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for(int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if(Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }
        for(int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if(Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}