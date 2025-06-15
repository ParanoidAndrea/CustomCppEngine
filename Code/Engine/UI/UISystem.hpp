#pragma once
#include <vector>
class Widget;
struct UISystemConfig
{

};
class UISystem
{
public:
    UISystem(UISystemConfig const& config)
        :m_config(config)
    {

    }
    void Startup();
    void BeginFrame();
    void EndFrame();
    void ShutDown();
    void DeleteWidget(Widget* widget);
private:
    std::vector<Widget*> m_currentGarbageWidgets; // Widgets to delete in current frame
    std::vector<Widget*> m_pendingGarbageWidgets; // Widgets to delete in next frame
    UISystemConfig m_config;
};