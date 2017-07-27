#ifndef __OzgGameEatFish__EnemyFishNode__
#define __OzgGameEatFish__EnemyFishNode__

#include "BaseEnemyFishNode.h"

namespace eatfish
{
	namespace element
	{
		class EnemyFishNode;
	}
}

class eatfish::element::EnemyFishNode : public eatfish::element::BaseEnemyFishNode
{

public:

	enum class EnemyFishType
	{
		Fish1 = 1,
		Fish2 = 2,
		Fish3 = 3,
		Fish4 = 4,
		Fish5 = 5,
		Fish6 = 6
	};

	EnemyFishType m_type;

	virtual ~EnemyFishNode();
	virtual bool init(EnemyFishType type);

	static EnemyFishNode* create(EnemyFishType type);
	
};

#endif
