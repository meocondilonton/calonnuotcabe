
#ifndef OzgGameEatFish_Strings_h
#define OzgGameEatFish_Strings_h

#include "cocos2d.h"

namespace eatfish
{
	class Strings;
}

class eatfish::Strings
{

public:
	
	static std::map<std::string, std::string> strings();
	static std::map<std::string, std::string> stringsEN();
	static std::map<std::string, std::string> stringsCNS();

};

#endif
