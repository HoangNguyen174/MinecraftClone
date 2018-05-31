#include "ChunkBlocks.hpp"
#include "MapChunks.hpp"

queue<BlockInfo> g_listOfBlockToPropagateLight;

ChunkBlocks::ChunkBlocks()
{
}

ChunkBlocks::ChunkBlocks(Vector2i worldCoord2D,bool generateHeightFlag)
{	
	m_worldCoords2D = worldCoord2D;
	m_minWorldCoords.x = worldCoord2D.x * CHUNK_WIDTH_X;
	m_minWorldCoords.y = worldCoord2D.y * CHUNK_DEPTH_Y;
	m_minWorldCoords.z = 0;
	m_frameWidth = 1.f/32.f;
	m_vboID = 0;
	m_secVboID = 0;
	m_isVboDirty = true;
	
	m_northNeighborForChunk = nullptr;
	m_southNeighborForChunk = nullptr;
	m_eastNeighborForChunk = nullptr;
	m_westNeighborForChunk = nullptr;

	m_outdoorLightLevel = g_globalOutDoorLightLevel;

	if(generateHeightFlag)
		GenerateHeightsList();
}


ChunkBlocks::~ChunkBlocks()
{
	GLRender::DeleteBuffers( 1, &m_vboID );
	GLRender::DeleteBuffers( 1, &m_secVboID );
	//glDeleteBuffers(1,&m_vboID);
}

void ChunkBlocks::ConnectToNeighborChunks(Vector2i worldCoord2D)
{
	if(g_mapChunk.find( Vector2i( worldCoord2D.x + 1,worldCoord2D.y ) ) != g_mapChunk.end())
	{
		if(m_northNeighborForChunk == nullptr)
		{
			m_northNeighborForChunk = g_mapChunk[ Vector2i( worldCoord2D.x + 1 ,worldCoord2D.y ) ];
			m_northNeighborForChunk->m_southNeighborForChunk = this;
			m_northNeighborForChunk->m_isVboDirty = true;
		}
	}
	
	if(g_mapChunk.find( Vector2i( worldCoord2D.x - 1,worldCoord2D.y ) ) != g_mapChunk.end())
	{
		if(m_southNeighborForChunk == nullptr)
		{
			m_southNeighborForChunk = g_mapChunk[ Vector2i( worldCoord2D.x - 1,worldCoord2D.y ) ];
			m_southNeighborForChunk->m_northNeighborForChunk = this;
			m_southNeighborForChunk->m_isVboDirty = true;
		}
	}

	if(g_mapChunk.find( Vector2i( worldCoord2D.x, worldCoord2D.y - 1 ) ) != g_mapChunk.end())
	{
		if(m_eastNeighborForChunk == nullptr)
		{
			m_eastNeighborForChunk = g_mapChunk[ Vector2i( worldCoord2D.x ,worldCoord2D.y - 1 ) ];
			m_eastNeighborForChunk->m_westNeighborForChunk = this;
			m_eastNeighborForChunk->m_isVboDirty = true;
		}
	}

	if(g_mapChunk.find( Vector2i( worldCoord2D.x ,worldCoord2D.y + 1 ) ) != g_mapChunk.end())
	{
		if(m_westNeighborForChunk == nullptr)
		{
			m_westNeighborForChunk = g_mapChunk[ Vector2i( worldCoord2D.x,worldCoord2D.y + 1 ) ];
			m_westNeighborForChunk->m_eastNeighborForChunk = this;
			m_westNeighborForChunk->m_isVboDirty = true;
		}
	}
}

void ChunkBlocks::DeleteNeighborChunksConnectPointers()
{
	if(m_eastNeighborForChunk != nullptr)
		m_eastNeighborForChunk->m_westNeighborForChunk = nullptr;
	if(m_westNeighborForChunk != nullptr)
		m_westNeighborForChunk->m_eastNeighborForChunk = nullptr;
	if(m_southNeighborForChunk != nullptr)
		m_southNeighborForChunk->m_northNeighborForChunk = nullptr;
	if(m_northNeighborForChunk != nullptr)
		m_northNeighborForChunk->m_southNeighborForChunk = nullptr;

	m_eastNeighborForChunk = nullptr;
	m_westNeighborForChunk = nullptr;
	m_southNeighborForChunk = nullptr;
	m_northNeighborForChunk = nullptr;
}

