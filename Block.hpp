#ifndef BLOCK_H
#define BLOCK_H
#include "MathUtilities.hpp"
#include "GameCommon.hpp"

struct BlockType
{
	Material material;
};


class Block
{
	public: 
		BlockType m_blockType;
		unsigned char m_lightingFlags;

	public:
		Block();
		~Block();

		inline unsigned char GetLightLevelForBlock()
		{
			return ( m_lightingFlags & LIGHT_LEVEL_MASK );
		}

		inline bool IsSkyBlockBitSet()
		{
			return ( (m_lightingFlags & SKY_VISIBLE_MASK) == SKY_VISIBLE_MASK );
		}

		inline void SetLightLevelForBlock(unsigned char newLightLevel) 
		{
			m_lightingFlags &= ( ~LIGHT_LEVEL_MASK ); 
			m_lightingFlags |= newLightLevel;
		}

		inline void SetSkyVisibleForBlock()
		{
			m_lightingFlags |= SKY_VISIBLE_MASK; 
		}

		inline void SetSkyNotVisibleForBlock()
		{
			m_lightingFlags &= ~SKY_VISIBLE_MASK;
		}

		inline void SetBlockDirty()
		{
			m_lightingFlags |= DIRTY_MASK;
		}

		inline void SetBlockClean()
		{
			m_lightingFlags &= ~DIRTY_MASK;
		}

		inline unsigned char GetNativeLightValueOfBlockType()
		{
			if(m_blockType.material != GLOW_STONE)
				return 0;
			else
				return static_cast<unsigned char>(GLOW_STONE_LIGHT_VALUE);
		}
};

#endif