#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include "Random.h"
#include "Tile.h"
using namespace std;
class Board {

    vector<vector<Tile>> tiles; // Game tiles
    map<int, int> mineMap; // map of mines

    // board info
    int width;
    int height;
    int initialNumMines;
    int tileCount;
    int numMines;
    int numClicked;
    int numFlagged;

    // booleans
    bool gameLost;
    bool gameWon;
    bool debugMode;

    // sprites
    sf::Sprite resetButton;
    sf::Sprite debugSprite;
    sf::Sprite testSprite1;
    sf::Sprite testSprite2;
    sf::Sprite testSprite3;
    sf::Sprite digitOne;
    sf::Sprite digitTen;
    sf::Sprite digitHun;
    sf::Sprite digitNeg;

public:

    Board(); // CONSTRUCTOR

    // UI SETUP
    void Draw(sf::RenderWindow& window);
    void UpdateDisplay();
    void SetUI();


    // GENERATE RANDOM MINES / LOAD FROM FILES  ---------   THEN GENERATE MINES
    void GenerateRandomBoard(map<int, int>& mines);
    void LoadMinesFromFile(const std::string& filepath, map<int, int>& mines);
    void GenerateMines();

    void CalculateAdjacent();

    // CHECKERS
    void CheckWon();
    void CheckFace();
    void MousePress(int x, int y, bool left);
    void ClickTile(int x, int y);

    // RESET / END IF GAME LOST
    void Reset();
    void End();

    // GETTERS
    int GetWidth() const;
    int GetHeight() const;
};

