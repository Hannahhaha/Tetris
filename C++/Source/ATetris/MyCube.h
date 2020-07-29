// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define DirectionCount 4

struct Cube
{
    int cro;
    int ver;
    Cube(int aCro = 0 ,int aVer = 0 ):cro(aCro),ver(aVer){}
};

class TetrisPieceGenerate
{
public:
    static class TetrisPiece* CreateRandomPiece();
};

class TetrisPiece
{
public:
    int CubeCount;
    Cube* tCubePositions[DirectionCount];

    int direction;
    Cube position;

    class TetrisBoard* tBoard;

    Cube GetCubeLocalCube(int index);
    Cube GetCubeWorldCube(int index);  
    bool Contain(Cube& cube);
    bool MoveLeft();
    bool MoveRight();
    bool Rotate();
    bool MoveDown();
    bool Isvalid();

};


enum class BlockState
{
    Empty, //空
    Block, //Eat定
    Droping //正在下落
};

class TetrisBoard
{
public:
    int bWidth;
    int bHeight;
    int** bGrid;
    class TetrisPiece* bCurPiece;
    void Init(int width, int height);
    bool IsSlotEmpty(int ver,int cro);
    bool MoveLeft();
    bool MoveRight();
    bool Rotate();

    bool MoveDown();
    void EatCurPiece();
    void CheckLine();

    bool IsGameOver();
    void CreateNewPiece();
    BlockState GetBlockState(int ver, int cro);
};








