#include "Engine/UI/Widget.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/UI/UISystem.hpp"
#include "Engine/Core/EngineCommon.hpp"
Widget::Widget (Renderer* render, AABB2 const& widgetArea, unsigned int widgetID,
	std::vector<Widget*> children, std::string const& backgroundTextureName, bool isNavigableCanvas, bool isVisible)
	:m_widgetArea(widgetArea),
	m_renderer(render),
	m_widgetID(widgetID),
	m_isVisible(isVisible),
	m_children(children),
	m_isNavigableCanvas(isNavigableCanvas)
{
    for (size_t i = 0; i < children.size(); ++i)
    {
        if (children[i])
        {
            children[i]->m_parent = this;
            SetChildWidgetArea(children[i]);
            if (m_isNavigableCanvas)
            {
                if (children[i]->m_isNavigable)
                {
                    m_navigableChildren.push_back(children[i]);
                }
            }
            //m_children.push_back(children[i]);
        }
    }
    if (backgroundTextureName.compare("") != 0)
    {
        m_backgroundTexture = m_renderer->CreateOrGetTextureFromFile(backgroundTextureName.c_str());
    }

}


Widget::~Widget()
{
    if(g_uiSystem)
    {
        DeleteChildren();
    }
}

void Widget::AddChildren(std::vector<Widget*> children)
{

    for (size_t i = 0; i < children.size(); ++i)
    {
        if (children[i])
        {
            children[i]->m_parent = this;
            SetChildWidgetArea(children[i]);
            if (m_isNavigableCanvas)
            {
                if (children[i]->m_isNavigable)
                {
                    m_navigableChildren.push_back(children[i]);
                }
            }
            m_children.push_back(children[i]);
        }
    }
}

void Widget::AddChild(Widget* child)
{
    if (child)
    {
        child->m_parent = this;
        SetChildWidgetArea(child);
        if (m_isNavigableCanvas)
        {
            if (child->m_isNavigable)
            {
                m_navigableChildren.push_back(child);
            }
        }
        m_children.push_back(child);
    }
}

void Widget::ClearChildren()
{
//     for (auto child : m_children)
//     {
//         delete child;
//     }
    m_children.clear();
}

void Widget::DeleteChildren()
{
    std::vector<Widget*> childrenToDelete;
    childrenToDelete.reserve(m_children.size());

    for (Widget* child : m_children)
    {
        if (child && child != reinterpret_cast<Widget*>(0xFFFFFFFFFFFFFFFF))
        {
            childrenToDelete.push_back(child);
        }
    }

    m_children.clear();

    for (Widget* child : childrenToDelete)
    {
        g_uiSystem->DeleteWidget(child);
    }
}

Widget::Widget(Renderer* render, AABB2 const& widgetArea, std::string const& backgroundTextureName, bool isVisible)
    :m_widgetArea(widgetArea),
    m_renderer(render),
    m_isVisible(isVisible)
{
    if (backgroundTextureName.compare("") != 0)
    {
        m_backgroundTexture = m_renderer->CreateOrGetTextureFromFile(backgroundTextureName.c_str());
    }
}

Widget::Widget(Renderer* render, Vec2 const& widgetDimension, Vec2 const& widgetRelativePos, Vec2 const& widgetPivot, std::string const& backgroundTextureName, bool isVisible)
	:m_renderer(render),
	m_widgetNormalizedDimension(widgetDimension),
	m_widgetRelativePos(widgetRelativePos),
	m_widgetPivot(widgetPivot),
	m_isVisible(isVisible)
{
    if (backgroundTextureName.compare("") != 0)
    {
        m_backgroundTexture = m_renderer->CreateOrGetTextureFromFile(backgroundTextureName.c_str());
    }
}

Widget::Widget(Renderer* render, AABB2 const& widgetArea, Vec2 const& widgetRelativePos, Vec2 const& widgetPivot, std::string const& backgroundTextureName /*= ""*/, bool isVisible /*= true*/)
    :m_renderer(render),
    m_widgetArea(widgetArea),
    m_widgetRelativePos(widgetRelativePos),
    m_widgetPivot(widgetPivot),
    m_isVisible(isVisible)
{
    if (backgroundTextureName.compare("") != 0)
    {
        m_backgroundTexture = m_renderer->CreateOrGetTextureFromFile(backgroundTextureName.c_str());
    }
}

