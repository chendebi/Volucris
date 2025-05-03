#ifndef __volucris_context_h__
#define __volucris_context_h__

namespace volucris
{
	class Context
	{
	public:
		Context();

		~Context();

		void makeCurrent();

		void swapBuffers();

	private:
		struct Impl;
		Impl* m_impl;
	};
}

#endif // !__volucris_context_h__
