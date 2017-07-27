
#include "Utility.h"

USING_NS_CC;
using namespace std;
using namespace ozgcc;

unsigned int Utility::randomInt(unsigned int min, unsigned int max)
{
	srand((unsigned int)time(NULL));
	unsigned int x = abs((int)(rand() * time(NULL) % (max - min)));
	x += min;
	return x;
}

void Utility::randomSeed(int seed)
{
	if (!seed)
		OzgCCUtilsRandomSeed = time(NULL);
	else
		OzgCCUtilsRandomSeed = seed;
}

float Utility::randomFloat(float min, float max)
{
	OzgCCUtilsRandomSeed = 214013 * OzgCCUtilsRandomSeed + 2531011;
	return min + (OzgCCUtilsRandomSeed >> 16) * (1.0f / 65535.0f) * (max - min);
}

int Utility::atoi(const char *src)
{
	char flag = 0;
	int sum = 0;
	int i = 0;
	int len = (int)strlen(src);
	/*参数的正确性*/
	if (NULL == src)
	{
		return 0;
	}
	/*是否存在符号问题*/
	if (src[i] == '-')
	{
		flag = '-';
		++i;
	}
	for (; i < len; ++i)
	{
		/*判断字符是否合法*/
		if (src[i] < 48 && src[i] > 57)
			return 0;
		/*数据求和，注意数值的转换问题src[i] - 48*/
		sum = sum * 10 + src[i] - 48;
	}
	/*根据标志位实现返回正确的正负数*/
	if (flag == '-')
		return -sum;
	else
		return sum;
}

char* Utility::itoa(int num, char *str)
{
	//char flag = 0;
	int i = 0, count = 0, j = 0;
	/*参数检测*/
	if (NULL == str)
	{
		return NULL;
	}
	/*判断数值的正负,设置对应的符号*/
	if (num < 0)
	{
		str[i++] = '-';
		/************************
		同时要对数值取绝对值
		保证后面取余整除操作正常
		*************************/
		num = -num;
	}
	while (num / 10)
	{
		/***************************
		count用来保存实际的数字个数,
		便于后期的顺序调换
		****************************/
		++count;
		/*实际保存的顺序为反序列*/
		str[i++] = num % 10 + 48;
		num /= 10;
	}
	/*判断是*/
	if (num % 10)
	{
		str[i++] = num % 10 + 48;
		++count;
	}
	/*字符串结束符*/
	str[i] = 0;
	/*实现数据的较好操作*/
	i = 0;
	if (str[0] == '-')
	{
		i = 1;
	}
	/*实际交换的下标区间为i~j*/
	j = count + i - 1;
	for (; i < j; ++i, --j)
	{
		/*数据的较好操作*/
		str[i] = str[i] + str[j];
		str[j] = str[i] - str[j];
		str[i] = str[i] - str[j];
	}
	/*返回实际的字符串*/
	return str;
}

char* Utility::formatTime(const char *format)
{
	return Utility::formatTime(format, time(NULL));
}

char* Utility::formatTime(const char *format, time_t utc_time)
{
	static char str_time[128];
	struct tm *local_time = NULL;
	utc_time = time(NULL);
	local_time = localtime(&utc_time);
	strftime(str_time, sizeof(str_time), format, local_time);
	return str_time;
}

char* Utility::base64Encode(const char* data, int data_len)
{
	//int data_len = strlen(data);
	int prepare = 0;
	int ret_len;
	int temp = 0;
	char *ret = NULL;
	char *f = NULL;
	int tmp = 0;
	char changed[4];
	int i = 0;
	ret_len = data_len / 3;
	temp = data_len % 3;
	if (temp > 0)
	{
		ret_len += 1;
	}
	ret_len = ret_len * 4 + 1;
	ret = (char *)malloc(ret_len);
	if (ret == NULL)
	{
		printf("No enough memory.\n");
		exit(0);
	}
	memset(ret, 0, ret_len);
	f = ret;
	while (tmp < data_len)
	{
		temp = 0;
		prepare = 0;
		memset(changed, '\0', 4);
		while (temp < 3)
		{
			//printf("tmp = %d\n", tmp);
			if (tmp >= data_len)
			{
				break;
			}
			prepare = ((prepare << 8) | (data[tmp] & 0xFF));
			tmp++;
			temp++;
		}
		prepare = (prepare << ((3 - temp) * 8));
		//printf("before for : temp = %d, prepare = %d\n", temp, prepare);
		for (i = 0; i < 4; i++)
		{
			if (temp < i)
			{
				changed[i] = 0x40;
			}
			else
			{
				changed[i] = (prepare >> ((3 - i) * 6)) & 0x3F;
			}
			*f = OZG_BASE64[changed[i]];
			//printf("%.2X", changed[i]);
			f++;
		}
	}
	*f = '\0';
	return ret;
}

