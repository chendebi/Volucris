#include <Engine/Application/Application.h>
#include <Engine/Core/VectorHelp.h>
#include <Engine/Core/Logging.h>
#include <Engine/FileSystem/FileSystem.h>
#include <filesystem>

namespace fs = std::filesystem;

VOLUCRIS_STATIC_LOG(FileSystemTest, Trace)

using namespace volucris;


int main(int argc, char** argv)
{
	gFileSystem.mount("/Engine/Config", fmt::format("{}/Binaries/Config", VOLUCRIS_ENGINE_ROOT));
	//gFileSystem.mount("/Engine/", VOLUCRIS_ENGINE_ROOT);
	{
		auto filepath = gFileSystem.virtualToPhysical("/Engine/test.txt");
		V_LOG_INFO(FileSystemTest, "virtual path: /Engine/test.txt");
		V_LOG_INFO(FileSystemTest, "physical path: {}", filepath);
	}
	{
		auto filepath = gFileSystem.physicalToVirtual("D:/Volucris/test.txt");
		V_LOG_INFO(FileSystemTest, "physical path: D:/Volucris/test.txt");
		V_LOG_INFO(FileSystemTest, "virtual path: {}", filepath);
	}

	auto nodes = gFileSystem.getFileNodes("/Engine/Config/");
	for (auto node : nodes)
	{
		if (node.type == EFileType::Directory)
		{
			V_LOG_INFO(FileSystemTest, "find directory: {}", node.path);
		}
		else
		{
			V_LOG_INFO(FileSystemTest, "find file: {}", node.path);
		}
	}

	std::string writeText = "hello volucris";
	if (!gFileSystem.createFile("/Engine/Build/TestFile/a.txt", writeText.c_str(), writeText.size()))
	{
		V_LOG_WARN(FileSystemTest, "create file failed: {}", "/Engine/Build/TestFile/a.txt");
	}

	if (!gFileSystem.deleteFile("/Engine/Build/TestFile/a.txt"))
	{
		V_LOG_WARN(FileSystemTest, "delete file failed: {}", "/Engine/Build/TestFile/a.txt");
	}

	if (!gFileSystem.createDirectory("/Engine/Build/TestFile/AAA"))
	{
		V_LOG_WARN(FileSystemTest, "create directory failed: {}", "/Engine/Build/TestFile/AAA");
	}

	if (!gFileSystem.deleteDirectory("/Engine/Build/TestFile"))
	{
		V_LOG_WARN(FileSystemTest, "delete directory failed: {}", "/Engine/Build/TestFile/AAA");
	}
}