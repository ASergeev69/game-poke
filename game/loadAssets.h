#pragma once
#include "raylib.h"
#include "raygui.h"
#include <string>
#include <map>

Texture2D GetTexture(const std::string& key);
void UnloadAllTextures();
void loadStyles();
void LoadTexturesFromDirectory(const std::string& directoryPath);