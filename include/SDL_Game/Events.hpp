#ifndef EVENTS_HEADER
#define EVENTS_HEADER

#include "Debug.hpp"
#include <iostream>
#include <memory>
using namespace std;

template<typename EventType>
class Event
{

public:

	Event(const EventType & eventType);
	Event(Event<EventType> && event);

	Event(const Event<EventType> &) = delete;
	Event<EventType> & operator=(const Event<EventType> &) = delete;

	const EventType & GetEventType(void) const;

	void Cancel(void);
	bool IsCancelled(void) const;

	~Event();

private:

	struct EventData;
	unique_ptr< EventData > _data;

};

enum class EventPriority : char
{
	LOW_PRIORITY 		= 0,
	HIGH_PRIORITY 		= 1,
	PRIORITIES 		= 2
};

template<typename EventType>
class EventListener
{

	using EventListenerID = int;

public:

	EventListener(	const EventType & eventType,
		 	const EventPriority & eventPriority,
			void (* const eventHandler)(unique_ptr< Event<EventType> > &)	);

	EventListener(EventListener<EventType> && eventListener);

	EventListener(const EventListener &) = delete;
	EventListener<EventType> & operator=(const EventListener &) = delete;

	const EventType & GetEventType(void) const;
	const EventPriority & GetEventPriority(void) const;
	const EventListenerID & GetID(void) const;

	void Handle(unique_ptr< Event<EventType> > & eventPtr) const;

	~EventListener(void);

private:

	struct EventListenerData;
	unique_ptr< EventListenerData > _data;

};

template<typename EventType>
class EventManager
{

public:

	EventManager(void);

	void QueueEvent(unique_ptr< Event<EventType> > && eventPtr);

	void RegisterListener(EventListener<EventType> && eventListener);

	~EventManager(void);

protected:

	void PushEvents(void);

	void ClearEvents(void);

private:

	struct EventManagerData;
	unique_ptr< EventManagerData > _data;

};

#include "SDL_Game/Events.tpp"

#endif
