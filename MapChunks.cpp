#include "MapChunks.hpp"
#include "GameCommon.hpp"

GameTexture* g_gameTexture = nullptr;
map< Vector2i, ChunkBlocks* > g_mapChunk;
vector<blockTypeInfo> g_blockTypeInfo;

MapChunks::MapChunks()
{
	Vector2i worldCoord2D;
	ChunkBlocks* tempChunk;
	m_cameraWorldCoord2D.SetXY(0,0);

	m_isFinishDrawing = false;

	InitializeLookUpBlockTypeList();

	for(int yCoord2D = 0; yCoord2D < MAP_HEIGHT; yCoord2D++)
	{
		for(int xCoord2D = 0; xCoord2D < MAP_WIDTH; xCoord2D++)
		{
			worldCoord2D.SetXY( xCoord2D, yCoord2D ) ;

			tempChunk = new ChunkBlocks(worldCoord2D,true);

			g_mapChunk[worldCoord2D] = tempChunk;

			tempChunk->InitializeLightingForChunk();

			tempChunk->ConnectToNeighborChunks(worldCoord2D);
		}
	}
}

MapChunks::~MapChunks()
{
	map< Vector2i, ChunkBlocks* >::iterator it = g_mapChunk.begin();
	for( it = g_mapChunk.begin(); it != g_mapChunk.end();++it)
	{
		WriteChunksToDisc(it->first);
	}	
	g_mapChunk.clear();
}

