#ifndef __volucris_material_h__
#define __volucris_material_h__

#include <Engine/Game/GameObject.h>
#include <Engine/Render/MaterialParameterInfo.h>

namespace volucris
{
	class Material : public GameObject
	{
	public:
		Material();

		Material(std::string vss, std::string fss);

		void setSource(std::string vss, std::string fss)
		{
			m_vss = std::move(vss);
			m_fss = std::move(fss);
		}

		void setParameters(std::vector<MaterialParameterInfo> parameters)
		{
			m_parameters = std::move(parameters);
		}

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& m_vss;
			ar& m_fss;
			ar& m_parameters;
		}

		std::string getClassName() const { return "Material"; }

	private:
		std::string m_vss;
		std::string m_fss;
		std::vector<MaterialParameterInfo> m_parameters;
	};
}

#endif // !__volucris_material_h__
