#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "World.hpp"

using namespace std;

World* theWorld = nullptr;
bool IsKeyDownKeyboard[256];
bool IsKeyDownLastFrame[256];
set<Vector3> g_debugPoints;
unsigned char g_globalOutDoorLightLevel;
AudioSystem g_audioSystem;

World::World()
{
	 Initialize();	 
}

World::~World()
{
	delete m_sunTexture;
	delete m_moonTexture;
	delete m_skyBoxTexture;
}

void World::Initialize()
{
	for(int i = 0; i < 256; i++)
	{
		IsKeyDownKeyboard[i] = false;
		IsKeyDownLastFrame[i] = false;
	}

	m_camera3D.m_cameraPosition = m_player.m_eyePosition;

	m_hitInfo.hitFace = NONE;
	m_rightMouseDown = false;
	m_leftMouseDown = false;
	m_skyBoxTexture = new Texture();
	m_skyBoxTexture = Texture::CreateOrGetTexture("skybox.png");
	m_sunTexture = new Texture();
	m_sunTexture = Texture::CreateOrGetTexture("sun1.jpg");
	m_moonTexture = new Texture();
	m_moonTexture = Texture::CreateOrGetTexture("moon.jpg");
	m_skyBoxTextWidth = 1.f/4.f;
	m_skyBoxTextHeight = 1.f/3.f;
	m_sunPos.SetXY(-150.f,-120.f);
	m_moonPos.SetXY(150.f,-120.f);
	m_sunMoonRotateRate = 0.f;
	g_debugPoints.insert(Vector3(0.f,0.f,100.f));
	m_jumpForce = 0.f;
	m_buoyancyForce = 10.01f;
	m_currentBlockType = STONE;
	g_globalOutDoorLightLevel = NIGHT_LIGHT_VALUE;
	m_timeOfDay = 0.f;
	g_audioSystem.Initialize();
	g_audioSystem.PlaySoundByName("Sound/background/calm2.mp3",1,true);
	m_startDay = true;
	m_endDay = false;
	m_doneCheckingBlockTime = true;
	m_chunkCount = 0;
	m_lastGlobalOutDoorLightLevel = g_globalOutDoorLightLevel;
}

void World::RenderSkyBox()
{
	GLRender::Disable( GL_DEPTH_TEST );//glDisable(GL_DEPTH_TEST);
	GLRender::Enable( GL_TEXTURE_2D );//glEnable(GL_TEXTURE_2D);
	GLRender::BindTexture( GL_TEXTURE_2D, m_skyBoxTexture->m_openglTextureID );//glBindTexture(GL_TEXTURE_2D,m_skyBoxTexture->m_openglTextureID);
	GLRender::PushMatrix();//glPushMatrix();
	GLRender::Translatef( m_camera3D.m_cameraPosition.x,m_camera3D.m_cameraPosition.y, 0.f );//glTranslatef(m_camera3D.m_cameraPosition.x,m_camera3D.m_cameraPosition.y, 0.f);
	GLRender::Scalef(2.f,2.f,2.f);//glScalef(2.f,2.f,2.f);
	//GLRender::Color4f(1.f,1.f,1.f,1.f);//glColor4f(1.f,1.f,1.f,1.f);
	GLRender::Color4f(0.5f + m_timeOfDay/56.f,0.5f + m_timeOfDay/56.f,0.5f + m_timeOfDay/56.f,1.f);
	GLRender::BeginDraw( GL_QUADS );//glBegin(GL_QUADS);
	{
		//south
		GLRender::TexCoord2d(  4 * m_skyBoxTextWidth ,  m_skyBoxTextHeight );//glTexCoord2d( 4 * m_skyBoxTextWidth ,  m_skyBoxTextHeight );
		GLRender::Vertex3f( -100.f, 100.f,100.f );//glVertex3f(-100.f, 100.f,100.f);
		GLRender::TexCoord2d( 3 * m_skyBoxTextWidth ,  m_skyBoxTextHeight );//glTexCoord2d( 3 * m_skyBoxTextWidth ,  m_skyBoxTextHeight );
		GLRender::Vertex3f( -100.f, -100.f,100.f );//glVertex3f(-100.f, -100.f,100.f);
		GLRender::TexCoord2d( 3 * m_skyBoxTextWidth ,  2 * m_skyBoxTextHeight );//glTexCoord2d( 3 * m_skyBoxTextWidth ,  2 * m_skyBoxTextHeight );
		GLRender::Vertex3f( -100.f,-100.f,0.f );//glVertex3f(-100.f,-100.f,0.f);
		GLRender::TexCoord2d( 4 * m_skyBoxTextWidth ,  2 * m_skyBoxTextHeight );//glTexCoord2d( 4 * m_skyBoxTextWidth ,  2 * m_skyBoxTextHeight );
		GLRender::Vertex3f( -100.f,100.f,0.f );//glVertex3f(-100.f,100.f,0.f);
		
		////north
		GLRender::TexCoord2d( m_skyBoxTextWidth ,  m_skyBoxTextHeight );//glTexCoord2d( m_skyBoxTextWidth ,  m_skyBoxTextHeight );
		GLRender::Vertex3f( 100.f, 100.f,100.f);//glVertex3f( 100.f, 100.f,100.f);
		GLRender::TexCoord2d( m_skyBoxTextWidth , 2 * m_skyBoxTextHeight );//glTexCoord2d( m_skyBoxTextWidth , 2 * m_skyBoxTextHeight );
		GLRender::Vertex3f( 100.f, 100.f,0.f );//glVertex3f( 100.f, 100.f,0.f);
		GLRender::TexCoord2d( 2 * m_skyBoxTextWidth , 2 * m_skyBoxTextHeight );//glTexCoord2d( 2 * m_skyBoxTextWidth , 2 * m_skyBoxTextHeight );
		GLRender::Vertex3f( 100.f,-100.f,0.f);//glVertex3f( 100.f,-100.f,0.f);
		GLRender::TexCoord2d( 2 * m_skyBoxTextWidth ,  m_skyBoxTextHeight );//glTexCoord2d( 2 * m_skyBoxTextWidth ,  m_skyBoxTextHeight );
		GLRender::Vertex3f(100.f, -100.f, 100.f); //glVertex3f( 100.f, -100.f, 100.f);
 		
 		////west
		GLRender::TexCoord2d(  0.f , 2 * m_skyBoxTextHeight );//glTexCoord2d( 0.f , 2 * m_skyBoxTextHeight );
		GLRender::Vertex3f(-100.f, 100.f,0.f);//glVertex3f(-100.f, 100.f,0.f);
		GLRender::TexCoord2d( m_skyBoxTextWidth , 2 * m_skyBoxTextHeight );//glTexCoord2d( m_skyBoxTextWidth , 2 * m_skyBoxTextHeight );
		GLRender::Vertex3f( 100.f, 100.f,0.f);//glVertex3f( 100.f, 100.f,0.f);
		GLRender::TexCoord2d( m_skyBoxTextWidth , m_skyBoxTextHeight );//glTexCoord2d( m_skyBoxTextWidth , m_skyBoxTextHeight );
		GLRender::Vertex3f( 100.f, 100.f,100.f);//glVertex3f( 100.f, 100.f,100.f);
		GLRender::TexCoord2d( 0 , m_skyBoxTextHeight );//glTexCoord2d( 0 , m_skyBoxTextHeight );
		GLRender::Vertex3f( -100.f, 100.f,100.f);//glVertex3f(-100.f, 100.f,100.f);

 		////east
		GLRender::TexCoord2d( 3 * m_skyBoxTextWidth , m_skyBoxTextHeight );//glTexCoord2d( 3 * m_skyBoxTextWidth , m_skyBoxTextHeight );
		GLRender::Vertex3f( -100.f,-100.f,100.f);//glVertex3f(-100.f,-100.f,100.f);
		GLRender::TexCoord2d( 2 * m_skyBoxTextWidth , 1 * m_skyBoxTextHeight);//glTexCoord2d( 2 * m_skyBoxTextWidth , 1 * m_skyBoxTextHeight);
		GLRender::Vertex3f( 100.f,-100.f,100.f);//glVertex3f(100.f,-100.f,100.f);
		GLRender::TexCoord2d( 2 * m_skyBoxTextWidth , 2 * m_skyBoxTextHeight);//glTexCoord2d( 2 * m_skyBoxTextWidth , 2 * m_skyBoxTextHeight);
		GLRender::Vertex3f( 100.f,-100.f,0.f);//glVertex3f( 100.f,-100.f,0.f);
		GLRender::TexCoord2d( 3 * m_skyBoxTextWidth , 2 * m_skyBoxTextHeight);//glTexCoord2d( 3 * m_skyBoxTextWidth , 2 * m_skyBoxTextHeight);
		GLRender::Vertex3f( -100.f,-100.f,0.f);//glVertex3f( -100.f,-100.f,0.f);

		////top
		GLRender::TexCoord2d( m_skyBoxTextWidth , 0.f);//glTexCoord2d( m_skyBoxTextWidth , 0.f);
		GLRender::Vertex3f( -100.f, 100.f,100.f);//glVertex3f(-100.f, 100.f,100.f);
		GLRender::TexCoord2d( m_skyBoxTextWidth , m_skyBoxTextHeight);//glTexCoord2d( m_skyBoxTextWidth , m_skyBoxTextHeight);
		GLRender::Vertex3f( 100.f, 100.f,100.f);//glVertex3f( 100.f, 100.f,100.f);
		GLRender::TexCoord2d( m_skyBoxTextWidth + m_skyBoxTextWidth , m_skyBoxTextHeight);//glTexCoord2d( m_skyBoxTextWidth + m_skyBoxTextWidth , m_skyBoxTextHeight);
		GLRender::Vertex3f( 100.f,-100.f,100.f);//glVertex3f( 100.f,-100.f,100.f);
		GLRender::TexCoord2d( m_skyBoxTextWidth + m_skyBoxTextWidth , 0.f);//glTexCoord2d( m_skyBoxTextWidth + m_skyBoxTextWidth , 0.f);
		GLRender::Vertex3f(-100.f,-100.f,100.f);//glVertex3f(-100.f,-100.f,100.f);
		
	}
	GLRender::EndDraw();//glEnd();
	GLRender::PopMatrix();//glPopMatrix();
	GLRender::Enable(GL_DEPTH_TEST);//glEnable(GL_DEPTH_TEST);
}

