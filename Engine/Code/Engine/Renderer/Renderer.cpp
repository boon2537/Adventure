#include "Renderer.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/MathUtils.hpp"

Renderer::~Renderer()
{

}

Renderer::Renderer()
{
	glLineWidth( 1.5f );
	glEnable( GL_BLEND );
	glEnable( GL_LINE_SMOOTH );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	//default value in case we forget to SetOrtho
	glOrtho( 0, 1000, 0, 1000, 0.f, 1.f );
}

void Renderer::BeginFrame()
{
	glLoadIdentity();

	glClearColor( 0.f, 0.f, 0.f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Renderer::EndFrame()
{
	HWND hWnd = GetActiveWindow();
	HDC hDC = GetDC( hWnd );
	SwapBuffers( hDC );
}

void Renderer::SetOrtho(const Vector2 & bottomLeft, const Vector2 & topRight)
{
	glOrtho( bottomLeft.x, topRight.x, bottomLeft.y, topRight.y, 0.f, 1.f );
}

Texture* Renderer::CreateOrGetTexture(std::string path)
{
	std::map<std::string, Texture*>::iterator search = m_textureMap.find(path);
	if (search != m_textureMap.end())
	{
		return search->second;
	}
	else
	{
		Texture* loadedTexture = new Texture(path);
		m_textureMap.insert(std::pair<std::string, Texture*>(path, loadedTexture));
		return loadedTexture;
	}
}

BitmapFont * Renderer::CreateOrGetBitmapFont(const char * bitmapFontName)
{
	std::map<std::string, BitmapFont*>::iterator search = m_loadedFonts.find(bitmapFontName);
	if (search != m_loadedFonts.end())
	{
		return search->second;
	}
	else
	{
		SpriteSheet* glyphSheet = new SpriteSheet(*CreateOrGetTexture("Data/Fonts/" + std::string(bitmapFontName) + ".png"), 16, 16); //should we clean this up?
		BitmapFont* loadedFont = new BitmapFont(bitmapFontName, *glyphSheet, 1);
		m_loadedFonts.insert(std::pair<std::string, BitmapFont*>(bitmapFontName, loadedFont));
		return loadedFont;
	}
}

void Renderer::DrawAABB(const AABB2 & bounds, const Rgba & color)
{
	Vertex3D_PCU vertexs[4];
	vertexs[0] = Vertex3D_PCU(Vector2(bounds.mins.x, bounds.maxs.y), color);
	vertexs[1] = Vertex3D_PCU(Vector2(bounds.mins.x, bounds.mins.y), color);
	vertexs[2] = Vertex3D_PCU(Vector2(bounds.maxs.x, bounds.mins.y), color);
	vertexs[3] = Vertex3D_PCU(Vector2(bounds.maxs.x, bounds.maxs.y), color);
	
	DrawMeshImmediate(vertexs, 4, DRAW_PRIMITIVE::QUADS);
}

void Renderer::DrawTexturedAABB(const AABB2 & bounds, const Texture & texture, const Vector2 & texCoordsAtMins, const Vector2 & texCoordsAtMaxs, const Rgba & tint)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture.m_textureID);

	Vertex3D_PCU vertexs[4];
	vertexs[0] = Vertex3D_PCU(Vector2(bounds.mins.x, bounds.maxs.y), tint, Vector2(texCoordsAtMins.x, texCoordsAtMins.y));
	vertexs[1] = Vertex3D_PCU(Vector2(bounds.mins.x, bounds.mins.y), tint, Vector2(texCoordsAtMins.x, texCoordsAtMaxs.y));
	vertexs[2] = Vertex3D_PCU(Vector2(bounds.maxs.x, bounds.mins.y), tint, Vector2(texCoordsAtMaxs.x, texCoordsAtMaxs.y));
	vertexs[3] = Vertex3D_PCU(Vector2(bounds.maxs.x, bounds.maxs.y), tint, Vector2(texCoordsAtMaxs.x, texCoordsAtMins.y));

	DrawMeshImmediate(vertexs, 4, DRAW_PRIMITIVE::QUADS);
	glDisable(GL_TEXTURE_2D);
}

void Renderer::DrawTextureFromVertexArray(const Vertex3D_PCU * verts, int numVerts, const Texture& texture)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture.m_textureID);

	DrawMeshImmediate(verts, numVerts, DRAW_PRIMITIVE::QUADS);

	glDisable(GL_TEXTURE_2D);
}

