#pragma once
#define WIN32_LEAN_AND_MEAN		// Always #define this before #including <windows.h>
#include <windows.h>			// #include this (massive, platform-specific) header in very few places
#include <gl/gl.h>					// Include basic OpenGL constants and function declarations
#pragma comment( lib, "opengl32" )	// Link in the OpenGL32.lib static library
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/Rgba.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Core/Vertex.hpp"
#include <map>

enum DRAW_PRIMITIVE
{
	LINES,
	LINE_LOOP,
	TRIANGLES,
	QUADS
};

enum TEXT_DRAW_MODE
{
	SHRINK_TO_FIT,
	WORD_WRAP,
	OVERRUN
};

class Renderer
{
public:
	~Renderer();
	Renderer();
	
	void BeginFrame();
	void EndFrame();
	void SetOrtho(const Vector2& bottomLeft, const Vector2& topRight);

	Texture* CreateOrGetTexture(std::string path);
	BitmapFont* Renderer::CreateOrGetBitmapFont(const char* bitmapFontName);

	void DrawAABB(const AABB2& bounds, const Rgba& color);
	void DrawTexturedAABB(const AABB2& bounds, const Texture& texture, const Vector2& texCoordsAtMins, const Vector2& texCoordsAtMaxs, const Rgba& tint);
	void DrawTextureFromVertexArray(const Vertex3D_PCU * verts, int numVerts, const Texture& texture);
	void DrawPoly(int numSides, float orientation, float radius, const Vector2& center, const Rgba& color);
	void DrawFromVertexArray(const Vector2 vertexArray[], int arraySize, const Vector2& center, float orientation, float scale, const Rgba& color);
	void DrawDashedCircle(const Vector2& center, float radius, Rgba& color);
	void DrawLine(const Vector2& p1, const Vector2& p2, const Rgba& color);
	void DrawLine(const Vector2& p1, const Vector2& p2, const Rgba& colorBegin, const Rgba& colorEnd);
	void DrawText2D( const Vector2& drawMins,
		const std::string& asciiText,
		float cellHeight,
		const Rgba& tint = Rgba::white,
		float aspectScale = 1.f, // multiplied by the font’s inherent m_baseAspect
		const BitmapFont* font = nullptr);
	void DrawTextInBox2D(const AABB2& bounds, 
		const std::string& asciiText,
		float cellHeight, 
		const Rgba & tint = Rgba::white, 
		float aspectScale = 1.f,
		const BitmapFont * font = nullptr,
		TEXT_DRAW_MODE textDrawMode = TEXT_DRAW_MODE::OVERRUN,
		const Vector2& alignment = Vector2(0, 0));

	void DrawMeshImmediate(const Vertex3D_PCU* verts, int numVerts, DRAW_PRIMITIVE drawPrimitive);

	void PushMatrix();
	void TranslateInMatrix(const Vector2& center);
	void RotateInMatrix(const float orientation);
	void ScaleInMatrix(const float scale);
	void PopMatrix();
	void SetAdditiveBlendingActive(bool active);

private:
	int GetPrimitiveEquivalentFromOpenGL(DRAW_PRIMITIVE drawPrimitive) const;

private:
	std::map<std::string, Texture*> m_textureMap;
	std::map<std::string, BitmapFont*> m_loadedFonts;
};