#ifndef __volucris_game_object_h__
#define __volucris_game_object_h__

#include <Engine/Core/Object.h>
#include <vector>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/export.hpp>
#include <rttr/registration.h>
#include <rttr/rttr_enable.h>
#include <Engine/Core/Delegate.h>
#include <Engine/Core/TypesHelp.h>

namespace volucris
{
	class Package;

	class GameObject;

	DECLARE_EVENT_MUTI_DELEGATE(ObjectDirtyEvent, void, GameObject*)

	class GameObject : public Object
	{
		RTTR_ENABLE()

	public:
		ObjectDirtyEvent DirtyStateChanged;

		enum DirtyFlags
		{
			DirtyFlag_None = 0,
			DirtyFlag_Normal = 1 << 1,
			DirtyFlag_Dependence = 1 << 2,
			DirtyFlag_RenderState = 1 << 3,
			DirtyFlag_Transform = 1 << 4,
			DirtyFlag_All = DirtyFlag_Normal | DirtyFlag_Dependence | DirtyFlag_RenderState | DirtyFlag_Transform
		};

	public:
		GameObject();

		virtual ~GameObject() override;

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version) 
		{
		}

		void setDisplayName(const std::string& name)
		{
			m_displayName = name;
		}

		const std::string& getDisplayName() const
		{
			return m_displayName;
		}

		virtual std::vector<std::string> collectDependencies() const
		{
			return {};
		}

		virtual bool replaceDependency(const std::string& oldPath, const std::string& newPath) { return false; }

		virtual std::string getClassName() const { return ""; }

		void markDirty(uint32 flags = DirtyFlag_Normal)
		{
			bool stateChanged = false;
			if (flags == 0 && m_dirtyFlags != 0)
			{
				m_dirtyFlags = 0;
				stateChanged = true;
			}
			else
			{
				if (m_dirtyFlags == 0)
				{
					stateChanged = true;
				}
				m_dirtyFlags = m_dirtyFlags | flags;
			}

			if (stateChanged)
			{
				DirtyStateChanged.invoke(this);
			}
		}

		uint32 getDirtyFlags() const { return m_dirtyFlags; }

		void removeDirtyFlags(uint32 flags)
		{
			m_dirtyFlags = m_dirtyFlags & (~flags);
		}

	private:
		std::string m_displayName;
		uint32 m_dirtyFlags;
	};
}

#endif // !__volucris_game_object_h__
