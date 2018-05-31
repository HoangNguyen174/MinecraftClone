#include "Player.hpp"

Player::Player()
{
	m_orientation = 0.f;

	m_modeIndex = 0;

	m_movementMode = WALKING ;

	m_bottomCenterPosition.SetXYZ(0.f,0.f,80.f);

	m_boundingBox.SetMinMax( Vector3( m_bottomCenterPosition.x - PLAYER_HALF_WIDTH, m_bottomCenterPosition.y - PLAYER_HALF_WIDTH, m_bottomCenterPosition.z),
							 Vector3( m_bottomCenterPosition.x + PLAYER_HALF_WIDTH, m_bottomCenterPosition.y + PLAYER_HALF_WIDTH, m_bottomCenterPosition.z + PLAYER_HEIGHT ) );

	m_eyePosition.SetXYZ(m_bottomCenterPosition.x + PLAYER_HALF_WIDTH, 
						 m_bottomCenterPosition.y + PLAYER_HALF_WIDTH,
						 m_bottomCenterPosition.z + PlAYER_EYES_HEIGHT);

	m_jumpForce = 35.f;
	m_isOnGround = false;
	m_rainTexture = Texture::CreateOrGetTexture("rain.jpg");
	m_texScrollX = 1.f;
	m_texScrollY = 1.f;
}

Player::~Player()
{

}

void Player::RenderWaterInFrontOfEyes()
{
	Vector3 temp;
	if(  m_eyePosition.z < SEA_LEVEL )
	{
		GLRender::Disable(GL_TEXTURE_2D);//glDisable(GL_TEXTURE_2D);
		GLRender::Enable(GL_BLEND);//glEnable(GL_BLEND);
		GLRender::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		GLRender::PushMatrix();//glPushMatrix();
		GLRender::Translatef(m_bottomCenterPosition.x,m_bottomCenterPosition.y,m_bottomCenterPosition.z);//glTranslatef(m_bottomCenterPosition.x,m_bottomCenterPosition.y,m_bottomCenterPosition.z);
		GLRender::Color4f(0.19f,0.26f,.57f,0.7f);//glColor4f(0.19f,0.26f,.57f,0.7f);
		GLRender::FrontFace(GL_CW);//glFrontFace(GL_CW);
		GLRender::PointSize(10.f);//glPointSize(10.f);
		GLRender::BeginDraw(GL_QUADS);//glBegin(GL_QUADS);
		{
			//bottom
			GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z);//glVertex3f(	temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z );
			GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z );//glVertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z );
			GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z);//glVertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z );
			GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z);//glVertex3f( temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z );

			//top
			GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT);//glVertex3f(	temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );
			GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT);//glVertex3f(	temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );
			GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT);//glVertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );
			GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );//glVertex3f( temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );

			//west
			GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT);//glVertex3f(	temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );
			GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z);//glVertex3f(	temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z );
			GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z);//glVertex3f(	temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z );
			GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT);//glVertex3f( temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );

			//east	 
			GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT);//glVertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );
			GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT);//glVertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );
			GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z);//glVertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z );
			GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z);//glVertex3f( temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z ); 

			//south	 
			GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT);//glVertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );
			GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z);//glVertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z );
			GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z);//glVertex3f(	temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z );
			GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT);//glVertex3f( temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );

			//north	 
			GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT);//glVertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );
			GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT);//glVertex3f(	temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );
			GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z);//glVertex3f(	temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z );
			GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z);//glVertex3f( temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z );
		}
		GLRender::EndDraw();//glEnd();
		GLRender::PopMatrix();//glPopMatrix();
		GLRender::FrontFace(GL_CCW);//glFrontFace(GL_CCW);
		GLRender::Enable(GL_TEXTURE_2D);//glEnable(GL_TEXTURE_2D);
		GLRender::Enable(GL_DEPTH_TEST);//glEnable(GL_DEPTH_TEST);
	}
}

