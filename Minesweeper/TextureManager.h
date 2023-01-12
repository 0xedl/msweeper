#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
using std::unordered_map;
using std::string;

class TextureManager {
    static unordered_map<string, sf::Texture> textures; // textures map
    static void LoadTexture(const string& fileName); // load texture

public:
    static sf::Texture& GetTexture(const string& textureName); // loads texture if not found, then returns
    static void Clear(); // clears textures map
};