void ChunkBlocks::GenerateHeightsList()
{
	int blockIndex = 0;

	for(int chunkCollumIndex = 0 ; chunkCollumIndex < CHUNK_LAYER_SIZE; chunkCollumIndex++)
	{
		Vector2i m_colCoords;
		m_colCoords.x = ( chunkCollumIndex & BOTTOM_4_BITS_MASK ) + ( m_worldCoords2D.x * CHUNK_WIDTH_X );//( chunkCollumIndex % CHUNK_WIDTH ) + ( m_worldCoords2D.x * CHUNK_WIDTH );
		m_colCoords.y = ( chunkCollumIndex >> 4 ) + ( m_worldCoords2D.y * CHUNK_WIDTH_X );;//( chunkCollumIndex / CHUNK_WIDTH ) + ( m_worldCoords2D.y * CHUNK_WIDTH );
		m_groundHeight =  64.f + ComputePerlinNoiseValueAtPosition2D(Vector2( m_colCoords.x,m_colCoords.y ),80.f,10,40.f,0.4f);
		m_grassChance = ComputePerlinNoiseValueAtPosition2D(Vector2( m_colCoords.x,m_colCoords.y ),40.f,10,1.f,0.4f);
		m_growGrassChance = ComputePerlinNoiseValueAtPosition2D(Vector2( m_colCoords.x,m_colCoords.y ),10.f,10,1.f,0.4f);
		m_groundHeightForEachCollum[chunkCollumIndex] = m_groundHeight;
		m_grassChanceForEachCollum[chunkCollumIndex] = m_grassChance;
		m_growGrassChanceForEachCollum[chunkCollumIndex] = m_growGrassChance;
	}

	for(blockIndex = 0; blockIndex < CHUNK_SIZE_IN_BLOCK;blockIndex++)
	{
		Vector3i blockLocalCoords3D = GetBlockLocalCoord3DFromBlockIndex(blockIndex);

		int blockCol = blockLocalCoords3D.x + ( CHUNK_WIDTH_X * blockLocalCoords3D.y );
		
		if(blockLocalCoords3D.z > m_groundHeightForEachCollum[blockCol])
		{
			m_blocksList[blockIndex].m_blockType.material = AIR;
		}
		else if(blockLocalCoords3D.z < ( m_groundHeightForEachCollum[blockCol] - 3) )
		{
			m_blocksList[blockIndex].m_blockType.material = DIRT;
		}
		else 		
		{
			m_blocksList[blockIndex].m_blockType.material = GROUND;

			if(m_grassChanceForEachCollum[blockCol] < 0.01f)
			{
				m_blocksList[blockIndex].m_blockType.material = GRASS;
			}
		}
	}

	for(blockIndex = 0; blockIndex < CHUNK_SIZE_IN_BLOCK;blockIndex++)
	{
		Vector3i blockLocalCoords3D = GetBlockLocalCoord3DFromBlockIndex(blockIndex);

		int blockCol = blockLocalCoords3D.x + ( CHUNK_WIDTH_X * blockLocalCoords3D.y );

		if(blockLocalCoords3D.z > 0 && blockLocalCoords3D.z < CHUNK_HEIGHT - 1 && blockLocalCoords3D.z > SEA_LEVEL + 1)
		{
			if( m_blocksList[blockIndex + CHUNK_LAYER_SIZE].m_blockType.material == AIR)
			{
				if( m_blocksList[blockIndex - CHUNK_LAYER_SIZE].m_blockType.material == GRASS)
				{	
					if(m_growGrassChanceForEachCollum[blockCol] < 0.01f )
					{
						m_blocksList[blockIndex].m_blockType.material = GROW_GRASS1;
					}
				}
				else if (m_blocksList[blockIndex - CHUNK_LAYER_SIZE].m_blockType.material == GROUND)
				{
					if(m_growGrassChanceForEachCollum[blockCol] < -0.1f )
					{
						m_blocksList[blockIndex].m_blockType.material = DEATH_TREE;
					}
				}
			}
		}
	}


	for(blockIndex = 0; blockIndex < CHUNK_SIZE_IN_BLOCK;blockIndex++)
	{
		Vector3i blockLocalCoords3D = GetBlockLocalCoord3DFromBlockIndex(blockIndex);

		if(blockLocalCoords3D.z < SEA_LEVEL && m_blocksList[blockIndex].m_blockType.material == AIR )
		{
			m_blocksList[blockIndex].m_blockType.material = WATER;
		}
	}

	for(blockIndex = 0; blockIndex < CHUNK_SIZE_IN_BLOCK;blockIndex++)
	{
		Vector3i blockLocalCoords3D = GetBlockLocalCoord3DFromBlockIndex(blockIndex);

		if(blockLocalCoords3D.z > 78 && m_blocksList[blockIndex].m_blockType.material != AIR )
		{
			m_blocksList[blockIndex].m_blockType.material = SNOW;
		}
	}

	for(blockIndex = 0; blockIndex < CHUNK_SIZE_IN_BLOCK - CHUNK_LAYER_SIZE; blockIndex++)
	{
		Vector3i blockLocalCoords3D = GetBlockLocalCoord3DFromBlockIndex(blockIndex);

		if(m_blocksList[blockIndex].m_blockType.material != AIR && m_blocksList[blockIndex + CHUNK_LAYER_SIZE].m_blockType.material == WATER )
		{
			m_blocksList[blockIndex].m_blockType.material = SAND;
		}
	}
}

void ChunkBlocks::Update()
{
	ProcessGrassGrowing();
}

void ChunkBlocks::Render()
{
	if(m_isVboDirty)
	{
		CreateVertexArray();
	}

	RenderNormalBlock();
	RenderSpecialBlock();
}