bool Player::IsPlayerInWater()
{
	ChunkBlocks* currentChunk = nullptr;
	//Block currentBlock;
	Vector3i currentBlock = ConvertWorldCoordToWorldBlockCoord(m_bottomCenterPosition);

	Vector3i currentLocalBlockCoord = ConvertWolrdBlockCoordToLocalBlockCoord(currentBlock);

	Vector2i currentWorldChunkCoord = ConvertWolrdBlockCoordToChunkCoord(currentBlock);

	if(g_mapChunk.find(currentWorldChunkCoord) != g_mapChunk.end())
		currentChunk = g_mapChunk[ currentWorldChunkCoord ];

	int currentBlockIndex = GetBlockIndexFromLocalCoord3D(currentLocalBlockCoord);

	if( currentChunk != nullptr && currentChunk->m_blocksList[currentBlockIndex].m_blockType.material == WATER )
	{
		return true;
	}
	return false;
}

bool Player::IsPlayerOnGround()
{
	Vector3 minPoint = m_boundingBox.m_minPoint;
	Vector3 maxPoint = m_boundingBox.m_maxPoint;

	//minPoint.z -= 0.1f;
	//maxPoint.z -= 0.1f;

	Vector3i currBlockOfMin = ConvertWorldCoordToWorldBlockCoord(minPoint);
	Vector3i currBlockOfMax = ConvertWorldCoordToWorldBlockCoord(maxPoint);

	currBlockOfMin.z--;
	currBlockOfMax.z--;

	ChunkBlocks* currentChunk = nullptr;

	for(int y = currBlockOfMin.y ; y <= currBlockOfMax.y; y++)
	{
		for(int x = currBlockOfMin.x; x <= currBlockOfMax.x; x++)
		{
			Vector3i currBlock = Vector3i( x, y, currBlockOfMin.z  );

			Vector2i currentWorldChunkCoord = ConvertWolrdBlockCoordToChunkCoord(currBlock);

			if(g_mapChunk.find(currentWorldChunkCoord) != g_mapChunk.end())
				currentChunk = g_mapChunk[ currentWorldChunkCoord ];

			Vector3i currentLocalBlockCoord = ConvertWolrdBlockCoordToLocalBlockCoord(currBlock);

			int currentBlockIndex = GetBlockIndexFromLocalCoord3D(currentLocalBlockCoord);

			if( currentChunk != nullptr && currentChunk->m_blocksList[currentBlockIndex].m_blockType.material != AIR 
				&& currentChunk->m_blocksList[currentBlockIndex].m_blockType.material != WATER)
			{
				return true;
			}
		}
	}
	return false;
}

void Player::UpdatePlayerPosition(Vector3 movementIntentVector)
{
	Vector3 realMovementVector;

	realMovementVector = ProcessingCollistionDetection(movementIntentVector);
	
	if(MathUtilities::CompareTwoFloatNumbers(m_movementVector.z,0.f,EPSILON))
	{
		m_isOnGround = true;
	}

	if(!IsPlayerOnGround())
		m_isOnGround = false;

	if(m_bottomCenterPosition.z > CHUNK_HEIGHT - 1)
		realMovementVector.z = 0.f;

	m_movementVector = realMovementVector;

	m_bottomCenterPosition += realMovementVector;

	m_eyePosition.SetXYZ(m_bottomCenterPosition.x,// + PLAYER_HALF_WIDTH , 
						 m_bottomCenterPosition.y,// + PLAYER_HALF_WIDTH,
						 m_bottomCenterPosition.z + PlAYER_EYES_HEIGHT);

	m_boundingBox.SetMinMax( Vector3( m_bottomCenterPosition.x - PLAYER_HALF_WIDTH, m_bottomCenterPosition.y - PLAYER_HALF_WIDTH, m_bottomCenterPosition.z ),
							 Vector3( m_bottomCenterPosition.x + PLAYER_HALF_WIDTH, m_bottomCenterPosition.y + PLAYER_HALF_WIDTH, m_bottomCenterPosition.z + PLAYER_HEIGHT ) );
}

