#include <Engine/Application/Application.h>
#include <Engine/Core/VectorHelp.h>
#include <Engine/Core/Logging.h>

VOLUCRIS_STATIC_LOG(VectorHelp, Trace)

using namespace volucris;


int main(int argc, char** argv)
{
	std::vector<int> test = { 1,2,2,3,4,2,6 };
	V_LOG_INFO(VectorHelp, "before remove all 2, {}", test);
	VectorHelp::quickRemoveAll(test, 2);
	V_LOG_INFO(VectorHelp, "after remove all 2, {}", test);
	return 0;
}