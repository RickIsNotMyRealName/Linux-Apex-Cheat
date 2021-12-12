// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs
#include "overlay.h"
#include "settings.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <stdio.h>
#include <thread>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include <unistd.h>
#include <linux/input.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <linux/input-event-codes.h>
#include "imgui/imguipp.h"
#include "imgui/icons.h"
#define STB_IMAGE_IMPLEMENTATION
#include "imgui/stb_image.h"
#include "offsets.h"
#include "struct.h"
#include "entity.h"
#include "aimBotSettings.h"
#include "ini.h"
#include <filesystem>
#include <vector>
#include <string>
#include "configManager.hpp"

// this is a very messy file, but it works

int display_w, display_h;
bool k_ins = false;
bool k_f4 = false;

//should move this and there is probably a better way to do this
typedef struct legendIcons
{
	int index; // 1-18
	GLuint my_texture = NULL;
	int my_image_width = 1280;
	int my_image_height = 1280;
};

GLuint docTex = NULL;
int doc_width = 513;
int doc_height = 513;


legendIcons legIcons[19];

// this function sorta works im not sure how to make it better. if you know how to make it better please let me know lol
int GetKeys()
{
	char *kbd_dev = "/dev/input/by-id/usb-Razer_Razer_Cynosa_Chroma-event-kbd";
	while (Settings::active)
	{
		// kbd_dev should be keyboard device found in /dev/input (need root by default)
		int kbd = open(kbd_dev, O_RDONLY);
		if (kbd != -1)
		{
			input_event ie;
			read(kbd, &ie, sizeof(ie));
			if (ie.code == 4 && ie.value == 458825 && k_ins == false)
			{
				Settings::showMenu = !Settings::showMenu;
				k_ins = true;
			}
			else if (ie.code == 4 && k_ins == true)
			{
				k_ins = false;
			}

			if (ie.code == 4 && ie.value == 458813 && k_ins == false)
			{
				Settings::showMenu = !Settings::showMenu;
				k_ins = true;
			}
			else if (ie.code == 4 && k_ins == true)
			{
				k_ins = false;
			}
			// std::cout << "Key Code: " << ie.code << "    Value: " << ie.value << "\n";
			close(kbd);
		}
	}
}

int tabs = 1;

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