Vector3 Player::ProcessingCollistionDetection(Vector3 movementIntentVector)
{
	if( movementIntentVector.x < 0.f )
		m_leadingCorner.x = m_boundingBox.m_minPoint.x;
	else 
		m_leadingCorner.x = m_boundingBox.m_maxPoint.x;

	if( movementIntentVector.y < 0.f)
		m_leadingCorner.y = m_boundingBox.m_minPoint.y;
	else 
		m_leadingCorner.y = m_boundingBox.m_maxPoint.y;

	if( movementIntentVector.z < 0.f )
		m_leadingCorner.z = m_boundingBox.m_minPoint.z;
	else
		m_leadingCorner.z = m_boundingBox.m_maxPoint.z;

// 	Vector3 leadCornerToCenterVec = m_bottomCenterPosition - m_leadingCorner;
// 	Vector3 offSetVector = leadCornerToCenterVec * .1f;
// 	Vector3 checkCorner = m_leadingCorner + offSetVector;
// 
// 	Vector3i currentBlock = ConvertWorldCoordToWorldBlockCoord( checkCorner );
// 
// 	Vector2i currentWorldChunkCoord = ConvertWolrdBlockCoordToChunkCoord(currentBlock);
// 
// 	ChunkBlocks* currentChunk = nullptr;
// 
// 	if(g_mapChunk.find(currentWorldChunkCoord) != g_mapChunk.end())
// 		currentChunk = g_mapChunk[ currentWorldChunkCoord ];
// 
// 	Vector3i currentLocalBlockCoord = ConvertWolrdBlockCoordToLocalBlockCoord(currentBlock);
// 
// 	int currentBlockIndex = GetBlockIndexFromLocalCoord3D(currentLocalBlockCoord);
// 
// 	if( currentChunk != nullptr && g_blockTypeInfo[currentChunk->m_blocksList[currentBlockIndex].m_blockType.material].isSolid  )
// 	{
// 		movementIntentVector = Vector3(0.f,0.f,0.f);
// 		return movementIntentVector;
// 	}

	return ApproveOrModifyMovementIntention( m_leadingCorner,movementIntentVector ); //ApproveOrModifyMovementIntention( checkCorner,movementIntentVector );
		 
}

