#include <Engine/Core/GUID.h>
#include <Engine/Core/Logging.h>

VOLUCRIS_STATIC_LOG(CoreTest, Trace)

using namespace volucris;

int main()
{
	GUID guid1 = GUID::generate();
	V_LOG_DEBUG(CoreTest, "Generated GUID: {}", guid1);
}