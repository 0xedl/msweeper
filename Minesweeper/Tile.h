#pragma once
#include "TextureManager.h"

class Tile {

    sf::Sprite tileSprite{};
    sf::Sprite mineSprite{};
    sf::Sprite flagSprite{};
    int numAdjacentMines{};

    bool mine = false;
    bool flagged = false;
    bool clicked = false;

public:

    Tile(); // CONSTRUCTOR

    // FUNCTIONS
    void Draw(sf::RenderWindow& window, bool debugMode);
    void Click();
    void Flag();

    // SETTERS
    void SetMine(bool val);
    void SetPosition(float x, float y);
    void SetNumAdjacentMines(int num);

    // GETTERS
    int GetNumAdjacentMines() const;
    bool GetClicked() const;
    bool GetFlagged() const;
    bool GetMine() const;
    bool Contains(float x, float y);
    static sf::Sprite GetSprite(const string& spriteName);
};

