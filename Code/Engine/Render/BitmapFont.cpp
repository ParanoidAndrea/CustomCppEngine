#include "BitmapFont.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
BitmapFont::BitmapFont(char const* fontFilePathNameWithNoExtension, Texture& fontTexture)
	:m_fontFilePathNameWithNoExtension(std::string(fontFilePathNameWithNoExtension)),
	m_fontGlyphsSpriteSheet(fontTexture, IntVec2(16, 16))
{
}

Texture& BitmapFont::GetTexture()
{
	return m_fontGlyphsSpriteSheet.GetTexture();
}

void BitmapFont::AddVertsForText2D(std::vector<Vertex_PCU>& vertexArray, Vec2 const& textMins, float cellHeight, std::string const& text, Rgba8 const& tint, float cellAspect, int maxGlyphsToDraw)
{
	float cellWidth = cellHeight * cellAspect;

	for (int i = 0; i < (int)text.length(); ++i)
	{
		if (i >= maxGlyphsToDraw)
			return;
		char glyph = text[i];
		Vec2 characterPosMins = textMins + Vec2(cellWidth * i, 0.0f);
		Vec2 characterPosMaxs = characterPosMins + Vec2(cellWidth, cellHeight);
		Vec2 uvMins, uvMaxs;
		m_fontGlyphsSpriteSheet.GetSpriteUVs(uvMins, uvMaxs, static_cast<int>(glyph));
		AddVertsForAABB2D(vertexArray, AABB2(characterPosMins, characterPosMaxs), tint, uvMins, uvMaxs);
	}
}

void BitmapFont::AddVertsForTextBox2D(std::vector<Vertex_PCU>& vertexArray, AABB2 const& box, float cellHeight, std::string const& text, Rgba8 const& tint, float cellAspect, Vec2 const& alignment, TextBoxMode mode, int maxGlyphsToDraw, Vec2 offset)
{
	float boxWidth = box.m_maxs.x - box.m_mins.x;
	float boxHeight = box.m_maxs.y - box.m_mins.y;
	Strings lineStrings = SplitStringOnDelimiter(text, '\n');
	float textHeight = (float)lineStrings.size() * cellHeight;
	float textMinsY = 0.f;
	float longestWidth = GetLongestLineWidth(lineStrings, cellHeight, cellAspect);
	float xScale = boxWidth / longestWidth;
	float yScale = boxHeight / textHeight;
	switch (mode)
	{
	    case OVERRUN:

			textMinsY = box.m_mins.y + boxHeight * alignment.y + textHeight * (1.f - alignment.y) - cellHeight;
			if ((textMinsY + cellHeight - textHeight) < box.m_mins.y)
			{
				textMinsY = box.m_mins.y - cellHeight + textHeight;
			}
	    	for (int i = 0; i < (int)lineStrings.size(); ++i)
	    	{
	    		float textWidth = GetTextWidth(cellHeight, lineStrings[i],cellAspect);
	    		float textMinsX = box.m_mins.x + (boxWidth - textWidth) * alignment.x;
	    		AddVertsForText2D(vertexArray, Vec2(textMinsX, textMinsY)+offset, cellHeight, lineStrings[i], tint, cellAspect, maxGlyphsToDraw);
				textMinsY -= cellHeight;
	    	}
	        break;
        case SHRINK_TO_FIT:

			if (yScale<1.f || xScale<1.f)
			{
				if (yScale <= xScale)
				{
					cellHeight *= yScale;
				}
				else
				{
					cellHeight *= xScale;
				}
			}
// 
// 			textHeight = (float)lineStrings.size() * cellHeight;
// 			textMinsY = box.m_mins.y + boxHeight * alignment.y - textHeight * 0.5f+cellHeight;
// 			for (int i = 0; i < (int)lineStrings.size(); ++i)
// 			{
// 				float textWidth = GetTextWidth(cellHeight, lineStrings[i], cellAspect);
// 				float textMinsX = box.m_mins.x + boxWidth* alignment.x-longestWidth*0.5f;
// 				AddVertsForText2D(vertexArray, Vec2(textMinsX, textMinsY), cellHeight, lineStrings[i], tint, cellAspect);
// 				textMinsY -= cellHeight;
// 			}
			textHeight = (float)lineStrings.size() * cellHeight;
/*			textMinsY = box.m_mins.y + (box.m_maxs.y - textHeight)* alignment.y;*/
			textMinsY = box.m_mins.y + boxHeight * alignment.y + textHeight * (1.f - alignment.y) - cellHeight;
			if ((textMinsY +cellHeight- textHeight * alignment.y) < box.m_mins.y)
			{
				textMinsY = box.m_mins.y -cellHeight+ textHeight * alignment.y;
			}
			for (int i = 0; i < (int)lineStrings.size(); ++i)
			{
				float textWidth = GetTextWidth(cellHeight, lineStrings[i], cellAspect);
				float textMinsX = box.m_mins.x + (boxWidth - textWidth) * alignment.x;
// 				if ((textMinsX +cellAspect*cellHeight - longestWidth) < box.m_mins.x)
// 				{
// 					textMinsX = box.m_mins.y - cellAspect * cellHeight + longestWidth;
// 				}
				AddVertsForText2D(vertexArray, Vec2(textMinsX, textMinsY)+offset, cellHeight, lineStrings[i], tint, cellAspect, maxGlyphsToDraw);
				textMinsY -= cellHeight;
			}
		break;
		case WRAP:
			Strings wrappedLines;
			for ( std::string const& line : lineStrings)
			{
				Strings words = SplitStringOnDelimiter(line, ' ');
				std::string currentLine;
				float currentLineWidth = 0.f;

				for ( std::string const& word : words)
				{
					float wordWidth = GetTextWidth(cellHeight, word, cellAspect);
					if (currentLineWidth + wordWidth > boxWidth)
					{
						wrappedLines.push_back(currentLine);
						currentLine = word + " ";
						currentLineWidth = wordWidth + GetTextWidth(cellHeight, " ", cellAspect);
					}
					else
					{
						currentLine += word + " ";
						currentLineWidth += wordWidth + GetTextWidth(cellHeight, " ", cellAspect);
					}
				}
				if (!currentLine.empty())
				{
					wrappedLines.push_back(currentLine);
				}
			}

			textHeight = (float)wrappedLines.size() * cellHeight;
			textMinsY = box.m_mins.y + boxHeight * alignment.y + textHeight * (1.f - alignment.y) - cellHeight;
			if ((textMinsY + cellHeight - textHeight * alignment.y) < box.m_mins.y)
			{
				textMinsY = box.m_mins.y - cellHeight + textHeight * alignment.y;
			}

			for (const std::string& line : wrappedLines)
			{
				float textWidth = GetTextWidth(cellHeight, line, cellAspect);
				float textMinsX = box.m_mins.x + (boxWidth - textWidth) * alignment.x;
				AddVertsForText2D(vertexArray, Vec2(textMinsX, textMinsY) + offset, cellHeight, line, tint, cellAspect, maxGlyphsToDraw);
				textMinsY -= cellHeight;
			}
		
		break;
     }
}

