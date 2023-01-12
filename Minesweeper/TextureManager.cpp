#include "TextureManager.h"

// redeclare variables
unordered_map<string, sf::Texture> TextureManager::textures;

void TextureManager::LoadTexture(const string& fileName)
{
    string path = "images/" + fileName + ".png";
    textures[fileName].loadFromFile(path);
}

sf::Texture& TextureManager::GetTexture(const string& textureName)
{
    // if texture does not exist, load then return.
    if(textures.find(textureName) == textures.end()) // if not found
        LoadTexture(textureName);

    return textures[textureName]; // return texture
}

void TextureManager::Clear()
{
    textures.clear(); // get rid of all stored objects
}
