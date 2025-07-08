#include <Engine/Profile/Timer.h>
#include <variant>
#include <Engine/Core/Logging.h>
#include <glm/glm.hpp>
#include <Engine/Core/TypesHelp.h>

#include <iostream>
#include <string>

#include <windows.h>


// 清屏函数
void ClearScreen() {
	// Windows 方案
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { 0, 0 };
	DWORD count;
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	// 获取控制台信息
	if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
		// 填充空格清屏
		FillConsoleOutputCharacter(hStdOut, ' ', csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
		// 重置光标位置
		SetConsoleCursorPosition(hStdOut, coord);
	}
}

VOLUCRIS_STATIC_LOG(MaterialCopyTest, Trace)

using namespace volucris;

using variant = std::variant<float, glm::vec4>;

int main()
{
	auto count = 1000000;
	auto hcount = count / 2;
	std::vector<std::vector<uint8>> copies;
	std::vector<variant> variants;

	copies.reserve(count);
	variants.reserve(count);

	Timer t;
	t.start();
	for (auto i = 0; i < hcount; i++)
	{
		float v = i;
		std::vector<uint8> data;
		auto size = sizeof(float);
		data.resize(size);
		memcpy(data.data(), &v, size);
		copies.emplace_back(std::move(data));
	}

	for (auto i = hcount; i < count; i++)
	{
		glm::vec4 v = { i, 0, 0, 0 };
		std::vector<uint8> data;
		auto size = sizeof(glm::vec4);
		data.resize(size);
		memcpy(data.data(), &v, size);
		copies.emplace_back(std::move(data));
	}
	t.stop();
	auto ctime = t.getDuration();

	t.start();
	for (auto i = 0; i < hcount; i++)
	{
		float v = i;
		variants.emplace_back(v);
	}

	for (auto i = hcount; i < count; i++)
	{
		glm::vec4 v = { i, 0, 0, 0 };
		variants.emplace_back(v);
	}
	t.stop();
	auto vtime = t.getDuration();

	V_LOG_INFO(MaterialCopyTest, "variant time: {}", t.getDuration());

	// 避免优化
	for (auto i = 0; i < copies.size(); ++i)
	{
		V_LOG_DEBUG(MaterialCopyTest, "TEMP: {}", copies[i].size());
	}

	for (auto i = 0; i < copies.size(); ++i)
	{
		V_LOG_DEBUG(MaterialCopyTest, "TEMP: {}", variants[i].index());
	}
	ClearScreen();
	V_LOG_INFO(MaterialCopyTest, "copy time: {}", ctime);
	V_LOG_INFO(MaterialCopyTest, "variant time: {}", vtime);
}