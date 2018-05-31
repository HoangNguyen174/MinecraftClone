#ifndef GAMECOMMON_HEADER
#define GAMECOMMON_HEADER

#include <windows.h>
#include <stdlib.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <stack>
#include <map>
#include "glext.h"
#include "Vector2.hpp"
#include "Vector2i.hpp"
#include "Vector3.h"
#include "Vector3i.hpp"
#include <climits>
#include "AudioSystem.hpp"

using namespace std;
// Put externs for these globals in some header that everyone (who cares about drawing) can see
extern PFNGLGENBUFFERSPROC		glGenBuffers;
extern PFNGLBINDBUFFERPROC		glBindBuffer;
extern PFNGLBUFFERDATAPROC		glBufferData;
extern PFNGLGENERATEMIPMAPPROC	glGenerateMipmap;
extern PFNGLDELETEBUFFERSPROC   glDeleteBuffers;


const int CHUNK_WIDTH_X = 16;
const int CHUNK_DEPTH_Y = 16;
const int CHUNK_HEIGHT = 128;
const int CHUNK_SIZE_IN_BLOCK = CHUNK_WIDTH_X * CHUNK_DEPTH_Y * CHUNK_HEIGHT;
const int CHUNK_LAYER_SIZE = CHUNK_WIDTH_X * CHUNK_DEPTH_Y;
const int VERTEX_PER_FACE = 4;
const int FACE_PER_BLOCK = 6;
const int VERTEX_PER_CHUNK = CHUNK_SIZE_IN_BLOCK * VERTEX_PER_FACE * FACE_PER_BLOCK;
const unsigned int BOTTOM_4_BITS_MASK = 15;
const float SEA_LEVEL = 58.8f;
const float FAKE_SEA_LEVEL = 0.9f;
const unsigned char DAY_LIGHT_VALUE = 15;
const unsigned char NIGHT_LIGHT_VALUE = 5;
const unsigned char EARLY_DAY_LIGHT_VALUE = 9;
const unsigned char MID_DAY_LIGHT_VALUE = 13;
const unsigned char GLOW_STONE_LIGHT_VALUE = 13;
const float BOTTOM_GROUND = 0.f;
const unsigned char LIGHT_LEVEL_MASK = 15; //00001111
const unsigned char SKY_VISIBLE_MASK = 16; //00010000
const unsigned char DIRTY_MASK = 16; //0010000
const int CURRENT_TYPE_NUM = 12;
const int TEXTURE_WIDTH_Y = 32;
const int SUNRISE_TIME = 4;// over 24hour
const int EARLY_MORNING_TIME = 8;// over 24hour
const int MID_MORNING_TIME = 12;// over 24hour
const int SUNSET_TIME = 16;// over 24hour

enum Material { GRASS, DIRT, AIR, STONE, WATER, SAND, GLOW_STONE, GROUND, SNOW, GROW_GRASS1, DEATH_TREE , WOOD };
enum CardinalDirection { EAST, WEST, SOUTH, NORTH, TOP, BOTTOM, NONE };

typedef struct _BlockType
{
	unsigned int topTextureIndex;
	unsigned int sideTextureIndex;
	unsigned int bottomTextureIndex;
	bool isObaque;
	bool isSolid;
	bool isSpecialBlock;
	char* diggingSound[2];
	char* placingSound[2];
	char* stepOnSound[2];
} blockTypeInfo;

typedef struct _HitInfo
{
	float hitDepth;
	CardinalDirection hitFace;
	int hitBlockIndex;
	Vector2i hitChunkCoord2D;
	Vector3i worldBlockCoord;
	Vector3i localBlockCoord;
} HitInfo;

typedef struct _RGBColor
{
	float red;
	float green;
	float blue;
	float alpha;
	void SetColor(float mred,float mgreen,float mblue, float malpha)
	{
		red = mred;
		green = mgreen;
		blue = mblue;
		alpha = malpha;
	}
} RGBColor;

typedef struct _Vertex3D
{
	Vector3	m_position;
	RGBColor m_color;
	Vector2 m_texCoords;
} Vertex3D;

typedef struct _BlockInfo
{
	int m_blockLocalIndex;
	Vector2i m_chunkCoord;
	void Validate()
	{
		if(m_blockLocalIndex < 0 || m_blockLocalIndex >= CHUNK_SIZE_IN_BLOCK)
			__debugbreak();
	}
} BlockInfo;

extern set<Vector3> g_debugPoints;
extern vector<blockTypeInfo> g_blockTypeInfo;
extern unsigned char g_globalOutDoorLightLevel;
extern AudioSystem g_audioSystem;

inline int GetTextureIndexAtCoord2D(int x, int y)
{
	return x + TEXTURE_WIDTH_Y * y;
}

inline Vector3i GetBlockLocalCoord3DFromBlockIndex(int blockIndex)
{
	Vector3i blockLocalCoords3D;
	blockLocalCoords3D.z = blockIndex >> 8;//(int)blockIndex / CHUNK_LAYER_SIZE;
	blockLocalCoords3D.y = (blockIndex - blockLocalCoords3D.z * CHUNK_LAYER_SIZE) >> 4;//(int)(blockIndex - blockLocalCoords3D.z * CHUNK_LAYER_SIZE)/CHUNK_WIDTH;
	blockLocalCoords3D.x = (blockIndex - blockLocalCoords3D.z * CHUNK_LAYER_SIZE) & BOTTOM_4_BITS_MASK; // %CHUNK_WIDTH;

	return blockLocalCoords3D;
}

