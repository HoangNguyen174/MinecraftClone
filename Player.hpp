#ifndef PLAYER_H
#define PLAYER_H
#include <Windows.h>
#include <gl/gl.h>
#include "MathUtilities.hpp"
#include "AABB3.hpp"
#include "Vector3.h"
#include "Vector3i.hpp"
#include "ChunkBlocks.hpp"
#include "Camera3D.hpp"
#include "GameCommon.hpp"
#include "MapChunks.hpp"

const float PLAYER_HEIGHT = 1.85f;
const float PlAYER_EYES_HEIGHT = 1.62f;
const float PLAYER_WIDTH = 0.6f;
const float PLAYER_HALF_WIDTH = 0.5f * PLAYER_WIDTH;
const float GRAVITY = -10.f;
const float WALKING_SPEED = 9.f;
const float FLYING_SPEED = 18.f;

enum PlayerMode { WALKING, FLY, NOCLIP };

class Player
{
	public:
		Vector3 m_bottomCenterPosition;
		Vector3 m_eyePosition;
		float m_orientation;
		AABB3 m_boundingBox;
		Vector3 m_velocity;
		ChunkBlocks* m_currentChunk;
		Vector3i m_worldBlockCoord;
		Vector3 m_gravity;
		Vector3 m_movementVector;
		PlayerMode m_movementMode;
		float m_rotateRate;
		Vector3 m_leadingCorner;
		float m_playerSpeed;
		float m_jumpForce;
		bool m_isOnGround;
		int m_modeIndex;
		Texture* m_rainTexture;
		float m_texScrollX;
		float m_texScrollY;

	public:
		Player();
		~Player();
		void SetRotateRate(float value);
		void UpdatePlayerPosition(Vector3 movementVector);
		Vector3 ProcessingCollistionDetection(Vector3 movementVector);
		Vector3 ApproveOrModifyMovementIntention(Vector3 startPoint, Vector3 movementVector );
		bool IsPlayerOnGround();
		bool IsPlayerInWater();
		void PlaySoundBasedOnBlockTypeUnderPlayer();
		void Update(float elapsedTime);
		void RenderPlayerAABB();
		void Render();
		Material GetBlockTypeOfBlockUnderPlayer();
		void RenderWaterInFrontOfEyes();
		void RenderRainAroundPlayer();
};
#endif