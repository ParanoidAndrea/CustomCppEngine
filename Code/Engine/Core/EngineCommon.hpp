#pragma once
#define  UNUSED(x) (void)(x)
#define  STATIC                   // does nothing; used as CPP marker for class static data & methods
#include <string>
#pragma  warning(disable : 26812) // prefer enum class to enum (yes, but not ALWAYS)
#pragma  warning(disable : 4127)  // conditional expression is constant - we do this a lot for size/align checks
#include "NamedStrings.hpp"
#include "EventSystem.hpp"
#include "ErrorWarningAssert.hpp"
class InputSystem;
class DevConsole;

extern NamedStrings g_gameConfigBlackboard;
extern DevConsole* g_theConsole;
extern EventSystem* g_theEventSystem;
extern InputSystem* g_theInput;