void MapChunks::InitializeLookUpBlockTypeList()
{
	blockTypeInfo blockInfo;

	for(int i = 0; i < CURRENT_TYPE_NUM; i++)
	{
		switch(i)
		{
			case GRASS: blockInfo.topTextureIndex = GetTextureIndexAtCoord2D(23,21);
						blockInfo.sideTextureIndex = GetTextureIndexAtCoord2D(19,19);
						blockInfo.bottomTextureIndex = GetTextureIndexAtCoord2D(18,19);
						blockInfo.isObaque = true;
						blockInfo.isSolid = true;
						blockInfo.isSpecialBlock = false;
						blockInfo.diggingSound[0] = "Sound/dig/grass1.ogg";
						blockInfo.diggingSound[1] = "Sound/dig/grass2.ogg";
						blockInfo.placingSound[0] = nullptr;
						blockInfo.placingSound[1] = nullptr;
						blockInfo.stepOnSound[0] = "Sound/step/grass1.ogg";
						blockInfo.stepOnSound[1] = "Sound/step/grass2.ogg";
						break;

			case DIRT: blockInfo.topTextureIndex = GetTextureIndexAtCoord2D(8,2);
					   blockInfo.sideTextureIndex = GetTextureIndexAtCoord2D(8,2);
					   blockInfo.bottomTextureIndex = GetTextureIndexAtCoord2D(8,2);
					   blockInfo.isObaque = true;
					   blockInfo.isSolid = true;
					   blockInfo.isSpecialBlock = false;
					   blockInfo.diggingSound[0] = "Sound/dig/gravel1.ogg";
					   blockInfo.diggingSound[1] = "Sound/dig/gravel2.ogg";
					   blockInfo.placingSound[0] = nullptr;
					   blockInfo.placingSound[1] = nullptr;
					   blockInfo.stepOnSound[0] = "Sound/step/gravel1.ogg";
					   blockInfo.stepOnSound[1] = "Sound/step/gravel2.ogg";
					   break;

			case AIR: blockInfo.topTextureIndex = GetTextureIndexAtCoord2D(0,0);
					  blockInfo.sideTextureIndex = GetTextureIndexAtCoord2D(0,0);
					  blockInfo.bottomTextureIndex = GetTextureIndexAtCoord2D(0,0);
					  blockInfo.isObaque = false;
					  blockInfo.isSolid = false;
					  blockInfo.isSpecialBlock = false;
					  blockInfo.diggingSound[0] = nullptr;
					  blockInfo.diggingSound[1] = nullptr;
					  blockInfo.placingSound[0] = nullptr;
					  blockInfo.placingSound[1] = nullptr;
					  blockInfo.stepOnSound[0] = nullptr;
					  blockInfo.stepOnSound[1] = nullptr;
					  break;

			case STONE: blockInfo.topTextureIndex = GetTextureIndexAtCoord2D(3,1);
						blockInfo.sideTextureIndex = GetTextureIndexAtCoord2D(3,1);
						blockInfo.bottomTextureIndex = GetTextureIndexAtCoord2D(3,1);
						blockInfo.isObaque = true;
						blockInfo.isSolid = true;
						blockInfo.isSpecialBlock = false;
						blockInfo.diggingSound[0] = "Sound/dig/stone1.ogg";
						blockInfo.diggingSound[1] = "Sound/dig/stone2.ogg";
						blockInfo.placingSound[0] = "Sound/dig/stone3.ogg";
						blockInfo.placingSound[1] = "Sound/dig/stone4.ogg";
						blockInfo.stepOnSound[0] = "Sound/step/stone1.ogg";
						blockInfo.stepOnSound[1] = "Sound/step/stone2.ogg";
						break;

			case WATER: blockInfo.topTextureIndex = GetTextureIndexAtCoord2D(0,0);
						blockInfo.sideTextureIndex = GetTextureIndexAtCoord2D(0,0);
						blockInfo.bottomTextureIndex = GetTextureIndexAtCoord2D(0,0);
						blockInfo.isObaque = false;
						blockInfo.isSolid = false;
						blockInfo.isSpecialBlock = false;
						blockInfo.isSpecialBlock = false;
						blockInfo.diggingSound[0] = nullptr;
						blockInfo.diggingSound[1] = nullptr;
						blockInfo.placingSound[0] = nullptr;
						blockInfo.placingSound[1] = nullptr;
						blockInfo.stepOnSound[0] = "Sound/step/water.mp3";
						blockInfo.stepOnSound[1] = "Sound/step/water.mp3";
						break;

			case SAND:  blockInfo.topTextureIndex = GetTextureIndexAtCoord2D(2,1);
						blockInfo.sideTextureIndex = GetTextureIndexAtCoord2D(2,1);
						blockInfo.bottomTextureIndex = GetTextureIndexAtCoord2D(2,1);
						blockInfo.isObaque = true;
						blockInfo.isSolid = true;
						blockInfo.isSpecialBlock = false;
						blockInfo.diggingSound[0] = "Sound/dig/gravel3.ogg";
						blockInfo.diggingSound[1] = "Sound/dig/gravel4.ogg";
						blockInfo.placingSound[0] = nullptr;
						blockInfo.placingSound[1] = nullptr;
						blockInfo.stepOnSound[0] = "Sound/step/sand1.ogg";
						blockInfo.stepOnSound[1] = "Sound/step/sand2.ogg";
						break;

			case GLOW_STONE: blockInfo.topTextureIndex = GetTextureIndexAtCoord2D(9,6);
							 blockInfo.sideTextureIndex = GetTextureIndexAtCoord2D(9,6);
						     blockInfo.bottomTextureIndex = GetTextureIndexAtCoord2D(9,6);
						     blockInfo.isObaque = true;
						     blockInfo.isSolid = true;
							 blockInfo.isSpecialBlock = false;
							 blockInfo.diggingSound[0] = "Sound/dig/gravel3.ogg";
							 blockInfo.diggingSound[1] = "Sound/dig/gravel4.ogg";
							 blockInfo.placingSound[0] = "Sound/step/stone3.ogg";
							 blockInfo.placingSound[1] = "Sound/step/stone4.ogg";
							 blockInfo.stepOnSound[0] = "Sound/step/stone3.ogg";
							 blockInfo.stepOnSound[1] = "Sound/step/stone4.ogg";
						     break;

			case GROUND: blockInfo.topTextureIndex = GetTextureIndexAtCoord2D(18,20);
						 blockInfo.sideTextureIndex = GetTextureIndexAtCoord2D(18,20);
				         blockInfo.bottomTextureIndex = GetTextureIndexAtCoord2D(18,20);
				         blockInfo.isObaque = true;
				         blockInfo.isSolid = true;
						 blockInfo.isSpecialBlock = false;
						 blockInfo.diggingSound[0] = "Sound/dig/gravel1.ogg";
						 blockInfo.diggingSound[1] = "Sound/dig/gravel2.ogg";
						 blockInfo.placingSound[0] = "Sound/dig/gravel3.ogg";
						 blockInfo.placingSound[1] = "Sound/dig/gravel4.ogg";
						 blockInfo.stepOnSound[0] = "Sound/step/gravel3.ogg";
						 blockInfo.stepOnSound[1] = "Sound/step/gravel4.ogg";
				         break;

			case SNOW: blockInfo.topTextureIndex = GetTextureIndexAtCoord2D(18,23);
					   blockInfo.sideTextureIndex = GetTextureIndexAtCoord2D(20,23);
					   blockInfo.bottomTextureIndex = GetTextureIndexAtCoord2D(18,19);
					   blockInfo.isObaque = true;
					   blockInfo.isSolid = true;
					   blockInfo.isSpecialBlock = false;
					   blockInfo.diggingSound[0] = "Sound/dig/snow1.ogg";
					   blockInfo.diggingSound[1] = "Sound/dig/snow2.ogg";
					   blockInfo.placingSound[0] = "Sound/dig/snow3.ogg";
					   blockInfo.placingSound[1] = "Sound/dig/snow4.ogg";
					   blockInfo.stepOnSound[0] = "Sound/step/snow1.ogg";
					   blockInfo.stepOnSound[1] = "Sound/step/snow2.ogg";
				       break;

			case GROW_GRASS1: blockInfo.topTextureIndex = GetTextureIndexAtCoord2D(0,0);
							  blockInfo.sideTextureIndex = GetTextureIndexAtCoord2D(12,5);
							  blockInfo.bottomTextureIndex = GetTextureIndexAtCoord2D(0,0);
							  blockInfo.isObaque = false;
							  blockInfo.isSolid = false;
							  blockInfo.isSpecialBlock = true;
							  blockInfo.diggingSound[0] = "Sound/dig/grass1.ogg";
							  blockInfo.diggingSound[1] = "Sound/dig/grass2.ogg";
							  blockInfo.placingSound[0] = nullptr;
							  blockInfo.placingSound[1] = nullptr;
							  blockInfo.stepOnSound[0] = "Sound/step/grass1.ogg";
							  blockInfo.stepOnSound[1] = "Sound/step/grass2.ogg";
				              break;

			case DEATH_TREE: blockInfo.topTextureIndex = GetTextureIndexAtCoord2D(0,0);
						      blockInfo.sideTextureIndex = GetTextureIndexAtCoord2D(7,3);
				              blockInfo.bottomTextureIndex = GetTextureIndexAtCoord2D(0,0);
				              blockInfo.isObaque = false;
				              blockInfo.isSolid = false;
				              blockInfo.isSpecialBlock = true;
							  blockInfo.diggingSound[0] = "Sound/dig/grass1.ogg";
							  blockInfo.diggingSound[1] = "Sound/dig/grass2.ogg";
							  blockInfo.placingSound[0] = nullptr;
							  blockInfo.placingSound[1] = nullptr;
							  blockInfo.stepOnSound[0] = "Sound/step/grass1.ogg";
							  blockInfo.stepOnSound[1] = "Sound/step/grass2.ogg";
				              break;

			case WOOD: blockInfo.topTextureIndex = GetTextureIndexAtCoord2D(4,0);
				       blockInfo.sideTextureIndex = GetTextureIndexAtCoord2D(4,0);
				       blockInfo.bottomTextureIndex = GetTextureIndexAtCoord2D(4,0);
				       blockInfo.isObaque = true;
				       blockInfo.isSolid = true;
				       blockInfo.isSpecialBlock = false;
				       blockInfo.diggingSound[0] = "Sound/dig/wood1.ogg";
				       blockInfo.diggingSound[1] = "Sound/dig/wood2.ogg";
				       blockInfo.placingSound[0] = "Sound/dig/wood3.ogg";
				       blockInfo.placingSound[1] = "Sound/dig/wood4.ogg";
				       blockInfo.stepOnSound[0] = "Sound/step/wood1.ogg";
				       blockInfo.stepOnSound[1] = "Sound/step/wood2.ogg";
				       break;
		}
		g_blockTypeInfo.push_back(blockInfo);
	}
}

