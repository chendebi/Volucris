#include <Core/GUID.h>
#include <windows.h>
#include <rpcdce.h>
#include <Core/Assert.h>

volucris::GUID volucris::GUID::generate()
{
	::GUID guid;
	if (CoCreateGuid(&guid) == S_OK)
	{
		volucris::GUID newGuid;
		newGuid.Data1 = guid.Data1;
		newGuid.Data2 = guid.Data2;
		newGuid.Data3 = guid.Data3;
		for (int i = 0; i < 8; ++i)
		{
			newGuid.Data4[i] = guid.Data4[i];
		}
		return newGuid;
	}
	v_check(false)
	return {};
}