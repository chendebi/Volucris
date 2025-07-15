#ifndef __volucris_primitive_info_h__
#define __volucris_primitive_info_h__

#include <Engine/Core/DataType.h>
#include <vector>
#include <Engine/Core/TypesHelp.h>

namespace volucris
{
	enum PrimitiveType
	{
		Vertex,
		Normal,
		Color,
		SecondColor,
		TextureCoordinate
	};

	struct PrimitiveBlock
	{
		PrimitiveType type;
		DataType dataType;
		size_t offset;
		size_t size;

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& type;
			ar& dataType;
			ar& offset;
			ar& size;
		}
	};

	struct PrimitiveSegment
	{
		ElementDataType type;
		size_t offset;
		size_t size;

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& type;
			ar& offset;
			ar& size;
		}
	};

	struct PrimitiveInfo
	{
		std::vector<uint8> data;
		std::vector<uint8> segmentData;
		std::vector<PrimitiveBlock> blocks;
		std::vector<PrimitiveSegment> segments;
		std::vector<std::string> materials;

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& data;
			ar& segmentData;
			ar& blocks;
			ar& segments;
			ar& materials;
		}
	};
}

#endif // !__volucris_primitive_info_h__
