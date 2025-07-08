#ifndef __volucris_material_h__
#define __volucris_material_h__

#include <Engine/Game/GameObject.h>
#include "MaterialParameter.h"

namespace volucris
{
	class Material : public GameObject
	{
	public:
		Material();

		Material(const std::string& vss, const std::string fss);

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& m_vss;
			ar& m_fss;
		}

		bool setFloatParameter(const std::string& name, float value);

		bool setVector4Parameter(const std::string& name, const glm::vec4& value);

	private:
		std::string m_vss;
		std::string m_fss;
		std::vector<MaterialFloatParameter> m_floatParameters;
		std::vector<MaterialVector4Parameter> m_vec4Parameters;
	};
}

#endif // !__volucris_material_h__
