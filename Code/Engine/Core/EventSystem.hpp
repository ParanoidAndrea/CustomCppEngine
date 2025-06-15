#pragma once
#include <vector>
#include "Engine/Core/NamedStrings.hpp"
#include "Engine/Core/NamedProperties.hpp"
#include <mutex>
//typedef NamedStrings EventArgs;
typedef NamedProperties EventArgs;
typedef bool (*EventCallbackFunction)(EventArgs& args);




struct EventSystemConfig
{

};

struct EventSubscriptionBase
{
public:
	EventSubscriptionBase() = default;
	virtual ~EventSubscriptionBase() = default;
	virtual bool Fire(EventArgs& args) = 0;
	virtual bool IsForObject(void* objectPtr)
	{
		(void)(objectPtr);
		return false;
	}

};

struct EventSubscriptionFunction : public EventSubscriptionBase
{
    EventSubscriptionFunction(EventCallbackFunction functionPtr)
        :m_functionPtr(functionPtr)
    {

    }
    EventCallbackFunction m_functionPtr = nullptr;
    virtual bool Fire(EventArgs& args) override
    {
        return (m_functionPtr)(args);
    }
};

template<typename T_ObjectType>
struct EventSubscriptionMemberMethod : public EventSubscriptionBase
{
    typedef bool (T_ObjectType::* EventCallbackMemberFunction) (EventArgs& args);
    EventSubscriptionMemberMethod(T_ObjectType* pointerToObject, EventCallbackMemberFunction methodPtr)
        :m_methodptr(methodPtr),
		m_objectPtr(pointerToObject)
    {

    }

    EventCallbackMemberFunction m_methodptr = nullptr;
    T_ObjectType* m_objectPtr = nullptr;

	virtual bool Fire(EventArgs& args) override
	{
		return (m_objectPtr->*m_methodptr)(args);
	}
    virtual bool IsForObject(void* objectPtr) override
    {
		return m_objectPtr == objectPtr;
    }
};

typedef std::vector<EventSubscriptionBase*> SubscriptionList;

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

	template<typename T_ObjectType>
	void SubscribeEventCallbackObjectMethod(std::string const& eventName, T_ObjectType* objectPtr,
		bool (T_ObjectType ::* methodPtr) (EventArgs& args));

    template<typename T_ObjectType>
    void UnsubscribeEventCallbackObjectMethod(std::string const& eventName, T_ObjectType* objectPtr,
        bool (T_ObjectType ::* methodPtr) (EventArgs& args));

    template<typename T_ObjectType>
    void UnsubscribeAllEventCallbackObjectMethods(T_ObjectType* objectPtr);

    template<typename T_ObjectType>
    void ReplaceEventCallbackObjectMethod(std::string const& eventName, T_ObjectType* objectPtr,
        bool (T_ObjectType ::* methodPtr) (EventArgs& args));

	void FireEvent(std::string const& eventName, EventArgs& args);
	void FireEvent(std::string const& eventName);
	std::vector <std::string> const GetAllRegisteredCommands() const;
	
protected:
	EventSystemConfig m_config;
	std::map<HashedCaseInsensitiveString, SubscriptionList> m_subscriptionListByName;
	std::vector<std::string> m_registeredCommands;
	mutable std::mutex m_eventSystemMutex;
};

template<typename T_ObjectType>
void EventSystem::SubscribeEventCallbackObjectMethod(std::string const& eventName, T_ObjectType* objectPtr, bool (T_ObjectType ::* methodPtr) (EventArgs& args))
{
    m_eventSystemMutex.lock();

    m_registeredCommands.push_back(eventName);
    EventSubscriptionBase* subscription = new EventSubscriptionMemberMethod<T_ObjectType>(objectPtr, methodPtr);
    m_subscriptionListByName[eventName].push_back(subscription);

    m_eventSystemMutex.unlock();

}

template<typename T_ObjectType>
void EventSystem::UnsubscribeEventCallbackObjectMethod(std::string const& eventName, T_ObjectType* objectPtr, bool (T_ObjectType ::* methodPtr) (EventArgs& args))
{
    UNUSED(methodPtr);
    m_eventSystemMutex.lock();

    auto found =  m_subscriptionListByName.find(eventName);
    if (found !=  m_subscriptionListByName.end())
    {
        std::vector<EventSubscriptionBase*>& subscribers = found->second;

        for (auto it = subscribers.begin(); it != subscribers.end(); )
        {
            if ((*it)->IsForObject(objectPtr))
            {
                delete* it;
                it = subscribers.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    m_eventSystemMutex.unlock();
}

template<typename T_ObjectType>
void EventSystem::UnsubscribeAllEventCallbackObjectMethods(T_ObjectType* objectPtr)
{
    m_eventSystemMutex.lock();
    for (auto eventName : m_registeredCommands)
    {
        auto found = m_subscriptionListByName.find(eventName);
        if (found != m_subscriptionListByName.end())
        {
            std::vector<EventSubscriptionBase*>& subscribers = found->second;

            for (auto it = subscribers.begin(); it != subscribers.end(); )
            {
                if ((*it)->IsForObject(objectPtr))
                {
                    delete* it;
                    it = subscribers.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }
    }
    m_eventSystemMutex.unlock();
}

template<typename T_ObjectType>
void EventSystem::ReplaceEventCallbackObjectMethod(std::string const& eventName, T_ObjectType* objectPtr, bool (T_ObjectType ::* methodPtr) (EventArgs& args))
{
     m_eventSystemMutex.lock();

     
    auto found =  m_subscriptionListByName.find(eventName);
    if (found !=  m_subscriptionListByName.end())
    {
        std::vector<EventSubscriptionBase*>& subscribers = found->second;
        bool replaced = false;
        for (auto it = subscribers.begin(); it != subscribers.end(); ++it)
        {
            if ((*it)->IsForObject(objectPtr))
            {
                delete* it;
                *it = new EventSubscriptionMemberMethod<T_ObjectType>(objectPtr, methodPtr);
                replaced = true;
                break;
            }
            
        }
        if (!replaced)
        {
            subscribers.push_back(new EventSubscriptionMemberMethod<T_ObjectType>(objectPtr, methodPtr));
        }
    }
    else
    {
        m_registeredCommands.push_back(eventName);
        EventSubscriptionBase* subscription = new EventSubscriptionMemberMethod<T_ObjectType>(objectPtr, methodPtr);
        m_subscriptionListByName[eventName].push_back(subscription);
    }

    m_eventSystemMutex.unlock();
}

void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
void UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
void FireEvent(std::string const& eventName, EventArgs& args);
void FireEvent(std::string const& eventName);

class EventRecipient
{
    virtual ~EventRecipient();
};