void World::Update(float elapsedTime)
{
	UpdateCameraFromKeyboardAndMouse(m_camera3D,elapsedTime);
	m_map.SetCameraPosition(m_camera3D.m_cameraPosition);
	m_map.SetCameraForwardVector(m_cameraForwardVector);
	m_hitInfo = RayTrace();
	m_sunMoonRotateRate += .02f;
	m_player.Update(elapsedTime);
	m_map.Update();
	CalculateMinMaxXYOfSeaSurface();
	ProcessToLightList();
	UpdateWorldTimeAndLightLevel(elapsedTime);
	g_audioSystem.Update();
	unsigned int lightListSize = g_listOfBlockToPropagateLight.size();

	DebuggerPrintf( "Light list size: %i.\n", lightListSize );
}

void World::ProcessToLightList()
{
	Block* topNeighborBlock = nullptr ;
	Block* bottomNeighborBlock = nullptr ;
	Block* northNeighborBlock = nullptr ;
	Block* southNeighborBlock = nullptr;
	Block* eastNeighborBlock = nullptr;
	Block* westNeighborBlock = nullptr;  
	Block* block = nullptr;

	BlockInfo topNeighborBlockInfo ;
	BlockInfo bottomNeighborBlockInfo ;
	BlockInfo northNeighborBlockInfo ;
	BlockInfo southNeighborBlockInfo ;
	BlockInfo eastNeighborBlockInfo ;
	BlockInfo westNeighborBlockInfo ;  

	Vector3i currentBlockLocalCoord3D;
	ChunkBlocks* currentChunk = nullptr;

	while( g_listOfBlockToPropagateLight.size() > 0 )
	{
		topNeighborBlock = nullptr ;
		bottomNeighborBlock = nullptr;
		northNeighborBlock = nullptr;
		southNeighborBlock = nullptr;
		eastNeighborBlock = nullptr;
		westNeighborBlock = nullptr;

		BlockInfo& currentBlock = g_listOfBlockToPropagateLight.front();
		g_listOfBlockToPropagateLight.pop();
		currentBlockLocalCoord3D = GetBlockLocalCoord3DFromBlockIndex(currentBlock.m_blockLocalIndex);

		if(g_mapChunk.find(currentBlock.m_chunkCoord) == g_mapChunk.end())
			break;

		currentChunk = g_mapChunk[currentBlock.m_chunkCoord];
		block = &currentChunk->m_blocksList[currentBlock.m_blockLocalIndex];

		//find all neighbors which are air
		//top neighbor
		if( currentBlockLocalCoord3D.z < ( CHUNK_HEIGHT - 1 ) )
		{
			topNeighborBlock = &currentChunk->m_blocksList[currentBlock.m_blockLocalIndex + CHUNK_LAYER_SIZE];
			topNeighborBlockInfo.m_blockLocalIndex = currentBlock.m_blockLocalIndex + CHUNK_LAYER_SIZE;
			topNeighborBlockInfo.m_chunkCoord = currentChunk->m_worldCoords2D;
		}

		//bottom neighbor
		if( currentBlockLocalCoord3D.z > 0 )
		{
			bottomNeighborBlock = &currentChunk->m_blocksList[currentBlock.m_blockLocalIndex - CHUNK_LAYER_SIZE];
			bottomNeighborBlockInfo.m_blockLocalIndex = currentBlock.m_blockLocalIndex - CHUNK_LAYER_SIZE;
			bottomNeighborBlockInfo.m_chunkCoord = currentChunk->m_worldCoords2D;
		}

		//east neighbor
		if( currentBlockLocalCoord3D.y > 0 )
		{
			eastNeighborBlock = &currentChunk->m_blocksList[ currentBlock.m_blockLocalIndex - CHUNK_DEPTH_Y ];
			eastNeighborBlockInfo.m_blockLocalIndex = currentBlock.m_blockLocalIndex - CHUNK_DEPTH_Y;
			eastNeighborBlockInfo.m_chunkCoord = currentChunk->m_worldCoords2D;
		}
		else if( currentBlockLocalCoord3D.y == 0)
		{
			if(currentChunk->m_eastNeighborForChunk != nullptr)
			{
				eastNeighborBlock = &currentChunk->m_eastNeighborForChunk->m_blocksList[ currentBlock.m_blockLocalIndex + (CHUNK_DEPTH_Y - 1) * CHUNK_DEPTH_Y ];
				eastNeighborBlockInfo.m_blockLocalIndex = currentBlock.m_blockLocalIndex + ( CHUNK_DEPTH_Y - 1 ) * CHUNK_DEPTH_Y ;
				eastNeighborBlockInfo.m_chunkCoord = currentChunk->m_eastNeighborForChunk->m_worldCoords2D;
			}
		}

		//west neighbor
		if( currentBlockLocalCoord3D.y < CHUNK_DEPTH_Y - 1 )
		{
			westNeighborBlock = &currentChunk->m_blocksList[ currentBlock.m_blockLocalIndex + CHUNK_DEPTH_Y ];
			westNeighborBlockInfo.m_blockLocalIndex = currentBlock.m_blockLocalIndex + CHUNK_DEPTH_Y ;
			westNeighborBlockInfo.m_chunkCoord = currentChunk->m_worldCoords2D;
		}
		else if( currentBlockLocalCoord3D.y == CHUNK_DEPTH_Y - 1)
		{
			if(currentChunk->m_westNeighborForChunk != nullptr)
			{
				westNeighborBlock = &currentChunk->m_westNeighborForChunk->m_blocksList[ currentBlock.m_blockLocalIndex - (CHUNK_DEPTH_Y - 1) * CHUNK_DEPTH_Y  ];
				westNeighborBlockInfo.m_blockLocalIndex = currentBlock.m_blockLocalIndex - (CHUNK_DEPTH_Y - 1) * CHUNK_DEPTH_Y   ;
				westNeighborBlockInfo.m_chunkCoord = currentChunk->m_westNeighborForChunk->m_worldCoords2D ;
			}
		}

		//north neighbor
		if( currentBlockLocalCoord3D.x  < ( CHUNK_WIDTH_X - 1 ) )
		{
			northNeighborBlock = &currentChunk->m_blocksList[ currentBlock.m_blockLocalIndex + 1 ];
			northNeighborBlockInfo.m_blockLocalIndex = currentBlock.m_blockLocalIndex + 1 ;
			northNeighborBlockInfo.m_chunkCoord = currentChunk->m_worldCoords2D;
		}
		else if( currentBlockLocalCoord3D.x == ( CHUNK_WIDTH_X - 1 ))
		{
			if(currentChunk->m_northNeighborForChunk != nullptr)
			{
				northNeighborBlock = &currentChunk->m_northNeighborForChunk->m_blocksList[ currentBlock.m_blockLocalIndex - (CHUNK_WIDTH_X - 1) ];
				northNeighborBlockInfo.m_blockLocalIndex = currentBlock.m_blockLocalIndex - ( CHUNK_WIDTH_X - 1 )  ;
				northNeighborBlockInfo.m_chunkCoord = currentChunk->m_northNeighborForChunk->m_worldCoords2D ;
			}
		}

		//south neighbor
		if( currentBlockLocalCoord3D.x  > 0 )
		{
			southNeighborBlock = &currentChunk->m_blocksList[ currentBlock.m_blockLocalIndex - 1 ];
			southNeighborBlockInfo.m_blockLocalIndex = currentBlock.m_blockLocalIndex  - 1 ;
			southNeighborBlockInfo.m_chunkCoord = currentChunk->m_worldCoords2D;
		}
		else if( currentBlockLocalCoord3D.x == 0)
		{
			if(currentChunk->m_southNeighborForChunk != nullptr)
			{
				southNeighborBlock = &currentChunk->m_southNeighborForChunk->m_blocksList[ currentBlock.m_blockLocalIndex + (CHUNK_WIDTH_X - 1) ];
				southNeighborBlockInfo.m_blockLocalIndex = currentBlock.m_blockLocalIndex + ( CHUNK_WIDTH_X - 1 )  ;
				southNeighborBlockInfo.m_chunkCoord = currentChunk->m_southNeighborForChunk->m_worldCoords2D ;
			}
		}

		//find brightest block
		unsigned char brightList[6];
		unsigned char brightestValue;

		if(topNeighborBlock != nullptr)
			brightList[0] = topNeighborBlock->GetLightLevelForBlock();
		if(bottomNeighborBlock != nullptr)
			brightList[1] = bottomNeighborBlock->GetLightLevelForBlock();
		if(eastNeighborBlock != nullptr)
			brightList[2] = eastNeighborBlock->GetLightLevelForBlock();
		if(westNeighborBlock != nullptr)
			brightList[3] = westNeighborBlock->GetLightLevelForBlock();
		if(northNeighborBlock != nullptr)
			brightList[4] = northNeighborBlock->GetLightLevelForBlock();
		if(southNeighborBlock != nullptr)
			brightList[5] = southNeighborBlock->GetLightLevelForBlock();

		//brightestValue = -1;
		brightestValue = 0;
		unsigned char correctLightValue = 0;

		if( block->IsSkyBlockBitSet() )
		{
			correctLightValue = currentChunk->m_outdoorLightLevel;
		}

		for(int i = 0; i < 6; i++)
		{
			if(brightList[i] > brightestValue)
				brightestValue =  brightList[i] ;
		}

		if( brightestValue > correctLightValue + 1 )
			correctLightValue = brightestValue - 1;

		if( block->m_blockType.material == GLOW_STONE )
		{
			if( GLOW_STONE_LIGHT_VALUE > currentChunk->m_outdoorLightLevel )
				correctLightValue = GLOW_STONE_LIGHT_VALUE;
			else
				correctLightValue = currentChunk->m_outdoorLightLevel;
		}

		if( correctLightValue < 0 )
			correctLightValue = 0;

		if( block->GetLightLevelForBlock() != correctLightValue )
		{
			currentChunk->m_isVboDirty = true;

			if( currentChunk->m_southNeighborForChunk )
				currentChunk->m_southNeighborForChunk->m_isVboDirty = true;

			if( currentChunk->m_northNeighborForChunk )
				currentChunk->m_northNeighborForChunk->m_isVboDirty = true;

			if( currentChunk->m_eastNeighborForChunk )
				currentChunk->m_eastNeighborForChunk->m_isVboDirty = true;

			if( currentChunk->m_westNeighborForChunk )
				currentChunk->m_westNeighborForChunk->m_isVboDirty = true;

			block->SetLightLevelForBlock( correctLightValue );
			//add neighbor to list
			if( topNeighborBlock != nullptr)
			{
				if( !g_blockTypeInfo[topNeighborBlock->m_blockType.material].isObaque || topNeighborBlock->m_blockType.material == GLOW_STONE )
				{
					g_listOfBlockToPropagateLight.push(topNeighborBlockInfo);
				}
			}

			if( bottomNeighborBlock != nullptr )
			{
				if(!g_blockTypeInfo[bottomNeighborBlock->m_blockType.material].isObaque || bottomNeighborBlock->m_blockType.material == GLOW_STONE)
				{
					g_listOfBlockToPropagateLight.push(bottomNeighborBlockInfo);
				}
			}

			if( eastNeighborBlock != nullptr )
			{
				if(!g_blockTypeInfo[eastNeighborBlock->m_blockType.material].isObaque || eastNeighborBlock->m_blockType.material == GLOW_STONE )
				{
					g_listOfBlockToPropagateLight.push(eastNeighborBlockInfo);
				}
			}

			if( westNeighborBlock != nullptr)
			{
				if( !g_blockTypeInfo[westNeighborBlock->m_blockType.material].isObaque || westNeighborBlock->m_blockType.material == GLOW_STONE ) 
				{
					g_listOfBlockToPropagateLight.push(westNeighborBlockInfo);
				}
			}

			if(northNeighborBlock != nullptr)
			{
				if(!g_blockTypeInfo[northNeighborBlock->m_blockType.material].isObaque || northNeighborBlock->m_blockType.material == GLOW_STONE )
				{
					g_listOfBlockToPropagateLight.push(northNeighborBlockInfo);
				}
			}

			if(southNeighborBlock != nullptr)
			{
				if(!g_blockTypeInfo[southNeighborBlock->m_blockType.material].isObaque || southNeighborBlock->m_blockType.material == GLOW_STONE )
				{
					g_listOfBlockToPropagateLight.push(southNeighborBlockInfo);
				}
			}
		}
	}
}

