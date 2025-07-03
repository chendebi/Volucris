#ifndef __volucris_package_h__
#define __volucris_package_h__

#include <Engine/Game/GameObject.h>
#include <string>

namespace volucris
{
	class Package : public GameObject
	{
	public:
		Package();

		~Package() override;
		
	private:
		std::string m_packagePath;
	};
}

#endif // !__volucris_package_h__
