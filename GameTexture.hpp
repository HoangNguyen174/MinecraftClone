#ifndef GAME_TEXTURE
#define GAME_TEXTURE
#include "Texture.hpp"
#include "Vector2.hpp"
#include "MathUtilities.hpp"

class GameTexture
{
	public:
		Texture *m_texture;
		Vector2 m_topLeftTextureCoordsList[ 1024 ];

	private:
		void CreateTextureCoordsList();
		
	public:
		GameTexture();
		~GameTexture();	
		void CreateTexture();
		int GetTextureIndexFromCoord2D(Vector2i textCoord2D);
		Vector2 GetTopLeftTextureCoordsFromFrameIndex(int index);
};

#endif