Vector3 Player::ApproveOrModifyMovementIntention( Vector3 leadingCornerStart, Vector3 movementIntentVector )
{
	Vector3 leadingCornerCurrent = leadingCornerStart; 

	Vector3i currentBlockCoord;
	Vector2i currentWorldChunkCoord = ConvertWorldCoordToChunkCoord2D(leadingCornerCurrent);

	Vector3i nextBlockCoord;
	Vector2i nextWorldChunkCoord;

	Vector3i blockCoordDelta;

	Vector3 startRay;
	Vector3 nextRayPosition;
	Vector3 currentRayPosition;

	Vector3 forwardDirection = movementIntentVector;

	Vector3 minPoint;
	Vector3 maxPoint;

	float percentageRemaining = 1.f;
	startRay = leadingCornerCurrent;
	currentRayPosition = startRay;

	nextRayPosition = currentRayPosition + forwardDirection;
	
	ChunkBlocks* currentChunk = nullptr;

	if(g_mapChunk.find(currentWorldChunkCoord) != g_mapChunk.end())
		currentChunk = g_mapChunk[currentWorldChunkCoord];

	Vector3i startBlockCoord = ConvertWorldCoordToWorldBlockCoord( startRay );
	currentBlockCoord = startBlockCoord;

	//------------------------------------------------------------------------------------------------
	Vector3 leadCornerToCenterVec = m_bottomCenterPosition - m_leadingCorner;
	Vector3 offSetVector = leadCornerToCenterVec * .1f;
	Vector3 checkCorner = m_leadingCorner + offSetVector;

	Vector3i currentCheckBlock = ConvertWorldCoordToWorldBlockCoord( checkCorner );

	Vector2i currentCheckWorldChunkCoord = ConvertWolrdBlockCoordToChunkCoord(currentCheckBlock);

	ChunkBlocks* currentCheckChunk = nullptr;

	if(g_mapChunk.find(currentCheckWorldChunkCoord) != g_mapChunk.end())
		currentCheckChunk = g_mapChunk[ currentCheckWorldChunkCoord ];

	Vector3i currentLocalBlockCoord = ConvertWolrdBlockCoordToLocalBlockCoord(currentCheckBlock);

	int currentBlockIndex = GetBlockIndexFromLocalCoord3D(currentLocalBlockCoord);

	if( currentCheckChunk != nullptr && g_blockTypeInfo[currentCheckChunk->m_blocksList[currentBlockIndex].m_blockType.material].isSolid  )
	{
		movementIntentVector = Vector3(0.f,0.f,0.f);
		return movementIntentVector;
	}
	//-------------------------------------------------------------------------------------------------------
	while(percentageRemaining > 0.f)
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
		CardinalDirection faceToCheck = NONE;

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

		if(crossX)
		{
			if(currentBlockCoord.x > startBlockCoord.x)
			{
				faceToCheck = WEST;
				minPoint = m_boundingBox.m_minPoint;
				minPoint.x += PLAYER_WIDTH;
				maxPoint = m_boundingBox.m_maxPoint;
			}
			else if( currentBlockCoord.x < startBlockCoord.x)
			{
				faceToCheck = EAST;
				minPoint = m_boundingBox.m_minPoint;
				maxPoint = m_boundingBox.m_maxPoint;
				maxPoint.x -= PLAYER_WIDTH ;
			}

			Vector3i currBlockOfMin = ConvertWorldCoordToWorldBlockCoord(minPoint);
			Vector3i currBlockOfMax = ConvertWorldCoordToWorldBlockCoord(maxPoint);

			if(faceToCheck == WEST)
			{
				currBlockOfMin.x++;
				currBlockOfMax.x++;
			}
			else if(faceToCheck == EAST)
			{
				currBlockOfMin.x--;
				currBlockOfMax.x--;
			}

			for(int z = currBlockOfMin.z ; z <= currBlockOfMax.z; z++)
			{
				for(int y = currBlockOfMin.y; y <= currBlockOfMax.y; y++)
				{
					Vector3i currBlock = Vector3i( currBlockOfMin.x, y, z  );

					currentWorldChunkCoord = ConvertWolrdBlockCoordToChunkCoord(currBlock);

					if(g_mapChunk.find(currentWorldChunkCoord) != g_mapChunk.end())
						currentChunk = g_mapChunk[ currentWorldChunkCoord ];

					Vector3i currentLocalBlockCoord = ConvertWolrdBlockCoordToLocalBlockCoord(currBlock);

					int currentBlockIndex = GetBlockIndexFromLocalCoord3D(currentLocalBlockCoord);

					if( currentChunk != nullptr && g_blockTypeInfo[currentChunk->m_blocksList[currentBlockIndex].m_blockType.material].isSolid ) 
					{
						movementIntentVector.x = 0.f;
						movementIntentVector = movementIntentVector * smallestDistanceToAxisIntercept;
						m_isOnGround = true;
						return movementIntentVector;
					}
				}
			}
		}
	    else if(crossY)
		{
			if(currentBlockCoord.y > startBlockCoord.y)
			{
				faceToCheck = NORTH;
				minPoint = m_boundingBox.m_minPoint;
				minPoint.y += PLAYER_WIDTH;
				maxPoint = m_boundingBox.m_maxPoint;
			}
			else if( currentBlockCoord.y < startBlockCoord.y)
			{
				faceToCheck = SOUTH;
				minPoint = m_boundingBox.m_minPoint;
				maxPoint = m_boundingBox.m_maxPoint;
				maxPoint.y -= PLAYER_WIDTH;
			}

			Vector3i currBlockOfMin = ConvertWorldCoordToWorldBlockCoord(minPoint);
			Vector3i currBlockOfMax = ConvertWorldCoordToWorldBlockCoord(maxPoint);

			if(faceToCheck == NORTH)
			{
				currBlockOfMin.y++;
				currBlockOfMax.y++;
			}
			else if(faceToCheck == SOUTH)
			{
				currBlockOfMin.y--;
				currBlockOfMax.y--;
			}

			for(int z = currBlockOfMin.z ; z <= currBlockOfMax.z; z++)
			{
				for(int x = currBlockOfMin.x; x <= currBlockOfMax.x; x++)
				{
					Vector3i currBlock = Vector3i( x, currBlockOfMin.y, z  );

					currentWorldChunkCoord = ConvertWolrdBlockCoordToChunkCoord(currBlock);

					if(g_mapChunk.find(currentWorldChunkCoord) != g_mapChunk.end())
						currentChunk = g_mapChunk[ currentWorldChunkCoord ];

					Vector3i currentLocalBlockCoord = ConvertWolrdBlockCoordToLocalBlockCoord(currBlock);

					int currentBlockIndex = GetBlockIndexFromLocalCoord3D(currentLocalBlockCoord);

					if( currentChunk != nullptr && g_blockTypeInfo[currentChunk->m_blocksList[currentBlockIndex].m_blockType.material].isSolid )
					{
						movementIntentVector.y = 0.f;
						movementIntentVector = movementIntentVector * smallestDistanceToAxisIntercept;
						return movementIntentVector;
					}
				}
			}
		}
		else if(crossZ)
		{
			if(currentBlockCoord.z > startBlockCoord.z)
			{
				faceToCheck = TOP;
				minPoint = m_boundingBox.m_minPoint;
				minPoint.z += PLAYER_HEIGHT;
				maxPoint = m_boundingBox.m_maxPoint;
			}
			else if( currentBlockCoord.z < startBlockCoord.z)
			{
				faceToCheck = BOTTOM;
				minPoint = m_boundingBox.m_minPoint;
				maxPoint = m_boundingBox.m_maxPoint;
				maxPoint.z -= PLAYER_HEIGHT ;
			}

			Vector3i currBlockOfMin = ConvertWorldCoordToWorldBlockCoord(minPoint);
			Vector3i currBlockOfMax = ConvertWorldCoordToWorldBlockCoord(maxPoint);

			if(faceToCheck == TOP)
			{
				currBlockOfMin.z++;
				currBlockOfMax.z++;
			}
			else if(faceToCheck == BOTTOM)
			{
				currBlockOfMin.z--;
				currBlockOfMax.z--;
			}

			for(int y = currBlockOfMin.y ; y <= currBlockOfMax.y; y++)
			{
				for(int x = currBlockOfMin.x; x <= currBlockOfMax.x; x++)
				{
					Vector3i currBlock = Vector3i( x, y, currBlockOfMin.z  );

					currentWorldChunkCoord = ConvertWolrdBlockCoordToChunkCoord(currBlock);

					if(g_mapChunk.find(currentWorldChunkCoord) != g_mapChunk.end())
						currentChunk = g_mapChunk[ currentWorldChunkCoord ];

					Vector3i currentLocalBlockCoord = ConvertWolrdBlockCoordToLocalBlockCoord(currBlock);

					int currentBlockIndex = GetBlockIndexFromLocalCoord3D(currentLocalBlockCoord);

					if( currentChunk != nullptr && g_blockTypeInfo[currentChunk->m_blocksList[currentBlockIndex].m_blockType.material].isSolid)
					{
						movementIntentVector.z = 0.f;
						movementIntentVector = movementIntentVector * smallestDistanceToAxisIntercept;
						return movementIntentVector;
					}
				}
			}
		}

		if(smallestDistanceToAxisIntercept != FLT_MAX)
			percentageRemaining -= smallestDistanceToAxisIntercept;
		else
			percentageRemaining = 0.f;
	}
	return forwardDirection;
}

