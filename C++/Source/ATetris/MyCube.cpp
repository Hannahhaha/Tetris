// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCube.h"

TetrisPiece* TetrisPieceGenerate::CreateRandomPiece()
{
    TetrisPiece* newPiece = new TetrisPiece();

    newPiece->CubeCount = 4;
    newPiece->tCubePositions[0] = new Cube[4]{ Cube(0,0),Cube(1,0),Cube(0,-1),Cube(0,1) };
    newPiece->tCubePositions[1] = new Cube[4]{ Cube(0,0),Cube(1,0),Cube(-1,0),Cube(0,1) };
    newPiece->tCubePositions[2] = new Cube[4]{ Cube(0,0),Cube(-1,0),Cube(0,-1),Cube(0,1) };
    newPiece->tCubePositions[3] = new Cube[4]{ Cube(0,0),Cube(1,0),Cube(0,-1),Cube(-1,0) };
    return newPiece;
}



Cube TetrisPiece::GetCubeLocalCube(int index)
{
    return tCubePositions[direction][index];
}


Cube TetrisPiece::GetCubeWorldCube(int index)
{
    auto c = GetCubeLocalCube(index);
    return Cube(position.cro + c.cro, position.ver + c.ver);
}

bool TetrisPiece::MoveDown()
{
    position.ver--;
    if (!Isvalid())
    {
        position.ver++;
        return false;
    }
    return true;
}
bool TetrisPiece::Isvalid()
{
    if (tBoard != nullptr)
    {
        for (int i = 0; i < CubeCount; ++i)
        {
            Cube c = GetCubeWorldCube(i);
            if (!tBoard->IsSlotEmpty(c.ver, c.cro))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}


bool TetrisPiece::Contain(Cube& cube)
{
    for (int i = 0; i < CubeCount; ++i)
    {
        Cube cubeCube = GetCubeWorldCube(i);
        if (cubeCube.cro == cube.cro && cubeCube.ver == cube.ver)
        {
            return true;
        }
    }
    return false;
}


void TetrisBoard::Init(int width, int height)
{
    bWidth=width;
    bHeight=height;

    bGrid = new int*[height];
    for (int ver = 0; ver < height; ++ver) 
    {
        bGrid[ver] = new int[width];
        for (int cro = 0; cro < width; ++cro)
        {
            bGrid[ver][cro] = 0;
        }
    }

}
bool TetrisPiece::MoveLeft()
{
    position.cro--;
    if (!Isvalid())
    {
        position.cro++;
        return false;
    }

    return true;
}

bool TetrisPiece::MoveRight()
{
    position.cro++;
    if (!Isvalid())
    {
        position.cro--;
        return false;
    }

    return true;
}

bool TetrisPiece::Rotate()
{
    direction = (direction + 1) % DirectionCount;
    if (!Isvalid())
    {
        direction = (direction - 1) % DirectionCount;
        return false;
    }

    return true;
}
bool TetrisBoard::MoveLeft()
{
    if (bCurPiece != nullptr)
    {
        return bCurPiece->MoveLeft();
    }
    return false;
}

bool TetrisBoard::MoveRight()
{
    if (bCurPiece != nullptr)
    {
        return bCurPiece->MoveRight();
    }
    return false;
}

bool TetrisBoard::Rotate()
{
    if (bCurPiece != nullptr)
    {
        return bCurPiece->Rotate();
    }
    return false;
}

bool TetrisBoard::MoveDown()
{
    bool canMoveDown = (bCurPiece != nullptr && bCurPiece->MoveDown());
    if (!canMoveDown)
    {
        EatCurPiece();
        CreateNewPiece();
    }
    return false;
}

void TetrisBoard::EatCurPiece()
{
    if (bCurPiece != nullptr)
    {
        for (int i = 0; i < bCurPiece->CubeCount; ++i)
        {
            Cube c = bCurPiece->GetCubeWorldCube(i);
            if (IsSlotEmpty(c.ver, c.cro))
            {
                bGrid[c.ver][c.cro] = 1;
            }
        }
        delete bCurPiece;
        bCurPiece = nullptr;
    }
}


bool  TetrisBoard::IsSlotEmpty(int ver, int cro)
{
    return ver >= 0 && ver <= bHeight && cro >= 0 && cro <= bWidth && bGrid[ver][cro] == 0;
}


void TetrisBoard::CreateNewPiece()
{
    bCurPiece = TetrisPieceGenerate::CreateRandomPiece();
    bCurPiece->tBoard=this;
    bCurPiece->position = Cube(4, 10);
}
void TetrisBoard::CheckLine()
{
    for (int ver = 0; ver < bHeight - 1; ++ver)
    {
        bool filled = true;
        for (int cro = 0; cro < bWidth; ++cro)
        {
            if (bGrid[ver][cro] == 0)
            {
                filled = false;
                break;
            }
        }

        if (filled)
        {
            for (int newVer = ver; newVer < bHeight - 1; ++newVer)
            {
                for (int cro = 0; cro < bWidth; ++cro)
                {
                    bGrid[newVer][cro] = bGrid[newVer + 1][cro];
                }
            }

            ver--;
        }
    }
}

bool TetrisBoard::IsGameOver()
{
    return false;
}

BlockState TetrisBoard::GetBlockState(int ver, int cro)
{
    Cube c(cro, ver);
    if (bCurPiece != nullptr && bCurPiece->Contain(c))
    {
        return BlockState::Droping;
    }

    return IsSlotEmpty(ver, cro)?BlockState::Empty : BlockState::Block;
}
