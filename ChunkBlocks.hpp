#ifndef CHUNKBLOCK_H
#define CHUNKBLOCK_H
#include "Vector3.h"
#include "Vector3i.hpp"
#include "Block.hpp"
#include "GameTexture.hpp"
#include "MathUtilities.hpp"
#include "Noise.hpp"
#include <vector>
#include <queue>
#include "GameCommon.hpp"
#include "GLRender.hpp"

const float minLightValue = 0.1f;
class ChunkBlocks
{
	public:
		Block m_blocksList[CHUNK_SIZE_IN_BLOCK];
		Vector3 m_minWorldCoords;
		int m_currentFrameOfGameTexture;
		float m_frameWidth;
		float m_groundHeightForEachCollum[ CHUNK_LAYER_SIZE ];
		float m_dustHeightForEachCollum[ CHUNK_LAYER_SIZE ];
		float m_groundHeight;
		float m_grassChance;
		float m_growGrassChance;
		float m_grassChanceForEachCollum[CHUNK_LAYER_SIZE];
		float m_growGrassChanceForEachCollum[CHUNK_LAYER_SIZE];
		Vector2i m_worldCoords2D;
		vector<Vertex3D> m_vertexList; 
		vector<Vertex3D> m_specialVertexList;
		GLuint m_vboID;
		GLuint m_secVboID;
		int m_isVboDirty;
		int m_numVertexes;
		int m_numSpecialVertexes;
		ChunkBlocks* m_southNeighborForChunk;
		ChunkBlocks* m_northNeighborForChunk;
		ChunkBlocks* m_eastNeighborForChunk;
		ChunkBlocks* m_westNeighborForChunk;
		unsigned char m_outdoorLightLevel;

	private:
		void GenerateHeightsList();
		float CalculateColorForBlock(Block* block);
		void CreateVertexArray();
		void AddBlockToVertexArray(Vector3 leftBottomCornerCoords, int blockIndex);
		void UpdateLightingForBlock();

	public:
		ChunkBlocks();
		ChunkBlocks(Vector2i worldCoord2D,bool generateHeightFlag);
		~ChunkBlocks();
		static Vector3i GetBlockLocalCoord3DFromBlockIndex(int blockIndex);
		static int GetBlockIndexFromLocalCoord3D(Vector3i localCoord3D);
		void ConnectToNeighborChunks(Vector2i worldCoord2D);
		void DeleteNeighborChunksConnectPointers();
		void Render();
		void RenderDebug();
		void Update();
		void FindNonSkyBlockNeighborOfSkyBlocksAndAddToToDoList();
		void AddAllAirBlocksAtBoundaryToLightList();
		void AddNonSkyBlockNeighBorToToLightList( Block* blockToAdd, BlockInfo blockInfo );
		void InitializeLightingForChunk();
		void ProcessGrassGrowing();
		void RenderNormalBlock();
		void RenderSpecialBlock();
};

extern GameTexture* g_gameTexture;
#endif