void Player::SetRotateRate(float value)
{
	m_rotateRate = value;
}

void Player::Update(float elapsedTime)
{
	//float r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)/0.005);

	m_texScrollX -= 0.005f;
	m_texScrollY -= 0.005f;

	if(m_texScrollX <= 0.f)
		m_texScrollX = 1.f;
	if(m_texScrollY <= 0.f)
		m_texScrollY = 1.f;
	if(m_modeIndex == 0)
	{
		m_movementMode = WALKING;
		m_playerSpeed = WALKING_SPEED;
	}
	else if(m_modeIndex == 1)
	{
		m_movementMode = FLY;
		m_playerSpeed = FLYING_SPEED;
	}
	else if(m_modeIndex == 2)
	{
		m_movementMode = NOCLIP;
		m_playerSpeed = FLYING_SPEED;
	}

	m_eyePosition.SetXYZ(m_bottomCenterPosition.x,// + PLAYER_HALF_WIDTH , 
						 m_bottomCenterPosition.y,// + PLAYER_HALF_WIDTH,
						 m_bottomCenterPosition.z + PlAYER_EYES_HEIGHT);

	m_boundingBox.SetMinMax( Vector3( m_bottomCenterPosition.x - PLAYER_HALF_WIDTH, m_bottomCenterPosition.y - PLAYER_HALF_WIDTH, m_bottomCenterPosition.z ),
						     Vector3( m_bottomCenterPosition.x + PLAYER_HALF_WIDTH, m_bottomCenterPosition.y + PLAYER_HALF_WIDTH, m_bottomCenterPosition.z + PLAYER_HEIGHT ) );

	PlaySoundBasedOnBlockTypeUnderPlayer();

	g_audioSystem.UpdateTimer(elapsedTime);

}