void MapChunks::SetCameraPosition(Vector3 cameraPosition)
{
	m_cameraPosition = cameraPosition;
}

void MapChunks::SetCameraForwardVector(Vector3 forwardVector)
{
	m_cameraForwardVector = forwardVector;
}

float MapChunks::CalcDistanceSquareFromCameraToChunk(Vector2i worldCoord2D)
{
	Vector3 temp;
	
	temp = CalcChunkCenterWorldCoord(worldCoord2D);

	return ( ( m_cameraPosition.x - temp.x ) * ( m_cameraPosition.x - temp.x )
		   + ( m_cameraPosition.y - temp.y ) * ( m_cameraPosition.y - temp.y)
		    );
}

void MapChunks::GenerateOrLoadChunkFromToDo()
{
	float closestChunkDistanceSquared = VERY_FAR;
	Vector2i closetChunkCoords;
	set< Vector2i >::iterator iter;

	for(iter = m_setOfChunksToGenerateOrLoadFromDisc.begin(); iter != m_setOfChunksToGenerateOrLoadFromDisc.end(); ++iter)
	{
		const Vector2i& chunkCoords = *iter;
		float distSquaredToChunk = CalcDistanceSquareFromCameraToChunk(chunkCoords);
		if(distSquaredToChunk < closestChunkDistanceSquared)
		{
			closestChunkDistanceSquared = distSquaredToChunk;
			closetChunkCoords = chunkCoords;
		}
	}
	
	if(!LoadChunksFromDisc(closetChunkCoords))
	{
		ChunkBlocks* newChunk = new ChunkBlocks(closetChunkCoords,true);
		g_mapChunk[closetChunkCoords] = newChunk;
		newChunk->InitializeLightingForChunk();
		newChunk->ConnectToNeighborChunks(closetChunkCoords);
	}

	m_setOfChunksToGenerateOrLoadFromDisc.erase(closetChunkCoords);
}

