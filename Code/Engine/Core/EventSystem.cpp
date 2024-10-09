#include "EventSystem.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/DevConsole.hpp"
EventSystem::EventSystem(EventSystemConfig const& config)
{
	UNUSED(config);
}

EventSystem::~EventSystem()
{
}

void EventSystem::Startup()
{
}

void EventSystem::Shutdown()
{
}

void EventSystem::BeginFrame()
{
}

void EventSystem::EndFrame()
{
}


void EventSystem::SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr)
{
// 	auto found = m_subscriptionListByName.find(eventName);
// 	if (found == m_subscriptionListByName.end())
// 	{
	m_eventSystemMutex.lock();
		m_registeredCommands.push_back(eventName);
	//}
	m_subscriptionListByName[eventName].push_back(functionPtr);
	m_eventSystemMutex.unlock();
}

void EventSystem::UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr)
{
	auto found = m_subscriptionListByName.find(eventName);
	if (found != m_subscriptionListByName.end())
	{
		SubscriptionList& subscribersForThisEventName = found->second;

		for (auto it = subscribersForThisEventName.begin(); it != subscribersForThisEventName.end(); ++it)
		{
			if (*it == functionPtr)
			{  
				m_eventSystemMutex.lock();
				subscribersForThisEventName.erase(it);
				m_eventSystemMutex.unlock();
				break; 
			}
		}
	}
}

void EventSystem::FireEvent(std::string const& eventName, EventArgs& args)
{
	//m_eventSystemMutex.lock();
	auto found = m_subscriptionListByName.find(eventName);
	if (found != m_subscriptionListByName.end())
	{
		SubscriptionList const& subscriberForThisEventName = found->second;
		
		for (int i = 0; i < (int)subscriberForThisEventName.size(); ++i)
		{
			
			EventCallbackFunction subscriber = subscriberForThisEventName[i];
			
			if (subscriber(args))
			{
				break;
			}
		}
	}
	//m_eventSystemMutex.unlock();
}

void EventSystem::FireEvent(std::string const& eventName)
{
	//m_eventSystemMutex.lock();
	EventArgs args;
	auto found = m_subscriptionListByName.find(eventName);
	if (found != m_subscriptionListByName.end())
	{
		SubscriptionList const& subscriberForThisEventName = found->second;
			
		for (int i = 0; i < (int)subscriberForThisEventName.size(); ++i)
		{
		
			EventCallbackFunction subscriber = subscriberForThisEventName[i];
		
			subscriber(args);
		}
	}
	//m_eventSystemMutex.unlock();
}

std::vector < std::string > EventSystem::GetAllRegisteredCommands()
{
	return m_registeredCommands;
}

void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction funtionPtr)
{
	g_theEventSystem->SubscribeEventCallbackFunction(eventName, funtionPtr);
}

void UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction funtionPtr)
{
	g_theEventSystem->UnsubscribeEventCallbackFunction(eventName, funtionPtr);
}

void FireEvent(std::string const& eventName, EventArgs& args)
{
	g_theEventSystem->FireEvent(eventName, args);
}

void FireEvent(std::string const& eventName)
{
	g_theEventSystem->FireEvent(eventName);
}