inline int GetBlockIndexFromLocalCoord3D(Vector3i localCoord3D)
{
	return localCoord3D.x + localCoord3D.y * CHUNK_DEPTH_Y + localCoord3D.z * CHUNK_LAYER_SIZE;
}

inline Vector2i ConvertWolrdBlockCoordToChunkCoord(Vector3i worldBlockCoord)
{
	Vector2i temp;

	temp.x = worldBlockCoord.x >> 4;
	temp.y = worldBlockCoord.y >> 4;

	return temp;
}

inline Vector2i ConvertWorldCoordToChunkCoord2D(Vector3 position)
{
	Vector2i chunkCoord;

	chunkCoord.x = static_cast<int>(floor(position.x)) >> 4;
	chunkCoord.y = static_cast<int>(floor(position.y)) >> 4;

	return chunkCoord;
}

inline Vector3i GetBlockWordCoord(int blockLocalIndex, Vector2i chunkCoord )
{
	Vector3i local3DCoord = GetBlockLocalCoord3DFromBlockIndex(blockLocalIndex);
	Vector3i position;
	position.x = local3DCoord.x + CHUNK_WIDTH_X * chunkCoord.x;
	position.y = local3DCoord.y + CHUNK_DEPTH_Y * chunkCoord.y;
	position.z = local3DCoord.z;

	return position;
}


inline Vector3i ConvertWolrdBlockCoordToLocalBlockCoord(Vector3i worldBlockCoord)
{
	Vector3i temp;
	Vector2i chunkCoord;

	chunkCoord = ConvertWolrdBlockCoordToChunkCoord(worldBlockCoord);

	if(worldBlockCoord.x >= 0)
		temp.x = worldBlockCoord.x % CHUNK_WIDTH_X;
	else
		temp.x = worldBlockCoord.x - CHUNK_WIDTH_X * chunkCoord.x;

	if(worldBlockCoord.y >= 0)
		temp.y = worldBlockCoord.y % CHUNK_DEPTH_Y;
	else
		temp.y = worldBlockCoord.y - CHUNK_DEPTH_Y * chunkCoord.y;

	temp.z = worldBlockCoord.z;

	return temp;
}


inline Vector3i ConvertWorldCoordToLocalBlockCoord(Vector3 position)
{
	Vector2i currentChunkPosition = ConvertWorldCoordToChunkCoord2D(position);
	Vector3i cameraLocalBlockCoord;

	cameraLocalBlockCoord.x = static_cast<int>(floor(position.x)) - CHUNK_WIDTH_X * currentChunkPosition.x;
	cameraLocalBlockCoord.y = static_cast<int>(floor(position.y)) - CHUNK_DEPTH_Y * currentChunkPosition.y;
	cameraLocalBlockCoord.z = static_cast<int>(floor( position.z ));

	return cameraLocalBlockCoord;
}

inline Vector3i ConvertWorldCoordToWorldBlockCoord(Vector3 position)
{
	Vector3i worldblockPosition;

	worldblockPosition.x = static_cast<int>(floor(position.x));
	worldblockPosition.y = static_cast<int>(floor(position.y));
	worldblockPosition.z = static_cast<int>(floor(position.z));

	return worldblockPosition;
}

extern queue<BlockInfo> g_listOfBlockToPropagateLight;

const unsigned int STRINGF_STACK_LOCAL_TEMP_LENGTH = 256;
//-----------------------------------------------------------------------------------------------
//Construct a temporary std::string using printf-style formatting.  For example:
//std::string studentInfoString = Stringf( "%s is a %d-year-old %s.", studentName, studentAge, studentGender );

inline const std::string Stringf( const char* format, ... )
{
	char textLiteral[ STRINGF_STACK_LOCAL_TEMP_LENGTH ];
	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, STRINGF_STACK_LOCAL_TEMP_LENGTH, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ STRINGF_STACK_LOCAL_TEMP_LENGTH - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	return std::string( textLiteral );
}


//-----------------------------------------------------------------------------------------------
//Same as the above version of Stringf(), but with an explicitly chosen maximum buffer size.

inline const std::string Stringf( const int maxLength, const char* format, ... )
{
	char textLiteralSmall[ STRINGF_STACK_LOCAL_TEMP_LENGTH ];
	char* textLiteral = textLiteralSmall;
	if( maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH )
		textLiteral = new char[ maxLength ];

	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, maxLength, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ maxLength - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	std::string returnValue( textLiteral );
	if( maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH )
		delete[] textLiteral;

	return returnValue;
}



//-----------------------------------------------------------------------------------------------
//Print a string to the Visual Studio "Output" debug window, if we are running in the debugger.
//Uses printf-style formatting, for example:
//DebuggerPrintf( "%s is a %d-year-old %s.", studentName, studentAge, studentGender );

inline void DebuggerPrintf( const char* messageFormat, ... )
{
	const int MESSAGE_MAX_LENGTH = 2048;
	char messageLiteral[ MESSAGE_MAX_LENGTH ];
	va_list variableArgumentList;
	va_start( variableArgumentList, messageFormat );
	vsnprintf_s( messageLiteral, MESSAGE_MAX_LENGTH, _TRUNCATE, messageFormat, variableArgumentList );
	va_end( variableArgumentList );
	messageLiteral[ MESSAGE_MAX_LENGTH - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	//#if defined( PLATFORM_WINDOWS )
	//	if( IsDebuggerAvailable() )
	//	{
	OutputDebugStringA( messageLiteral ); // Print the string to the "Output" debug window in Visual Studio
	//	}
	//#endif

	std::cout << messageLiteral; // Also write it out to standard out, if anyone is listening
}

#endif