#pragma once
#include "asset.h"
#include "sprite.h"


class AssetManager
{
public:
		AssetManager();
		~AssetManager();

		Asset* GetAsset(int id);

		Sprite* LoadSprite(int id, const char* path);
		SpriteSheet* LoadSpriteSheet(int id, const char* path, Vector2D size);
private:
		Asset** assets = nullptr;
		int count = 0;
};


AssetManager* GetAssetManager();