char Utility::base64FindPos(char ch)
{
	char *ptr = (char*)strrchr(OZG_BASE64, ch);//the last position (the only) in base[]
	return (ptr - OZG_BASE64);
}

char* Utility::base64Decode(const char *data, int data_len)
{
	int ret_len = (data_len / 4) * 3;
	int equal_count = 0;
	char *ret = NULL;
	char *f = NULL;
	int tmp = 0;
	int temp = 0;
	char need[3];
	int prepare = 0;
	int i = 0;
	if (*(data + data_len - 1) == '=')
	{
		equal_count += 1;
	}
	if (*(data + data_len - 2) == '=')
	{
		equal_count += 1;
	}
	if (*(data + data_len - 3) == '=')
	{//seems impossible
		equal_count += 1;
	}
	switch (equal_count)
	{
	case 0:
		ret_len += 4;//3 + 1 [1 for NULL]
		break;
	case 1:
		ret_len += 4;//Ceil((6*3)/8)+1
		break;
	case 2:
		ret_len += 3;//Ceil((6*2)/8)+1
		break;
	case 3:
		ret_len += 2;//Ceil((6*1)/8)+1
		break;
	}
	ret = (char *)malloc(ret_len);
	if (ret == NULL)
	{
		printf("No enough memory.\n");
		exit(0);
	}
	memset(ret, 0, ret_len);
	f = ret;
	while (tmp < (data_len - equal_count))
	{
		temp = 0;
		prepare = 0;
		memset(need, 0, 4);
		while (temp < 4)
		{
			if (tmp >= (data_len - equal_count))
			{
				break;
			}
			prepare = (prepare << 6) | (Utility::base64FindPos(data[tmp]));
			temp++;
			tmp++;
		}
		prepare = prepare << ((4 - temp) * 6);
		for (i = 0; i < 3; i++)
		{
			if (i == temp)
			{
				break;
			}
			*f = (char)((prepare >> ((2 - i) * 8)) & 0xFF);
			f++;
		}
	}
	*f = '\0';
	return ret;
}

float Utility::roundf(float num)
{
	float num1 = num - floorf(num);
	float num2 = ceilf(num) - num;
	if (num1 > num2)
		return ceilf(num);
	else
		return floorf(num);
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
//windows的乱码解决方案
//字符转换，使cocos2d-x在win32平台支持中文显示
int Utility::GBKToUTF8(string &gbkStr, const char* toCode, const char* formCode)
{
	iconv_t iconvH;
	iconvH = iconv_open(formCode, toCode);
	if (iconvH == 0)
	{
		return -1;
	}
	const char* strChar = gbkStr.c_str();
	const char** pin = &strChar;
	size_t strLength = gbkStr.length();
	char* outbuf = (char*)malloc(strLength * 4);
	char* pBuff = outbuf;
	memset(outbuf, 0, strLength * 4);
	size_t outLength = strLength * 4;
	if (-1 == iconv(iconvH, pin, &strLength, &outbuf, &outLength))
	{
		iconv_close(iconvH);
		return -1;
	}
	gbkStr = pBuff;
	iconv_close(iconvH);
	return 0;
}
#endif

string Utility::generalString(std::string &str)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	Utility::GBKToUTF8(str, "gbk", "utf-8"); //后面两个参数就默认了,免得后面再传参麻烦
	return str;
#else
	return str;
#endif
}

string Utility::generalString(const char* str)
{
	std::string s(str);
	return Utility::generalString(s);
}