void Widget::Update()
{
    if (!m_isVisible)
    {
        return;
    }
	if (m_isNavigableCanvas)
	{
        bool isChildHoverByMouse = false;
        for (size_t i = 0; i < m_children.size(); ++i)
        {
            if (m_children[i])
            {
                m_children[i]->Update();
                if (m_children[i]->m_isCanvasMouseControl)
                {
                    isChildHoverByMouse = true;
                    break;
                }
            }
        }
        if (!isChildHoverByMouse)
        {
            if (g_theInput->WasKeyJustPressed('W') || g_theInput->WasKeyJustPressed(KEYCODE_UPARROW))
            {
                if (m_selectedWidgetIndex == 0 || m_selectedWidgetIndex == INVALID_INDEX)
                {
                    m_selectedWidgetIndex = (unsigned int)m_navigableChildren.size() - 1;
                }
                else
                {
                    --m_selectedWidgetIndex;
                }
                if (m_selectedWidgetIndex != INVALID_INDEX)
                {
                    for (size_t i = 0; i < m_navigableChildren.size(); ++i)
                    {
                        if ((unsigned int)i != m_selectedWidgetIndex)
                        {
                            m_navigableChildren[i]->OnUnhover();
                        }
                        else
                        {
                            m_navigableChildren[i]->OnHover();
                        }
                    }
                }
            }
            if (g_theInput->WasKeyJustPressed('S') || g_theInput->WasKeyJustPressed(KEYCODE_DOWNARROW))
            {
                if (m_selectedWidgetIndex == (unsigned int)m_navigableChildren.size() - 1 || m_selectedWidgetIndex == INVALID_INDEX)
                {
                    m_selectedWidgetIndex = 0;
                }
                else
                {
                    ++m_selectedWidgetIndex;
                }
                if (m_selectedWidgetIndex != INVALID_INDEX)
                {
                    for (size_t i = 0; i < m_navigableChildren.size(); ++i)
                    {
                        if (i != m_selectedWidgetIndex)
                        {
                            m_navigableChildren[i]->OnUnhover();
                        }
                        else
                        {
                            m_navigableChildren[i]->OnHover();
                        }
                    }
                }
            }
        }
		if (g_theInput->WasKeyJustPressed(KEYCODE_ESC))
		{
			m_selectedWidgetIndex = INVALID_INDEX;
		}

		if (g_theInput->WasKeyJustPressed(KEYCODE_ENTER))
		{
			if (m_selectedWidgetIndex!=INVALID_INDEX && m_navigableChildren[m_selectedWidgetIndex])
			{
				m_navigableChildren[m_selectedWidgetIndex]->OnClick();
			}
		}

	}
    else
    {
        for (size_t i = 0; i < m_children.size(); ++i)
        {
            if (m_children[i])
            {
                m_children[i]->Update();
            }
        }
    }
    
}

void Widget::Render() const
{
	if (m_isVisible)
	{	
        m_renderer->SetBlendMode(BlendMode::ALPHA);
        m_renderer->SetDepthMode(DepthMode::ENABLED);
        m_renderer->SetSamplerMode(SamplerMode::TRILINEAR_WRAP);
        m_renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
        if (m_isShowingBorder)
        {
            RenderBorder();
        }
        if (m_isShowingBackground)
        {
            RenderBackground();
        }
		for (int i = 0; i < (int)m_children.size(); i++)
		{
            if (m_children[i])
            {
                m_children[i]->Render();
            }
		}
	}
}

void Widget::RenderBackground() const
{
    std::vector<Vertex_PCU> verts;
    //m_renderer->SetBlendMode(BlendMode::OPAQUE);
    //m_renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
    AddVertsForAABB2D(verts, m_widgetArea, Rgba8::WHITE, Vec2(), Vec2(1.f, 1.f));
    m_renderer->BindTexture(m_backgroundTexture);
    m_renderer->BindShader(nullptr);
    m_renderer->SetModelConstants(m_transformMatrix, m_renderColor);
    m_renderer->DrawVertexArray((int)verts.size(), verts.data());
    m_renderer->SetDepthMode(DepthMode::DISABLED);

}

