#pragma once

#include "EnginePCH.h"
#include "Engine/Core/Core.h"

namespace Engine {
	
	// OpenGL-Engine events are currently "blocking events", meaning they are dispatched immediately
	// and must be dealt with right away. In the future this may be updated to a buffered event system
	// where there would be an event bus, and a "ProcessEvents" section in the "OnUpdate" stage.

	enum class EventType 
	{
		NONE = 0,
		// Windowing events
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMove,
		// Keyboard events
		KeyPress, KeyRelease, KeyTyped,
		// Mouse events
		MouseButtonPress, MouseButtonRelease, MouseMove, MouseScroll,
		// Client Application events
		AppRender, AppUpdate, AppTick,
	};

	// Used to help filter events
	enum EventCategory
	{
		// Bitshift operator macro helps to create bit-field for Event category filtering
		None						= 0,	  // ie 00000
		EventCategoryApplication	= BIT(0), // ie 00001
		EventCategoryInput			= BIT(1), // ie 00010
		EventCategoryKeyboard		= BIT(2), // ie 00100
		EventCategoryMouse			= BIT(3), // ie 01000
		EventCategoryMouseButton	= BIT(4)  // ie 10000
	};

	// Macros to easily implement Event superclass' pure virtual functions in subclasses
	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

	#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class EventDispatcher;

	class ENGINE_API Event
	{
		friend class EventDispatcher;

	public:
		bool Handled = false;

		// Pure virtual functions (every Event object has to implement these)
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;

		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			// Bit AND operator to check if in bit-field category
			return GetCategoryFlags() & (int)category; 
		}
	
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		// Dispatch allows us to call it multiple times for a single event with different event functions (EventFn)
		// 'T' can be any EventType, 'EventFn' must return a bool
		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	// Overload useful for logging Events
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}