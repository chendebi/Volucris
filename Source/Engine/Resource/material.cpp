#include "Resource/material.h"

namespace volucris
{
	Material::Material()
		: m_vss()
		, m_fss()
		, m_proxy(nullptr)
	{
	}

	Material::Material(const std::string& vss, const std::string& fss)
		: Material()
	{
		setSource(vss, fss);
	}

	void Material::setSource(const std::string& vss, const std::string& fss)
	{
		m_vss = vss;
		m_fss = fss;
	}
}