void World::RenderSun()
{
	GLRender::Disable(GL_DEPTH_TEST);//glDisable(GL_DEPTH_TEST);
	GLRender::Enable(GL_TEXTURE_2D);//glEnable(GL_TEXTURE_2D);
	GLRender::Enable(GL_BLEND);//glEnable (GL_BLEND);
	GLRender::BlendFunc( GL_SRC_ALPHA, GL_ONE);//glBlendFunc (GL_SRC_ALPHA,GL_ONE);
	GLRender::BindTexture( GL_TEXTURE_2D, m_sunTexture->m_openglTextureID );//glBindTexture(GL_TEXTURE_2D,m_sunTexture->m_openglTextureID);
	GLRender::PushMatrix();//glPushMatrix();
	GLRender::Rotatef(m_sunMoonRotateRate * 0.5f,0.f,1.f,0.f );//glRotatef(m_sunMoonRotateRate * 0.5f,0.f,1.f,0.f);
	GLRender::Translatef( m_sunPos.x + m_camera3D.m_cameraPosition.x,m_sunPos.y + m_camera3D.m_cameraPosition.y , 30.f);//80.f);//glTranslatef(m_sunPos.x ,m_sunPos.y ,80.f);
	GLRender::Scalef(25.f,25.f,25.f);//glScalef(25.f,25.f,50.f);
	
	GLRender::BeginDraw(GL_QUADS);//glBegin(GL_QUADS);
	GLRender::Color4f(1.f,1.f,1.f,1.f);//glColor4f(1.f,1.f,1.f,1.f);

	GLRender::TexCoord2d(1.f,0.f);//glTexCoord2d(1.f,0.f);
	GLRender::Vertex3f(-1.f,1.f,1.f);//glVertex3f(-1.f,1.f,1.f);

	GLRender::TexCoord2d(0.f,0.f);//glTexCoord2d(0.f,0.f);
	GLRender::Vertex3f(-1.f,-1.f,1.f);//glVertex3f(-1.f,-1.f,1.f);

	GLRender::TexCoord2d(0.f,1.f);//glTexCoord2d(0.f,1.f);
	GLRender::Vertex3f(-1.f,-1.f,0.f);//glVertex3f(-1.f,-1.f,0.f);

	GLRender::TexCoord2d(1.f,1.f);//glTexCoord2d(1.f,1.f);
	GLRender::Vertex3f(-1.f,1.f,0.f);//glVertex3f(-1.f,1.f,0.f);
	GLRender::EndDraw();//glEnd();

	GLRender::PopMatrix();//glPopMatrix();

	GLRender::Disable( GL_BLEND );//glDisable(GL_BLEND);
	GLRender::Disable( GL_TEXTURE_2D );//glDisable(GL_TEXTURE_2D);
	GLRender::Enable( GL_DEPTH_TEST );//glEnable(GL_DEPTH_TEST);
}

void World::RenderMoon()
{
	GLRender::Disable( GL_DEPTH_TEST );//glDisable(GL_DEPTH_TEST);
	GLRender::Enable( GL_TEXTURE_2D );//glEnable(GL_TEXTURE_2D);
	GLRender::Enable( GL_BLEND ) ;//glEnable (GL_BLEND);
	GLRender::BlendFunc( GL_SRC_ALPHA, GL_ONE);//glBlendFunc (GL_SRC_ALPHA,GL_ONE);
	GLRender::BindTexture( GL_TEXTURE_2D,m_moonTexture->m_openglTextureID );//glBindTexture(GL_TEXTURE_2D,m_moonTexture->m_openglTextureID);

	GLRender::PushMatrix();//glPushMatrix();
	GLRender::Rotatef( m_sunMoonRotateRate * 0.5f,0.f,1.f,0.f );//glRotatef(m_sunMoonRotateRate * 0.5f,0.f,1.f,0.f);
	GLRender::Translatef( m_moonPos.x + m_camera3D.m_cameraPosition.x, m_moonPos.y + m_camera3D.m_cameraPosition.y , 128.f );//glTranslatef(m_moonPos.x, m_moonPos.y ,80.f);
	GLRender::Scalef( 25.f,25.f,25.f );//glScalef(25.f,25.f,50.f);
	
	GLRender::BeginDraw(GL_QUADS);//glBegin(GL_QUADS);
	GLRender::Color4f( 1.f,1.f,1.f,1.f);//glColor4f(1.f,1.f,1.f,1.f);

	GLRender::TexCoord2d( 0.f,0.f );//glTexCoord2d(0.f,0.f);
	GLRender::Vertex3f( 1.f,1.f,1.f );//glVertex3f(1.f,1.f,1.f);

	GLRender::TexCoord2d( 0.f,1.f );//glTexCoord2d(0.f,1.f);
	GLRender::Vertex3f( 1.f,1.f,0.f );//glVertex3f(1.f,1.f,0.f);

	GLRender::TexCoord2d( 1.f,1.f );//glTexCoord2d(1.f,1.f);
	GLRender::Vertex3f( 1.f,-1.f,0.f );//glVertex3f(1.f,-1.f,0.f);

	GLRender::TexCoord2d( 1.f,0.f );//glTexCoord2d(1.f,0.f);
	GLRender::Vertex3f( 1.f,-1.f,1.f );//glVertex3f(1.f,-1.f,1.f);
	GLRender::EndDraw();//glEnd();

	GLRender::PopMatrix();//glPopMatrix();

	GLRender::Disable(GL_BLEND);//glDisable(GL_BLEND);
	GLRender::Disable(GL_TEXTURE_2D);//glDisable(GL_TEXTURE_2D);
	GLRender::Enable(GL_DEPTH_TEST);//glEnable(GL_DEPTH_TEST);
}


Vector2 World::GetMouseSinceLastChecked()
{
	POINT centerCursorPos = { 800, 450 };
	POINT cursorPos;
	WindowFunc::GetCursorPosition(&cursorPos);//GetCursorPos( &cursorPos );
	WindowFunc::SetCursorPosition( centerCursorPos.x, centerCursorPos.y);//SetCursorPos( centerCursorPos.x, centerCursorPos.y );
	Vector2i mouseDeltaInts( cursorPos.x - centerCursorPos.x, cursorPos.y - centerCursorPos.y );
	Vector2 mouseDeltas( (float) mouseDeltaInts.x, (float) mouseDeltaInts.y );
	return mouseDeltas;
}