void Player::RenderPlayerAABB()
{
	Vector3 temp(0.f,0.f,0.f);

	GLRender::PushMatrix();//glPushMatrix();
	GLRender::Disable(GL_TEXTURE_2D);//glDisable(GL_TEXTURE_2D);
	GLRender::Enable(GL_DEPTH_TEST);//glEnable(GL_DEPTH_TEST);
	GLRender::Color4f(1.f,1.f,0.f,1.f);//glColor4f(1.f,1.f,0.f,1.f);
	GLRender::Translatef(m_bottomCenterPosition.x,m_bottomCenterPosition.y,m_bottomCenterPosition.z);//glTranslatef(m_bottomCenterPosition.x,m_bottomCenterPosition.y,m_bottomCenterPosition.z);
	GLRender::BeginDraw(GL_LINES);//glBegin(GL_LINES);
	{
		//bottom
		GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z);//glVertex3f(	temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z );
		GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z);//glVertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z );

		GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z);//glVertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z );
		GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z);//glVertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z );

		GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z );//glVertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z );
		GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z);//glVertex3f( temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z );

		GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z);//glVertex3f( temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z );
		GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z);//glVertex3f(	temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z );

		//top
		GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z);//glVertex3f(	temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z );
		GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );//glVertex3f(	temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );

		GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z);//glVertex3f( temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z );
		GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT);//glVertex3f(	temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );

		GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z);//glVertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z );
		GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT);//glVertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );

		GLRender::Vertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z);//glVertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z );
		GLRender::Vertex3f( temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT);//glVertex3f( temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );

		//west
		GLRender::Vertex3f(	temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT);//glVertex3f(	temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );
		GLRender::Vertex3f( temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z);//glVertex3f(	temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z );

		GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z);//glVertex3f(	temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z );
		GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z);//glVertex3f(	temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z );

		GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z);//glVertex3f(	temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z );
		GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT);//glVertex3f( temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );

		GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT);//glVertex3f( temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );
		GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT);//glVertex3f(	temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );

		//east	 
		GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT);//glVertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );
		GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT);//glVertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );

		GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );//glVertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );
		GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z);//glVertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z );

		GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z);//glVertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z );
		GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z);//glVertex3f( temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z );

		GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z );//glVertex3f( temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z );
		GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT);//glVertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );	 

		//south	 
		GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT);//glVertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );
		GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z);//glVertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z );

		GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z);//glVertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z );
		GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z);//glVertex3f(	temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z );

		GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z);//glVertex3f(	temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z );
		GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT);//glVertex3f( temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );

		GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT);//glVertex3f( temp.x + PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );
		GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT);//glVertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y - PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );		 
		//north	 
		GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT);//glVertex3f(	temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );
		GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );//glVertex3f(	temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z + PLAYER_HEIGHT );
		GLRender::Vertex3f(temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z);//glVertex3f(	temp.x + PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z );
		GLRender::Vertex3f(temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z);//glVertex3f( temp.x - PLAYER_HALF_WIDTH, temp.y + PLAYER_HALF_WIDTH, temp.z );
	}
	GLRender::EndDraw();//glEnd();
	GLRender::PolygonMode(GL_FRONT_AND_BACK,GL_FILL);//glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	GLRender::Enable(GL_TEXTURE_2D);//glEnable(GL_TEXTURE_2D);
	GLRender::Disable(GL_DEPTH_TEST);//glDisable(GL_DEPTH_TEST);
	GLRender::PopMatrix();//glPopMatrix();
}