ImU32 GetU32(int r, int b, int g, int a)
{
	return ((a & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8) + (r & 0xff);
}

bool LoadTextureFromFile(const char *filename, GLuint *out_texture, int *out_width, int *out_height)
{
	// Load from file
	int image_width = 0;
	int image_height = 0;
	unsigned char *image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	*out_texture = image_texture;
	*out_width = image_width;
	*out_height = image_height;

	return true;
}

void loadTex()
{
	bool ret = LoadTextureFromFile(".//icons/menuIcons/greenDrone.png", &docTex, &doc_width, &doc_height);
	IM_ASSERT(ret);
}

void Overlay::loadMultiTex(int index)
{
	std::string finalStr = ".//icons/legendIcons/" + std::to_string(index) + ".png";
	const char *finalChar = finalStr.c_str();

	std::string strIndex = std::to_string(index);
	const char *s = strIndex.c_str();
	bool ret = LoadTextureFromFile(finalChar, &legIcons[index].my_texture, &legIcons[index].my_image_width, &legIcons[index].my_image_height);
	IM_ASSERT(ret);
	legIcons[index].index = index;
}

void Overlay::SetTheme()
{
	ImGuiStyle *style = &ImGui::GetStyle();
	style->WindowTitleAlign = ImVec2(0.5, 0.5);

	style->WindowBorderSize = 1;
	style->TabBorderSize = 1;
	style->FrameBorderSize = 1;
	style->WindowRounding = 12;
	style->FrameRounding = 12;
	style->TabRounding = 9;
	style->Alpha = .95;

	style->WindowMinSize = ImVec2(700, 450);

	ImVec4 darkBlue = ImColor(19, 57, 84);
	ImVec4 lightBlue = ImColor(98, 130, 144);
	ImVec4 darkGreen = ImColor(70, 137, 2);
	ImVec4 brightGreen = ImColor(110, 222, 0);
	ImVec4 black = ImColor(0, 0, 0);

	ImVec4 col_text = ImColor(30, 255, 0); // green -
	ImVec4 col_main = ImColor(255, 0, 0);  // red - title bar,slider,silder bar,button checks,button hover
	ImVec4 col_back = ImColor(0, 0, 0);	   // black
	ImVec4 col_area = ImColor(41, 41, 41); // dark grey


	ImColor limeGreen = ImColor(30, 255, 0, 255);


	style->Colors[ImGuiCol_Text] = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(col_text.x, col_text.y, col_text.z, 0.58f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(col_back.x, col_back.y, col_back.z, 1.f);
	// style->Colors[ImGuiCol_ChildWindowBg] = ImColor(43, 48, 58);
	style->Colors[ImGuiCol_Border] = ImVec4(brightGreen.x, brightGreen.y, brightGreen.z, 1.00f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(brightGreen.x, brightGreen.y, brightGreen.z, 1.0f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(col_area.x, col_area.y, col_area.z, col_area.w + .1f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(col_area.x, col_area.y, col_area.z, col_area.w + .1f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(col_area.x, col_area.y, col_area.z, col_area.w + .1f);
	style->Colors[ImGuiCol_TitleBg] = black;		  //
	style->Colors[ImGuiCol_TitleBgCollapsed] = black; //
	style->Colors[ImGuiCol_TitleBgActive] = black;	  //
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(col_area.x, col_area.y, col_area.z, 0.57f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(col_text.x, col_text.y, col_text.z, 0.85f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(col_text.x, col_text.y, col_text.z, 0.85f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);

	style->Colors[ImGuiCol_CheckMark] = ImVec4(col_text.x, col_text.y, col_text.z, 255);  //---------------
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(col_text.x, col_text.y, col_text.z, 255); //
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);

	style->Colors[ImGuiCol_Button] = ImVec4(black.x, black.y, black.z, 0.50f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(black.x, black.y, black.z, col_area.w + .1f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(black.x + 20, black.y + 20, black.z + 20, 0);

	style->Colors[ImGuiCol_Header] = ImVec4(col_main.x, col_main.y, col_main.z, 0.76f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.86f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);

	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(darkGreen.x, darkGreen.y, darkGreen.z, 1.f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(brightGreen.x, brightGreen.y, brightGreen.z, 0.78f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(brightGreen.x, brightGreen.y, brightGreen.z, 1.00f);

	style->Colors[ImGuiCol_PlotLines] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.43f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(col_back.x, col_back.y, col_back.z, 1.00f);

	style->Colors[ImGuiCol_Tab] = black;
	style->Colors[ImGuiCol_TabActive] = black;
	style->Colors[ImGuiCol_TabHovered] = black;
	style->Colors[ImGuiCol_TabUnfocused] = black;
	style->Colors[ImGuiCol_TabUnfocusedActive] = black;


	style->Colors[ImGuiCol_Separator] = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);

}

void Overlay::RenderMenu()
{
	ImGui::Begin("D.O.C", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImGui::Columns(2);
	ImGui::SetColumnOffset(1, 175);
	{
		// Left Side

		if (ImGui::Button("Aimbot", ImVec2(175 - 15, 41)))
		{
			tabs = 1;
		}
		ImGui::Spacing();

		if (ImGui::Button("ESP", ImVec2(175 - 15, 41)))
		{
			tabs = 2;
		}
		ImGui::Spacing();

		if (ImGui::Button("Radar", ImVec2(175 - 15, 41)))
		{
			tabs = 3;
		}
		ImGui::Spacing();

		if (ImGui::Button("Weapon Config", ImVec2(175 - 15, 41)))
		{
			tabs = 4;
		}
		ImGui::Spacing();

		if (ImGui::Button("Other", ImVec2(175 - 15, 41)))
		{
			tabs = 5;
		}


		ImVec2 oldPos = ImGui::GetCursorPos();
		ImGui::SetCursorPos(ImVec2(25, ImGui::GetWindowHeight() - 165));
		ImGui::Image((void *)(intptr_t)docTex, ImVec2(doc_width / 4, doc_height / 4));

		// ImGui::SetCursorPos(ImVec2(67, ImGui::GetWindowHeight() - 85));
		// ImGui::Image(legIcons[3].my_texture, ImVec2(50, 50));

		ImGui::SetCursorPos(ImVec2(oldPos.x, ImGui::GetWindowHeight() - 30));
		imguipp::center_text_ex("RickIsntMyRealName#3966", 175, 1, false);
	}

	ImGui::NextColumn();

	if (tabs == 1)
	{
		if (ImGui::BeginTabBar("Aimbot Tab Bar"))
		{
			if (ImGui::BeginTabItem("General Aimbot"))
			{
				ImGui::Checkbox("Aimbot Enabled", &Settings::curConfig.aimbotEnabled);
				if (Settings::curConfig.aimbotEnabled)
				{
					ImGui::SameLine();
					ImGui::Checkbox("Visibility check", &Settings::curConfig.visCheck);
					ImGui::SameLine();
					ImGui::Checkbox("RCS Enabled", &Settings::curConfig.rcsEnabled);
					ImGui::SameLine();
					ImGui::Checkbox("Friendly Fire", &Settings::curConfig.friendlyFire);
					ImGui::Checkbox("Dynamic Bone", &Settings::curConfig.dynamicBone);
					ImGui::SliderFloat("No Recoil X%", &Settings::curConfig.rcsX, 0.f, 100.f, "%1.f");
					ImGui::SliderFloat("No Recoil Y%", &Settings::curConfig.rcsY, 0.f, 100.f, "%1.f");
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Smooth Aimbot"))
			{
				ImGui::Text("Smooth aim value:");
				ImGui::SliderFloat("##2", &Settings::curConfig.aimbotSmooth, 12.0f, 150.0f, "%.2f");

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
	if (tabs == 2)
	{
		if (ImGui::BeginTabBar("ESP Tab Bar"))
		{
			if (ImGui::BeginTabItem("Esp Config"))
			{
				ImGui::Checkbox("ESP Enabled", &Settings::curConfig.espEnabled);
				ImGui::Checkbox("Debug Glow", &Settings::curConfig.debugGlowEnabled);
				ImGui::Separator();
				ImGui::Text("ESP options:");
				ImGui::Text("Max ESP distance");
				ImGui::SliderFloat("maxEspDist", &Settings::curConfig.maxEspDist, 100.f * 40, 800.f * 40, "%.2f");
				ImGui::Checkbox("Box", &Settings::curConfig.boxEnabled);
				ImGui::SameLine(0, 70.0f);
				ImGui::Checkbox("Name", &Settings::curConfig.nameEnabled);
				ImGui::Checkbox("Line", &Settings::curConfig.lineEnabled);

				// ImGui::Checkbox(XorStr("Distance"), &v.distance);
				ImGui::Checkbox("Health bar", &Settings::curConfig.healthEnabled);

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Item Esp Config"))
			{
				ImGui::Checkbox("Item ESP Enabled", &Settings::curConfig.itemEspEnabled);
				ImGui::Separator();
				ImGui::Text("Item ESP options:");
				ImGui::Text("Max distance");
				ImGui::SliderFloat("maxItemEspDist", &Settings::curConfig.itemEspDist, 100.f * 40, 800.f * 40, "%.2f");
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Item Glow Config"))
			{
				ImGui::Checkbox("Enable Item Glow", &Settings::curConfig.itemGlowEnabled);
				ImGui::Separator();
				ImGui::ColorPicker3("Color Picker", Settings::curConfig.itemColor, ImGuiColorEditFlags_NoInputs);

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Player Glow Config"))
			{
				ImGui::Checkbox("Enable Player Glow", &Settings::curConfig.playerGlowEnabled);
				ImGui::Separator();
				ImGui::ColorPicker3("Color Picker", Settings::curConfig.playerColor, ImGuiColorEditFlags_NoInputs);

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Dummy Glow Config"))
			{
				ImGui::Checkbox("Enable Dummy Glow", &Settings::curConfig.dummyGlowEnabled);
				ImGui::Separator();
				ImGui::ColorPicker3("Color Picker", Settings::curConfig.dummyColor, ImGuiColorEditFlags_NoInputs);

				ImGui::EndTabItem();
			}

			if (Settings::curConfig.debugGlowEnabled)
			{
				if (ImGui::BeginTabItem("Glow Mode Config"))
				{
					static int8_t step = 1;
					static int8_t step_fast = 64;
					ImGui::InputScalar("General Glow Mode", ImGuiDataType_S8, &Settings::curConfig.glowMode.GeneralGlowMode, &step, &step_fast);
					ImGui::InputScalar("Border Glow Mode", ImGuiDataType_S8, &Settings::curConfig.glowMode.BorderGlowMode, &step, &step_fast);
					ImGui::InputScalar("Border Size", ImGuiDataType_S8, &Settings::curConfig.glowMode.BorderSize, &step, &step_fast);
					ImGui::InputScalar("Transparent Level", ImGuiDataType_S8, &Settings::curConfig.glowMode.TransparentLevel, &step, &step_fast);
					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabBar();
		}
	}
	if (tabs == 3)
	{
		ImGui::Checkbox("Enabled: ", &Settings::curConfig.radarEnabled);
		ImGui::SameLine();
		ImGui::Checkbox("Full Map: ", &Settings::fullMap);
		ImGui::SameLine();
		ImGui::Checkbox("Debug: ", &Settings::debugRadar);
		ImGui::Checkbox("Battle Royale Map", &Settings::brMap);
		ImGui::Checkbox("Arena Map", &Settings::areanaMap);
		ImGui::Text("Radar Dist:");
		// ImGui::SliderFloat(XorStr("##1"), &max_radar_dist, 100.f * 40, 80000.f, "%.2f");
		// ImGui::Text(XorStr("Radar Type:"));
		// ImGui::SliderInt(XorStr("##2"), &radarType, 0, 3);
		ImGui::Text("Radar Zoom:");
		ImGui::SliderFloat("##3", &Settings::zoom, 0.0f, 3.f, "%.6f");
		ImGui::Text("Blip Size:");
		ImGui::SliderInt("Radar Blip Size", &Settings::curConfig.radarBlipSize, 0, 7);
		if (Settings::debugRadar)
		{
			ImGui::Text("Radar Pixel Offset X:");
			ImGui::SliderInt("##5", &Settings::radarPixelOffsetX, -100, 45);
			ImGui::Text("Radar Pixel Offset Y:");
			ImGui::SliderInt("##6", &Settings::radarPixelOffsetY, -100, 45);
		}
		ImGui::Text("Current Map:");
		ImGui::SameLine();
		ImGui::Text(codeToHumanName.at(Settings::mapName).c_str());
	}
	if (tabs == 4)
	{
		if (ImGui::BeginTabBar("Weapon Config Tab Bar"))
		{

			if (ImGui::BeginTabItem("SMG Config"))
			{
				ImGui::Checkbox("Enabled", &Settings::wepConf.smg.enabled);
				ImGui::Checkbox("Silent Aim: ", &Settings::wepConf.smg.silent);
				ImGui::Checkbox("Friendly Fire: ", &Settings::wepConf.smg.friendly);
				ImGui::SliderFloat("Fov", &Settings::wepConf.smg.fov, 0.1f, 250.f, "%.2f");
				ImGui::SliderFloat("Smooth", &Settings::wepConf.smg.smooth, 0.1f, 150.f, "%.2f");
				ImGui::SliderFloat("No Recoil X%", &Settings::wepConf.smg.RCSX, 0.1f, 100.f, "%1.f");
				ImGui::SliderFloat("No Recoil Y%", &Settings::wepConf.smg.RCSY, 0.1f, 100.f, "%1.f");
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("AR Config"))
			{
				ImGui::Checkbox("Enabled", &Settings::wepConf.ar.enabled);
				ImGui::Checkbox("Silent Aim: ", &Settings::wepConf.ar.silent);
				ImGui::Checkbox("Friendly Fire: ", &Settings::wepConf.ar.friendly);
				ImGui::SliderFloat("Fov", &Settings::wepConf.ar.fov, 0.1f, 250.f, "%.2f");
				ImGui::SliderFloat("Smooth", &Settings::wepConf.ar.smooth, 0.1f, 150.f, "%.2f");
				ImGui::SliderFloat("No Recoil X%", &Settings::wepConf.ar.RCSX, 0.1f, 100.f, "%1.f");
				ImGui::SliderFloat("No Recoil Y%", &Settings::wepConf.ar.RCSY, 0.1f, 100.f, "%1.f");
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Sniper Config"))
			{
				ImGui::Checkbox("Enabled", &Settings::wepConf.sniper.enabled);
				ImGui::Checkbox("Silent Aim: ", &Settings::wepConf.sniper.silent);
				ImGui::Checkbox("Friendly Fire: ", &Settings::wepConf.sniper.friendly);
				ImGui::SliderFloat("Fov", &Settings::wepConf.sniper.fov, 0.1f, 250.f, "%.2f");
				ImGui::SliderFloat("Smooth", &Settings::wepConf.sniper.smooth, 0.1f, 150.f, "%.2f");
				ImGui::SliderFloat("No Recoil X%", &Settings::wepConf.sniper.RCSX, 0.1f, 100.f, "%1.f");
				ImGui::SliderFloat("No Recoil Y%", &Settings::wepConf.sniper.RCSY, 0.1f, 100.f, "%1.f");

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Pistol Config"))
			{
				ImGui::Checkbox("Enabled", &Settings::wepConf.pistol.enabled);
				ImGui::Checkbox("Silent Aim: ", &Settings::wepConf.pistol.silent);
				ImGui::Checkbox("Friendly Fire: ", &Settings::wepConf.pistol.friendly);
				ImGui::SliderFloat("Fov", &Settings::wepConf.pistol.fov, 0.1f, 250.f, "%.2f");
				ImGui::SliderFloat("Smooth", &Settings::wepConf.pistol.smooth, 0.1f, 150.f, "%.2f");
				ImGui::SliderFloat("No Recoil X%", &Settings::wepConf.pistol.RCSX, 0.1f, 100.f, "%1.f");
				ImGui::SliderFloat("No Recoil Y%", &Settings::wepConf.pistol.RCSY, 0.1f, 100.f, "%1.f");

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Marksman Config"))
			{
				ImGui::Checkbox("Enabled", &Settings::wepConf.marksman.enabled);
				ImGui::Checkbox("Silent Aim: ", &Settings::wepConf.marksman.silent);
				ImGui::Checkbox("Friendly Fire: ", &Settings::wepConf.marksman.friendly);
				ImGui::SliderFloat("Fov", &Settings::wepConf.marksman.fov, 0.1f, 250.f, "%.2f");
				ImGui::SliderFloat("Smooth", &Settings::wepConf.marksman.smooth, 0.1f, 150.f, "%.2f");
				ImGui::SliderFloat("No Recoil X%", &Settings::wepConf.marksman.RCSX, 0.1f, 100.f, "%1.f");
				ImGui::SliderFloat("No Recoil Y%", &Settings::wepConf.marksman.RCSY, 0.1f, 100.f, "%1.f");

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Shotgun Config"))
			{
				ImGui::Checkbox("Enabled", &Settings::wepConf.shotgun.enabled);
				ImGui::Checkbox("Silent Aim: ", &Settings::wepConf.shotgun.silent);
				ImGui::Checkbox("Friendly Fire: ", &Settings::wepConf.shotgun.friendly);
				ImGui::SliderFloat("Fov", &Settings::wepConf.shotgun.fov, 0.1f, 250.f, "%.2f");
				ImGui::SliderFloat("Smooth", &Settings::wepConf.shotgun.smooth, 0.1f, 150.f, "%.2f");
				ImGui::SliderFloat("No Recoil X%", &Settings::wepConf.shotgun.RCSX, 0.1f, 100.f, "%1.f");
				ImGui::SliderFloat("No Recoil Y%", &Settings::wepConf.shotgun.RCSY, 0.1f, 100.f, "%1.f");

				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}
	if (tabs == 5)
	{
		if (ImGui::BeginTabBar("Other Tab Bar"))
		{
			if (ImGui::BeginTabItem("Save/Load Configs"))
			{

				ImGui::Text("Configs:");
				const char *curConFigName[100];
				for (auto &item : Settings::curConfig.configs)
				{
					if (item.selected)
					{
						curConFigName[0] = item.filename.c_str();
					}
				}
				if (ImGui::BeginCombo("Configs", curConFigName[0]))
				{

					for (auto &item : Settings::curConfig.configs)
					{
						if (item.selected && Settings::curConfig.currentConfigNumber != item.value)
						{
							item.selected = false;
						}
						if (ImGui::Selectable(item.filename.c_str(), &item.selected))
						{
							// Settings::Load(item);
						}
						if (item.selected)
						{
							Settings::curConfig.currentConfigNumber = item.value;
						}
					}
					ImGui::EndCombo();
				}

				if (ImGui::Button("Save"))
				{
					Settings::curConfig.saveConfig(Settings::curConfig.currentConfigNumber);
				}
				ImGui::SameLine();
				if (ImGui::Button("Load"))
				{
					// config curConfig;
					Settings::curConfig.loadConfig(Settings::curConfig.currentConfigNumber);

					// LoadCode
				}
				ImGui::SameLine();
				if (ImGui::Button("Delete"))
				{
					// Delete code
				}
				ImGui::SameLine();
				if (ImGui::Button("Create"))
				{
					// Create code
				}

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Other"))
			{
				//should set a keybind for this
				ImGui::Text("Set Observe Mode");
				ImGui::Checkbox("Enabled", &Settings::curConfig.forceObserverMode);
				ImGui::SliderInt("Mode", &Settings::curConfig.observerMode, 0, 7);
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}

	ImGui::Text("Overlay FPS: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void setClickThough(bool clickThoughVal, GLFWwindow *window)
{
	glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, !Settings::showMenu);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_F4 && action == GLFW_PRESS)
	{
		Settings::active = false;
	}
	// if(key == GLFW_KEY_INSERT && action == GLFW_PRESS)
	// {
	//    Settings::showMenu = !Settings::showMenu;
	// }
}

static void glfw_error_callback(int error, const char *description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

// there has to be a better spot for this
bool WorldToScreen(Vector from, float *m_vMatrix, int targetWidth, int targetHeight, Vector &to)
{
	float w = m_vMatrix[12] * from.x + m_vMatrix[13] * from.y + m_vMatrix[14] * from.z + m_vMatrix[15];

	if (w < 0.01f)
		return false;

	to.x = m_vMatrix[0] * from.x + m_vMatrix[1] * from.y + m_vMatrix[2] * from.z + m_vMatrix[3];
	to.y = m_vMatrix[4] * from.x + m_vMatrix[5] * from.y + m_vMatrix[6] * from.z + m_vMatrix[7];

	float invw = 1.0f / w;
	to.x *= invw;
	to.y *= invw;

	float x = targetWidth / 2;
	float y = targetHeight / 2;

	x += 0.5 * to.x * targetWidth + 0.5;
	y -= 0.5 * to.y * targetHeight + 0.5;

	to.x = x;
	to.y = y;
	to.z = 0;
	return true;
}

void Overlay::RenderPlayerEsp()
{
	uint64_t gBase = Settings::apex.get_proc_baseaddr();
	if (Settings::curConfig.espEnabled && gBase != 0)
	{
		ImGui::Begin("playerEspWindow", (bool *)true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus); // ImGuiWindowFlags_NoTitleBar ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2((float)display_w, (float)display_h));
		if (Settings::curConfig.espEnabled)
		{
			uint64_t localPlayerPtr = Settings::apex.Read<uint64_t>(gBase + ApexOffsets::local_player);
			if (localPlayerPtr == 0)
			{

				ImGui::End();
				return;
			}
			Entity localPlayer = getEntity(localPlayerPtr);
			Vector localPlayerPos = localPlayer.getPosition();
			uint64_t viewRenderer = Settings::apex.Read<uint64_t>(gBase + ApexOffsets::view_render);
			uint64_t viewMatrix = Settings::apex.Read<uint64_t>(viewRenderer + ApexOffsets::view_matrix);
			Matrix matrix = Settings::apex.Read<Matrix>(viewMatrix);
			uint64_t entitylist = gBase + ApexOffsets::cl_entitylist;

			for (uint64_t curEntPtr : Settings::currentEnts)
			{
				if (curEntPtr == 0)
					continue;
				Entity curEnt = getEntity(curEntPtr);

				if (curEnt.ptr == localPlayerPtr)
					continue;

				if (!(curEnt.isPlayer() || curEnt.isDummy()))
				{
					continue;
				}
				if (!curEnt.isAlive())
				{
					continue;
				}

				Vector entPos = curEnt.getPosition();
				float dist = localPlayerPos.DistTo(entPos);
				if (dist > Settings::curConfig.maxEspDist)
				{
					continue;
				}
				if (true)
				{

					// printf("dist: %f",dist);
					Vector bs = Vector();
					WorldToScreen(entPos, matrix.matrix, 1920, 1080, bs);

					// printf("player\n");
					if (bs.x > 0 && bs.y > 0)
					{
						Vector hs = Vector();
						Vector headPos = curEnt.getBonePos(8);
						WorldToScreen(headPos, matrix.matrix, 1920, 1080, hs);

						float height = abs(abs(hs.y) - abs(bs.y));
						float width = height / 2.0f;
						float boxMiddle = bs.x - (width / 2.0f);

						if (Settings::curConfig.boxEnabled)
						{
							Overlay::DrawBox(RED, boxMiddle, hs.y, width, height); // BOX
						}
						if (Settings::curConfig.lineEnabled)
						{
							Overlay::DrawLine(ImVec2((display_w / 2), (display_h / 2)), ImVec2(hs.x, hs.y), RED, 1); // LINE FROM MIDDLE SCREEN
						}
						if (Settings::curConfig.nameEnabled)
						{
							std::string strHpAP = std::to_string(curEnt.getHealth() + curEnt.getShield());
							const char *chHPAp = strHpAP.c_str();

							std::string topText = "";
							const char *s = {};
							char name[64] = {};
							if (curEnt.isPlayer())
							{
								char dummyName[33] = "Player";
								for (int x = 0; x < 6; x++)
								{
									name[x] = dummyName[x];
								}
								// curEnt.get_name(gBase,i,name);
							}
							else if (curEnt.isDummy())
							{
								char dummyName[33] = "Dummy";
								for (int x = 0; x < 5; x++)
								{
									name[x] = dummyName[x];
								}
							}

							topText = ":   ";
							topText = name + topText;
							s = topText.c_str();

							std::string totalsize = topText + strHpAP;
							ImVec2 totalSize = ImGui::CalcTextSize(totalsize.c_str());

							std::string sizeStr = topText;
							ImVec2 size = ImGui::CalcTextSize(topText.c_str());

							std::string hlthStr = chHPAp;
							ImVec2 hlthSize = ImGui::CalcTextSize(hlthStr.c_str());

							Overlay::RectFilled(hs.x - (totalSize.x / 2), bs.y - height - ImGui::GetFontSize(), hs.x + (totalSize.x / 2), bs.y - height, ImColor(0, 0, 0, 180), 30, 15);
							String(ImVec2(hs.x, (bs.y - height)), WHITE, s, true);
							String(ImVec2(hs.x + (totalSize.x / 2) - hlthSize.x, (bs.y - height)), GREEN, chHPAp, false);
						}

					}
				}
			}
		}
		ImGui::End();
	}
}

void Overlay::RenderItemEsp()
{
	uint64_t gBase = Settings::apex.get_proc_baseaddr();
	if (gBase != 0 && Settings::curConfig.itemEspEnabled)
	{
		ImGui::Begin("itemEspWindow", (bool *)true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus); // ImGuiWindowFlags_NoTitleBar ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2((float)display_w, (float)display_h));
		uint64_t localPlayerPtr = Settings::apex.Read<uint64_t>(gBase + ApexOffsets::local_player);
		if (localPlayerPtr == 0)
		{
			ImGui::End();
			return;
		}

		Entity localPlayer = getEntity(localPlayerPtr);

		Vector localPlayerPos = localPlayer.getPosition();

		uint64_t viewRenderer = Settings::apex.Read<uint64_t>(gBase + ApexOffsets::view_render);
		uint64_t viewMatrix = Settings::apex.Read<uint64_t>(viewRenderer + ApexOffsets::view_matrix);
		Matrix matrix = Settings::apex.Read<Matrix>(viewMatrix);

		for (uint64_t curEntPtr : Settings::currentEnts)
		{
			// printf("1\n");
			if (curEntPtr == 0)
			{
				continue;
			}
			// printf("2\n");
			Entity curEnt = getEntity(curEntPtr);
			// printf("3\n");

			// curEnt.updateBuffer();

			if (!curEnt.isItem())
			{
				continue;
			}
			// printf("4\n");
			Vector itemPos = curEnt.getPosition();
			float dist = localPlayerPos.DistTo(itemPos);

			if (dist > Settings::curConfig.itemEspDist)
			{
				continue;
			}
			// printf("5\n");

			int itemId = curEnt.getItemId();

			Vector bs = Vector();
			WorldToScreen(itemPos, matrix.matrix, 1920, 1080, bs);
			// printf("6\n");
			if (true) // bs.x > 0 && bs.y > 0
			{
				// printf("7\n");
				if (PropList.count(itemId))
				{
					// printf("8\n");
					//  std::cout << itemId << std::endl;
					StringOutlined(ImVec2(bs.x, bs.y), GREEN, PropList[itemId].c_str(), true);
					// String(ImVec2(bs.x, bs.y), BLACK, PropList.at(itemId).c_str(), true);
				}
				else
				{
					// printf("9\n");
					std::cout << "Item ID: " << itemId << std::endl;
					std::string itemName = "Unknown: " + std::to_string(itemId);
					String(ImVec2(bs.x, bs.y), RED, itemName.c_str(), true);
				}
			}
		}
		ImGui::End();
	}
}

void Overlay::RenderRadar()
{
	uint64_t gBase = Settings::apex.get_proc_baseaddr();
	std::string mapNameStr = "";
	if (gBase != 0)
	{
		std::string mapNameStr = getMapNameStr(gBase);
		Settings::mapName = mapNameStr;
	}
	else
	{
		Settings::mapName = "mp_lobby";
	}

	if (Settings::curConfig.radarEnabled && gBase != 0)
	{
		ImGui::Begin("radarWindow", (bool *)true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus); // ImGuiWindowFlags_NoTitleBar ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2((float)display_w, (float)display_h));

		ImDrawList *windowDrawList = ImGui::GetWindowDrawList();

		ImVec2 pos = ImVec2(Settings::radarPos.x, Settings::radarPos.y);
		ImVec2 siz = ImVec2(Settings::radarSize.x, Settings::radarSize.y);

		ImU32 clr = GREEN;
		ImColor color = GREEN;

		windowDrawList->AddLine(ImVec2(pos.x + (siz.x / 2), pos.y + 0), ImVec2(pos.x + (siz.x / 2), pos.y + siz.y), GetU32(255, 255, 255, 255), 1.0f);
		windowDrawList->AddLine(ImVec2(pos.x + 0, pos.y + (siz.y / 2)), ImVec2(pos.x + siz.x, pos.y + (siz.y / 2)), GetU32(255, 255, 255, 255), 1.0f);

		uint64_t entitylist = gBase + ApexOffsets::cl_entitylist;

		uint64_t localPlayerPtr = Settings::apex.Read<uint64_t>(gBase + ApexOffsets::local_player);
		if (localPlayerPtr == 0)
		{
			ImGui::End();
			return;
		}
		Entity localPlayer = getEntity(localPlayerPtr);
		Vector localPlayerPos = localPlayer.getPosition();
		if (Settings::debugRadar)
		{
			Vector mapOrigin = {0, 0, -10000};
			Vector topOfMap = {0, 0, 10000};

			uint64_t viewRenderer = Settings::apex.Read<uint64_t>(gBase + ApexOffsets::view_render);
			uint64_t viewMatrix = Settings::apex.Read<uint64_t>(viewRenderer + ApexOffsets::view_matrix);
			Vector bs = Vector();
			Vector hs = Vector();
			Matrix matrix = Settings::apex.Read<Matrix>(viewMatrix);
			WorldToScreen(mapOrigin, matrix.matrix, 1920, 1080, bs);
			WorldToScreen(topOfMap, matrix.matrix, 1920, 1080, hs);

			windowDrawList->AddLine(ImVec2(bs.x, bs.y), ImVec2(hs.x, hs.y), RED, 5.0f);
		}

		if (Settings::fullMap)
		{
			Settings::radarPos = {420.f, 0.f, 0.f};
			Settings::radarSize = {1015.f, 1015.f, 0.f};
			try
			{
				Settings::scale = mapNameToScale.at(mapNameStr);
			}
			catch (const std::exception &e)
			{
				std::cout << "Map not found: " << mapNameStr << std::endl;
				Settings::scale = 1.f;
				std::cerr << e.what() << '\n';
			}
			try
			{
				Settings::radarPixelOffset = mapNameTooOffsets.at(mapNameStr);
			}
			catch (const std::exception &e)
			{
				std::cout << "Map not found: " << mapNameStr << std::endl;
				Settings::radarPixelOffset = {0.f, 0.f};
				std::cerr << e.what() << '\n';
			}
			if (Settings::debugRadar)
			{
				Settings::radarPixelOffset = {Settings::radarPixelOffsetX, Settings::radarPixelOffsetY};
				Settings::scale = Settings::zoom;
			}
		}
		else
		{
			if (!localPlayer.isAlive())
			{
				Settings::radarPos = {50.f, 115.f, 0.f};
				Settings::radarSize = {240.f, 240.f, 0.f};
			}
			else
			{
				Settings::radarPos = {50.f, 50.f, 0.f};
				Settings::radarSize = {240.f, 240.f, 0.f};
			}
			if (Settings::brMap)
			{
				Settings::scale = 0.33333333f;
			}
			else if (Settings::areanaMap)
			{
				Settings::scale = 1.32716f;
			}
		}
		// int i = 0;
		for (uint64_t curEntityPtr : Settings::currentEnts)
		{
			if (curEntityPtr == 0)
			{
				continue;
			}
			Entity curEntity = getEntity(curEntityPtr);
			// std::cout << i << std::endl;
			if (!curEntity.isPlayer() && !curEntity.isDummy())
			{
				continue;
			}
			if (!curEntity.isAlive())
			{
				continue;
			}

			if (curEntity.getTeamId() == localPlayer.getTeamId())
			{
				clr = GREEN;
				color = GREEN;
			}
			else if (curEntity.isKnocked() == true)
			{
				clr = BLUE;
				color = BLUE;
			}
			else if (curEntity.getLastVisTime() > curEntity.lastVisTime)
			{
				// printf("ye");
				clr = RED;
				color = RED;
			}
			else if (curEntity.isDummy())
			{
				clr = ORANGE;
				color = ORANGE;
			}
			else
			{
				clr = WHITE;
				color = WHITE;
			}

			bool checkVeiw = false;
			Vector screenPoint = RotatePoint(curEntity.getPosition(), localPlayerPos, Settings::radarPos.x, Settings::radarPos.y, Settings::radarSize.x, Settings::radarSize.y, localPlayer.GetViewAngles().y, Settings::scale, &checkVeiw, Settings::fullMap, Settings::radarPixelOffset);
			if (Settings::curConfig.radarType == 1)
			{
				// printf("x%f y%f\n",screenPoint.x,screenPoint.y);
				// printf("----------------------");
				windowDrawList->AddCircleFilled(ImVec2((screenPoint.x), (screenPoint.y)), Settings::curConfig.radarBlipSize, clr);
			}

			curEntity.lastVisTime = curEntity.getLastVisTime();
		}
		ImGui::End();
	}
}

bool runOnce = false;

int Overlay::RenderLoop()
{
	printf("Overlay Started \n");
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	// GL 3.0 + GLSL 130
	const char *glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		   // 3.0+ only

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED, GL_FALSE); // working
	// glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);//working
	//  Create window with graphics context
	GLFWwindow *window = glfwCreateWindow(1920, 1080, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
	if (window == NULL)
		return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	loadTex();
	for (int i = 0; i <= 9; i++)
	{
		loadMultiTex(i);
	}
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	// io.Fonts->AddFontDefault();
	// io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	// io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	// io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	// io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	// ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	// IM_ASSERT(font != NULL);
	glfwSetKeyCallback(window, key_callback);
	// Our state
	ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 0.00f);
	glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);

	// glfwSetWindowAttrib(window,GLFW_FLOATING, GLFW_TRUE);
	glfwSetWindowAttrib(window, GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	// glfwSetWindowAttrib(window,GLFW_MAXIMIZED,GLFW_TRUE);
	//  Main loop
	std::thread(GetKeys).detach();
	Settings::waitingForOverlay == false;
	SetTheme();
	while (!glfwWindowShouldClose(window) && Settings::active)
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		// GetKeys("/dev/input/by-id/usb-Razer_Razer_Cynosa_Chroma-event-kbd");
		glfwPollEvents();
		if (Settings::showMenu)
		{
			setClickThough(false, window);
		}
		else
		{
			setClickThough(true, window);
		}

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (Settings::showMenu == true)
		{
			ImGui::ShowDemoWindow();
			Overlay::RenderMenu();
		}
		// Overlay::test();
		// ImGui::SetNextWindowPos(ImVec2(0, 0));
		// ImGui::SetNextWindowSize(ImVec2((float)display_w,(float)display_h));
		Overlay::MainLoop();
		Overlay::RenderPlayerEsp();
		Overlay::RenderItemEsp();
		Overlay::RenderRadar();
		// Rendering
		// ImGui::Render();
		ImGui::Render();
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		// glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	if (glfwWindowShouldClose(window))
	{
		Settings::active = false;
	}
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

//i should move these to a file like "drawing.hpp" and "drawing.cpp"

void Overlay::DrawLine(ImVec2 a, ImVec2 b, ImColor color, float width)
{
	ImGui::GetWindowDrawList()->AddLine(a, b, color, width);
}

void Overlay::DrawBox(ImColor color, float x, float y, float w, float h)
{
	DrawLine(ImVec2(x, y), ImVec2(x + w, y), color, 1.0f);
	DrawLine(ImVec2(x, y), ImVec2(x, y + h), color, 1.0f);
	DrawLine(ImVec2(x + w, y), ImVec2(x + w, y + h), color, 1.0f);
	DrawLine(ImVec2(x, y + h), ImVec2(x + w, y + h), color, 1.0f);
}

void Overlay::Text(ImVec2 pos, ImColor color, const char *text_begin, const char *text_end, float wrap_width, const ImVec4 *cpu_fine_clip_rect, bool centerText)
{
	ImVec2 drawPos = pos;
	if (centerText)
	{
		ImVec2 textSize = ImGui::CalcTextSize(text_begin);
		drawPos.x = pos.x - textSize.x / 2;
		drawPos.y = pos.y - textSize.y;
	}
	ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), drawPos, color, text_begin);
}


//i dont think this works idk
void Overlay::DrawOutlinedText(ImVec2 pos, ImColor color, const char *text_begin, const char *text_end, float wrap_width, const ImVec4 *cpu_fine_clip_rect, bool centerText)
{
	ImVec2 drawPos = pos;
	ImVec2 textSize = ImGui::CalcTextSize(text_begin);
	if (centerText)
	{

		drawPos.x = pos.x - textSize.x / 2;
		drawPos.y = pos.y - textSize.y;
	}
	Overlay::RectFilled(pos.x - textSize.x / 2 - 1, pos.y - textSize.y, pos.x + textSize.x / 2 + 1, pos.y, ImColor(0, 0, 0, 180), 30, 15);
	ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), drawPos, color, text_begin);
}

void Overlay::String(ImVec2 pos, ImColor color, const char *text, bool centerText)
{
	Text(pos, color, text, text + strlen(text), 200, 0, true);
}

void Overlay::StringOutlined(ImVec2 pos, ImColor color, const char *text, bool centerText)
{
	DrawOutlinedText(pos, color, text, text + strlen(text), 200, 0, true);
}

void Overlay::RectFilled(float x0, float y0, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags)
{
	ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), color, rounding, rounding_corners_flags);
}

