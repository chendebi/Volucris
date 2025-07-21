#ifndef __volucris_material_loader_h__
#define __volucris_material_loader_h__

#include <string>

namespace volucris
{
	class MaterialLoader
	{
	public:
		MaterialLoader();

		bool setVertexSource(const std::string& filepath);

		bool setFragmentSource(const std::string& filepath);

	private:
		void init(const std::string& filepath);

		std::string getKey(const std::string& filepath);

	private:
		std::string m_name;
		std::string m_vsp;
		std::string m_fsp;
		std::string m_key;
	};
}

#endif // !__volucris_material_loader_h__
