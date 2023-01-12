#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>
#include "Board.h"

Board::Board()
{

    width = 0;
    height = 0;
    initialNumMines = 0;
    numMines = 0;
    numClicked = 0;
    numFlagged = 0;

    ifstream in("boards/config.cfg");
    in >> width >> height >> initialNumMines;

    tileCount = width*height;
    gameLost = false;
    gameWon = false;
    debugMode = false;

    Reset();
    SetUI(); // Sets UI Elements
    GenerateRandomBoard(mineMap);
}

void Board::Draw(sf::RenderWindow& window)
{
    for (const auto& ti : tiles)
        for(auto t : ti)
            t.Draw(window, debugMode);

    CheckFace(); // Set Face - Default, Win, Lose
    UpdateDisplay(); // Scoreboard

    window.draw(resetButton);
    window.draw(debugSprite);
    window.draw(testSprite1);
    window.draw(testSprite2);
    window.draw(testSprite3);
    window.draw(digitOne);
    window.draw(digitTen);
    window.draw(digitHun);
    window.draw(digitNeg);
}

void Board::UpdateDisplay()
{
    int dig = numMines - numFlagged;
    int digOne = dig % 10;
    int digTen = dig / 10 % 10;
    int digHundred = dig / 100;

    digitOne.setPosition(66, (float) height * 32);
    digitTen.setPosition(44, (float) height * 32);
    digitHun.setPosition(22, (float) height * 32);
    digitNeg.setPosition(00, (float) height * 32);
    digitNeg.setColor(sf::Color(255,255,255,0));

    if (dig < 0)
    {
        digOne *= -1;
        digTen *= -1;
        digHundred *=-1;
        digitNeg.setColor(sf::Color(255,255,255,255));
    }

    if (gameWon)
    {
        digOne = 0;
        digTen = 0;
        digHundred = 0;
    }

    digitOne.setTextureRect(sf::Rect<int>(digOne * 21, 0, 21, 32));
    digitTen.setTextureRect(sf::Rect<int>(digTen * 21, 0, 21, 32));
    digitHun.setTextureRect(sf::Rect<int>(digHundred * 21, 0, 21, 32));
    digitNeg.setTextureRect(sf::Rect<int>(10 * 21, 0, 21, 32));
}

void Board::SetUI()
{
    debugSprite.setTexture(TextureManager::GetTexture("debug"));
    testSprite1.setTexture(TextureManager::GetTexture("test_1"));
    testSprite2.setTexture(TextureManager::GetTexture("test_2"));
    testSprite3.setTexture(TextureManager::GetTexture("test_3"));
    digitOne.setTexture(TextureManager::GetTexture("digits"));
    digitTen.setTexture(TextureManager::GetTexture("digits"));
    digitHun.setTexture(TextureManager::GetTexture("digits"));
    digitNeg.setTexture(TextureManager::GetTexture("digits"));

    testSprite3.setPosition((float)width*32-(32*2), (float)height*32);
    testSprite2.setPosition(testSprite3.getPosition().x-(32*2), (float)height*32);
    testSprite1.setPosition(testSprite2.getPosition().x-(32*2), (float)height*32);
    debugSprite.setPosition(testSprite1.getPosition().x-(32*2), (float)height*32);
    resetButton.setPosition((float)width*32/2, (float)height*32);
}

void Board::MousePress(int x, int y, bool left)
{
    if (y < height*32) // TILE BOARD CLICKED
    {
        for (int i = 0; i < width; i++)
            for (int j = 0; j < height; j++)

                if (tiles[i][j].Contains((float) x, (float) y))
                {
                    if (left)  // Left-Clicked
                    {
                        ClickTile(x / 32, y / 32);
                        CheckWon();
                    }
                    else // Right-Clicked
                    {
                        if (tiles[i][j].GetClicked() || gameWon || gameLost) return;
                        tiles[i][j].Flag();
                        if (tiles[i][j].GetFlagged()) numFlagged++;
                        else numFlagged--;
                    }
                }
    }

    else if(left) // UI BUTTONS CLICKED
    {
        if(debugSprite.getGlobalBounds().contains((float)x,(float)y) && !gameLost && !gameWon)
            debugMode = !debugMode;

        else
        {
            if(resetButton.getGlobalBounds().contains((float)x,(float)y))
                GenerateRandomBoard(mineMap);

            else if(testSprite1.getGlobalBounds().contains((float)x,(float)y))
             // test 1
                LoadMinesFromFile("testboard1", mineMap);

            else if(testSprite2.getGlobalBounds().contains((float)x,(float)y))
             // test 2
                LoadMinesFromFile("testboard2", mineMap);

            else if(testSprite3.getGlobalBounds().contains((float)x,(float)y))
             // test 3
                LoadMinesFromFile("testboard3", mineMap);

        }
    }
}

