#include "GameTexture.hpp"

GameTexture::GameTexture()
{
	
}

void GameTexture::CreateTexture()
{
	m_texture = Texture::CreateOrGetTexture("SimpleMinerAtlas.png");
	CreateTextureCoordsList();
}

GameTexture::~GameTexture()
{
	delete m_texture;
	m_texture = nullptr;
}

void GameTexture::CreateTextureCoordsList()
{
	float frameWidth = 1.f/32.f;
	int frameRows;
	int frameCols;
	Vector2 topLeft;

	for(frameCols = 0 ; frameCols < 32;frameCols++)
	{
		for(frameRows = 0; frameRows < 32;frameRows++)
		{
			topLeft = Vector2(frameRows * frameWidth, frameCols * frameWidth);
			m_topLeftTextureCoordsList[ frameRows + frameCols * 32] = topLeft;
		}
	}
}

Vector2 GameTexture::GetTopLeftTextureCoordsFromFrameIndex(int index)
{
	if(index > 1024)
		return Vector2(0,0);
	return m_topLeftTextureCoordsList[index];
}