void World::UpdateCameraFromKeyboardAndMouse(Camera3D& camera,float elapsedTime)
{
	const float degreesPerMouseDelta = 0.04f;
	Vector2 mouseDeltas = GetMouseSinceLastChecked();
	camera.m_yawDegreesAboutZ -= (degreesPerMouseDelta * mouseDeltas.x);

	camera.m_pitchDegreesAboutY += (degreesPerMouseDelta * mouseDeltas.y);
	camera.m_pitchDegreesAboutY = MathUtilities::Clamp(-90.f,90.f,camera.m_pitchDegreesAboutY);

	float cameraYawRadians = MathUtilities::DegToRad(camera.m_yawDegreesAboutZ);
	Vector3 cameraForwardXYVector( cos(cameraYawRadians), sin( cameraYawRadians ),0.f);

	m_cameraForwardVector = Vector3(cameraForwardXYVector.x * cos(MathUtilities::DegToRad(camera.m_pitchDegreesAboutY)),
							   cameraForwardXYVector.y * cos(MathUtilities::DegToRad(camera.m_pitchDegreesAboutY)),
							  -sin(MathUtilities::DegToRad(camera.m_pitchDegreesAboutY)));
	m_cameraForwardVector.Normalize();
	//m_cameraForwardVector.RotateAboutYAxisDegree(camera.m_pitchDegreesAboutY);

	Vector3 movementVector( 0.f,0.f,0.f );

	if( IsKeyDownKeyboard[ 'W' ] )
		movementVector += cameraForwardXYVector * m_player.m_playerSpeed;

	if( IsKeyDownKeyboard[ 'A' ] )
		movementVector += Vector3(-cameraForwardXYVector.y,cameraForwardXYVector.x,0.f) * m_player.m_playerSpeed;

	if( IsKeyDownKeyboard[ 'D' ] )
		movementVector += Vector3(cameraForwardXYVector.y,-cameraForwardXYVector.x,0.f) * m_player.m_playerSpeed;

	if( IsKeyDownKeyboard[ 'S' ] )
		movementVector += Vector3(-cameraForwardXYVector.x,-cameraForwardXYVector.y,0.f) * m_player.m_playerSpeed;

	if( IsKeyDownKeyboard[ 'E' ] )
		movementVector += Vector3(0.f,0.f,10.f);

	if( IsKeyDownKeyboard[ 'C' ] )
		movementVector += Vector3(0.f,0.f,-10.f);
	
	if(m_leftMouseDown && m_hitInfo.hitFace != NONE)
	{
		PlaceBlock(m_hitInfo);
	}

	if( IsKeyDownKeyboard[ 'M' ] && ( IsKeyDownLastFrame[ 'M' ] != IsKeyDownKeyboard[ 'M' ] ) )
	{
		m_player.m_modeIndex++;
		if(m_player.m_modeIndex > 2)
			m_player.m_modeIndex = 0;
	}

	if( IsKeyDownKeyboard[ '1' ] && ( IsKeyDownLastFrame[ '1' ] != IsKeyDownKeyboard[ '1' ] ) )
	{
		m_currentBlockType = STONE;
	}
	else if( IsKeyDownKeyboard[ '2' ] && ( IsKeyDownLastFrame[ '2' ] != IsKeyDownKeyboard[ '2' ] ) )
	{
		m_currentBlockType = WOOD;
	}
	else if( IsKeyDownKeyboard[ '3' ] && ( IsKeyDownLastFrame[ '3' ] != IsKeyDownKeyboard[ '3' ] ) )
	{
		m_currentBlockType = GLOW_STONE;
	}

	if(m_rightMouseDown && m_hitInfo.hitFace != NONE)
	{
		KillBlock(m_hitInfo);
	}

	if(m_player.m_movementMode == WALKING)
	{
		if( IsKeyDownKeyboard[VK_SPACE] && ( IsKeyDownLastFrame[VK_SPACE] != IsKeyDownKeyboard[VK_SPACE] ) && m_player.m_isOnGround )
		{
			m_jumpForce = m_player.m_jumpForce;
		}

		if( !m_player.m_isOnGround && !m_player.IsPlayerInWater() )
			movementVector += Vector3(0.f,0.f,-10.f);

		if(m_player.IsPlayerInWater() && m_player.m_eyePosition.z > SEA_LEVEL && !m_player.m_isOnGround)
		{
			static float counter = 0.f;
			counter += 0.09f;
			float mCos = cos ( counter);
			movementVector += Vector3(0.f,0.f, mCos);
		}
		
		movementVector += Vector3(0.f,0.f,m_jumpForce);

		if(m_jumpForce > 0.f)
			m_jumpForce -= 2.5f;
	}

	if(m_player.m_movementMode != NOCLIP)
	{
		if( IsKeyDownKeyboard[VK_SPACE] && m_player.m_movementMode == FLY )
		{
			movementVector += Vector3(0.f,0.f,10.f);
		}

		m_player.UpdatePlayerPosition( movementVector * elapsedTime );
		camera.m_cameraPosition = m_player.m_eyePosition;
	}
	else
	{
		camera.m_cameraPosition += movementVector * elapsedTime;
		m_player.m_bottomCenterPosition += movementVector * elapsedTime;
	}

	for(int i = 0; i < 256;i++)
	{
		IsKeyDownLastFrame[i] = IsKeyDownKeyboard[i];
	}
}

void World::KillBlock(HitInfo m_hitInfo)
{
	BlockInfo blockInfo;
	ChunkBlocks* currentChunk = nullptr; 
	char* soundToPlay;
	int random;

	if(g_mapChunk.find(m_hitInfo.hitChunkCoord2D) != g_mapChunk.end())
		currentChunk = g_mapChunk[m_hitInfo.hitChunkCoord2D];
	
	if( currentChunk != nullptr )
	{
		if(currentChunk->m_blocksList[m_hitInfo.hitBlockIndex].m_blockType.material != AIR)
		{
			Material blockMaterial = currentChunk->m_blocksList[m_hitInfo.hitBlockIndex].m_blockType.material;
			random = MathUtilities::GetRandomNumber(0,1);
			soundToPlay = g_blockTypeInfo[blockMaterial].diggingSound[random];
			g_audioSystem.PlaySoundByName(soundToPlay,1,false);

			currentChunk->m_blocksList[m_hitInfo.hitBlockIndex].m_blockType.material = AIR;

			if(currentChunk->m_blocksList[m_hitInfo.hitBlockIndex + CHUNK_LAYER_SIZE ].IsSkyBlockBitSet()
			   && currentChunk->m_blocksList[m_hitInfo.hitBlockIndex + CHUNK_LAYER_SIZE ].m_blockType.material == AIR)
			{
				currentChunk->m_blocksList[m_hitInfo.hitBlockIndex].SetSkyVisibleForBlock();
				currentChunk->m_blocksList[m_hitInfo.hitBlockIndex].SetLightLevelForBlock(DAY_LIGHT_VALUE);
				blockInfo.m_blockLocalIndex = m_hitInfo.hitBlockIndex;
				blockInfo.m_chunkCoord = m_hitInfo.hitChunkCoord2D;
				g_listOfBlockToPropagateLight.push(blockInfo);
				SetAllAirBLockBelowToSkyBlock(m_hitInfo.hitBlockIndex,currentChunk);
				PushNeighborBlockToLightList(m_hitInfo.hitBlockIndex,currentChunk);
				currentChunk->m_isVboDirty = true;
				currentChunk->m_eastNeighborForChunk->m_isVboDirty = true;
				currentChunk->m_westNeighborForChunk->m_isVboDirty = true;
				currentChunk->m_northNeighborForChunk->m_isVboDirty = true;
				currentChunk->m_southNeighborForChunk->m_isVboDirty = true;
			}
			else
			{
				blockInfo.m_blockLocalIndex = m_hitInfo.hitBlockIndex;
				blockInfo.m_chunkCoord = m_hitInfo.hitChunkCoord2D;
				g_listOfBlockToPropagateLight.push(blockInfo);
				PushNeighborBlockToLightList(m_hitInfo.hitBlockIndex,currentChunk);
				currentChunk->m_isVboDirty = true;
				currentChunk->m_eastNeighborForChunk->m_isVboDirty = true;
				currentChunk->m_westNeighborForChunk->m_isVboDirty = true;
				currentChunk->m_northNeighborForChunk->m_isVboDirty = true;
				currentChunk->m_southNeighborForChunk->m_isVboDirty = true;
			}
		}
	}
	//currentChunk->m_isVboDirty = true;
	m_rightMouseDown = false;
}

