#include <SFML/Graphics.hpp>
#include <iostream>
#include "Tile.h"
#include "TextureManager.h"

Tile::Tile()
{
    tileSprite.setTexture(TextureManager::GetTexture("tile_hidden"));
    flagSprite.setTexture(TextureManager::GetTexture("flag"));
    mineSprite.setTexture(TextureManager::GetTexture("mine"));
}

void Tile::Draw(sf::RenderWindow& window, bool debugMode)
{
    window.draw(tileSprite);
    if(flagged)
        window.draw(flagSprite);
    if((debugMode && mine) || (clicked && mine) || (clicked && numAdjacentMines > 0))
        window.draw(mineSprite);
}

void Tile::Click()
{
    if(flagged || clicked)
    {
        return;
    }
    clicked = true;
    if(mine)
    {
        return;
    }
    tileSprite.setTexture(TextureManager::GetTexture("tile_revealed"));
}

void Tile::SetNumAdjacentMines(int num)
{
    numAdjacentMines = num;
    if(!mine && numAdjacentMines > 0)
        mineSprite.setTexture(TextureManager::GetTexture("number_" + std::to_string(num)));
}

void Tile::SetMine(bool val) {
    mine = val;
}

void Tile::SetPosition(float x, float y) {
    tileSprite.setPosition(x*32, y*32);
    flagSprite.setPosition(x*32, y*32);
    mineSprite.setPosition(x*32, y*32);
}

void Tile::Flag()
{
    if(!clicked)
        flagged = !flagged;
}

bool Tile::GetClicked() const {
    return clicked;
}

bool Tile::GetFlagged() const {
    return flagged;
}

bool Tile::GetMine() const {
    return mine;
}

int Tile::GetNumAdjacentMines() const {
    return numAdjacentMines;
}

bool Tile::Contains(float x, float y)
{
    return tileSprite.getGlobalBounds().contains(x,y);
}

sf::Sprite Tile::GetSprite(const string& spriteName) {
    sf::Sprite sprite;
    sprite.setTexture(TextureManager::GetTexture(spriteName));
    return sprite;
}