void MapChunks::DeleteAndSaveChunksFromToDo()
{
	float furthestChunkDistanceSquared = VERY_CLOSE;
	Vector2i furthestChunkCoords;
	set< Vector2i >::iterator iter;

	for(iter = m_setOfChunksToSaveAndDelete.begin(); iter != m_setOfChunksToSaveAndDelete.end(); ++iter)
	{
		const Vector2i& chunkCoords = *iter;
		float distSquaredToChunk = CalcDistanceSquareFromCameraToChunk(chunkCoords);
		if(distSquaredToChunk > furthestChunkDistanceSquared)
		{
			furthestChunkDistanceSquared = distSquaredToChunk;
			furthestChunkCoords = chunkCoords;
		}
	}

	//save the furthest block to hard disk
	WriteChunksToDisc(furthestChunkCoords);

	ChunkBlocks* chunkToDelete = g_mapChunk[furthestChunkCoords];
	chunkToDelete->DeleteNeighborChunksConnectPointers();
	delete chunkToDelete;
	//then erase from map

	g_mapChunk.erase(furthestChunkCoords);

	m_setOfChunksToSaveAndDelete.erase(furthestChunkCoords);
}

void MapChunks::GenerateToDoList()
{
	CalcCameraWorldCoord2DFromWorldCoord(m_cameraPosition);

	Vector2i bottomLeftWorldCoord2DofCheckArea;
	Vector2i tempChunkWorldCoord2D;
	float distSquare;

	//Generate ToGenerateOrToLoadList
	bottomLeftWorldCoord2DofCheckArea.x = m_cameraWorldCoord2D.x - INNER_RADIUS_IN_CHUNK;
	bottomLeftWorldCoord2DofCheckArea.y = m_cameraWorldCoord2D.y - INNER_RADIUS_IN_CHUNK;

	for( int yOffset = 0; yOffset < ( 2 * INNER_RADIUS_IN_CHUNK + 1 ); yOffset++ )
	{
		for( int xOffset = 0; xOffset < ( 2 * INNER_RADIUS_IN_CHUNK + 1 ); xOffset++ )
		{
			tempChunkWorldCoord2D.x = bottomLeftWorldCoord2DofCheckArea.x + xOffset;
			tempChunkWorldCoord2D.y = bottomLeftWorldCoord2DofCheckArea.y + yOffset;

			distSquare = CalcDistanceSquareFromCameraToChunk(tempChunkWorldCoord2D);

			if(distSquare <= INNER_RADIUS_IN_PIXEL * INNER_RADIUS_IN_PIXEL)
			{
				if(g_mapChunk.find(tempChunkWorldCoord2D) == g_mapChunk.end())
				{
					m_setOfChunksToGenerateOrLoadFromDisc.insert(tempChunkWorldCoord2D);
				}
			}
		}
	}

	//Generate ToDelete List
	for(map< Vector2i, ChunkBlocks* >::iterator it = g_mapChunk.begin(); it != g_mapChunk.end();++it)
	{
			distSquare = CalcDistanceSquareFromCameraToChunk(it->first);

			if(distSquare >= OUTTER_RADIUS_IN_PIXEL * OUTTER_RADIUS_IN_PIXEL)
			{
				m_setOfChunksToSaveAndDelete.insert(it->first);
			}
	}
}

