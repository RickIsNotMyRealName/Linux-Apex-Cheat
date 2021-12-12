#pragma once
#include "imgui/imgui.h"
#include "struct.h"

namespace Overlay
{
    //int tabs = 1;
    int RenderLoop();
    void RenderMenu();
    void SetTheme();
    void loadMultiTex(int index);
    void MainLoop();
    void RenderPlayerEsp();
    void RenderItemEsp();
    void RenderRadar();
    void test();


    void DrawLine(ImVec2 a, ImVec2 b, ImColor color, float width);
	void DrawBox(ImColor color, float x, float y, float w, float h);
	void Text(ImVec2 pos, ImColor color, const char* text_begin, const char* text_end, float wrap_width, const ImVec4* cpu_fine_clip_rect,bool centerText);
    void DrawOutlinedText(ImVec2 pos, ImColor color, const char *text_begin, const char *text_end, float wrap_width, const ImVec4 *cpu_fine_clip_rect, bool centerText);
	void RectFilled(float x0, float y0, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags);
	void ProgressBar(float x, float y, float w, float h, int value, int v_max);
	void String(ImVec2 pos, ImColor color, const char* text,bool centerText);
    void StringOutlined(ImVec2 pos, ImColor color, const char *text, bool centerText);
	void drawBackGroundBox(int charAmt, bool bulleted);
}


//idk if this should be here or where it should be
bool WorldToScreen(Vector from, float* m_vMatrix, int targetWidth, int targetHeight, Vector& to);

#define GREEN ImColor(0, 255, 0)
#define RED ImColor(255, 0, 0)
#define BLUE ImColor(0, 0, 255)
#define ORANGE ImColor(255, 165, 0)
#define WHITE ImColor(255, 255, 255)
#define BLACK ImColor(0, 0, 0)