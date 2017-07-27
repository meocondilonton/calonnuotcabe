
#include "Strings.h"
#include "Utility.h"

using namespace eatfish;
using namespace std;
using namespace ozgcc;
USING_NS_CC;

map<string, string> Strings::strings()
{
	map<string, string> strings;
	LanguageType lang = Application::getInstance()->getCurrentLanguage();

	if (lang == LanguageType::CHINESE)
		strings = Strings::stringsCNS();
	else
		strings = Strings::stringsEN();

	return strings;
}

map<string, string> Strings::stringsEN()
{
	map<string, string> strings;

	//开始场景
	strings.insert(pair<string, string>("start_scene_btn_start", "Start"));
	strings.insert(pair<string, string>("start_scene_btn_help", "Help"));

	//游戏场景
	strings.insert(pair<string, string>("game_scene_lab_stage_num", "Stage: %i"));
	strings.insert(pair<string, string>("game_scene_lab_score", "Score: %i"));

	//Dialog
	strings.insert(pair<string, string>("dialog_btn_yes", "Yes"));
	strings.insert(pair<string, string>("dialog_btn_no", "No"));
	strings.insert(pair<string, string>("dialog_btn_ok", "OK"));

	//help
	strings.insert(pair<string, string>("help_btn_back", "Back"));
	strings.insert(pair<string, string>("help_title", "Help:"));
	strings.insert(pair<string, string>("help_lab1", "Eat smaller fish"));
	strings.insert(pair<string, string>("help_lab2", "Avoid fish larger than themselves"));
	strings.insert(pair<string, string>("help_lab3", "Eat enough fish to grow"));

	//暂停界面
	strings.insert(pair<string, string>("pause_resume", "Resume"));
	strings.insert(pair<string, string>("pause_sound", "Sound"));
	strings.insert(pair<string, string>("pause_effect", "Effect"));
	strings.insert(pair<string, string>("pause_exit", "Exit"));
	strings.insert(pair<string, string>("pause_off", "OFF"));
	strings.insert(pair<string, string>("pause_on", "ON"));

	//过关界面
	strings.insert(pair<string, string>("clear_title", "Clear"));
	strings.insert(pair<string, string>("clear_exit", "Exit"));
	strings.insert(pair<string, string>("clear_next", "Next"));

	//game over
	strings.insert(pair<string, string>("gameover_title", "GAME OVER"));
	strings.insert(pair<string, string>("gameover_content", "Thank you for your play"));
	strings.insert(pair<string, string>("gameover_exit", "Exit"));
	strings.insert(pair<string, string>("gameover_restart", "Restart"));
	strings.insert(pair<string, string>("settings_exit_dialog", "Are you sure you want to exit?"));
	strings.insert(pair<string, string>("title", "Eat Fish"));

	return strings;
}
map<string, string> Strings::stringsCNS()
{
	map<string, string> strings;
	
	//开始场景
	strings.insert(pair<string, string>("start_scene_btn_start", Utility::generalString("开始")));
	strings.insert(pair<string, string>("start_scene_btn_help", Utility::generalString("帮助")));

	//游戏场景
	strings.insert(pair<string, string>("game_scene_lab_stage_num", Utility::generalString("关卡：%i")));
	strings.insert(pair<string, string>("game_scene_lab_score", Utility::generalString("分数：%i")));

	//Dialog
	strings.insert(pair<string, string>("dialog_btn_yes", Utility::generalString("是")));
	strings.insert(pair<string, string>("dialog_btn_no", Utility::generalString("否")));
	strings.insert(pair<string, string>("dialog_btn_ok", Utility::generalString("确定")));

	//help
	strings.insert(pair<string, string>("help_btn_back", Utility::generalString("后退")));
	strings.insert(pair<string, string>("help_title", Utility::generalString("帮助：")));
	strings.insert(pair<string, string>("help_lab1", Utility::generalString("吃掉比自己小的鱼")));
	strings.insert(pair<string, string>("help_lab2", Utility::generalString("避开比自己大的鱼")));
	strings.insert(pair<string, string>("help_lab3", Utility::generalString("吃鱼可以令自己成长")));

	//暂停界面
	strings.insert(pair<string, string>("pause_resume", Utility::generalString("暂停")));
	strings.insert(pair<string, string>("pause_sound", Utility::generalString("声音")));
	strings.insert(pair<string, string>("pause_effect", Utility::generalString("效果")));
	strings.insert(pair<string, string>("pause_exit", Utility::generalString("退出")));
	strings.insert(pair<string, string>("pause_off", Utility::generalString("关")));
	strings.insert(pair<string, string>("pause_on", Utility::generalString("开")));

	//过关界面
	strings.insert(pair<string, string>("clear_title", Utility::generalString("过关了")));
	strings.insert(pair<string, string>("clear_exit", Utility::generalString("退出")));
	strings.insert(pair<string, string>("clear_next", Utility::generalString("下一关")));

	//game over
	strings.insert(pair<string, string>("gameover_title", Utility::generalString("GAME OVER")));
	strings.insert(pair<string, string>("gameover_content", Utility::generalString("游戏结束了")));
	strings.insert(pair<string, string>("gameover_exit", Utility::generalString("退出")));
	strings.insert(pair<string, string>("gameover_restart", Utility::generalString("重来")));
	strings.insert(pair<string, string>("settings_exit_dialog", Utility::generalString("确定退出吗？")));
	strings.insert(pair<string, string>("title", Utility::generalString("大鱼吃小鱼")));

	return strings;
}