void BitmapFont::AddVertsForTextBox3DArOriginXForward(std::vector<Vertex_PCU>& verts, float cellHeight, std::string const& text, Rgba8 const& tint, float cellAspect, Vec2 const& alignment, int maxGlyphsToDraw)
{

	Strings lineStrings = SplitStringOnDelimiter(text, '\n');
	float textHeight = (float)lineStrings.size() * cellHeight;
	float textMinsY = 0.f;
	textMinsY = -textHeight * alignment.y - cellHeight;
	if ((textMinsY + cellHeight - textHeight) <0.f)
	{
		textMinsY = 0.f- cellHeight + textHeight;
	}
	for (int i = 0; i < (int)lineStrings.size(); ++i)
	{
		float textWidth = GetTextWidth(cellHeight, lineStrings[i], cellAspect);
		float textMinsX = - textWidth * alignment.x;
		AddVertsForText2D(verts ,Vec2(textMinsX, textMinsY), cellHeight, lineStrings[i], tint, cellAspect, maxGlyphsToDraw);

		//fixupTransform.SetTranslation3D(Vec3(textMinsX, 0.f, textMinsY)); 

		textMinsY -= cellHeight;
	}
	Mat44 fixupTransform = EulerAngles(90.f, 0.f, 90.f).GetAsMatrix_IFwd_JLeft_KUp();  // Rotate around Y-axis
	TransformVertexArray3D(verts, fixupTransform);
}

float BitmapFont::GetTextWidth(float cellHeight, std::string const& text, float cellAspect)
{
	float totalWidth = 0.0f;
	for (int i = 0; i < (int)text.length(); ++i)
	{
		totalWidth += cellHeight*cellAspect; 
	}
	return totalWidth;
}

float BitmapFont::GetGlyphAspect(int glyphUnicode) const
{
	UNUSED(glyphUnicode);
	return 1.f;
}

float BitmapFont::GetLongestLineWidth(Strings textLines, float cellHeight, float cellAspect)
{
	float longestWidth = 0.f;
	for (int i = 0; i < (int)textLines.size(); ++i)
	{
		if ((float)textLines[i].length()*cellAspect*cellHeight > longestWidth)
			longestWidth = (float)textLines[i].length() * cellAspect * cellHeight;
	}
	return longestWidth;
}