void World::PlaceBlock(HitInfo hitInfo)
{
	Block* topNeighborBlock = nullptr;
	Block* bottomNeighborBlock = nullptr;
	Block* northNeighborBlock = nullptr;
	Block* southNeighborBlock = nullptr;
	Block* eastNeighborBlock = nullptr;
	Block* westNeighborBlock = nullptr;
	Block* currentBlock = nullptr;
	Vector3i blockLocal3Dcoord;

	ChunkBlocks* currentChunk = nullptr;

	int tempIndex = 0;

	if(g_mapChunk.find(m_hitInfo.hitChunkCoord2D) != g_mapChunk.end())
		currentChunk = g_mapChunk[m_hitInfo.hitChunkCoord2D];

	if(currentChunk != nullptr)
		currentBlock = &currentChunk->m_blocksList[hitInfo.hitBlockIndex];

	if(m_hitInfo.hitFace == TOP)
	{
		if(hitInfo.worldBlockCoord.z < CHUNK_HEIGHT - 1 )
		{
			topNeighborBlock = &currentChunk->m_blocksList[hitInfo.hitBlockIndex + CHUNK_LAYER_SIZE];
			ReplaceAirWithCurrentBlockType(topNeighborBlock,currentChunk, hitInfo.hitBlockIndex + CHUNK_LAYER_SIZE);
		}
	}

	if(m_hitInfo.hitFace == BOTTOM)
	{
		if(hitInfo.worldBlockCoord.z > 0 )
		{
			bottomNeighborBlock = &currentChunk->m_blocksList[hitInfo.hitBlockIndex - CHUNK_LAYER_SIZE];
 			ReplaceAirWithCurrentBlockType(bottomNeighborBlock,currentChunk, hitInfo.hitBlockIndex - CHUNK_LAYER_SIZE);
		}
	}

	if(m_hitInfo.hitFace == NORTH)
	{
		if( hitInfo.localBlockCoord.x < ( CHUNK_WIDTH_X - 1) )
		{
			northNeighborBlock = &currentChunk->m_blocksList[ hitInfo.hitBlockIndex + 1 ];
			tempIndex = hitInfo.hitBlockIndex + 1;
			ReplaceAirWithCurrentBlockType(northNeighborBlock,currentChunk,tempIndex);
		}
		else if( hitInfo.localBlockCoord.x == ( CHUNK_WIDTH_X - 1 ) )
		{
			if(currentChunk->m_northNeighborForChunk != nullptr)
			{
				northNeighborBlock = &currentChunk->m_northNeighborForChunk->m_blocksList[ hitInfo.hitBlockIndex - ( CHUNK_WIDTH_X - 1 ) ];
				tempIndex = hitInfo.hitBlockIndex - ( CHUNK_WIDTH_X - 1 );
				ReplaceAirWithCurrentBlockType(northNeighborBlock,currentChunk->m_northNeighborForChunk,tempIndex);
				//currentChunk->m_northNeighborForChunk->m_isVboDirty = true;
			}
		}
	}

	if(m_hitInfo.hitFace == SOUTH)
	{
		if( hitInfo.localBlockCoord.x > 0 )
		{
			southNeighborBlock = &currentChunk->m_blocksList[ hitInfo.hitBlockIndex - 1 ];
			tempIndex = hitInfo.hitBlockIndex - 1 ;
			ReplaceAirWithCurrentBlockType(southNeighborBlock,currentChunk,tempIndex);
		}
		else if( hitInfo.localBlockCoord.x == 0 )
		{
			if(currentChunk->m_southNeighborForChunk != nullptr)
			{
				southNeighborBlock = &currentChunk->m_southNeighborForChunk->m_blocksList[ hitInfo.hitBlockIndex + ( CHUNK_WIDTH_X - 1 ) ];
				tempIndex = hitInfo.hitBlockIndex + ( CHUNK_WIDTH_X - 1 ) ;
				//currentChunk->m_southNeighborForChunk->m_isVboDirty = true;
				ReplaceAirWithCurrentBlockType(southNeighborBlock,currentChunk->m_southNeighborForChunk,tempIndex);
			}
		}
	}

	if(m_hitInfo.hitFace == WEST)
	{
		if( hitInfo.localBlockCoord.y < CHUNK_DEPTH_Y - 1 )
		{
			westNeighborBlock = &currentChunk->m_blocksList[ hitInfo.hitBlockIndex + CHUNK_DEPTH_Y ];
			tempIndex = hitInfo.hitBlockIndex + CHUNK_DEPTH_Y;
			ReplaceAirWithCurrentBlockType(westNeighborBlock,currentChunk,tempIndex);
		}
		else if( hitInfo.localBlockCoord.y == CHUNK_DEPTH_Y - 1 )
		{
			if(currentChunk->m_westNeighborForChunk != nullptr)
			{
				westNeighborBlock = &currentChunk->m_westNeighborForChunk->m_blocksList[ hitInfo.hitBlockIndex - ( CHUNK_DEPTH_Y - 1) * CHUNK_DEPTH_Y ];
				tempIndex = hitInfo.hitBlockIndex - ( CHUNK_DEPTH_Y - 1) * CHUNK_DEPTH_Y;
				ReplaceAirWithCurrentBlockType(westNeighborBlock,currentChunk->m_westNeighborForChunk,tempIndex);
				//currentChunk->m_westNeighborForChunk->m_isVboDirty = true;
			}
		}
	}

	if(m_hitInfo.hitFace == EAST)
	{
		if( hitInfo.localBlockCoord.y > 0 )
		{
			eastNeighborBlock = &currentChunk->m_blocksList[ hitInfo.hitBlockIndex - CHUNK_DEPTH_Y ];
			tempIndex = hitInfo.hitBlockIndex - CHUNK_DEPTH_Y;
			ReplaceAirWithCurrentBlockType(eastNeighborBlock,currentChunk,tempIndex);
		}
		else if( hitInfo.localBlockCoord.y == 0 )
		{
			if(currentChunk->m_eastNeighborForChunk != nullptr)
			{
				eastNeighborBlock = &currentChunk->m_eastNeighborForChunk->m_blocksList[ hitInfo.hitBlockIndex + ( CHUNK_DEPTH_Y - 1 ) * CHUNK_DEPTH_Y ];
				tempIndex = hitInfo.hitBlockIndex + ( CHUNK_DEPTH_Y - 1 ) * CHUNK_DEPTH_Y ;
				ReplaceAirWithCurrentBlockType(eastNeighborBlock,currentChunk->m_eastNeighborForChunk,tempIndex);
				//currentChunk->m_eastNeighborForChunk->m_isVboDirty = true;
			}
		}
	}

 	//currentChunk->m_isVboDirty = true;
 	m_leftMouseDown = false;
// 	g_debugPoints.clear();
// 	for(int i = 0; i < CHUNK_SIZE_IN_BLOCK; i++)
// 	{
// 		if(currentChunk->m_blocksList[i].IsSkyBlockBitSet())
// 		{
// 			Vector3i temp = currentChunk->GetBlockLocalCoord3DFromBlockIndex(i);
// 			Vector3 temp1;
// 			temp1.SetXYZ(temp.x,temp.y,temp.z);
// 			g_debugPoints.insert( Vector3(temp1.x + 0.5f,temp1.y + 0.5f, temp1.z + 0.5f) );
// 		}
// 	}
}

void World::SetAllAirBLockBelowToSkyBlock(int blockIndex, ChunkBlocks* currentChunk)
{
	Block* currentBlock = nullptr;
	BlockInfo blockInfo;

	while(blockIndex > 0 )
	{
		blockIndex -= CHUNK_LAYER_SIZE;
		if(currentChunk != nullptr)
			currentBlock = &currentChunk->m_blocksList[blockIndex];
		if(currentBlock->m_blockType.material == AIR)
		{
			currentBlock->SetSkyVisibleForBlock();
			blockInfo.m_blockLocalIndex = blockIndex;
			blockInfo.m_chunkCoord = currentChunk->m_worldCoords2D;
			g_listOfBlockToPropagateLight.push(blockInfo);
			PushNeighborBlockToLightList(blockIndex,currentChunk);
		}
		else
			break;
	}
}

void World::PushAirBlockBelowSolidBlockToLightList(int blockIndex, ChunkBlocks* currentChunk)
{
	Block* currentBlock = nullptr;
	BlockInfo blockInfo;
	
	while(blockIndex > 0 )
	{
		blockIndex -= CHUNK_LAYER_SIZE;
		if(currentChunk != nullptr)
			currentBlock = &currentChunk->m_blocksList[blockIndex];
		if(currentBlock->m_blockType.material == AIR)
		{
			currentBlock->SetSkyNotVisibleForBlock();
			blockInfo.m_blockLocalIndex = blockIndex;
			blockInfo.m_chunkCoord = currentChunk->m_worldCoords2D;
			g_listOfBlockToPropagateLight.push(blockInfo);
		}
		else
			break;
	}
	//currentChunk->m_isVboDirty = true;
}

void World::ReplaceAirWithCurrentBlockType(Block* block, ChunkBlocks* currentChunk, int blockIndex)
{
	int random;
	char* placingSound;
	if(block != nullptr && block->m_blockType.material == AIR)
	{
		block->m_blockType.material = m_currentBlockType;
		switch(m_currentBlockType)
		{
			case WOOD:
			case STONE:	     block->SetLightLevelForBlock(block->GetNativeLightValueOfBlockType());
							 block->SetSkyNotVisibleForBlock();
							 PushNeighborBlockToLightList(blockIndex,currentChunk);
							 PushAirBlockBelowSolidBlockToLightList(blockIndex,currentChunk);
							 random = MathUtilities::GetRandomNumber(0,1);
							 placingSound = g_blockTypeInfo[m_currentBlockType].placingSound[random];
							 g_audioSystem.PlaySoundByName(placingSound,1,false);
							 break;

			case GLOW_STONE: block->SetLightLevelForBlock(block->GetNativeLightValueOfBlockType());
							 PushAirBlockBelowSolidBlockToLightList(blockIndex,currentChunk);
							 PushNeighborBlockToLightList(blockIndex,currentChunk);
							 random = MathUtilities::GetRandomNumber(0,1);
							 placingSound = g_blockTypeInfo[GLOW_STONE].placingSound[random];
							 g_audioSystem.PlaySoundByName(placingSound,1,false);
							 break;
		}
		currentChunk->m_isVboDirty = true;
	}
}

