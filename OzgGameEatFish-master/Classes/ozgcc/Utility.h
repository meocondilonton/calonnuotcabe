#ifndef __ozgcc__Utility__
#define __ozgcc__Utility__

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
#include "external\win32-specific\icon\include\iconv.h"
#endif

namespace ozgcc
{
	class Utility;
}

#define OZG_BASE64 "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/="

static unsigned int OzgCCUtilsRandomSeed = 0;
//string转数字类型
/*
string str("00801");
cout << stringToNumber<int>(str) << endl;
*/
template <class Type>
Type stringToNumber(const std::string& str)
{
	std::istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}

class ozgcc::Utility
{

public:

	static unsigned int randomInt(unsigned int min, unsigned int max); //范围随机数
	//随机例子，第一次调用randomFloat的值都是一样的，所以第一次的值可以忽略
	// OzgCCUtils::randomSeed(0);
	// for (int i = 0; i < 10; i++)
	// {
	// CCLog("%f, %f", OzgCCUtils::randomFloat(0.0f, 3.0f), OzgCCUtils::randomFloat(0.0f, 3.0f));
	// sleep(1);
	// }
	static void randomSeed(int seed);
	static float randomFloat(float min, float max); //范围随机float
	static int atoi(const char *src); //atoi的加强版，字符转整数
	static char* itoa(int num, char *str); //整数转字符
	static char* formatTime(const char* format); //返回格式化的时间，参数1 demos:"%Y-%m-%d %H:%M:%S"
	static char* formatTime(const char* format, time_t utc_time);
	/*
	base64 demo
	const char *t = "this is ozg demo";
	int j = strlen(t);
	char *enc = OzgCCUtils::base64Encode(t, j);
	int len = strlen(enc);
	char *dec = OzgCCUtils::base64Decode(enc, len);
	printf("\noriginal: %s\n", t);
	printf("\nencoded : %s\n", enc);
	printf("\ndecoded : %s\n", dec);
	free(enc);
	free(dec);
	*/
	static char *base64Encode(const char* data, int data_len);
	static char base64FindPos(char ch);
	static char *base64Decode(const char *data, int data_len);
	static float roundf(float num); //四舍五入到整数
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	//windows的乱码解决方案
	static int GBKToUTF8(std::string &gbkStr, const char* toCode, const char* formCode);
#endif
	static std::string generalString(std::string &str); //通用的乱码解决方案
	static std::string generalString(const char* str);

};

#endif
