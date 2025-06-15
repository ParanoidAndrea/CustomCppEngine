#pragma once
#define  UNUSED(x) (void)(x)
#define  STATIC                   // does nothing; used as CPP marker for class static data & methods
#include <string>
#pragma  warning(disable : 26812) // prefer enum class to enum (yes, but not ALWAYS)
#pragma  warning(disable : 4127)  // conditional expression is constant - we do this a lot for size/align checks
#include "Engine/Core/NamedStrings.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/UI/UISystem.hpp"
class InputSystem;
class DevConsole;
extern UISystem* g_uiSystem;
extern NamedStrings g_gameConfigBlackboard;
extern DevConsole* g_theConsole;
extern EventSystem* g_theEventSystem;
extern InputSystem* g_theInput;