void World::PushNeighborBlockToLightList(int blockIndex,ChunkBlocks* currentChunk)
{
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

	Vector3i localCoord3DofBlock = GetBlockLocalCoord3DFromBlockIndex(blockIndex);
	//currentChunk->m_isVboDirty = true;
	//if block is not in top layer of chunk
	if( localCoord3DofBlock.z < ( CHUNK_HEIGHT - 1 ) )
	{
		topNeighborBlock = &currentChunk->m_blocksList[ blockIndex + CHUNK_LAYER_SIZE ];
		topNeighborBlockInfo.m_blockLocalIndex = blockIndex + CHUNK_LAYER_SIZE;
		topNeighborBlockInfo.m_chunkCoord = currentChunk->m_worldCoords2D;
	}

	//if block is not in bottom layer of chunk
	if( localCoord3DofBlock.z > 0)
	{
		bottomNeighborBlock = &currentChunk->m_blocksList[ blockIndex - CHUNK_LAYER_SIZE ];
		bottomNeighborBlockInfo.m_blockLocalIndex = blockIndex - CHUNK_LAYER_SIZE;
		bottomNeighborBlockInfo.m_chunkCoord = currentChunk->m_worldCoords2D;

	}

	//if block is not on north border
	if( localCoord3DofBlock.x < ( CHUNK_WIDTH_X - 1) )
	{
		northNeighborBlock = &currentChunk->m_blocksList[ blockIndex + 1 ];
		northNeighborBlockInfo.m_blockLocalIndex = blockIndex + 1;
		northNeighborBlockInfo.m_chunkCoord = currentChunk->m_worldCoords2D;
	}
	else if( localCoord3DofBlock.x == ( CHUNK_WIDTH_X - 1 ) )
	{
		if(currentChunk->m_northNeighborForChunk != nullptr)
		{
			northNeighborBlock = &currentChunk->m_northNeighborForChunk->m_blocksList[ blockIndex - ( CHUNK_WIDTH_X - 1 ) ];
			northNeighborBlockInfo.m_blockLocalIndex = blockIndex - ( CHUNK_WIDTH_X - 1 ) ;
			northNeighborBlockInfo.m_chunkCoord = currentChunk->m_northNeighborForChunk->m_worldCoords2D;
			//currentChunk->m_northNeighborForChunk->m_isVboDirty = true;
		}
	}


	//if block is not on south border
	if( localCoord3DofBlock.x > 0 )
	{
		southNeighborBlock = &currentChunk->m_blocksList[ blockIndex - 1 ];
		southNeighborBlockInfo.m_blockLocalIndex = blockIndex - 1  ;
		southNeighborBlockInfo.m_chunkCoord = currentChunk->m_worldCoords2D;
	}
	else if( localCoord3DofBlock.x == 0 )
	{
		if(currentChunk->m_southNeighborForChunk != nullptr)
		{
			southNeighborBlock = &currentChunk->m_southNeighborForChunk->m_blocksList[ blockIndex + ( CHUNK_WIDTH_X - 1 ) ];
			southNeighborBlockInfo.m_blockLocalIndex = blockIndex + ( CHUNK_WIDTH_X - 1 )  ;
			southNeighborBlockInfo.m_chunkCoord = currentChunk->m_southNeighborForChunk->m_worldCoords2D ;
			//currentChunk->m_southNeighborForChunk->m_isVboDirty = true;
		}
	}


	//if block not on west border
	if( localCoord3DofBlock.y < ( CHUNK_DEPTH_Y - 1 ))
	{
		westNeighborBlock = &currentChunk->m_blocksList[ blockIndex + CHUNK_DEPTH_Y ];
		westNeighborBlockInfo.m_blockLocalIndex = blockIndex + CHUNK_DEPTH_Y ;
		westNeighborBlockInfo.m_chunkCoord = currentChunk->m_worldCoords2D;
	}
	else if( localCoord3DofBlock.y == ( CHUNK_DEPTH_Y - 1 ) )
	{
		if( currentChunk->m_westNeighborForChunk != nullptr )
		{
			westNeighborBlock = &currentChunk->m_westNeighborForChunk->m_blocksList[ blockIndex - ( CHUNK_DEPTH_Y - 1) * CHUNK_DEPTH_Y ];
			westNeighborBlockInfo.m_blockLocalIndex = blockIndex - ( CHUNK_WIDTH_X - 1 ) * CHUNK_DEPTH_Y ;
			westNeighborBlockInfo.m_chunkCoord = currentChunk->m_westNeighborForChunk->m_worldCoords2D ;
			//currentChunk->m_westNeighborForChunk->m_isVboDirty = true;
		}
	}
	

	//if block not on east border
	if( localCoord3DofBlock.y > 0 )
	{
		eastNeighborBlock = &currentChunk->m_blocksList[ blockIndex - CHUNK_DEPTH_Y ];
		eastNeighborBlockInfo.m_blockLocalIndex = blockIndex - CHUNK_DEPTH_Y ;
		eastNeighborBlockInfo.m_chunkCoord = currentChunk->m_worldCoords2D;
	}
	else if( localCoord3DofBlock.y == 0)
	{
		if(currentChunk-> m_eastNeighborForChunk != nullptr )
		{
			eastNeighborBlock = &currentChunk->m_eastNeighborForChunk->m_blocksList[ blockIndex + ( CHUNK_DEPTH_Y - 1 ) * CHUNK_DEPTH_Y ];
			eastNeighborBlockInfo.m_blockLocalIndex = blockIndex + ( CHUNK_WIDTH_X - 1 ) * CHUNK_DEPTH_Y ;
			eastNeighborBlockInfo.m_chunkCoord = currentChunk->m_eastNeighborForChunk->m_worldCoords2D ;
		}
	}

	if(eastNeighborBlock != nullptr && !g_blockTypeInfo[eastNeighborBlock->m_blockType.material].isObaque)//eastNeighborBlock->m_blockType.material == AIR)
	{
		g_listOfBlockToPropagateLight.push(eastNeighborBlockInfo);
	}

	if(westNeighborBlock != nullptr && !g_blockTypeInfo[westNeighborBlock->m_blockType.material].isObaque)//westNeighborBlock->m_blockType.material == AIR)
	{
		g_listOfBlockToPropagateLight.push(westNeighborBlockInfo);
	}

	if(southNeighborBlock != nullptr && !g_blockTypeInfo[southNeighborBlock->m_blockType.material].isObaque)//southNeighborBlock->m_blockType.material == AIR)
	{
		g_listOfBlockToPropagateLight.push(southNeighborBlockInfo);
	}

	if(northNeighborBlock != nullptr && !g_blockTypeInfo[northNeighborBlock->m_blockType.material].isObaque)//northNeighborBlock->m_blockType.material == AIR)
	{
		g_listOfBlockToPropagateLight.push(northNeighborBlockInfo);
	}

	if(topNeighborBlock != nullptr && !g_blockTypeInfo[topNeighborBlock->m_blockType.material].isObaque)//topNeighborBlock->m_blockType.material == AIR)
	{
		g_listOfBlockToPropagateLight.push(topNeighborBlockInfo);
	}

	if(bottomNeighborBlock != nullptr && !g_blockTypeInfo[bottomNeighborBlock->m_blockType.material].isObaque)//bottomNeighborBlock->m_blockType.material == AIR)
	{
		g_listOfBlockToPropagateLight.push(bottomNeighborBlockInfo);
	}
}

void World::SetupPerspectiveProjection()
{
	float aspect = (16.f/9.f);
	float fovX = 70.f;
	float fovY = (fovX / aspect);
	float zNear = 0.1f;
	float zFar = 1000.f;
	GLRender::LoadIdentityMatrix();//glLoadIdentity();
	gluPerspective(fovY,aspect,zNear,zFar);
}

void World::ApplyCameraTransform(Camera3D camera)
{
	GLRender::Rotatef(-90.f,1.f,0.f,0.f);//glRotatef( -90.f,1.f,0.f,0.f);
	GLRender::Rotatef(90.f, 0.f,0.f,1.f);//glRotatef( 90.f, 0.f,0.f,1.f);

	GLRender::Rotatef(-camera.m_rollDegreesAboutX, 1.f,0.f,0.f);//glRotatef( -camera.m_rollDegreesAboutX, 1.f,0.f,0.f);
	GLRender::Rotatef(-camera.m_pitchDegreesAboutY, 0.f,1.f,0.f);//glRotatef( -camera.m_pitchDegreesAboutY, 0.f,1.f,0.f);
	GLRender::Rotatef(-camera.m_yawDegreesAboutZ , 0.f,0.f,1.f);//glRotatef( -camera.m_yawDegreesAboutZ , 0.f,0.f,1.f);

	GLRender::Translatef(-camera.m_cameraPosition.x, -camera.m_cameraPosition.y, -camera.m_cameraPosition.z);//glTranslatef(-camera.m_cameraPosition.x, -camera.m_cameraPosition.y, -camera.m_cameraPosition.z);
}

void World::RenderRay()
{
	Vector3 startRay = m_camera3D.m_cameraPosition;
	Vector3 endRay = startRay + Vector3(m_cameraForwardVector.x * 8.f,m_cameraForwardVector.y * 8.f,m_cameraForwardVector.z * 8.f);

	GLRender::Disable(GL_TEXTURE_2D);//glDisable(GL_TEXTURE_2D);
	GLRender::Color4f(1.f,0.f,0.f,1.f);//glColor4f(1.f,0.f,0.f,1.f);
	GLRender::PointSize(10);//glPointSize(10);
	GLRender::BeginDraw(GL_POINTS);//glBegin(GL_POINTS);
	GLRender::Vertex3f(startRay.x,startRay.y,startRay.z);//glVertex3f(startRay.x,startRay.y,startRay.z);
	GLRender::Vertex3f(endRay.x,endRay.y,endRay.z);//glVertex3f(endRay.x,endRay.y,endRay.z);
	GLRender::EndDraw();//glEnd();
	GLRender::Enable(GL_TEXTURE_2D);//glEnable(GL_TEXTURE_2D);

	Vector3 rayHitPosition = startRay + (m_cameraForwardVector * m_hitInfo.hitDepth);
	GLRender::Disable(GL_DEPTH_TEST);//glDisable( GL_DEPTH_TEST );
	GLRender::Disable(GL_TEXTURE_2D);//glDisable(GL_TEXTURE_2D);
	GLRender::Color4f(1.f,0.f,1.f,1.f);//glColor4f(1.f,0.f,1.f,1.f);
	GLRender::PointSize(6);//glPointSize(6);
	GLRender::BeginDraw(GL_POINTS);//glBegin(GL_POINTS);
	{
		GLRender::Vertex3f( rayHitPosition.x, rayHitPosition.y, rayHitPosition.z );//	glVertex3f( rayHitPosition.x, rayHitPosition.y, rayHitPosition.z );
	}
	GLRender::EndDraw();//glEnd();
	GLRender::Enable(GL_TEXTURE_2D);//glEnable(GL_TEXTURE_2D);
	GLRender::Enable(GL_DEPTH_TEST);//glEnable( GL_DEPTH_TEST );
}

 void World::Render()
 {
	 //Draw World
	 GLRender::ClearColor( 0.3f,0.5f,1.f,1.f );//glClearColor(0.3f,0.5f,1.f,1.f);
	 GLRender::ClearDepth( 1.f );//glClearDepth(1.f);
	 GLRender::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		 
	 GLRender::Disable(GL_DEPTH_TEST);//glDisable(GL_DEPTH_TEST);
	 GLRender::DisableDepthMask();//glDepthMask(GL_FALSE);
	 RenderSkyBox();
	 RenderSun();
	 RenderMoon();
 	 GLRender::EnableDepthMask();//glDepthMask(GL_TRUE);
 	 GLRender::Enable(GL_DEPTH_TEST);//glEnable(GL_DEPTH_TEST);
 	 SetupPerspectiveProjection();
 	 ApplyCameraTransform( m_camera3D );

 	 GLRender::Enable(GL_TEXTURE_2D);//glEnable(GL_TEXTURE_2D);
	 GLRender::BindTexture( GL_TEXTURE_2D, g_gameTexture->m_texture->m_openglTextureID);//glBindTexture(GL_TEXTURE_2D, g_gameTexture->m_texture->m_openglTextureID);
	 m_map.Render();
 	 m_map.Render();
     RenderSeaSurface();
	 m_player.Render();
	 RenderRay();
	 if(m_hitInfo.hitFace != NONE)
		RenderSelectedBlock(m_hitInfo);
	 RenderWorldAxes();
	 //RenderDebugPoints();
	
 }

 void World::RenderDebugPoints()
 {
	 GLRender::Disable(GL_TEXTURE_2D);//glDisable(GL_TEXTURE_2D);
	 GLRender::BeginDraw(GL_POINTS);//glBegin(GL_POINTS);
	 GLRender::Color4f(1.f,1.f,0.f,1.f);//glColor4f(1.f,1.f,0.f,1.f);
	
	 std::set<Vector3>::iterator iter;
	 for(iter = g_debugPoints.begin(); iter != g_debugPoints.end(); ++iter)
	 {
		 Vector3 debugPoint = *iter;
		 
		 GLRender::Vertex3f(debugPoint.x,debugPoint.y,debugPoint.z);//glVertex3f(debugPoint.x,debugPoint.y,debugPoint.z);
	 }

	 //g_debugPoints.clear();
	 GLRender::EndDraw();//glEnd();
	 GLRender::Enable(GL_TEXTURE_2D);//glEnable(GL_TEXTURE_2D);
 }