unsigned char* MapChunks::CreateRLECompressionForChunk(Vector2i chunkCoord2D,int& sizeOfBuffer)
{
	const int BUFFER_MAX_SIZE_BYTES = 3 * CHUNK_SIZE_IN_BLOCK;
	int currentBufferWriteIndex = 0;
	ChunkBlocks* tempChunk = nullptr;

	if(g_mapChunk.find(chunkCoord2D) == g_mapChunk.end())
		return nullptr;

	tempChunk = g_mapChunk[chunkCoord2D];
	unsigned char currentType = tempChunk->m_blocksList[0].m_blockType.material;
	unsigned short howManyOfCurrentType = 1;
	unsigned char* buffer = new unsigned char[BUFFER_MAX_SIZE_BYTES];

	for(int blockIndex = 1; blockIndex < CHUNK_SIZE_IN_BLOCK; blockIndex++)
	{
		Block& currentBlock = tempChunk->m_blocksList[blockIndex];

		if(currentBlock.m_blockType.material != currentType)
		{
			//Write to buffer
			buffer[currentBufferWriteIndex] = currentType;
			++currentBufferWriteIndex;
			//unsigned char* des = buffer + currentBufferWriteIndex;
			
			memcpy(buffer + currentBufferWriteIndex, &howManyOfCurrentType,sizeof(unsigned short));
			currentBufferWriteIndex += sizeof(unsigned short);

			currentType = currentBlock.m_blockType.material;
			howManyOfCurrentType = 1;
		}
		else
			howManyOfCurrentType++;
	}
	
	buffer[currentBufferWriteIndex] = currentType;
	++currentBufferWriteIndex;
	memcpy(buffer + currentBufferWriteIndex, &howManyOfCurrentType,sizeof(unsigned short));
	currentBufferWriteIndex += sizeof(unsigned short);

	sizeOfBuffer = currentBufferWriteIndex;

	return buffer;
}

