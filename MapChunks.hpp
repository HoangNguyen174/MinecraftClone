#ifndef MAPCHUNK_H
#define MAPCHUNK_H
#include "ChunkBlocks.hpp"
#include "Vector2.hpp"
#include "Vector3.h"
#include "Vector2i.hpp"
#include "GameCommon.hpp"
#include <map>
#include <set>
#include <fstream>

const int MAP_WIDTH = 0;
const int MAP_HEIGHT = 0;
const int CHUNK_WIDTH_SIZE = 16;
const int CHUNK_DEPTH_SIZE = 16;
const int INNER_RADIUS_IN_CHUNK = 10;
const int OUTTER_RADIUS_IN_CHUNK = 13;
const int INNER_RADIUS_IN_PIXEL = INNER_RADIUS_IN_CHUNK * CHUNK_WIDTH_X;
const int OUTTER_RADIUS_IN_PIXEL = OUTTER_RADIUS_IN_CHUNK * CHUNK_WIDTH_X;
const float VERY_FAR = 99999999999.f;
const float VERY_CLOSE = -VERY_FAR;

class MapChunks
{
	private:
		Vector3 m_cameraPosition;
		Vector3 m_cameraForwardVector;
		Vector2i m_cameraWorldCoord2D;
		bool m_isFinishDrawing;
		
	public :
		set< Vector2i > m_setOfChunksToGenerateOrLoadFromDisc;
		set< Vector2i > m_setOfChunksToWriteToDisc;
		set< Vector2i > m_setOfChunksToSaveAndDelete;
	
	private:
		void CalcCameraWorldCoord2DFromWorldCoord(Vector3 position);
		Vector3 CalcChunkCenterWorldCoord(Vector2i chunkCood2D);
		float CalcDistanceSquareFromCameraToChunk(Vector2i chunkCoord2D);
		unsigned char* CreateRLECompressionForChunk(Vector2i chunkCoord2D,int& sizeOfBuffer);
		void DecompressRLEAndGenerateChunk(FILE* fp,ChunkBlocks* chunkBlocks);
		bool IsChunkInFrontOfCamera(ChunkBlocks* chunk);
		void InitializeLookUpBlockTypeList();

	public:
		MapChunks();
		~MapChunks();
		void GenerateToDoList();
		void GenerateOrLoadChunkFromToDo();
		void DeleteAndSaveChunksFromToDo();
		void SetCameraPosition( Vector3 camera );
		void SetCameraForwardVector( Vector3 cameraForward);
		void WriteChunksFromToDoToDisc( Vector2 chunkCoord2D );
		void WriteChunksToDisc( Vector2i chunkCoord2D );
		bool LoadChunksFromDisc( Vector2i chukCoord2D );
		void Render();
		void Update();
};

extern map< Vector2i, ChunkBlocks* > g_mapChunk;
extern GameTexture *g_gameTexture;
#endif