void Player::Render()
{
	RenderWaterInFrontOfEyes();
	//RenderRainAroundPlayer();
}

void Player::PlaySoundBasedOnBlockTypeUnderPlayer()
{
	Material currentBlockType;
	char* soundToPlay;
	int random;
	float playerSpeed = m_movementVector.CalcLength();
	if(playerSpeed != 0.f && m_movementMode == WALKING)
	{
		if(IsPlayerOnGround())
		{
			currentBlockType = GetBlockTypeOfBlockUnderPlayer();
			random = MathUtilities::GetRandomNumber(0,1);
			soundToPlay = g_blockTypeInfo[currentBlockType].stepOnSound[random];
			if(g_audioSystem.m_secondRemainingTillNextSound <= 0.f)
			{
				g_audioSystem.PlaySoundByName(soundToPlay,1,false);
				g_audioSystem.SetTimer(playerSpeed);
			}
		}
	}
}

void Player::RenderRainAroundPlayer()
{
	Vector3 temp(5.f,5.f,5.f);

	GLRender::Enable(GL_TEXTURE_2D);//glDisable(GL_TEXTURE_2D);
	GLRender::Enable(GL_BLEND);//glEnable(GL_BLEND);
	//GLRender::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.5f);
	GLRender::PushMatrix();//glPushMatrix();
	GLRender::Translatef(m_bottomCenterPosition.x,m_bottomCenterPosition.y,0.f);
	GLRender::Scalef(.1f,.1f,1.f);
	GLRender::Color4f(1.f,1.f,1.f,1.f);//glColor4f(0.19f,0.26f,.57f,0.7f);
	GLRender::FrontFace(GL_CW);//glFrontFace(GL_CW);
	GLRender::BindTexture(GL_TEXTURE_2D,m_rainTexture->m_openglTextureID);

	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	
	//GLRender::PointSize(10.f);//glPointSize(10.f);
	for(int i = 1; i < 100; i++)
	{
		GLRender::Rotatef(i * 45.f,0.f,0.f,1.f);
		//GLRender::Scalef( i,i,1.f);
		GLRender::BeginDraw(GL_QUADS);//glBegin(GL_QUADS);
		{
			//west
			GLRender::TexCoord2f(0.f + m_texScrollX,0.f + m_texScrollY);
			GLRender::Vertex3f(1.f * i , -1.f * i, temp.z + CHUNK_HEIGHT);

			GLRender::TexCoord2f(1.f + m_texScrollX ,0.f + m_texScrollY);
			GLRender::Vertex3f(1.f * i, -1.f * i, temp.z);

			GLRender::TexCoord2f(1.f + m_texScrollX,1.f + m_texScrollY);
			GLRender::Vertex3f(1.f * i, 1.f * i, temp.z);

			GLRender::TexCoord2f(0.f + m_texScrollY,1.f + m_texScrollY);
			GLRender::Vertex3f(1.f * i, 1.f * i, temp.z + CHUNK_HEIGHT);

			//east
			GLRender::TexCoord2f(0.f + m_texScrollX,0.f + m_texScrollY);
			GLRender::Vertex3f(-1.f * i, -1.f * i, temp.z + CHUNK_HEIGHT);

			GLRender::TexCoord2f(1.f + m_texScrollX ,0.f + m_texScrollY);
			GLRender::Vertex3f(-1.f * i, 1.f * i, temp.z + CHUNK_HEIGHT);

			GLRender::TexCoord2f(1.f + m_texScrollX,1.f + m_texScrollY);
			GLRender::Vertex3f(-1.f * i, 1.f * i, temp.z);

			GLRender::TexCoord2f(0.f + m_texScrollY,1.f + m_texScrollY);
			GLRender::Vertex3f(-1.f * i, -1.f * i, temp.z);

			//south	
			GLRender::TexCoord2f(0.f + m_texScrollX,0.f + m_texScrollY);
			GLRender::Vertex3f(-1.f * i, -1.f * i, temp.z + CHUNK_HEIGHT);

			GLRender::TexCoord2f(1.f + m_texScrollX ,0.f + m_texScrollY);
			GLRender::Vertex3f(-1.f * i, -1.f * i, temp.z);

			GLRender::TexCoord2f(1.f + m_texScrollX,1.f + m_texScrollY);
			GLRender::Vertex3f(1.f * i, -1.f * i, temp.z);

			GLRender::TexCoord2f(0.f + m_texScrollY,1.f + m_texScrollY);
			GLRender::Vertex3f(1.f * i, -1.f * i, temp.z + CHUNK_HEIGHT);

			//north	
			GLRender::TexCoord2f(0.f + m_texScrollX,0.f + m_texScrollY);
			GLRender::Vertex3f(-1.f * i, 1.f * i, temp.z + CHUNK_HEIGHT);

			GLRender::TexCoord2f(1.f + m_texScrollX ,0.f + m_texScrollY);
			GLRender::Vertex3f(1.f * i, 1.f * i, temp.z + CHUNK_HEIGHT);

			GLRender::TexCoord2f(1.f + m_texScrollX,1.f + m_texScrollY);
			GLRender::Vertex3f(1.f * i, 1.f * i, temp.z);

			GLRender::TexCoord2f(0.f + m_texScrollY,1.f + m_texScrollY);
			GLRender::Vertex3f(-1.f * i, 1.f * i, temp.z);
		}
		GLRender::EndDraw();//glEnd();
	}

	GLRender::PopMatrix();//glPopMatrix();
	GLRender::FrontFace(GL_CCW);//glFrontFace(GL_CCW);
	GLRender::Enable(GL_TEXTURE_2D);//glEnable(GL_TEXTURE_2D);
	GLRender::Enable(GL_DEPTH_TEST);//glEnable(GL_DEPTH_TEST);
}

Material Player::GetBlockTypeOfBlockUnderPlayer()
{
	ChunkBlocks* currentChunk = nullptr;

	Vector3 temp = m_bottomCenterPosition - Vector3(0.f,0.f,0.5f);

	Vector3i currentBlock = ConvertWorldCoordToWorldBlockCoord(temp);

	Vector3i currentLocalBlockCoord = ConvertWolrdBlockCoordToLocalBlockCoord(currentBlock);

	Vector2i currentWorldChunkCoord = ConvertWolrdBlockCoordToChunkCoord(currentBlock);

	if(g_mapChunk.find(currentWorldChunkCoord) != g_mapChunk.end())
		currentChunk = g_mapChunk[ currentWorldChunkCoord ];

	int currentBlockIndex = GetBlockIndexFromLocalCoord3D(currentLocalBlockCoord);

	if( currentChunk != nullptr )
		return currentChunk->m_blocksList[currentBlockIndex].m_blockType.material;
	return AIR;
}