#pragma once


enum AssetType
{
	ASSET_NONE,
	SPRITE,
	SPRITESHEET
};


struct Asset
{
	int id = 0;
	AssetType type = AssetType::ASSET_NONE;
};