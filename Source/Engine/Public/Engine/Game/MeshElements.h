#ifndef __volucris_mesh_elements_h__
#define __volucris_mesh_elements_h__

#include <Engine/Core/DataType.h>
#include <Engine/Core/TypesHelp.h>
#include <vector>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/export.hpp>
#include <rttr/registration.h>

namespace volucris
{
	class SmallMeshElements;
	class MediumMeshElements;
	class LargeMeshElements;

	class MeshElements
	{
	public:
		MeshElements(ElementDrawMode mode)
			: m_mode(mode)
		{ }

		virtual ~MeshElements() = default;

		virtual size_t getCount() const = 0;

		virtual size_t getBufferSize() const = 0;

		virtual ElementDataType getDataType() const = 0;

		virtual const uint8* getData() = 0;

		ElementDrawMode getDrawMode() const { return m_mode; }

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& m_mode;
		}

		SmallMeshElements* asSmall();
		MediumMeshElements* asMedium();
		LargeMeshElements* asLarge();

	private:
		ElementDrawMode m_mode;
	};


	class SmallMeshElements : public MeshElements
	{
	public:
		SmallMeshElements(ElementDrawMode mode = ElementDrawMode::Traingles)
			: MeshElements(mode)
			, m_elements()
		{

		}

		uint8& create() 
		{
			m_elements.push_back(0);
			return *m_elements.rbegin();
		}

		void reserve(size_t capacity)
		{
			m_elements.reserve(capacity);
		}

		void init(uint8* data, size_t size)
		{
			m_elements.resize(size);
			memcpy(m_elements.data(), data, size);
		}

		uint8& get(size_t index)
		{
			if (m_elements.size() <= index)
			{
				m_elements.resize(index + 1);
			}
			return m_elements[index];
		}

		size_t getCount() const override { return  m_elements.size(); }

		size_t getBufferSize() const override { return m_elements.size(); }

		ElementDataType getDataType() const override { return ElementDataType::UByte; }

		const uint8* getData() override { return m_elements.data(); }

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boost::serialization::base_object<MeshElements>(*this);
			ar& m_elements;
		}

	private:
		std::vector<uint8> m_elements;
	};

	class MediumMeshElements : public MeshElements
	{
	public:
		MediumMeshElements(ElementDrawMode mode = ElementDrawMode::Traingles)
			: MeshElements(mode)
			, m_elements()
		{

		}

		uint16& create()
		{
			m_elements.push_back(0);
			return *m_elements.rbegin();
		}

		void reserve(size_t capacity)
		{
			m_elements.reserve(capacity);
		}

		void init(uint8* data, size_t size)
		{
			auto count = size / sizeof(uint16);
			m_elements.resize(count);
			memcpy(m_elements.data(), data, count * sizeof(uint16));
		}

		uint16& get(size_t index)
		{
			if (m_elements.size() <= index)
			{
				m_elements.resize(index + 1);
			}
			return m_elements[index];
		}

		size_t getCount() const override { return  m_elements.size(); }

		size_t getBufferSize() const override { return m_elements.size() * sizeof(uint16); }

		ElementDataType getDataType() const override { return ElementDataType::UShort; }

		const uint8* getData() override { return (uint8*)m_elements.data(); }

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boost::serialization::base_object<MeshElements>(*this);
			ar& m_elements;
		}

	private:
		std::vector<uint16> m_elements;
	};

	class LargeMeshElements : public MeshElements
	{
	public:
		LargeMeshElements(ElementDrawMode mode = ElementDrawMode::Traingles)
			: MeshElements(mode)
			, m_elements()
		{

		}

		uint32& create()
		{
			m_elements.push_back(0);
			return *m_elements.rbegin();
		}

		void reserve(size_t capacity)
		{
			m_elements.reserve(capacity);
		}

		void init(uint8* data, size_t size)
		{
			auto count = size / sizeof(uint32);
			m_elements.resize(count);
			memcpy(m_elements.data(), data, count * sizeof(uint32));
		}

		uint32& get(size_t index);

		size_t getCount() const override { return  m_elements.size(); }

		size_t getBufferSize() const override { return m_elements.size() * sizeof(uint32); }

		ElementDataType getDataType() const override { return ElementDataType::UInt; }

		const uint8* getData() override { return (uint8*)m_elements.data(); }

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boost::serialization::base_object<MeshElements>(*this);
			ar& m_elements;
		}

	private:
		std::vector<uint32> m_elements;
	};
}

BOOST_SERIALIZATION_ASSUME_ABSTRACT(MeshElements)
BOOST_CLASS_EXPORT_KEY(volucris::SmallMeshElements)
BOOST_CLASS_EXPORT_KEY(volucris::MediumMeshElements)
BOOST_CLASS_EXPORT_KEY(volucris::LargeMeshElements)

#endif // !__volucris_mesh_elements_h__