void World::RenderWorldAxes()
{
	GLRender::Disable(GL_DEPTH_TEST);//glDisable(GL_DEPTH_TEST);
	GLRender::Disable(GL_TEXTURE_2D);//glDisable(GL_TEXTURE_2D);
	GLRender::BeginDraw(GL_LINES);//glBegin(GL_LINES);
	{
		GLRender::Color4f(1.f,0.f,0.f,1.f);//glColor4f(1.f,0.f,0.f,1.f);
		GLRender::Vertex3f(0.f,0.f,0.f);//glVertex3f(0.f,0.f,0.f);
		GLRender::Vertex3f(1.f,0.f,0.f);//glVertex3f(1.f,0.f,0.f);

		GLRender::Color4f(0.f,1.f,0.f,1.f);//glColor4f(0.f,1.f,0.f,1.f);
		GLRender::Vertex3f(0.f,0.f,0.f);//glVertex3f(0.f,0.f,0.f);
		GLRender::Vertex3f(0.f,1.f,0.f);//glVertex3f(0.f,1.f,0.f);

		GLRender::Color4f(0.f,0.f,1.f,1.f);//glColor4f(0.f,0.f,1.f,1.f);
		GLRender::Vertex3f(0.f,0.f,0.f);//glVertex3f(0.f,0.f,0.f);
		GLRender::Vertex3f(0.f,0.f,1.f);//glVertex3f(0.f,0.f,1.f);
	}
	GLRender::EndDraw();//glEnd();
	GLRender::Enable(GL_DEPTH_TEST);//glEnable(GL_DEPTH_TEST);
}


bool World::ProcessKeyDownEvent(HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam )
{
	unsigned char asKey = (unsigned char) wParam;
	switch( wmMessageCode )
	{
		case WM_KEYDOWN:
			IsKeyDownKeyboard[asKey] = true;
			return true;
			break;
			
		case WM_KEYUP:
			IsKeyDownKeyboard[asKey] = false;
			return true;
			break;

		case WM_LBUTTONDOWN:
			m_leftMouseDown = true;	
			break;

		case WM_LBUTTONUP:
			//m_leftMouseDown = false;	
			break;

		case WM_RBUTTONDOWN:
			m_rightMouseDown = true;
			break;

		case WM_RBUTTONUP:
			m_rightMouseDown = false;	
			break;
	}
	return true;
}

HitInfo World::RayTrace()
{
	Vector3 cameraCurrentPosition = m_camera3D.m_cameraPosition; 
	 
	Vector3i currentBlockCoord;
	Vector2i currentWorldChunkCoord = ConvertWorldCoordToChunkCoord2D(cameraCurrentPosition);

	Vector3i nextBlockCoord;
	Vector2i nextWorldChunkCoord;

	Vector3i blockCoordDelta;
	
	Vector3 startRay;
	Vector3 nextRayPosition;
	Vector3 currentRayPosition;

	float maxLength = 12.f;

	Vector3 forwardDirection = m_cameraForwardVector;
	
	HitInfo hitInfo;

	hitInfo.hitDepth = 0;
	hitInfo.hitFace = NONE;

	float travelDistance = 0.f;
	startRay = cameraCurrentPosition;
	currentRayPosition = startRay;
	forwardDirection.Normalize();
	nextRayPosition = currentRayPosition + forwardDirection;
	
	ChunkBlocks* currentChunk = nullptr;

	if(g_mapChunk.find(currentWorldChunkCoord) != g_mapChunk.end())
		currentChunk = g_mapChunk[currentWorldChunkCoord];

	currentBlockCoord = ConvertWorldCoordToWorldBlockCoord(startRay);
	//nextBlockCoord = ConvertWolrdCoordToWorldBlockCoord(nextRayPosition);

	while(travelDistance <= maxLength)
	{
		int xIntercept = INT_MAX;
		int yIntercept = INT_MAX;
		int zIntercept = INT_MAX;

		float distanceFromStartToXintercept = FLT_MAX;
		float distanceFromStartToYintercept = FLT_MAX;
		float distanceFromStartToZintercept = FLT_MAX;
		float smallestDistanceToAxisIntercept = FLT_MAX;

		bool crossX = false;
		bool crossY = false;
		bool crossZ = false;

		//currentBlockCoord = ConvertWolrdCoordToWorldBlockCoord(currentRayPosition);
		nextBlockCoord = ConvertWorldCoordToWorldBlockCoord(nextRayPosition);
		blockCoordDelta = nextBlockCoord - currentBlockCoord;

		if(blockCoordDelta.x != 0)
		{
			if(blockCoordDelta.x > 0)
				xIntercept = currentBlockCoord.x + 1;
			else
			{
				xIntercept = currentBlockCoord.x;
			}
		}

		if(blockCoordDelta.y != 0)
		{
			if(blockCoordDelta.y > 0)
				yIntercept = currentBlockCoord.y + 1;
			else
				yIntercept = currentBlockCoord.y;
		}

		if(blockCoordDelta.z != 0)
		{
			if(blockCoordDelta.z > 0)
				zIntercept = currentBlockCoord.z + 1;
			else
				zIntercept = currentBlockCoord.z;
		}

		if(xIntercept != INT_MAX)
		{
			float xtempDistance = ( static_cast<float>(xIntercept)  - currentRayPosition.x ) / forwardDirection.x ;
			distanceFromStartToXintercept = xtempDistance;
		}

		if(yIntercept != INT_MAX)
		{
			float ytempDistance = ( static_cast<float>(yIntercept) - currentRayPosition.y ) / forwardDirection.y ;
			distanceFromStartToYintercept = ytempDistance;
		}

		if(zIntercept != INT_MAX)
		{
			float ztempDistance = ( static_cast<float>(zIntercept)  - currentRayPosition.z ) / forwardDirection.z ;
			distanceFromStartToZintercept = ztempDistance;
		}

		smallestDistanceToAxisIntercept = MathUtilities::Min3(distanceFromStartToXintercept,distanceFromStartToYintercept,distanceFromStartToZintercept);

		if(smallestDistanceToAxisIntercept != FLT_MAX)
			currentRayPosition += ( forwardDirection * smallestDistanceToAxisIntercept );
		else
			currentRayPosition += forwardDirection;

		if( distanceFromStartToXintercept != FLT_MAX && MathUtilities::CompareTwoFloatNumbers(smallestDistanceToAxisIntercept,distanceFromStartToXintercept,EPSILON))
		{
			crossX = true;
			currentBlockCoord.x += blockCoordDelta.x;
		}
		else if(  distanceFromStartToYintercept != FLT_MAX  && MathUtilities::CompareTwoFloatNumbers(smallestDistanceToAxisIntercept,distanceFromStartToYintercept,EPSILON))
		{
			crossY = true;
			currentBlockCoord.y += blockCoordDelta.y;
		}
		else if(  distanceFromStartToZintercept != FLT_MAX  && MathUtilities::CompareTwoFloatNumbers(smallestDistanceToAxisIntercept,distanceFromStartToZintercept,EPSILON))
		{
			crossZ = true;
			currentBlockCoord.z += blockCoordDelta.z;
		}

		currentWorldChunkCoord = ConvertWolrdBlockCoordToChunkCoord(currentBlockCoord);

		if(g_mapChunk.find(currentWorldChunkCoord) != g_mapChunk.end())
			currentChunk = g_mapChunk[ currentWorldChunkCoord ];

		Vector3i currentLocalBlockCoord = ConvertWolrdBlockCoordToLocalBlockCoord(currentBlockCoord);

		int currentBlockIndex = GetBlockIndexFromLocalCoord3D(currentLocalBlockCoord);

		if( currentChunk != nullptr && currentChunk->m_blocksList[currentBlockIndex].m_blockType.material != AIR
			&& currentChunk->m_blocksList[currentBlockIndex].m_blockType.material != WATER)
		{
 			hitInfo.hitBlockIndex = currentBlockIndex;
			hitInfo.hitChunkCoord2D = currentWorldChunkCoord;
			if(crossX)
			{
				if(currentRayPosition.x < nextRayPosition.x)
					hitInfo.hitFace = SOUTH;

				if(currentRayPosition.x > nextRayPosition.x)
					hitInfo.hitFace = NORTH;
			}
			else if(crossY)
			{
				if(currentRayPosition.y < nextRayPosition.y)
					hitInfo.hitFace = EAST;

				if(currentRayPosition.y > nextRayPosition.y)
					hitInfo.hitFace = WEST;
			}
			else if(crossZ)
			{
				if(currentRayPosition.z > nextRayPosition.z)
					hitInfo.hitFace = TOP;

				if(currentRayPosition.z < nextRayPosition.z)
					hitInfo.hitFace = BOTTOM;
			}

			hitInfo.hitDepth = travelDistance;
			hitInfo.worldBlockCoord = currentBlockCoord;
			hitInfo.localBlockCoord = currentLocalBlockCoord;
			return hitInfo;
		}

		nextRayPosition = currentRayPosition + forwardDirection;

		if(smallestDistanceToAxisIntercept != FLT_MAX)
			travelDistance += smallestDistanceToAxisIntercept;
		else
			travelDistance += forwardDirection.CalcLength();
	}
	hitInfo.hitFace = NONE;
	return hitInfo;
}