void MapChunks::WriteChunksToDisc(Vector2i chunkCoord2D)
{
	//Generate file name
	FILE* fp;
	unsigned char* buffer = nullptr;
	int sizeOfBuffer;
	string fileName;

	buffer = CreateRLECompressionForChunk(chunkCoord2D,sizeOfBuffer);

	if(buffer == nullptr)
		return;

	fileName = Stringf("Chunks/Chunk_X_%i_Y_%i.chunk",chunkCoord2D.x,chunkCoord2D.y);

	//fp = fopen(fileName.c_str(),"wb");
	fopen_s(&fp,fileName.c_str(),"wb");
	if(fp != nullptr)
	{
		fwrite(buffer,1,sizeOfBuffer,fp);
		fclose(fp);
	}
	
	delete[] buffer;
}

bool MapChunks::LoadChunksFromDisc(Vector2i chunkCoord2D)
{
	FILE* fp;
	string fileName = Stringf("Chunks/Chunk_X_%i_Y_%i.chunk",chunkCoord2D.x,chunkCoord2D.y);
	ChunkBlocks* tempChunk = new ChunkBlocks(chunkCoord2D,false); 
	

	//fp = fopen(fileName.c_str(),"rb");
	fopen_s(&fp,fileName.c_str(),"rb");
	if(fp != nullptr)
	{
		DecompressRLEAndGenerateChunk(fp,tempChunk);
		
		g_mapChunk[chunkCoord2D] = tempChunk;
		tempChunk->InitializeLightingForChunk();
		tempChunk->ConnectToNeighborChunks(chunkCoord2D);
	
		return true;
	}
	else
	{
		delete tempChunk;
		return false;
	}
}


void MapChunks::DecompressRLEAndGenerateChunk(FILE* fp,ChunkBlocks* tempChunk)
{
	unsigned char* buffer;
	int fileSize;
	int blockIndex = 0;
	size_t result;

	fseek(fp,0, SEEK_END);
	fileSize = ftell(fp);
	rewind(fp);

	buffer = new unsigned char[fileSize];

	result = fread(buffer,1,fileSize,fp);

	fclose(fp);

	for(int i = 0; i < fileSize;i += 3)
	{
		unsigned short numCurrentType;
		unsigned char currentType;
			
		currentType = buffer[i];//AIR;
		memcpy(&numCurrentType,buffer + (i + 1), sizeof(unsigned short) );

		for( int j = 0; j < numCurrentType;j++ )
		{
			Block& currentBlock = tempChunk->m_blocksList[blockIndex];
			switch(currentType)
			{
				case GRASS: currentBlock.m_blockType.material = GRASS;
							break;
				case DIRT: currentBlock.m_blockType.material = DIRT;
						   break;
				case AIR:	currentBlock.m_blockType.material = AIR;
							break;
				case STONE:	currentBlock.m_blockType.material = STONE;
							break;
				case WATER:	currentBlock.m_blockType.material = WATER;
							break;
				case SAND:	currentBlock.m_blockType.material = SAND;
							break;
				case GLOW_STONE: currentBlock.m_blockType.material = GLOW_STONE;
								 break;
				case GROUND: currentBlock.m_blockType.material = GROUND;
							 break;
				case SNOW: currentBlock.m_blockType.material = SNOW;
						   break;
				case GROW_GRASS1: currentBlock.m_blockType.material = GROW_GRASS1;
							      break;
				case DEATH_TREE: currentBlock.m_blockType.material = DEATH_TREE;
					             break;
				case WOOD: currentBlock.m_blockType.material = WOOD;
						   break;
			}
			blockIndex++;
		}
	}

	delete[] buffer;
}