void ChunkBlocks::RenderSpecialBlock()
{
	GLRender::Enable( GL_TEXTURE_2D );//glEnable(GL_TEXTURE_2D);
	GLRender::EnableClientState( GL_VERTEX_ARRAY );//glEnableClientState( GL_VERTEX_ARRAY );
	GLRender::EnableClientState( GL_COLOR_ARRAY );//glEnableClientState( GL_COLOR_ARRAY );
	GLRender::EnableClientState( GL_TEXTURE_COORD_ARRAY );//glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	GLRender::Enable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.5f);

	//GLRender::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLRender::Disable(GL_CULL_FACE);
	GLRender::BindBuffer( GL_ARRAY_BUFFER, m_secVboID );//glBindBuffer(GL_ARRAY_BUFFER,m_vboID);

	GLRender::VertexPointer(3, GL_FLOAT, sizeof( Vertex3D ), (const GLvoid*) offsetof( Vertex3D, m_position ));//glVertexPointer( 3, GL_FLOAT, sizeof( Vertex3D ), (const GLvoid*) offsetof( Vertex3D, m_position ) );	
	GLRender::ColorPointer(4, GL_FLOAT, sizeof( Vertex3D ), (const GLvoid*) offsetof( Vertex3D, m_color ));//glColorPointer ( 4, GL_FLOAT, sizeof( Vertex3D ), (const GLvoid*) offsetof( Vertex3D, m_color ) );	
	GLRender::TexCoordPointer(2, GL_FLOAT, sizeof( Vertex3D ), (const GLvoid*) offsetof( Vertex3D, m_texCoords ));//glTexCoordPointer( 2, GL_FLOAT, sizeof( Vertex3D ), (const GLvoid*) offsetof( Vertex3D, m_texCoords ) );	

	GLRender::DrawArray( GL_QUADS,0 , m_numSpecialVertexes);//glDrawArrays( GL_QUADS, 0, m_numVertexes);

	GLRender::DisableClientState( GL_VERTEX_ARRAY );//glDisableClientState( GL_VERTEX_ARRAY );
	GLRender::DisableClientState( GL_COLOR_ARRAY);//glDisableClientState( GL_COLOR_ARRAY );
	GLRender::DisableClientState( GL_TEXTURE_COORD_ARRAY);//glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	GLRender::Disable(GL_ALPHA_TEST);
	GLRender::Enable(GL_CULL_FACE);
	GLRender::BindBuffer( GL_ARRAY_BUFFER, 0 );//glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void ChunkBlocks::RenderNormalBlock()
{
	GLRender::Enable( GL_TEXTURE_2D );//glEnable(GL_TEXTURE_2D);
	GLRender::EnableClientState( GL_VERTEX_ARRAY );//glEnableClientState( GL_VERTEX_ARRAY );
	GLRender::EnableClientState( GL_COLOR_ARRAY );//glEnableClientState( GL_COLOR_ARRAY );
	GLRender::EnableClientState( GL_TEXTURE_COORD_ARRAY );//glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	GLRender::BindBuffer( GL_ARRAY_BUFFER, m_vboID );//glBindBuffer(GL_ARRAY_BUFFER,m_vboID);

	GLRender::VertexPointer(3, GL_FLOAT, sizeof( Vertex3D ), (const GLvoid*) offsetof( Vertex3D, m_position ));
	//glVertexPointer( 3, GL_FLOAT, sizeof( Vertex3D ), (const GLvoid*) offsetof( Vertex3D, m_position ) );	
	GLRender::ColorPointer(4, GL_FLOAT, sizeof( Vertex3D ), (const GLvoid*) offsetof( Vertex3D, m_color ));//glColorPointer ( 4, GL_FLOAT, sizeof( Vertex3D ), (const GLvoid*) offsetof( Vertex3D, m_color ) );	
	GLRender::TexCoordPointer(2, GL_FLOAT, sizeof( Vertex3D ), (const GLvoid*) offsetof( Vertex3D, m_texCoords ));//glTexCoordPointer( 2, GL_FLOAT, sizeof( Vertex3D ), (const GLvoid*) offsetof( Vertex3D, m_texCoords ) );	

	GLRender::DrawArray( GL_QUADS,0 , m_numVertexes);//glDrawArrays( GL_QUADS, 0, m_numVertexes);

	GLRender::DisableClientState( GL_VERTEX_ARRAY );//glDisableClientState( GL_VERTEX_ARRAY );
	GLRender::DisableClientState( GL_COLOR_ARRAY);//glDisableClientState( GL_COLOR_ARRAY );
	GLRender::DisableClientState( GL_TEXTURE_COORD_ARRAY);//glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	GLRender::Disable(GL_ALPHA_TEST);
	GLRender::BindBuffer( GL_ARRAY_BUFFER, 0 );//glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void ChunkBlocks::RenderDebug()
{
	for(int i = 0; i < CHUNK_SIZE_IN_BLOCK; i++)
	{
		Vector3i localCoord3DofBlock = GetBlockLocalCoord3DFromBlockIndex(i);
		if(m_blocksList[i].m_blockType.material == AIR && m_blocksList[i].IsSkyBlockBitSet())
		{
			GLRender::Disable( GL_TEXTURE_2D );//glDisable(GL_TEXTURE_2D);
			GLRender::Color4f( 0.f,0.f,1.f,0.5f);//glColor4f(0.f,0.f,1.f,0.5f);
			GLRender::PointSize( 10 );//glPointSize(10);
			GLRender::BeginDraw( GL_POINTS ); //glBegin(GL_POINTS);
			{
				GLRender::Vertex3f( localCoord3DofBlock.x + 0.5f,localCoord3DofBlock.y + 0.5f,localCoord3DofBlock.z + 0.5f );//glVertex3f(localCoord3DofBlock.x + 0.5f,localCoord3DofBlock.y + 0.5f,localCoord3DofBlock.z + 0.5f);
			}
			GLRender::EndDraw();//glEnd();
			GLRender::Enable(GL_TEXTURE_2D);//glEnable(GL_TEXTURE_2D);
		}
	}
}

void ChunkBlocks::AddBlockToVertexArray(Vector3 leftBottomCornerCoords, int blockIndex)
{
	Vector3 temp = leftBottomCornerCoords;
	Vector2 texCoords;
	Vertex3D tempVertex;
	Vector3i localCoord3DofBlock;

	Block currentBlock = m_blocksList[blockIndex];
	Block* topBlock = nullptr;
	Block* bottomBlock = nullptr;
	Block* northBlock = nullptr;
	Block* southBlock = nullptr;
	Block* eastBlock = nullptr;
	Block* westBlock = nullptr;

	blockTypeInfo currentBlockInfo;

	currentBlockInfo = g_blockTypeInfo[currentBlock.m_blockType.material];

	localCoord3DofBlock = GetBlockLocalCoord3DFromBlockIndex(blockIndex);

	float color = 0.f;
	
	if( currentBlock.m_blockType.material == AIR  || currentBlock.m_blockType.material == WATER )
		return;

	//top face
	if( localCoord3DofBlock.z < ( CHUNK_HEIGHT - 1 ) )
	{
		topBlock = &m_blocksList[ blockIndex + CHUNK_LAYER_SIZE ];
	}

	if(!g_blockTypeInfo[topBlock->m_blockType.material].isObaque)
	{
		color = CalculateColorForBlock(topBlock);

		if(color <= 0.f)
			color = minLightValue;

		tempVertex.m_color.SetColor(color,color,color,1.f);

		texCoords = g_gameTexture->GetTopLeftTextureCoordsFromFrameIndex(currentBlockInfo.topTextureIndex);
		if(!g_blockTypeInfo[currentBlock.m_blockType.material].isSpecialBlock)
		{
			tempVertex.m_position = Vector3(temp.x , temp.y, temp.z + 1.f);
			tempVertex.m_texCoords = Vector2(texCoords.x + m_frameWidth, texCoords.y + m_frameWidth);
			m_vertexList.push_back(tempVertex);

			tempVertex.m_position = Vector3(temp.x + 1.f, temp.y , temp.z + 1.f);
			tempVertex.m_texCoords = Vector2(texCoords.x + m_frameWidth, texCoords.y);
			m_vertexList.push_back(tempVertex);

			tempVertex.m_position = Vector3(temp.x + 1.f, temp.y + 1.f, temp.z + 1.f);
			tempVertex.m_texCoords = Vector2( texCoords.x, texCoords.y );
			m_vertexList.push_back(tempVertex);

			tempVertex.m_position = Vector3(temp.x , temp.y + 1.f, temp.z + 1.f);
			tempVertex.m_texCoords = Vector2(texCoords.x, texCoords.y + m_frameWidth);
			m_vertexList.push_back(tempVertex);
		}
	}
	//south face
	if( localCoord3DofBlock.x > 0 )
	{
		southBlock = &m_blocksList[ blockIndex - 1 ];
	}
	else if( localCoord3DofBlock.x == 0)
	{
		if( m_southNeighborForChunk != nullptr )
		{
			southBlock = &m_southNeighborForChunk->m_blocksList[ blockIndex + ( CHUNK_WIDTH_X - 1 )];
		}
	}
	
	if( southBlock != nullptr && !g_blockTypeInfo[southBlock->m_blockType.material].isObaque )
	{
		color = CalculateColorForBlock(southBlock);

		if(color <= 0.f)
			color = minLightValue;

		tempVertex.m_color.SetColor(color,color,color,1.f);

		texCoords = g_gameTexture->GetTopLeftTextureCoordsFromFrameIndex(currentBlockInfo.sideTextureIndex);

		if(!g_blockTypeInfo[currentBlock.m_blockType.material].isSpecialBlock)
		{
			tempVertex.m_position = Vector3(temp.x , temp.y + 1.f ,temp.z );
			tempVertex.m_texCoords = Vector2(texCoords.x,texCoords.y + m_frameWidth );
			m_vertexList.push_back(tempVertex);

			tempVertex.m_position = Vector3(temp.x, temp.y ,temp.z );
			tempVertex.m_texCoords = Vector2( texCoords.x + m_frameWidth , texCoords.y + m_frameWidth );
			m_vertexList.push_back(tempVertex);

			tempVertex.m_position = Vector3( temp.x, temp.y ,temp.z + 1.f);
			tempVertex.m_texCoords = Vector2( texCoords.x + m_frameWidth,texCoords.y );
			m_vertexList.push_back(tempVertex);

			tempVertex.m_position = Vector3( temp.x , temp.y + 1.f ,temp.z + 1.f);
			tempVertex.m_texCoords = Vector2( texCoords.x,texCoords.y );
			m_vertexList.push_back(tempVertex);
		}
		else
		{
			tempVertex.m_position = Vector3(temp.x , temp.y + 1.f ,temp.z );
			tempVertex.m_texCoords = Vector2(texCoords.x,texCoords.y + m_frameWidth);
			m_specialVertexList.push_back(tempVertex);

			tempVertex.m_position = Vector3( temp.x , temp.y + 1.f ,temp.z + 1.f);
			tempVertex.m_texCoords = Vector2( texCoords.x,texCoords.y );
			m_specialVertexList.push_back(tempVertex);

			tempVertex.m_position = Vector3( temp.x + 1.f, temp.y ,temp.z + 1.f);
			tempVertex.m_texCoords = Vector2( texCoords.x + m_frameWidth, texCoords.y  );
			m_specialVertexList.push_back(tempVertex);

			tempVertex.m_position = Vector3( temp.x + 1.f, temp.y ,temp.z);
			tempVertex.m_texCoords = Vector2( texCoords.x + m_frameWidth,texCoords.y + m_frameWidth );
			m_specialVertexList.push_back(tempVertex);
		}
	}

	//north face
	if( localCoord3DofBlock.x < CHUNK_WIDTH_X - 1 )
	{
		northBlock = &m_blocksList[ blockIndex + 1 ];
	}
	else if( localCoord3DofBlock.x == CHUNK_WIDTH_X - 1)
	{
		if(m_northNeighborForChunk != nullptr)
		{
			northBlock = &m_northNeighborForChunk->m_blocksList[ blockIndex - ( CHUNK_WIDTH_X - 1 ) ];
		}
	}
	
	if( northBlock != nullptr && !g_blockTypeInfo[northBlock->m_blockType.material].isObaque )
	{
		color = CalculateColorForBlock(northBlock);

		if(color <= 0.f)
			color = minLightValue;

		texCoords = g_gameTexture->GetTopLeftTextureCoordsFromFrameIndex(currentBlockInfo.sideTextureIndex);
		tempVertex.m_color.SetColor(color,color,color,1.f);

		if(!g_blockTypeInfo[currentBlock.m_blockType.material].isSpecialBlock)
		{
			tempVertex.m_position = Vector3( temp.x + 1.f, temp.y + 1.f , temp.z );
			tempVertex.m_texCoords = Vector2( texCoords.x + m_frameWidth, texCoords.y + m_frameWidth );
			m_vertexList.push_back(tempVertex);

			tempVertex.m_position = Vector3( temp.x + 1.f, temp.y + 1.f , temp.z + 1.f  );
			tempVertex.m_texCoords = Vector2( texCoords.x + m_frameWidth, texCoords.y );
			m_vertexList.push_back(tempVertex);

			tempVertex.m_position = Vector3( temp.x + 1.f, temp.y , temp.z + 1.f  );
			tempVertex.m_texCoords = Vector2( texCoords.x , texCoords.y  );
			m_vertexList.push_back(tempVertex);

			tempVertex.m_position = Vector3( temp.x + 1.f, temp.y , temp.z  );
			tempVertex.m_texCoords = Vector2( texCoords.x , texCoords.y + m_frameWidth  );
			m_vertexList.push_back(tempVertex);
		}
		else
		{
			tempVertex.m_position = Vector3( temp.x + 1.f, temp.y + 1.f , temp.z );
			tempVertex.m_texCoords = Vector2( texCoords.x , texCoords.y + m_frameWidth );
			m_specialVertexList.push_back(tempVertex);

			tempVertex.m_position = Vector3( temp.x + 1.f, temp.y + 1.f , temp.z + 1.f  );
			tempVertex.m_texCoords = Vector2( texCoords.x , texCoords.y );
			m_specialVertexList.push_back(tempVertex);

			tempVertex.m_position = Vector3( temp.x , temp.y , temp.z + 1.f  );
			tempVertex.m_texCoords = Vector2( texCoords.x + m_frameWidth, texCoords.y );
			m_specialVertexList.push_back(tempVertex);

			tempVertex.m_position = Vector3( temp.x , temp.y , temp.z );
			tempVertex.m_texCoords = Vector2( texCoords.x + m_frameWidth, texCoords.y + m_frameWidth );
			m_specialVertexList.push_back(tempVertex);
		}
	}

	//west face
	if( localCoord3DofBlock.y < CHUNK_DEPTH_Y - 1)
	{
		westBlock = &m_blocksList[ blockIndex + CHUNK_DEPTH_Y ];
	}
	else if( localCoord3DofBlock.y == CHUNK_DEPTH_Y - 1 )
	{
		if( m_westNeighborForChunk != nullptr )
		{
			westBlock = &m_westNeighborForChunk->m_blocksList[ blockIndex - ( CHUNK_DEPTH_Y - 1 ) * CHUNK_DEPTH_Y ];
		}
	}
	
	if( westBlock != nullptr && !g_blockTypeInfo[westBlock->m_blockType.material].isObaque)
	{
		color = CalculateColorForBlock(westBlock);

		if(color <= 0.f)
			color = minLightValue;

		texCoords = g_gameTexture->GetTopLeftTextureCoordsFromFrameIndex(currentBlockInfo.sideTextureIndex);

		tempVertex.m_color.SetColor(color,color,color,1.f);

		if(!g_blockTypeInfo[currentBlock.m_blockType.material].isSpecialBlock)
		{
			tempVertex.m_position = Vector3( temp.x , temp.y + 1.f , temp.z  );
			tempVertex.m_texCoords = Vector2( texCoords.x + m_frameWidth , texCoords.y + m_frameWidth );
			m_vertexList.push_back(tempVertex);	

			tempVertex.m_position = Vector3( temp.x , temp.y + 1.f, temp.z + 1.f  );
			tempVertex.m_texCoords = Vector2( texCoords.x + m_frameWidth, texCoords.y );
			m_vertexList.push_back(tempVertex);

			tempVertex.m_position = Vector3( temp.x + 1.f, temp.y + 1.f, temp.z + 1.f );
			tempVertex.m_texCoords = Vector2( texCoords.x , texCoords.y  );
			m_vertexList.push_back(tempVertex);

			tempVertex.m_position = Vector3( temp.x + 1.f, temp.y + 1.f, temp.z );
			tempVertex.m_texCoords = Vector2(texCoords.x , texCoords.y + m_frameWidth );
			m_vertexList.push_back(tempVertex);
		}
	}

 	//east face
	if( localCoord3DofBlock.y > 0 )
	{
		eastBlock = &m_blocksList[ blockIndex - CHUNK_DEPTH_Y ];
	}
	else if( localCoord3DofBlock.y == 0 )
	{
		if( m_eastNeighborForChunk != nullptr )
		{
			eastBlock = &m_eastNeighborForChunk->m_blocksList[ blockIndex + ( CHUNK_DEPTH_Y - 1) * CHUNK_DEPTH_Y ];
		}
	}

	if( eastBlock != nullptr && !g_blockTypeInfo[eastBlock->m_blockType.material].isObaque )
	{
		color = CalculateColorForBlock(eastBlock);

		if(color <= 0.f)
			color = minLightValue;

		texCoords = g_gameTexture->GetTopLeftTextureCoordsFromFrameIndex(currentBlockInfo.sideTextureIndex);

		tempVertex.m_color.SetColor(color,color,color,1.f);

		if(!g_blockTypeInfo[currentBlock.m_blockType.material].isSpecialBlock)
		{
			tempVertex.m_position = Vector3( temp.x , temp.y , temp.z );
			tempVertex.m_texCoords = Vector2( texCoords.x , texCoords.y  + m_frameWidth );
			m_vertexList.push_back(tempVertex);

			tempVertex.m_position = Vector3( temp.x + 1.f, temp.y , temp.z );
			tempVertex.m_texCoords = Vector2( texCoords.x + m_frameWidth, texCoords.y + m_frameWidth );
			m_vertexList.push_back(tempVertex);

			tempVertex.m_position = Vector3( temp.x + 1.f, temp.y, temp.z + 1.f);
			tempVertex.m_texCoords = Vector2( texCoords.x + m_frameWidth, texCoords.y );
			m_vertexList.push_back(tempVertex);

			tempVertex.m_position = Vector3( temp.x , temp.y , temp.z + 1.f );
			tempVertex.m_texCoords = Vector2( texCoords.x, texCoords.y );
			m_vertexList.push_back(tempVertex);
		}

	}
	//bottom face
	if( localCoord3DofBlock.z > 0)
	{
		bottomBlock = &m_blocksList[ blockIndex - CHUNK_LAYER_SIZE ];
	}
	
	if(bottomBlock != nullptr && !g_blockTypeInfo[bottomBlock->m_blockType.material].isObaque)
	{
		color = CalculateColorForBlock(bottomBlock);

		if(color <= 0.f)
			color = minLightValue;

		tempVertex.m_color.SetColor(color,color,color,1.f);

		texCoords = g_gameTexture->GetTopLeftTextureCoordsFromFrameIndex(currentBlockInfo.bottomTextureIndex);

		if(!g_blockTypeInfo[currentBlock.m_blockType.material].isSpecialBlock)
		{
			tempVertex.m_position = Vector3(  temp.x, temp.y, temp.z );
			tempVertex.m_texCoords = Vector2( texCoords.x + m_frameWidth,texCoords.y );
			m_vertexList.push_back(tempVertex);

			tempVertex.m_position = Vector3(  temp.x , temp.y + 1.f, temp.z );
			tempVertex.m_texCoords = Vector2( texCoords.x , texCoords.y );
			m_vertexList.push_back(tempVertex);

			tempVertex.m_position = Vector3(  temp.x + 1.f, temp.y + 1.f, temp.z );
			tempVertex.m_texCoords = Vector2( texCoords.x , texCoords.y + m_frameWidth );
			m_vertexList.push_back(tempVertex);

			tempVertex.m_position = Vector3(  temp.x + 1.f, temp.y , temp.z );
			tempVertex.m_texCoords = Vector2( texCoords.x + m_frameWidth, texCoords.y + m_frameWidth );
			m_vertexList.push_back(tempVertex);
		}
	}
}

void ChunkBlocks::CreateVertexArray()
{
	Vector3 minLeftBottomBlockCornerCoords;
	int z = 0;
	int y = 0;
	int x = 0;
	int blockIndex = 0;

	m_vertexList.clear();
	for(z = 0; z < CHUNK_HEIGHT; z++)
	{
		for(y = 0; y < CHUNK_DEPTH_Y; y++)
		{
			for(x = 0;x < CHUNK_WIDTH_X; x++)
			{
				blockIndex = x + CHUNK_WIDTH_X * y + ( CHUNK_LAYER_SIZE * z);
				minLeftBottomBlockCornerCoords = Vector3(x,y,z) + m_minWorldCoords;
				AddBlockToVertexArray(minLeftBottomBlockCornerCoords, blockIndex);
			}
		}
	}

	if(m_vboID == 0)
	{
		GLRender::GenerateBuffer( 1, &m_vboID );//glGenBuffers(1,&m_vboID);
		GLRender::GenerateBuffer( 1, &m_secVboID );
	}

	GLRender::BindBuffer( GL_ARRAY_BUFFER, m_vboID );//glBindBuffer( GL_ARRAY_BUFFER, m_vboID );
	GLRender::BufferData(GL_ARRAY_BUFFER, sizeof( Vertex3D ) * m_vertexList.size(), m_vertexList.data(), GL_STATIC_DRAW);//glBufferData( GL_ARRAY_BUFFER, sizeof( Vertex3D ) * m_vertexList.size(), m_vertexList.data(), GL_STATIC_DRAW );
	GLRender::BindBuffer( GL_ARRAY_BUFFER, 0 );//glBindBuffer( GL_ARRAY_BUFFER, 0 );

	GLRender::BindBuffer( GL_ARRAY_BUFFER, m_secVboID );//glBindBuffer( GL_ARRAY_BUFFER, m_vboID );
	GLRender::BufferData(GL_ARRAY_BUFFER, sizeof( Vertex3D ) * m_specialVertexList.size(), m_specialVertexList.data(), GL_STATIC_DRAW);//glBufferData( GL_ARRAY_BUFFER, sizeof( Vertex3D ) * m_vertexList.size(), m_vertexList.data(), GL_STATIC_DRAW );
	GLRender::BindBuffer( GL_ARRAY_BUFFER, 1 );//glBindBuffer( GL_ARRAY_BUFFER, 0 );

	m_numVertexes = m_vertexList.size();
	m_numSpecialVertexes = m_specialVertexList.size();
	m_vertexList.clear();
	vector<Vertex3D>().swap(m_vertexList);
	m_specialVertexList.clear();
	vector<Vertex3D>().swap(m_specialVertexList);

	m_isVboDirty = false;
}

void ChunkBlocks::InitializeLightingForChunk()
{
	//initialize all blocks in chunk
	for( int blockIndex = 0; blockIndex < CHUNK_SIZE_IN_BLOCK; blockIndex++ )
	{
		Block& currentBlock = m_blocksList[ blockIndex ];

		currentBlock.SetLightLevelForBlock(0);
		currentBlock.SetSkyNotVisibleForBlock();
	}

	//loop through column find and mark sky block
	for( int chunkCollumIndex = 0 ; chunkCollumIndex < CHUNK_LAYER_SIZE; chunkCollumIndex++ )
	{
		int blockTopCollumIndex =  CHUNK_SIZE_IN_BLOCK - CHUNK_LAYER_SIZE + chunkCollumIndex ;

		for( int blockIndex = blockTopCollumIndex; blockIndex > 0; blockIndex -= CHUNK_LAYER_SIZE )
		{
			Block& currentBlock = m_blocksList[blockIndex];

			if( g_blockTypeInfo[currentBlock.m_blockType.material].isObaque )
			{
				//	currentBlock.SetLightLevelForBlock(GLOW_STONE_LIGHT_VALUE);
				break;
			}

			currentBlock.SetLightLevelForBlock(m_outdoorLightLevel);//DAY_LIGHT_VALUE);
			currentBlock.SetSkyVisibleForBlock();		
		}
	}

	FindNonSkyBlockNeighborOfSkyBlocksAndAddToToDoList();
	AddAllAirBlocksAtBoundaryToLightList();
}

void ChunkBlocks::AddAllAirBlocksAtBoundaryToLightList()
{
	BlockInfo temp;

	for(int x = 0; x < CHUNK_WIDTH_X; x++)
	{
		for(int z = 0; z < CHUNK_HEIGHT; z++ )
		{
			if(!g_blockTypeInfo[m_blocksList[x + CHUNK_LAYER_SIZE * z].m_blockType.material].isObaque)
			{
				temp.m_blockLocalIndex = x + CHUNK_LAYER_SIZE * z;
				temp.m_chunkCoord = m_worldCoords2D;
				g_listOfBlockToPropagateLight.push(temp);
			}
		}
	}

	for( int x = 0; x <= CHUNK_LAYER_SIZE - CHUNK_WIDTH_X; x += CHUNK_DEPTH_Y )
	{
		for( int z = 0; z < CHUNK_HEIGHT; z++ )
		{
			if(!g_blockTypeInfo[m_blocksList[x + CHUNK_LAYER_SIZE * z].m_blockType.material].isObaque)
			{
				temp.m_blockLocalIndex = x + CHUNK_LAYER_SIZE * z;
				temp.m_chunkCoord = m_worldCoords2D;
				g_listOfBlockToPropagateLight.push(temp);
			}
		}
	}

	for( int x = CHUNK_WIDTH_X - 1 ; x <= CHUNK_LAYER_SIZE - 1; x += CHUNK_DEPTH_Y )
	{
		for( int z = 0; z < CHUNK_HEIGHT; z++ )
		{
			if(!g_blockTypeInfo[m_blocksList[x + CHUNK_LAYER_SIZE * z].m_blockType.material].isObaque)
			{
				temp.m_blockLocalIndex = x + CHUNK_LAYER_SIZE * z;
				temp.m_chunkCoord = m_worldCoords2D;
				g_listOfBlockToPropagateLight.push(temp);
			}
		}
	}

	for( int x = CHUNK_LAYER_SIZE - CHUNK_WIDTH_X ; x <= CHUNK_LAYER_SIZE - 1; x++ )
	{
		for( int z = 0; z < CHUNK_HEIGHT; z++ )
		{
			if(!g_blockTypeInfo[m_blocksList[x + CHUNK_LAYER_SIZE * z].m_blockType.material].isObaque)
			{
				temp.m_blockLocalIndex = x + CHUNK_LAYER_SIZE * z;
				temp.m_chunkCoord = m_worldCoords2D;
				g_listOfBlockToPropagateLight.push(temp);
			}
		}
	}
}

void ChunkBlocks::FindNonSkyBlockNeighborOfSkyBlocksAndAddToToDoList()
{
	Vector3i localCoord3DofBlock;
	Block* topNeighborBlock = nullptr;
	Block* bottomNeighborBlock = nullptr;
	Block* northNeighborBlock = nullptr;
	Block* southNeighborBlock = nullptr;
	Block* eastNeighborBlock = nullptr;
	Block* westNeighborBlock = nullptr;

	BlockInfo topNeighborBlockInfo ;
	BlockInfo bottomNeighborBlockInfo ;
	BlockInfo northNeighborBlockInfo ;
	BlockInfo southNeighborBlockInfo ;
	BlockInfo eastNeighborBlockInfo ;
	BlockInfo westNeighborBlockInfo ;  

	for(int blockIndex = 0; blockIndex < CHUNK_SIZE_IN_BLOCK; blockIndex++)
	{
		topNeighborBlock = nullptr;
		bottomNeighborBlock = nullptr;
		northNeighborBlock = nullptr;
		southNeighborBlock = nullptr;
		eastNeighborBlock = nullptr;
		westNeighborBlock = nullptr;

		if(m_blocksList[blockIndex].IsSkyBlockBitSet() && ( !g_blockTypeInfo[m_blocksList[blockIndex].m_blockType.material].isObaque ) )
		{
			localCoord3DofBlock = GetBlockLocalCoord3DFromBlockIndex(blockIndex);
			
			//if block is not in top layer of chunk
			if( localCoord3DofBlock.z < ( CHUNK_HEIGHT - 1 ) )
			{
				topNeighborBlock = &m_blocksList[ blockIndex + CHUNK_LAYER_SIZE ];
				topNeighborBlockInfo.m_blockLocalIndex = blockIndex + CHUNK_LAYER_SIZE;
				topNeighborBlockInfo.m_chunkCoord = m_worldCoords2D;
			}

			//if block is not in bottom layer of chunk
			if( localCoord3DofBlock.z > 0)
			{
				bottomNeighborBlock = &m_blocksList[ blockIndex - CHUNK_LAYER_SIZE ];
				bottomNeighborBlockInfo.m_blockLocalIndex = blockIndex - CHUNK_LAYER_SIZE;
				bottomNeighborBlockInfo.m_chunkCoord = m_worldCoords2D;
			}

			//if block is not on north border
			if( localCoord3DofBlock.x < ( CHUNK_WIDTH_X - 1) )
			{
				northNeighborBlock = &m_blocksList[ blockIndex + 1 ];
				northNeighborBlockInfo.m_blockLocalIndex = blockIndex + 1;
				northNeighborBlockInfo.m_chunkCoord = m_worldCoords2D;
			}
			else if( localCoord3DofBlock.x == ( CHUNK_WIDTH_X - 1 ) )
			{
				if(m_northNeighborForChunk != nullptr)
				{
					northNeighborBlock = &m_northNeighborForChunk->m_blocksList[ blockIndex - ( CHUNK_WIDTH_X - 1 ) ];
					northNeighborBlockInfo.m_blockLocalIndex = blockIndex - ( CHUNK_WIDTH_X - 1 ) ;
					northNeighborBlockInfo.m_chunkCoord = m_northNeighborForChunk->m_worldCoords2D;
				}
			}

			//if block is not on south border
			if( localCoord3DofBlock.x > 0 )
			{
				southNeighborBlock = &m_blocksList[ blockIndex - 1 ];
				southNeighborBlockInfo.m_blockLocalIndex = blockIndex - 1  ;
				southNeighborBlockInfo.m_chunkCoord = m_worldCoords2D;
			}
			else if( localCoord3DofBlock.x == 0 )
			{
				if(m_southNeighborForChunk != nullptr)
				{
					southNeighborBlock = &m_southNeighborForChunk->m_blocksList[ blockIndex + ( CHUNK_WIDTH_X - 1 ) ];
					southNeighborBlockInfo.m_blockLocalIndex = blockIndex + ( CHUNK_WIDTH_X - 1 )  ;
					southNeighborBlockInfo.m_chunkCoord = m_southNeighborForChunk->m_worldCoords2D ;
				}
			}

			//if block not on west border
			if( localCoord3DofBlock.y < ( CHUNK_DEPTH_Y - 1 ))
			{
				westNeighborBlock = &m_blocksList[ blockIndex + CHUNK_DEPTH_Y ];
				westNeighborBlockInfo.m_blockLocalIndex = blockIndex + CHUNK_DEPTH_Y ;
				westNeighborBlockInfo.m_chunkCoord = m_worldCoords2D;
			}
			else if( localCoord3DofBlock.y == ( CHUNK_DEPTH_Y - 1 ) )
			{
				if( m_westNeighborForChunk != nullptr )
				{
					westNeighborBlock = &m_westNeighborForChunk->m_blocksList[ blockIndex - ( CHUNK_DEPTH_Y - 1) * CHUNK_DEPTH_Y ];
					westNeighborBlockInfo.m_blockLocalIndex = blockIndex - ( CHUNK_DEPTH_Y - 1 ) * CHUNK_DEPTH_Y ;
					westNeighborBlockInfo.m_chunkCoord = m_westNeighborForChunk->m_worldCoords2D ;
				}
			}

			//if block not on east border
			if( localCoord3DofBlock.y > 0 )
			{
				eastNeighborBlock = &m_blocksList[ blockIndex - CHUNK_DEPTH_Y ];
				eastNeighborBlockInfo.m_blockLocalIndex = blockIndex - CHUNK_DEPTH_Y ;
				eastNeighborBlockInfo.m_chunkCoord = m_worldCoords2D;
			}
			else if( localCoord3DofBlock.y == 0)
			{
				if( m_eastNeighborForChunk != nullptr )
				{
					eastNeighborBlock = &m_eastNeighborForChunk->m_blocksList[ blockIndex + ( CHUNK_DEPTH_Y - 1 ) * CHUNK_DEPTH_Y ];
					eastNeighborBlockInfo.m_blockLocalIndex = blockIndex + ( CHUNK_WIDTH_X - 1 ) * CHUNK_DEPTH_Y ;
					eastNeighborBlockInfo.m_chunkCoord = m_eastNeighborForChunk->m_worldCoords2D ;
				}
			}

			AddNonSkyBlockNeighBorToToLightList( topNeighborBlock , topNeighborBlockInfo );
			AddNonSkyBlockNeighBorToToLightList( bottomNeighborBlock , bottomNeighborBlockInfo );
	  		AddNonSkyBlockNeighBorToToLightList( eastNeighborBlock , eastNeighborBlockInfo );
	  		AddNonSkyBlockNeighBorToToLightList( westNeighborBlock , westNeighborBlockInfo );
	  		AddNonSkyBlockNeighBorToToLightList( northNeighborBlock , northNeighborBlockInfo );
	  		AddNonSkyBlockNeighBorToToLightList( southNeighborBlock , southNeighborBlockInfo );
		}
	}
}

void ChunkBlocks::AddNonSkyBlockNeighBorToToLightList( Block* blockToAdd, BlockInfo blockInfo )
{
	if(blockToAdd != nullptr)
	{
		if( (!blockToAdd->IsSkyBlockBitSet() && !g_blockTypeInfo[blockToAdd->m_blockType.material].isObaque ) || blockToAdd->m_blockType.material == GLOW_STONE  )
		{
			g_listOfBlockToPropagateLight.push(blockInfo);
		}
	}
}



float ChunkBlocks::CalculateColorForBlock(Block* block)
{
	float color;

	if(block != nullptr)
		color = (float)block->GetLightLevelForBlock()/15.f;
	else
		color = 0.f;

	return color * color;

}

Vector3i ChunkBlocks::GetBlockLocalCoord3DFromBlockIndex(int blockIndex)
{
	Vector3i blockLocalCoords3D;
	blockLocalCoords3D.z = blockIndex >> 8;//(int)blockIndex / CHUNK_LAYER_SIZE;
	blockLocalCoords3D.y = (blockIndex - blockLocalCoords3D.z * CHUNK_LAYER_SIZE) >> 4;//(int)(blockIndex - blockLocalCoords3D.z * CHUNK_LAYER_SIZE)/CHUNK_WIDTH;
	blockLocalCoords3D.x = (blockIndex - blockLocalCoords3D.z * CHUNK_LAYER_SIZE) & BOTTOM_4_BITS_MASK; // %CHUNK_WIDTH;

	return blockLocalCoords3D;
}

int ChunkBlocks::GetBlockIndexFromLocalCoord3D(Vector3i localCoord3D)
{
	return localCoord3D.x + localCoord3D.y * CHUNK_DEPTH_Y + localCoord3D.z * CHUNK_LAYER_SIZE;
}

void ChunkBlocks::ProcessGrassGrowing()
{
	Block* topNeighborBlock = nullptr;
	Block* bottomNeighborBlock = nullptr;
	Block* northNeighborBlock = nullptr;
	Block* southNeighborBlock = nullptr;
	Block* eastNeighborBlock = nullptr;
	Block* westNeighborBlock = nullptr;

	int blockIndex = MathUtilities::GetRandomNumber( 10 * CHUNK_LAYER_SIZE,CHUNK_SIZE_IN_BLOCK - 10 * CHUNK_LAYER_SIZE);
	Vector3i localCoord3DofBlock;

	if(m_blocksList[blockIndex].m_blockType.material == GROUND)
	{
		localCoord3DofBlock = GetBlockLocalCoord3DFromBlockIndex(blockIndex);

		//if block is not in top layer of chunk
		if( localCoord3DofBlock.z < ( CHUNK_HEIGHT - 1 ) )
		{
			topNeighborBlock = &m_blocksList[ blockIndex + CHUNK_LAYER_SIZE ];
		}

		//if block is not in bottom layer of chunk
		if( localCoord3DofBlock.z > 0)
		{
			bottomNeighborBlock = &m_blocksList[ blockIndex - CHUNK_LAYER_SIZE ];
		}

		//if block is not on north border
		if( localCoord3DofBlock.x < ( CHUNK_WIDTH_X - 1) )
		{
			northNeighborBlock = &m_blocksList[ blockIndex + 1 ];
		}
		else if( localCoord3DofBlock.x == ( CHUNK_WIDTH_X - 1 ) )
		{
			if(m_northNeighborForChunk != nullptr)
			{
				northNeighborBlock = &m_northNeighborForChunk->m_blocksList[ blockIndex - ( CHUNK_WIDTH_X - 1 ) ];
			}
		}

		//if block is not on south border
		if( localCoord3DofBlock.x > 0 )
		{
			southNeighborBlock = &m_blocksList[ blockIndex - 1 ];
		}
		else if( localCoord3DofBlock.x == 0 )
		{
			if(m_southNeighborForChunk != nullptr)
			{
				southNeighborBlock = &m_southNeighborForChunk->m_blocksList[ blockIndex + ( CHUNK_WIDTH_X - 1 ) ];
			}
		}

		//if block not on west border
		if( localCoord3DofBlock.y < ( CHUNK_DEPTH_Y - 1 ))
		{
			westNeighborBlock = &m_blocksList[ blockIndex + CHUNK_DEPTH_Y ];
		}
		else if( localCoord3DofBlock.y == ( CHUNK_DEPTH_Y - 1 ) )
		{
			if( m_westNeighborForChunk != nullptr )
			{
				westNeighborBlock = &m_westNeighborForChunk->m_blocksList[ blockIndex - ( CHUNK_DEPTH_Y - 1) * CHUNK_DEPTH_Y ];
			}
		}

		//if block not on east border
		if( localCoord3DofBlock.y > 0 )
		{
			eastNeighborBlock = &m_blocksList[ blockIndex - CHUNK_DEPTH_Y ];
		}
		else if( localCoord3DofBlock.y == 0)
		{
			if( m_eastNeighborForChunk != nullptr )
			{
				eastNeighborBlock = &m_eastNeighborForChunk->m_blocksList[ blockIndex + ( CHUNK_DEPTH_Y - 1 ) * CHUNK_DEPTH_Y ];
			}
		}

		if( ( topNeighborBlock != nullptr && ( topNeighborBlock->m_blockType.material == GRASS || topNeighborBlock->IsSkyBlockBitSet() ) )
			&& (( bottomNeighborBlock != nullptr && bottomNeighborBlock->m_blockType.material == GRASS )
			||	( westNeighborBlock != nullptr && westNeighborBlock->m_blockType.material == GRASS )
			||	( eastNeighborBlock != nullptr && eastNeighborBlock->m_blockType.material == GRASS )
			||	( northNeighborBlock != nullptr && northNeighborBlock->m_blockType.material == GRASS )
			||	( southNeighborBlock != nullptr && southNeighborBlock->m_blockType.material == GRASS ) ) )
		{
			m_blocksList[blockIndex].m_blockType.material = GRASS;
			m_isVboDirty = true;
		}
	}
	else if( m_blocksList[blockIndex].m_blockType.material == GRASS 
		  && m_blocksList[blockIndex + CHUNK_LAYER_SIZE].IsSkyBlockBitSet()
		  && g_globalOutDoorLightLevel > 8)
	{
		m_blocksList[blockIndex + CHUNK_LAYER_SIZE].m_blockType.material = GROW_GRASS1;
		m_isVboDirty = true;
	}
}