void Renderer::DrawPoly(int numSides, float orientation, float radius, const Vector2& center, const Rgba& color)
{
	float degreesPerVertex = 360.f / (float) numSides;
	for (float i = 0; i < numSides; i++)
	{
		float startDegrees = orientation + degreesPerVertex * i;
		float endDegrees = orientation + degreesPerVertex * (i + 1);
		float startX = center.x + (radius * CosDegrees(startDegrees));
		float startY = center.y + (radius * SinDegrees(startDegrees));
		float endX = center.x + (radius * CosDegrees(endDegrees));
		float endY = center.y + (radius * SinDegrees(endDegrees));

		DrawLine(Vector2(startX, startY), Vector2(endX, endY), color);
	}
}

void Renderer::DrawFromVertexArray(const Vector2 vertexArray[], int arraySize, const Vector2& center, float orientation, float scale, const Rgba& color)
{
	glPushMatrix();
	glTranslatef(center.x, center.y, 0);
	glRotatef(orientation, 0, 0, 1);
	glScalef(scale, scale, scale);

	for (int i = 0; i < arraySize - 1; i++)
		DrawLine(Vector2(vertexArray[i].x, vertexArray[i].y), Vector2(vertexArray[i + 1].x, vertexArray[i + 1].y), color);

	glPopMatrix();
}

void Renderer::DrawDashedCircle(const Vector2& center, float radius, Rgba& color)
{
	float degreesPerVertex = 360.f / 30.f;
	for (int i = 0; i < 30; i++)
	{
		if (i % 2 == 0)
		{
			float startDegrees = degreesPerVertex * i;
			float endDegrees = degreesPerVertex * (i + 1);
			float startX = center.x + (radius * CosDegrees(startDegrees));
			float startY = center.y + (radius * SinDegrees(startDegrees));
			float endX = center.x + (radius * CosDegrees(endDegrees));
			float endY = center.y + (radius * SinDegrees(endDegrees));

			DrawLine(Vector2(startX, startY), Vector2(endX, endY), color);
		}
	}
}

void Renderer::DrawLine(const Vector2& p1, const Vector2& p2, const Rgba& color)
{
	Vertex3D_PCU vertexs[2];
	vertexs[0] = Vertex3D_PCU(p1, color);
	vertexs[1] = Vertex3D_PCU(p2, color);

	DrawMeshImmediate(vertexs, 2, DRAW_PRIMITIVE::LINES);
}

void Renderer::DrawLine(const Vector2 & p1, const Vector2 & p2, const Rgba & colorBegin, const Rgba & colorEnd)
{
	Vertex3D_PCU vertexs[2];
	vertexs[0] = Vertex3D_PCU(p1, colorBegin);
	vertexs[1] = Vertex3D_PCU(p2, colorEnd);

	DrawMeshImmediate(vertexs, 2, DRAW_PRIMITIVE::LINES);
}

void Renderer::DrawText2D(const Vector2 & drawMins, const std::string & asciiText, float cellHeight, const Rgba & tint, float aspectScale, const BitmapFont * font)
{
	const BitmapFont* fontToUse = font;
	//use fist font we have as a fall back (if we have any)
	if (fontToUse == nullptr)
	{
		if (m_loadedFonts.size() == 0)
			return;

		fontToUse = m_loadedFonts.begin()->second;
	}

	float cellWidth = cellHeight * aspectScale; //TODO: make cellWidth dynamic when we upgrade from Bitmap font
	float anchorX = drawMins.x;
	float anchorY = drawMins.y;
	AABB2 characterUV;

	for (unsigned int character = 0; character < asciiText.length(); character++)
	{
		characterUV = fontToUse->GetUVsForGlyph(asciiText[character]);
		DrawTexturedAABB(AABB2(anchorX, anchorY, anchorX  + cellWidth, anchorY + cellHeight), fontToUse->m_spriteSheet.GetTexture(), characterUV.mins, characterUV.maxs, tint);
		anchorX += cellWidth;
	}
}

