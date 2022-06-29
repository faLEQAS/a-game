#include "asset_manager.h"
#include <stdlib.h>
#include <memory.h>



AssetManager::AssetManager()
{

}


AssetManager::~AssetManager()
{
	if (assets)
	{
		for (int i = 0; i < count; i++)
		{
			if (assets[i])
			{
				delete assets[i];
			}
		}
		free(assets);
	}
}


Asset* AssetManager::GetAsset(int id)
{
	if (assets)
	{
		for (int i = 0; i < count; i++)
		{
			if (assets[i])
			{
				if (assets[i]->id == id)
				{
					return assets[i];
				}
			}
		}
	}
	return nullptr;
}


Sprite* AssetManager::LoadSprite(int id, const char* path)
{
	Sprite* sprite = (Sprite*)malloc(sizeof(Sprite));
	sprite->id = id;
	sprite->type = AssetType::SPRITE;
	sprite->texture = LoadTexture(path);
	count++;

	size_t size = sizeof(Asset*) * count;

	if (assets)
	{
		assets = (Asset**)realloc(assets, size);
		if (assets)
		{
			assets[count - 1] = sprite;
		}
	}
	else
	{
		assets = (Asset**)malloc(size);
		if (assets)
		{
			assets[count - 1] = sprite;
		}
	}

	return sprite;
}


SpriteSheet* AssetManager::LoadSpriteSheet(int id, const char* path, Vector2D size)
{
	SpriteSheet* ss = (SpriteSheet*)malloc(sizeof(SpriteSheet));
	ss->id = id;
	ss->type = AssetType::SPRITESHEET;
	ss->texture = LoadTexture(path);
	ss->size = size;
	count++;

	size_t size2 = sizeof(Asset*) * count;

	if (assets)
	{
		assets = (Asset**)realloc(assets, size2);
		if (assets)
		{
			assets[count - 1] = ss;
		}
	}
	else
	{
		assets = (Asset**)malloc(size2);
		if (assets)
		{
			assets[count - 1] = ss;
		}
	}

	return ss;
}