void Widget::RenderBorder() const
{
	std::vector<Vertex_PCU> verts;
	//Vec2 edgePoints[4];
    m_renderer->SetModelConstants(m_transformMatrix);
    m_renderer->BindTexture(nullptr);
    Vec2 dimension = m_widgetArea.GetDimensions();
    if (m_borderOuterColor != m_borderColor)
    {
        AABB2 biggerBox = m_widgetArea;
        biggerBox.SetDimensions(Vec2(dimension.x + 4.f, dimension.y + 4.f));
        AddVertsForAABB2D(verts, biggerBox, m_borderOuterColor);
    }

    AABB2 smallerBox = m_widgetArea;
    smallerBox.SetDimensions(Vec2(dimension.x + 2.f, dimension.y + 2.f));
    AddVertsForAABB2D(verts, smallerBox, m_borderColor);
    //m_renderer->SetDepthMode(DepthMode::ENABLED);
    m_renderer->DrawVertexArray((int)verts.size(), verts.data());
}

Mat44 const Widget::GetWidgetModelMatrix() const
{
	return m_transformMatrix;
}



void Widget::SetChildWidgetArea(Widget* child)
{
    if (child->m_widgetNormalizedDimension != Vec2::ZERO)
    {
		child->m_widgetArea = AABB2(0.f, 0.f, child->m_widgetNormalizedDimension.x * m_widgetArea.GetDimensions().x, child->m_widgetNormalizedDimension.y * m_widgetArea.GetDimensions().y);
    }
	Vec2 absolutePos = m_widgetArea.m_mins + (m_widgetArea.GetDimensions() * child->m_widgetRelativePos);
	Vec2 pivotOffset = -child->m_widgetPivot * child->m_widgetArea.GetDimensions();
	Vec2 finalPos = absolutePos + pivotOffset;

    AABB2 newWidgetArea;
    newWidgetArea.m_mins = finalPos;
    newWidgetArea.m_maxs = newWidgetArea.m_mins + child->m_widgetArea.GetDimensions();
	child->m_widgetArea = newWidgetArea;
	for (size_t i = 0; i < child->m_children.size(); ++i)
	{
		child->SetChildWidgetArea(child->m_children[i]);
	}
}

void Widget::SetWidgetAreaFromParent(Widget* parent, bool isUsingNormalizedDimension)
{
    if (!isUsingNormalizedDimension)
    {
        Vec2 widgetSize = m_widgetArea.GetDimensions();
        Vec2 absolutePosition = parent->m_widgetArea.m_mins + (parent->m_widgetArea.GetDimensions() * m_widgetRelativePos);
        Vec2 pivotOffset = -m_widgetPivot * m_widgetArea.GetDimensions();
        Vec2 finalPosition = absolutePosition + pivotOffset;
        m_widgetArea.m_mins = finalPosition;
        m_widgetArea.m_maxs = m_widgetArea.m_mins + widgetSize;
    }
    else
    {
        Vec2 parentSize = parent->m_widgetArea.GetDimensions();
        Vec2 widgetSize = Vec2(m_widgetNormalizedDimension.x * parentSize.x, m_widgetNormalizedDimension.y * parentSize.y);
        Vec2 absolutePosition = parent->m_widgetArea.m_mins + (parentSize * m_widgetRelativePos);
        Vec2 pivotOffset = -m_widgetPivot * widgetSize;
        Vec2 finalPosition = absolutePosition + pivotOffset;
        m_widgetArea.m_mins = finalPosition;
        m_widgetArea.m_maxs = m_widgetArea.m_mins + widgetSize;
    }
    m_transformMatrix = parent->m_transformMatrix;
}

void Widget::SetChildSTransform(Mat44 transformMatrix /*= Mat44()*/)
{
    for (size_t i = 0; i < m_children.size(); ++i)
    {
        if (m_children[i])
        {
            m_children[i]->m_transformMatrix = transformMatrix;
            m_children[i]->SetChildSTransform(transformMatrix);
        }
    }
}

void Widget::SetBorderColor(Rgba8 const& borderMainColor, Rgba8 const& borderOuterColor)
{
    m_borderColor = borderMainColor;
    m_borderOuterColor = borderOuterColor;
    m_isShowingBorder = true;
}

Widget* Widget::GetChildWidget(unsigned int childWidgetID) const
{
    if (m_children[childWidgetID])
    {
        return m_children[childWidgetID];
    }
    else
    {
        return nullptr;
    }
}
