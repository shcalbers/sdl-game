#include <list>
#include <mutex>
#include <queue>
#include <unordered_map>
using namespace std;

template<typename EventType>
struct Event<EventType>::EventData
{

	const EventType eventType;
	bool isCancelled;

	mutex dataMutex;

};

template<typename EventType>
struct EventListener<EventType>::EventListenerData
{

	inline static EventListenerID currentID = 0;

	const EventType eventType;
	const EventPriority eventPriority;

	const EventListenerID eventListenerID;

	//EventHandler function taking an Event reference (unique_ptr) as argument.
	void (* const eventHandler)(unique_ptr< Event<EventType> > &);

	mutex dataMutex;

};

template<typename EventType>
struct EventManager<EventType>::EventManagerData
{

	using EventQueue = queue< unique_ptr< Event<EventType> > >;
	using EventMap = unordered_map< EventType,
	 				list< EventListener<EventType> > >;

	EventMap eventMap;
	EventQueue eventQueue;

	mutex dataMutex;

};



template<typename EventType>
Event<EventType>::Event(const EventType & eventType)
	:	_data(unique_ptr< EventData >{ new EventData{ eventType, false} } )
{}

template<typename EventType>
Event<EventType>::Event(Event<EventType> && event)
	:	_data( move(event._data) )
{}

template<typename EventType>
const EventType & Event<EventType>::GetEventType(void) const
{
	return _data->eventType;
}

template<typename EventType>
void Event<EventType>::Cancel(void)
{
	//Ensure that this is the only thread accessing the class' data.
	lock_guard<mutex> lock(_data->dataMutex);

	if( !(_data->isCancelled) ){
		_data->isCancelled = true;
	}

	return;
}

template<typename EventType>
bool Event<EventType>::IsCancelled(void) const
{
	//Ensure that this is the only thread accessing the class' data.
	lock_guard<mutex> lock(_data->dataMutex);

	return _data->isCancelled;
}

template<typename EventType>
Event<EventType>::~Event(void)	{}



template<typename EventType>
EventListener<EventType>::EventListener(
	const EventType & eventType,
	const EventPriority & eventPriority,
	void (* const eventHandler)(unique_ptr< Event<EventType> > &) )
	:	_data(	unique_ptr< EventListenerData >{
				new EventListenerData{	eventType,
							eventPriority,
							(EventListenerData::currentID)++,
							eventHandler	}
			} )
{}

template<typename EventType>
EventListener<EventType>::EventListener(EventListener<EventType> && eventListener)
	:	_data( move(eventListener._data) )
{}

template<typename EventType>
const EventType & EventListener<EventType>::GetEventType(void) const
{
	return _data->eventType;
}

template<typename EventType>
const EventPriority & EventListener<EventType>::GetEventPriority(void) const
{
	return _data->eventPriority;
}

template<typename EventType>
const typename EventListener<EventType>::EventListenerID &
EventListener<EventType>::GetID(void) const
{
	return _data->eventListenerID;
}

template<typename EventType>
void EventListener<EventType>::Handle(unique_ptr< Event<EventType> > & eventPtr) const
{
	//Ensure that this is the only thread accessing the class' data.
	lock_guard<mutex> lock(_data->dataMutex);

	_data->eventHandler(eventPtr);

	return;
}

template<typename EventType>
EventListener<EventType>::~EventListener(void)	{}



template<typename EventType>
EventManager<EventType>::EventManager(void)
	: _data(unique_ptr< EventManagerData >{ new EventManagerData{} })
{}

template<typename EventType>
void EventManager<EventType>::QueueEvent(unique_ptr< Event<EventType> > && eventPtr)
{
	//Ensure that this is the only thread accessing the class' data.
	lock_guard<mutex> lock(_data->dataMutex);

	(_data->eventQueue).push( move(eventPtr) );

	return;
}

template<typename EventType>
void EventManager<EventType>::RegisterListener(
	EventListener<EventType> && eventListener)
{
	//Ensure that this is the only thread accessing the class' data.
	lock_guard<mutex> lock(_data->dataMutex);

	typename EventManagerData::EventMap & eventMap = _data->eventMap;
	auto search = eventMap.find(eventListener.GetEventType());

	//If there is no list matching the EventListener's EventType.
	const bool && couldNotFindList = (search == eventMap.end());
	if( couldNotFindList ){
		EventType eventType = eventListener.GetEventType();

		//Add a new list and register EventListener.
		list< EventListener<EventType> > newList{};
		newList.push_back( move(eventListener) );

		eventMap.insert( { move(eventType), move(newList) } );

	//Else if a list matching the EventListener's EventType is found.
	} else {

		//Add EventListener to the list.
		if(eventListener.GetEventPriority() == EventPriority::LOW_PRIORITY){
			(search->second).push_back( move(eventListener) );
		} else {
			(search->second).push_front( move(eventListener) );
		}
	}

	return;
}

template<typename EventType>
EventManager<EventType>::~EventManager(void)	{}

template<typename EventType>
void EventManager<EventType>::PushEvents(void)
{
	//Ensure that this is the only thread accessing the class' data.
	lock_guard<mutex> lock(_data->dataMutex);

	typename EventManagerData::EventQueue & eventQueue = _data->eventQueue;
	typename EventManagerData::EventMap & eventMap = _data->eventMap;

	while( !( eventQueue.empty() ) ){
		unique_ptr< Event<EventType> > & eventPtr = eventQueue.front();

		list< EventListener<EventType> > & listenerList =
			(eventMap.find( eventPtr->GetEventType() ))->second;

		for( EventListener<EventType> & listener : listenerList ){
			if(eventPtr->IsCancelled()) break;

			listener.Handle(eventPtr);
		}

		eventQueue.pop();
	}

	return;
}

template<typename EventType>
void EventManager<EventType>::ClearEvents(void)
{
	//Ensure that this is the only thread accessing the class' data.
	lock_guard<mutex> lock(_data->dataMutex);

	queue< unique_ptr< Event<EventType> > > emptyQueue;
	swap( _data->eventQueue, emptyQueue);

	return;
}
