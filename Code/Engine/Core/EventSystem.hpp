#pragma once
#include <vector>
#include "Engine/Core/NamedStrings.hpp"
#include <mutex>
typedef NamedStrings EventArgs;
typedef bool (*EventCallbackFunction)(EventArgs& args);
typedef std::vector<EventCallbackFunction> SubscriptionList;


struct EventSubscription
{

};
struct EventSystemConfig
{

};


class EventSystem
{
public:
	EventSystem(EventSystemConfig const& config);
	~EventSystem();
	void Startup();
	void Shutdown();
	void BeginFrame();
	void EndFrame();
	void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
	void UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
	void FireEvent(std::string const& eventName, EventArgs& args);
	void FireEvent(std::string const& eventName);
	std::vector < std::string > GetAllRegisteredCommands();
	
protected:
	EventSystemConfig m_config;
	std::map<std::string, SubscriptionList> m_subscriptionListByName;
	std::vector<std::string> m_registeredCommands;
	mutable std::mutex m_eventSystemMutex;
};
void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
void UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
void FireEvent(std::string const& eventName, EventArgs& args);
void FireEvent(std::string const& eventName);