void Renderer::DrawTextInBox2D(const AABB2 & bounds, const std::string & asciiText, float cellHeight, const Rgba & tint, float aspectScale, const BitmapFont * font, TEXT_DRAW_MODE textDrawMode, const Vector2 & alignment)
{
	float scale = 1;
	float leftPadding = 0;
	float topPadding = 0;
	float boxHeight = bounds.GetDimensions().y;
	float boxWidth = bounds.GetDimensions().x;
	float cellHeightWithScale = cellHeight;
	float cellWidthWithScale = cellHeightWithScale * aspectScale; //TODO: make cellWidth dynamic when we upgrade from Bitmap font
	Strings textList = Split(asciiText, '\n');
	float textHeight = cellHeightWithScale * textList.size();
	
	//DEBUG draw box
	//DrawAABB(bounds, Rgba::cyan);

	if (textDrawMode == TEXT_DRAW_MODE::SHRINK_TO_FIT)
	{
		float textMaxWidth = 0;
		for (unsigned int line = 0; line < textList.size(); line++)
		{
			float lineWidth = textList[line].size() * cellWidthWithScale;
			if (lineWidth > textMaxWidth)
				textMaxWidth = lineWidth;
		}

		//determine exact scale to shrink
		float scaleX = 1;
		float scaleY = 1;
		if (textHeight > boxHeight)
			scaleY = boxHeight / textHeight;
		if (textMaxWidth > boxWidth)
			scaleX = boxWidth / textMaxWidth;
		scale = min(scaleX, scaleY);

		//apply scale
		cellHeightWithScale *= scale;
		cellWidthWithScale *= scale;
		textHeight = cellHeightWithScale * textList.size();
	}

	if (textDrawMode == TEXT_DRAW_MODE::WORD_WRAP)
	{
		Strings wrappedTextList;
		for (unsigned int line = 0; line < textList.size(); line++)
		{
			float lineWidth = textList[line].size() * cellWidthWithScale;
			if (lineWidth > boxWidth)
			{
				Strings choppedLine = Split(textList[line], ' ');

				//in case there is no way to split a line
				if (choppedLine.size() <= 1)
				{
					wrappedTextList.push_back(textList[line]);
					continue;
				}

				while (choppedLine.size() > 0)
				{
					std::string newLine = choppedLine[0];
					for (unsigned int index = 0; index < choppedLine.size(); index++)
					{
						// there is only one word left, forced to use it; there is no next word; when you add a new word and a space (hence +1), you will overflow
						if (choppedLine.size() == 1 || index + 1 == choppedLine.size() || (newLine.size() + choppedLine[index + 1].size() + 1) * cellWidthWithScale > boxWidth)
						{
							wrappedTextList.push_back(newLine);
							choppedLine.erase(choppedLine.begin(), choppedLine.begin() + index + 1);
							break;
						}
						// keep adding a word otherwise
						newLine = newLine + " " + choppedLine[index + 1];
					}
				}
			}
			else
				wrappedTextList.push_back(textList[line]);
		}

		//replace text list used to draw
		textList = wrappedTextList;

		//scale down
		textHeight = cellHeightWithScale * textList.size();
		if (textHeight > boxHeight)
		{
			scale = boxHeight / textHeight;
			cellHeightWithScale *= scale;
			cellWidthWithScale *= scale;
			textHeight = cellHeightWithScale * textList.size();
		}
	}

	topPadding = (boxHeight - textHeight) * alignment.y;
	float startY = bounds.mins.y + boxHeight - topPadding - cellHeightWithScale;
	for (unsigned int line = 0; line < textList.size(); line++)
	{
		leftPadding = (boxWidth - (textList[line].size() * cellWidthWithScale)) * alignment.x;
		float startX = bounds.mins.x + leftPadding;
		DrawText2D(Vector2(startX, startY), textList[line], cellHeightWithScale, tint, aspectScale, font);
		startY -= cellHeightWithScale;
	}
}

void Renderer::DrawMeshImmediate(const Vertex3D_PCU * verts, int numVerts, DRAW_PRIMITIVE drawPrimitive)
{
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	glVertexPointer(	3, GL_FLOAT,			sizeof( Vertex3D_PCU ), &verts[0].m_position );
	glColorPointer(		4, GL_UNSIGNED_BYTE,	sizeof( Vertex3D_PCU ), &verts[0].m_color );
	glTexCoordPointer(	2, GL_FLOAT,			sizeof( Vertex3D_PCU ), &verts[0].m_UVs );

	glDrawArrays( GetPrimitiveEquivalentFromOpenGL(drawPrimitive), 0, numVerts );

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
}

void Renderer::PushMatrix()
{
	glPushMatrix();
}

void Renderer::TranslateInMatrix(const Vector2 & center)
{
	glTranslatef(center.x, center.y, 0);
}

void Renderer::RotateInMatrix(const float orientation)
{
	glRotatef(orientation, 0, 0, 1);
}

void Renderer::ScaleInMatrix(const float scale)
{
	glScalef(scale, scale, scale);
}

void Renderer::PopMatrix()
{
	glPopMatrix();
}

void Renderer::SetAdditiveBlendingActive(bool active)
{
	if (active)
		glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	else
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}

int Renderer::GetPrimitiveEquivalentFromOpenGL(DRAW_PRIMITIVE drawPrimitive) const
{
	if (drawPrimitive == DRAW_PRIMITIVE::LINES)			return GL_LINES;
	if (drawPrimitive == DRAW_PRIMITIVE::LINE_LOOP)		return GL_LINE_LOOP;
	if (drawPrimitive == DRAW_PRIMITIVE::TRIANGLES)		return GL_TRIANGLES;
	if (drawPrimitive == DRAW_PRIMITIVE::QUADS)			return GL_QUADS;

	ERROR_RECOVERABLE("Cannot Find drawPrimitive: " + drawPrimitive);
	return -1;
}
