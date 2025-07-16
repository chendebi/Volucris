#ifndef __volucris_datatype_h__
#define __volucris_datatype_h__

namespace volucris
{
	enum class DataType
	{
		Float,
		UInt
	};

	enum class ElementDataType
	{
		UByte,
		UShort,
		UInt
	};

	enum class ElementDrawMode
	{
		Points,
		LineStrip,
		LineLoop,
		Lines,
		LineStripAdjacency,
		LinesAdjacency,
		TraingleStrip,
		TraingleFan,
		Traingles
	};
}

#endif // !__volucris_datatype_h__
