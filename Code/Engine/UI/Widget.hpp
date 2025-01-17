#pragma once
#include <vector>
#include "Engine/Render/Renderer.hpp"
class Renderer;
constexpr unsigned int INVALID_INDEX = 0xffffffff;
class Widget
{
public:
	Widget() = default;
	Widget(Renderer* render) :m_renderer(render){}
	Widget(Renderer* render,AABB2 const& widgetArea, unsigned int widgetID, 
		std::vector<Widget*> children, std::string const& backgroundTextureName = "", bool isNavigableCanvas = false, bool isVisible = true);
    Widget(Renderer* render, AABB2 const& widgetArea, std::string const& backgroundTextureName = "", bool isVisible = true);
	Widget(Renderer* render, AABB2 const& widgetArea, Vec2 const& widgetRelativePos, Vec2 const& widgetPivot, std::string const& backgroundTextureName = "", bool isVisible = true);
	Widget(Renderer* render, Vec2 const& widgetDimension, Vec2 const& widgetRelativePos, Vec2 const& widgetPivot, std::string const& backgroundTextureName = "", bool isVisible = true);
	virtual void Update();
	virtual void Render() const;
	virtual void OnClick() {}
    virtual void OnHover()  {}
    virtual void OnUnhover()  {}
	void RenderBackground() const;
	void RenderBorder() const;
	Mat44 const GetWidgetModelMatrix() const;
	void AddChildren(std::vector<Widget*> children);
	void AddChild(Widget* child);
	void ClearChildren() { m_children.clear(); }
	void SetChildWidgetArea(Widget* child);
	void SetWidgetAreaFromParent(Widget* parent, bool isUsingNormalizedDimension);
	void SetChildSTransform(Mat44 transformMatrix = Mat44());
	void SetBorderColor(Rgba8 const& borderMainColor, Rgba8 const& borderOuterColor);
	Widget* GetChildWidget(unsigned int childWidgetID) const;
	void SetWidgetArea(AABB2 const& widgetArea, Vec2 const& widgetRelativePos = Vec2(), Vec2 const& widgetPivot = Vec2()) 
	{ 
		m_widgetArea = widgetArea; m_widgetRelativePos = widgetRelativePos; m_widgetPivot = widgetPivot;
	}
	void SetWidgetArea(Vec2 const& widgetNormalizedDimension, Vec2 const& widgetRelativePos, Vec2 const& widgetPivot)
	{
		m_widgetNormalizedDimension = widgetNormalizedDimension; m_widgetRelativePos = widgetRelativePos; m_widgetPivot = widgetPivot;
	}
	void SetVisibility(bool isVisible) { m_isVisible = isVisible; }
	void SetIsNavigable(bool isNavigable) { m_isNavigable = isNavigable; }
	void SetIsShowingBackground(bool isShowingBackground) { m_isShowingBackground = isShowingBackground; }
	void SetBackgroundTexture(Texture* backgroundTexture) { m_backgroundTexture = backgroundTexture; }
public:
	Mat44 m_transformMatrix;
	Rgba8 m_renderColor;
	bool m_isShowingBackground = true;
	bool m_isNavigableCanvas = false;
	Renderer* m_renderer = nullptr;
	std::vector<Widget*> m_children;
	std::vector<Widget*> m_navigableChildren;
	bool m_isVisible = true;
	unsigned int m_widgetID = 0;
	unsigned int m_selectedWidgetIndex = INVALID_INDEX;
	bool m_isNavigable = false;
	Texture* m_backgroundTexture = nullptr;
	AABB2 m_widgetArea;
	Vec2 m_widgetRelativePos;
	Vec2 m_widgetPivot;
	Vec2 m_widgetNormalizedDimension;
	bool m_isShowingBorder = false;
	Rgba8 m_borderColor = Rgba8::WHITE;
	Rgba8 m_borderOuterColor = Rgba8::BLACK;
	Widget* m_parent = nullptr;
	bool m_isCanvasMouseControl = false;
};