void Board::ClickTile(int x, int y)
{
    if(tiles[x][y].GetClicked() || tiles[x][y].GetFlagged() || gameWon || gameLost ) return;
    if(tiles[x][y].GetMine()) End();

    tiles[x][y].Click();
    numClicked++;

    // need to click the surrounding tiles
    int adjacentMines = tiles[x][y].GetNumAdjacentMines();
    if(adjacentMines == 0)
    {
        if (x > 0) {
            ClickTile(x - 1, y);
        }
        if (x < width-1) {
            ClickTile(x + 1, y);
        }
        if (y > 0) {
            ClickTile(x, y - 1);
        }
        if (y < height-1) {
            ClickTile(x, y + 1);
        }
        if (x > 0 && y > 0) {
            ClickTile(x - 1, y - 1);
        }
        if (x > 0 && y < height-1) {
            ClickTile(x - 1, y + 1);
        }
        if (x < width-1 && y > 0) {
            ClickTile(x + 1, y - 1);
        }
        if (x < width-1 && y < height-1) {
            ClickTile(x + 1, y + 1);
        }
    }
}

void Board::CheckWon()
{
    if(numClicked + numMines >= tileCount)
    {
        gameWon = true;
        for(int x = 0; x < width; x++)
        {
            for(int y = 0; y < height; y++)
            {
                int mine = y * width + x;
                if(mineMap[mine] == 1)
                {
                    if (!tiles[x][y].GetFlagged())
                        tiles[x][y].Flag();

                    tiles[x][y].GetSprite("flag");
                }
            }
        }
    }
}

void Board::End()
{
    gameLost = true;
}

void Board::Reset()
{
    numMines = initialNumMines;
    numClicked = 0;
    numFlagged = 0;
    gameLost = false;
    gameWon = false;
    debugMode = false;

    mineMap.clear();
    tiles.clear();
    tiles.resize(width);

    for(unsigned int x = 0; x < width; x++)
    {
        for (unsigned int y = 0; y < height; y++)
        {
            Tile ti;
            ti.SetPosition((float)x,(float)y);
            tiles[x].push_back(ti);
        }
    }
}

void Board::GenerateRandomBoard(map<int, int>& mines)
{
    Reset();

    for(int i = 0; i < tileCount; i++) mines[i] = 0;

    for(int i = 0 ;i < numMines;)
    {
        int random = Random::Int(0, tileCount-1);
        if(mines[random] == 0) //add mine if not present at position random
        {
            mines[random] =  1;
            i++;
        }
    }

    GenerateMines();
    CalculateAdjacent();
}

void Board::LoadMinesFromFile(const std::string& fileName, map<int, int>& mines)
{
    Reset();
    string path;
    path = "boards/" + fileName + ".brd";
    string line;
    ifstream file(path);
    int count = 0;

    for(unsigned int i = 0; i < height; i++)
    {
        getline(file, line);
        for(unsigned int j = 0; j < width; j++)
        {
            int mine = stoi(line.substr(j, 1));
            mines[count] = mine;
            count++;
        }
    }
    GenerateMines();
    CalculateAdjacent();
}

void Board::GenerateMines()
{
    numMines = 0;
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            int mine = width * y + x;
            tiles[x][y].SetMine(mineMap[mine] != 0);
            if(mineMap[mine] == 1) {
                numMines++;
            }
        }
    }
}

void Board::CalculateAdjacent()
{
    for(unsigned int x = 0; x < width; x++)
    {
        for(unsigned int y = 0; y < height; y++)
        {
            // calculate the number of tiles around this one
            int numAdjacent = 0;
            if(x > 0)
                if(tiles[x-1][y].GetMine()) numAdjacent++;

            if(x < width-1)
                if(tiles[x+1][y].GetMine()) numAdjacent++;

            if(y > 0)
                if(tiles[x][y-1].GetMine()) numAdjacent++;

            if(y < height-1)
                if(tiles[x][y+1].GetMine()) numAdjacent++;

            if(x > 0 && y > 0)
                if(tiles[x-1][y-1].GetMine()) numAdjacent++;

            if(x > 0 && y < height-1)
                if(tiles[x-1][y+1].GetMine()) numAdjacent++;

            if(x < width-1 && y > 0)
                if(tiles[x+1][y-1].GetMine()) numAdjacent++;

            if(x < width-1 && y < height-1)
                if(tiles[x+1][y+1].GetMine()) numAdjacent++;

            tiles[x][y].SetNumAdjacentMines(numAdjacent);
        }
    }
}

void Board::CheckFace()
{
    if(gameLost)
        resetButton.setTexture(TextureManager::GetTexture("face_lose"));
    else if (gameWon)
        resetButton.setTexture(TextureManager::GetTexture("face_win"));
    else
        resetButton.setTexture(TextureManager::GetTexture("face_happy"));
}

int Board::GetWidth() const
{
    return width*32;
}

int Board::GetHeight() const
{
    return height*32;
}