void World::RenderSelectedBlock(HitInfo hitInfo)
{
	Vector3i temp = hitInfo.worldBlockCoord;

	GLRender::Disable(GL_DEPTH_TEST);//glDisable(GL_DEPTH_TEST);
	GLRender::Disable(GL_TEXTURE_2D);//glDisable(GL_TEXTURE_2D);

	GLRender::PolygonMode(GL_FRONT_AND_BACK,GL_LINE);//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glLineWidth( 3.f );
	GLRender::Color4f(1.f,1.f,1.f,1.f);//glColor4f(1.f,1.f,1.f,1.f);
	GLRender::BeginDraw(GL_QUADS);//glBegin(GL_QUADS);

	GLRender::Vertex3i(temp.x,temp.y,temp.z);//glVertex3i(	temp.x,temp.y,temp.z );
	GLRender::Vertex3i(temp.x + 1, temp.y,temp.z);//glVertex3i(	temp.x + 1, temp.y,temp.z ) ;
	GLRender::Vertex3i(temp.x + 1, temp.y,temp.z + 1 );//glVertex3i(	temp.x + 1, temp.y,temp.z + 1 );
	GLRender::Vertex3i(temp.x, temp.y, temp.z + 1);//glVertex3i( temp.x, temp.y, temp.z + 1);
			 
	GLRender::Vertex3i(temp.x,temp.y,temp.z + 1);//glVertex3i(	temp.x,temp.y,temp.z + 1 );
	GLRender::Vertex3i(temp.x + 1, temp.y,temp.z + 1);//glVertex3i(	temp.x + 1, temp.y,temp.z + 1 ) ;
	GLRender::Vertex3i(temp.x + 1, temp.y + 1,temp.z + 1);//glVertex3i(	temp.x + 1, temp.y + 1,temp.z + 1 );
	GLRender::Vertex3i(temp.x, temp.y + 1, temp.z + 1);//glVertex3i(	temp.x, temp.y + 1, temp.z + 1);
			 
	GLRender::Vertex3i(temp.x, temp.y + 1,temp.z );//glVertex3i(	temp.x, temp.y + 1,temp.z );
	GLRender::Vertex3i(temp.x, temp.y,temp.z);//glVertex3i(	temp.x, temp.y,temp.z  ) ;
	GLRender::Vertex3i(temp.x, temp.y,temp.z + 1);//glVertex3i(	temp.x, temp.y,temp.z + 1 );
	GLRender::Vertex3i(temp.x, temp.y + 1, temp.z + 1);//glVertex3i(	temp.x, temp.y + 1, temp.z + 1);
			 
	GLRender::Vertex3i(temp.x + 1,temp.y + 1, temp.z);//glVertex3i(	temp.x + 1,temp.y + 1, temp.z  );
	GLRender::Vertex3i(temp.x + 1, temp.y + 1,temp.z + 1 );//glVertex3i(	temp.x + 1, temp.y + 1,temp.z + 1 ) ;
	GLRender::Vertex3i(temp.x + 1, temp.y ,temp.z + 1);//glVertex3i(	temp.x + 1, temp.y ,temp.z + 1 );
	GLRender::Vertex3i(temp.x + 1, temp.y , temp.z);//glVertex3i(	temp.x + 1, temp.y , temp.z );
			 
	GLRender::Vertex3i(temp.x,temp.y + 1,temp.z);//glVertex3i(	temp.x,temp.y + 1,temp.z );
	GLRender::Vertex3i(temp.x , temp.y + 1,temp.z + 1 );//glVertex3i(	temp.x , temp.y + 1,temp.z + 1 ) ;
	GLRender::Vertex3i(temp.x + 1, temp.y + 1,temp.z + 1);//glVertex3i(	temp.x + 1, temp.y + 1,temp.z + 1 );
	GLRender::Vertex3i(temp.x + 1, temp.y + 1, temp.z);//glVertex3i(	temp.x + 1, temp.y + 1, temp.z);
			 
	GLRender::Vertex3i(temp.x, temp.y + 1,temp.z);//glVertex3i(	temp.x, temp.y + 1,temp.z  );
	GLRender::Vertex3i(temp.x + 1, temp.y + 1,temp.z);//glVertex3i(	temp.x + 1, temp.y + 1,temp.z  ) ;
	GLRender::Vertex3i(temp.x + 1, temp.y ,temp.z );//glVertex3i(	temp.x + 1, temp.y ,temp.z  );
	GLRender::Vertex3i(temp.x, temp.y, temp.z);//glVertex3i(	temp.x, temp.y, temp.z );

	GLRender::EndDraw();//glEnd();
	GLRender::PolygonMode(GL_FRONT_AND_BACK,GL_FILL);//glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glLineWidth( 1.f );
	GLRender::Enable(GL_TEXTURE_2D);//glEnable(GL_TEXTURE_2D);
	GLRender::Enable(GL_DEPTH_TEST);//lEnable(GL_DEPTH_TEST);
}

void World::CalculateMinMaxXYOfSeaSurface()
{
	vector<int> xList;
	vector<int> yList;
	int minX = INT_MAX;
	int maxX = INT_MIN;
	int minY = INT_MAX;
	int maxY = INT_MIN;

	for(map< Vector2i, ChunkBlocks* >::iterator it = g_mapChunk.begin(); it != g_mapChunk.end();++it)
	{
		Vector2i temp = it->first;
		xList.push_back( temp.x );
		yList.push_back( temp.y );
	}

	for(int i = 0; i < xList.size(); i++)
	{
		if(xList[i] < minX)
			minX = xList[i];
		if(xList[i] > maxX)
			maxX = xList[i];
	}	for(int i = 0; i < yList.size(); i++)
	{
		if(yList[i] < minY)
			minY = yList[i];
		if(yList[i] > maxY)
			maxY = yList[i];
	}

	m_minSeaSurface.SetXY( minX * CHUNK_WIDTH_X, minY * CHUNK_DEPTH_Y );
	m_minSeaSurface.SetXY( maxX * CHUNK_WIDTH_X, maxY * CHUNK_DEPTH_Y );
}

void World::RenderSeaSurface()
{
	double height = FAKE_SEA_LEVEL;
	GLRender::Disable(GL_TEXTURE_2D);//glDisable(GL_TEXTURE_2D);
	GLRender::Enable(GL_DEPTH_TEST);//glEnable(GL_DEPTH_TEST);
	GLRender::Enable(GL_BLEND);//glEnable(GL_BLEND);
	GLRender::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLRender::Color4f(0.19f,0.26f,.57f,0.7f);//glColor4f(0.19f,0.26f,.57f,0.7f);

	GLRender::PushMatrix();//glPushMatrix();
	GLRender::Translatef(m_camera3D.m_cameraPosition.x ,m_camera3D.m_cameraPosition.y,0);
	GLRender::Scalef(10.f,10.f,1.f);//glScalef(20.f,20.f,1.f);
	GLRender::BeginDraw(GL_QUADS);//glBegin(GL_QUADS);
	
	while(height <= SEA_LEVEL)
	{
		GLRender::Vertex3d(-50.f, -50.f, height );//glVertex3d(	-50.f, -50.f, height );
		GLRender::Vertex3d( 50.f, -50.f, height );//glVertex3d(  50.f, -50.f, height );
		GLRender::Vertex3d( 50.f,  50.f, height );//glVertex3d(  50.f,  50.f, height );
		GLRender::Vertex3d(-50.f,  50.f, height );//glVertex3d( -50.f,  50.f, height );

		height += 1.f;
	}

	GLRender::Vertex3d( -50.f, -50.f, SEA_LEVEL );//glVertex3d( -50.f, -50.f, SEA_LEVEL );
	GLRender::Vertex3d( 50.f, -50.f, SEA_LEVEL);//glVertex3d(  50.f, -50.f, SEA_LEVEL );
	GLRender::Vertex3d( 50.f,  50.f, SEA_LEVEL);//glVertex3d(  50.f,  50.f, SEA_LEVEL );
	GLRender::Vertex3d( -50.f,  50.f, SEA_LEVEL);//glVertex3d( -50.f,  50.f, SEA_LEVEL );

	GLRender::EndDraw();//glEnd();
	GLRender::PopMatrix();//glPopMatrix();
	GLRender::Enable(GL_TEXTURE_2D);//glEnable(GL_TEXTURE_2D);
	GLRender::Enable(GL_DEPTH_TEST);//glEnable(GL_DEPTH_TEST);
}

void World::UpdateWorldTimeAndLightLevel(float elapsedTime)
{	
	if( !m_doneCheckingBlockTime )
	{
		for(map< Vector2i, ChunkBlocks* >::iterator it = g_mapChunk.begin(); it != g_mapChunk.end();++it)
		{
			if(g_mapChunk.find(it->first) != g_mapChunk.end())
			{
				if( it->second->m_outdoorLightLevel != g_globalOutDoorLightLevel && g_listOfBlockToPropagateLight.size() == 0 )
				{
					it->second->m_outdoorLightLevel = g_globalOutDoorLightLevel;
					m_chunkCount++;

					for(int i = 0; i < CHUNK_SIZE_IN_BLOCK; i++)
					{
						if(it->second->m_blocksList[i].IsSkyBlockBitSet())
						{
 							BlockInfo temp;
 							temp.m_blockLocalIndex = i;
 							temp.m_chunkCoord = it->second->m_worldCoords2D;
 							g_listOfBlockToPropagateLight.push(temp);
						}
					}
					//it->second->m_isVboDirty = true;
					break;
				}
			}
		}
	}

	if( m_chunkCount >= g_mapChunk.size() )
	{
		m_doneCheckingBlockTime = true;
		m_chunkCount = 0;
	}

	if( m_timeOfDay >= 0.f && m_doneCheckingBlockTime )
	{
		m_timeOfDay += elapsedTime * 0.1f ;

		if( m_timeOfDay >= SUNRISE_TIME && m_timeOfDay < EARLY_MORNING_TIME )
		{
			g_globalOutDoorLightLevel = EARLY_DAY_LIGHT_VALUE;
		}
		if( m_timeOfDay >= EARLY_MORNING_TIME && m_timeOfDay < MID_MORNING_TIME )
		{
			g_globalOutDoorLightLevel = MID_DAY_LIGHT_VALUE;
		}
		if( m_timeOfDay >= MID_MORNING_TIME && m_timeOfDay < SUNSET_TIME )
		{
			g_globalOutDoorLightLevel = DAY_LIGHT_VALUE;
		}
		if( m_timeOfDay >=  SUNSET_TIME && m_timeOfDay < SUNSET_TIME + 4.f  )
		{
			g_globalOutDoorLightLevel = MID_DAY_LIGHT_VALUE;
		}
		if( m_timeOfDay >=  SUNSET_TIME + 4.f && m_timeOfDay < SUNSET_TIME + 8.f  )
		{
			g_globalOutDoorLightLevel = EARLY_DAY_LIGHT_VALUE;
		}
		if( m_timeOfDay >=  SUNSET_TIME + 8.f && m_timeOfDay < SUNSET_TIME + 12.f  )
		{
			g_globalOutDoorLightLevel = NIGHT_LIGHT_VALUE;
		}
		if( m_timeOfDay >= SUNSET_TIME + 12.f )
		{
			m_timeOfDay = 0.f;
		}
	}

	if( m_lastGlobalOutDoorLightLevel != g_globalOutDoorLightLevel )
	{
		m_doneCheckingBlockTime = false;
		m_lastGlobalOutDoorLightLevel = g_globalOutDoorLightLevel;
	}
}