#ifndef __OzgGameEatFish__JellyfishNode__
#define __OzgGameEatFish__JellyfishNode__

#include "BaseEnemyFishNode.h"

namespace eatfish
{
	namespace element
	{
		class JellyfishNode;
	}
}

class eatfish::element::JellyfishNode : public eatfish::element::BaseEnemyFishNode
{

public:
	
	virtual ~JellyfishNode();
	virtual bool init();

	CREATE_FUNC(JellyfishNode);
	
};

#endif
