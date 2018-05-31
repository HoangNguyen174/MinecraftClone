#include "Block.hpp"

Block::Block()
{
	m_blockType.material = AIR;
	m_lightingFlags = 0;
}

Block::~Block()
{
}