Vector3 MapChunks::CalcChunkCenterWorldCoord(Vector2i worldCoord2D)
{
	Vector3 chunkCenterWorldCoord; 

	chunkCenterWorldCoord.x = worldCoord2D.x * CHUNK_WIDTH_X + (float)CHUNK_WIDTH_X * .5f;
	chunkCenterWorldCoord.y = worldCoord2D.y * CHUNK_WIDTH_X + (float)CHUNK_WIDTH_X * .5f;
	chunkCenterWorldCoord.z = (float)CHUNK_HEIGHT * .5f;

	return chunkCenterWorldCoord;
}

void MapChunks::CalcCameraWorldCoord2DFromWorldCoord(Vector3 position)
{
	if(position.x > 0)
		m_cameraWorldCoord2D.x = static_cast<int>(position.x / CHUNK_WIDTH_X);
	else
		m_cameraWorldCoord2D.x = static_cast<int>(position.x / CHUNK_WIDTH_X) - 1;

	if(position.y > 0)
		m_cameraWorldCoord2D.y = static_cast<int>(position.y / CHUNK_WIDTH_X);
	else
		m_cameraWorldCoord2D.y = static_cast<int>(position.y / CHUNK_WIDTH_X) - 1;
}

void MapChunks::Update()
{
 	GenerateToDoList();
 	
 	if(m_setOfChunksToGenerateOrLoadFromDisc.size() != 0)
 		GenerateOrLoadChunkFromToDo();
 
 	if(m_setOfChunksToSaveAndDelete.size() != 0)
 		DeleteAndSaveChunksFromToDo();

	for(map< Vector2i, ChunkBlocks* >::iterator it = g_mapChunk.begin(); it != g_mapChunk.end();++it)
	{
		it->second->Update();
	}
}

void MapChunks::Render()
{
	for(map< Vector2i, ChunkBlocks* >::iterator it = g_mapChunk.begin(); it != g_mapChunk.end();++it)
	{
		if(IsChunkInFrontOfCamera(it->second))
			it->second->Render();
	}

}

bool MapChunks::IsChunkInFrontOfCamera(ChunkBlocks* chunk)
{
	Vector3 forwardVector = m_cameraForwardVector;
	Vector3 cameraPosition = m_cameraPosition;
	//m_cameraPosition.SetXYZ(0,0,70);

	if(chunk != nullptr)
	{
		if(forwardVector.DotProduct( chunk->m_minWorldCoords - m_cameraPosition ) >= 0.f)
			return true;
		if(forwardVector.DotProduct( chunk->m_minWorldCoords + Vector3(CHUNK_WIDTH_X,0.f,0.f) - m_cameraPosition ) >= 0.f)
			return true;
		if(forwardVector.DotProduct(chunk->m_minWorldCoords + Vector3(CHUNK_WIDTH_X,CHUNK_DEPTH_Y,0.f) - m_cameraPosition ) >= 0.f)
			return true;
		if(forwardVector.DotProduct(chunk->m_minWorldCoords + Vector3(0.f,CHUNK_DEPTH_Y,0.f) - m_cameraPosition ) >= 0.f)
			return true;
		if(forwardVector.DotProduct(chunk->m_minWorldCoords + Vector3(CHUNK_WIDTH_X,0.f,CHUNK_HEIGHT) - m_cameraPosition ) >= 0.f)
			return true;
		if(forwardVector.DotProduct(chunk->m_minWorldCoords + Vector3(CHUNK_WIDTH_X,CHUNK_DEPTH_Y,CHUNK_HEIGHT) - m_cameraPosition ) >= 0.f)
			return true;
		if(forwardVector.DotProduct(chunk->m_minWorldCoords + Vector3(0.f,CHUNK_DEPTH_Y,CHUNK_HEIGHT) - m_cameraPosition ) >= 0.f)
			return true;
		if(forwardVector.DotProduct(chunk->m_minWorldCoords + Vector3(0.f,0.f,CHUNK_HEIGHT) - m_cameraPosition ) >= 0.f)
			return true;
	}
	return false;
}
