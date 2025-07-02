#ifndef __volucris_content_browser_events_h__
#define __volucris_content_browser_events_h__

#include <string>
#include <Engine/Core/Delegate.h>


DECLARE_EVENT_DELEGATE(OnPathSelectedEvent, void, std::string)

#endif // !__volucris_content_browser_events_h__
