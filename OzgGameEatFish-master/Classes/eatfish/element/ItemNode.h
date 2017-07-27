#ifndef __OzgGameEatFish__ItemNode__
#define __OzgGameEatFish__ItemNode__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "BaseNode.h"

namespace eatfish
{
	namespace element
	{
		class ItemNode;
	}
}

class eatfish::element::ItemNode : public eatfish::element::BaseNode
{

protected:
		
public:
	
	enum class ItemNodeType
	{
		GOLD = 1
	};

	ItemNodeType m_type;

	virtual ~ItemNode();
	virtual bool init(ItemNodeType type);

	static ItemNode* create(ItemNodeType type);

};

#endif
