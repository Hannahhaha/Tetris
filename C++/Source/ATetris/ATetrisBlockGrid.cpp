// Copyright Epic Games, Inc. All Rights Reserved.

#include "ATetrisBlockGrid.h"
#include "ATetrisBlock.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"
#include "MyCube.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

AATetrisBlockGrid::AATetrisBlockGrid()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	ScoreText->SetRelativeLocation(FVector(200.f,0.f,0.f));
	ScoreText->SetRelativeRotation(FRotator(90.f,0.f,0.f));
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(0)));
	ScoreText->SetupAttachment(DummyRoot);

	// Set defaults
	Width = 9;
	Height = 12;
	BlockSpacing = 300.f;
}


void AATetrisBlockGrid::BeginPlay()
{
	Super::BeginPlay();

	tGrid = new AATetrisBlock**[Height];
	for (int ver = 0; ver < Height; ++ver)
	{
		tGrid[ver] = new AATetrisBlock *[Width];
		for (int cro = 0; cro < Width; ++cro)
		{
			FVector cubePos = GetCubePosition(cro, ver);
			AATetrisBlock* NewBlock = GetWorld()->SpawnActor<AATetrisBlock>(cubePos, FRotator(0, 0, 0));
			tGrid[ver][cro] = NewBlock;
		}
	}
	tTetrisBoard = new TetrisBoard();
	tTetrisBoard->Init(Width, Height);
	Refresh();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AATetrisBlockGrid::OnTimer, 1, true);
	GetWorld()->GetFirstPlayerController()->InputComponent->BindAction("MoveLeft", EInputEvent::IE_Pressed, this, &AATetrisBlockGrid::MoveLeft);
	GetWorld()->GetFirstPlayerController()->InputComponent->BindAction("MoveRight", EInputEvent::IE_Pressed, this, &AATetrisBlockGrid::MoveRight);
	GetWorld()->GetFirstPlayerController()->InputComponent->BindAction("Rotate", EInputEvent::IE_Pressed, this, &AATetrisBlockGrid::Rotate);
	GetWorld()->GetFirstPlayerController()->InputComponent->BindAction("MoveDown", EInputEvent::IE_Pressed, this, &AATetrisBlockGrid::MoveDown);
}




FVector AATetrisBlockGrid::GetCubePosition(int cro, int ver)
{
	float SumWidth = Width * BlockSpacing;
	float SumHeight = Height * BlockSpacing;
	float croPos = cro * BlockSpacing - SumWidth * 0.5f;
	float verPos = ver * BlockSpacing - SumHeight * 0.5f;
	return FVector(verPos, croPos, 0);

}


void AATetrisBlockGrid::OnTimer()
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("OnTimer"));
	tTetrisBoard->MoveDown();
	Refresh();
}



void AATetrisBlockGrid::MoveLeft()
{
	tTetrisBoard->MoveLeft();
	Refresh();
}


void AATetrisBlockGrid::MoveRight()
{
	tTetrisBoard->MoveRight();
	Refresh();

}


void AATetrisBlockGrid::MoveDown()
{
	tTetrisBoard->MoveDown();
	Refresh();
}


void AATetrisBlockGrid::Rotate()
{
	tTetrisBoard->Rotate();
	Refresh();

}

void AATetrisBlockGrid::Refresh()
{
	if (tTetrisBoard != nullptr)
	{
		for (int ver = 0; ver < Height; ++ver)
		{
			for (int cro = 0; cro > Width; ++cro)
			{
				tGrid[ver][cro]->SetState(tTetrisBoard->GetBlockState(ver, cro));
			}
		}
	}
}

void AATetrisBlockGrid::AddScore()
{
	// Increment score
	Score++;

	// Update text
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
}

#undef LOCTEXT_NAMESPACE
