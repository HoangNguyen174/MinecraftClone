#ifndef WORLD_H
#define WORLD_H
#include <vector>
#include <Xinput.h>
#include "Timer.h"
#include "Vector3.h"
#include "Camera3D.hpp"
#include "Vector2.hpp"
#include "Vector2i.hpp"
#include "MathUtilities.hpp"
#include "Texture.hpp"
#include "GameTexture.hpp"
#include "ChunkBlocks.hpp"
#include "MapChunks.hpp"
#include "GameCommon.hpp"
#include "Player.hpp"
#include <set>
#include "WindowFunc.hpp"

using namespace std;

class World
{
	public:
		Player m_player;
		Camera3D m_camera3D;
		ChunkBlocks m_testChunk;
		MapChunks m_map;
		Texture* m_skyBoxTexture;
		Texture* m_sunTexture;
		Texture* m_moonTexture;
		float m_skyBoxTextWidth;
		float m_skyBoxTextHeight;
		Vector3 m_cameraForwardVector;
		HitInfo m_hitInfo;
		bool m_leftMouseDown;
		bool m_rightMouseDown;
		Vector2 m_sunPos;
		Vector2 m_moonPos;
		float m_sunMoonRotateRate;
		Material m_currentBlockTypeToPlace;
		float m_jumpForce;
		Vector2 m_minSeaSurface;
		Vector2 m_maxSeaSurface;
		float m_buoyancyForce;
		Material m_currentBlockType;
		float m_timeOfDay;
		bool m_startDay;
		bool m_endDay;
		bool m_doneCheckingBlockTime;
		unsigned int m_chunkCount;
		int m_lastGlobalOutDoorLightLevel;

	public:
		World();
		~World();
		void Initialize();
		bool ProcessKeyDownEvent(HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam );
		bool ProcessKeyUpEnvent(HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam );
		void SetupPerspectiveProjection();
		void ApplyCameraTransform(Camera3D camera);
		void UpdateCameraFromKeyboardAndMouse(Camera3D& m_camera3D,float elapsedTime);
		Vector2 GetMouseSinceLastChecked();
		HitInfo RayTrace();
		void PlaceBlock(HitInfo hitInfo);
		void KillBlock(HitInfo hitInfo);
		void ReplaceAirWithCurrentBlockType(Block* block,ChunkBlocks* currentChunk, int blockIndex);
		void PushNeighborBlockToLightList(int blockIndex, ChunkBlocks* currentChunk);
		void PushAirBlockBelowSolidBlockToLightList(int blockIndex, ChunkBlocks* currentChunk);
		void SetAllAirBLockBelowToSkyBlock(int blockIndex, ChunkBlocks* currentChunk);
		void CalculateMinMaxXYOfSeaSurface();
		void Update(float elapsedTime);
		void UpdateWorldTimeAndLightLevel(float elaspedTime);
		void Render();
		void RenderWorldAxes();
		void RenderDebugPoints();
		void RenderSkyBox();
		void RenderMoon();
		void RenderSun();
		void RenderRay();
		void RenderSelectedBlock(HitInfo hitInfo);
		void RenderSeaSurface();
		void ProcessToLightList();
};

extern const int WINDOW_PHYSICAL_WIDTH;
extern const int WINDOW_PHYSICAL_HEIGHT;
extern World* theWorld;
extern bool IsKeyDownKeyboard[256];
extern bool IsKeyDownLastFrame[256];

#endif