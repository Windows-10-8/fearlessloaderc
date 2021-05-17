#pragma once
#include <iostream>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_internal.h"

#include "Icondapizda.h"
#include "sksemi.h"
#include "skmed.h"

int active_tab = 0;

ImFont* Logo;
ImFont* Icons;
ImFont* Tabs;
ImFont* Child;
ImFont* Elements;
ImFont* SubElements;

namespace LamantineSDK {
    ImVec4 rgb_to_imvec4(float r, float g, float b, float a)
    {
        return ImVec4(r / 255, g / 255, b / 255, a / 255);
    }
    void InitFonts() {
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromMemoryTTF(&IconFont, sizeof IconFont, 40, nullptr, io.Fonts->GetGlyphRangesCyrillic());
        Logo = io.Fonts->AddFontFromMemoryTTF(&IconFont, sizeof IconFont, 40, nullptr, io.Fonts->GetGlyphRangesCyrillic());
        Icons = io.Fonts->AddFontFromMemoryTTF(&IconFont, sizeof IconFont, 28, nullptr, io.Fonts->GetGlyphRangesCyrillic());
        Tabs = io.Fonts->AddFontFromMemoryTTF(&MediumFont, sizeof MediumFont, 17, nullptr, io.Fonts->GetGlyphRangesCyrillic());
        Child = io.Fonts->AddFontFromMemoryTTF(&SemiBold, sizeof SemiBold, 22, nullptr, io.Fonts->GetGlyphRangesCyrillic());
        Elements = io.Fonts->AddFontFromMemoryTTF(&SemiBold, sizeof SemiBold, 17, nullptr, io.Fonts->GetGlyphRangesCyrillic());
        SubElements = io.Fonts->AddFontFromMemoryTTF(&SemiBold, sizeof SemiBold, 12, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    }

    void InitStyles() {
        ImGui::GetStyle().WindowRounding = 0.0f;
        ImGui::GetStyle().FrameRounding = 0.0f;
        ImGui::GetStyle().WindowPadding = ImVec2(0.0f, 0.0f);
        ImGui::GetStyle().ChildRounding = 0.0f;
        ImGui::GetStyle().WindowBorderSize = 0.0f;
        ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5, 0.5);
        ImGui::GetStyle().PopupBorderSize = 0.0f;
    }

    void ColorStyles() {
        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.73f, 0.75f, 0.74f, 1.f);
        colors[ImGuiCol_WindowBg] = ImColor(39, 39, 61);
        colors[ImGuiCol_ChildBg] = ImVec4(0.39f, 0.00f, 0.63f, 0.f);
        colors[ImGuiCol_PopupBg] = ImColor(35, 35, 54);
        colors[ImGuiCol_Border] = ImColor(35, 35, 54);
        colors[ImGuiCol_BorderShadow] = ImColor(35, 35, 54);
        colors[ImGuiCol_FrameBg] = ImVec4(0.239f, 0.239f, 0.349f, 0.5);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.216f, 0.216f, 0.376f, 0.5f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.216f, 0.216f, 0.376f, 0.6f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.75f, 0.01f, 0.94f, 1.f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.39f, 0.00f, 0.63f, 1.f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.39f, 0.00f, 0.63f, 1.f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.34f, 0.16f, 0.16f, 1.f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.94f, 0.02f, 1.28f, 1.f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.94f, 0.02f, 1.28f, 1.f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.94f, 0.02f, 1.28f, 1.f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.94f, 0.02f, 1.28f, 1.f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.063f, 0.349f, 0.851f, 1.f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.325f, 0.451f, 0.839f, 1.f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.325f, 0.451f, 0.839f, 1.f);
        colors[ImGuiCol_Button] = ImVec4(0.008f, 0.357f, 0.557f, 1.f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.008f, 0.357f, 0.557f, 0.8f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.008f, 0.357f, 0.557f, 0.5f);
        colors[ImGuiCol_Header] = ImColor(54, 54, 79);
        colors[ImGuiCol_HeaderHovered] = ImColor(54, 54, 90);
        colors[ImGuiCol_HeaderActive] = ImColor(54, 54, 90);
        colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 1.f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.71f, 0.39f, 0.39f, 1.f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.71f, 0.39f, 0.39f, 1.f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.71f, 0.39f, 0.39f, 1.f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.84f, 0.66f, 0.66f, 1.f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.84f, 0.66f, 0.66f, 1.f);
        colors[ImGuiCol_Tab] = ImVec4(0.71f, 0.39f, 0.39f, 1.f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.84f, 0.66f, 0.66f, 1.f);
        colors[ImGuiCol_TabActive] = ImVec4(0.84f, 0.66f, 0.66f, 1.f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 1.f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.f);
        colors[ImGuiCol_TextSelectedBg] = ImColor(35, 35, 54);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 1.f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.41f, 0.41f, 0.41f, 1.f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 1.f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 1.f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 1.f);
    }
    void renderTabHover(const char* _text, int _id) //flowxrc
    {
        ImGui::PushStyleColor(ImGuiCol_Button, rgb_to_imvec4(53, 53, 83, 255));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, rgb_to_imvec4(53, 53, 83, 255));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, rgb_to_imvec4(53, 53, 83, 255));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7);
        {
            ImGui::PushFont(Tabs);
            if (ImGui::Button(_text, ImVec2(ImGui::CalcTextSize(_text).x + 15, 35)))
                active_tab = _id;
            ImGui::PopFont();
        }
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
    }

    void tab(const char* label, int _id, const char* text) //flowxrc
    {
        ImGui::PushFont(Icons);
        ImGui::PushStyleColor(ImGuiCol_Button, rgb_to_imvec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, rgb_to_imvec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, rgb_to_imvec4(0, 0, 0, 0));
        if (active_tab == _id)
            ImGui::PushStyleColor(ImGuiCol_Text, rgb_to_imvec4(110, 110, 157, 255));
        else
            ImGui::PushStyleColor(ImGuiCol_Text, rgb_to_imvec4(110, 110, 157, 255));
        if (ImGui::Button(label, ImVec2(62, 40)))
            active_tab = _id;
        if (ImGui::IsItemHovered())
        {
            float y = ImGui::GetCursorPos().y;
            ImGui::SetCursorPos(ImVec2(5, y - 40));
            renderTabHover(text, _id);
        }
        if (active_tab == _id)
        {
            float y = ImGui::GetCursorPos().y;
            ImGui::SetCursorPos(ImVec2(5, y - 40));
            renderTabHover(text, _id);
        }
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
    }
    void RenderFunctionChild(const char* label, ImVec2 item_size) {
        ImVec2 pos = ImGui::GetCursorPos();
        ImGui::PushFont(Child);
        ImGui::SetCursorPos(pos);
        ImGui::TextColored(ImColor(110, 110, 157), label);
        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(pos.x, pos.y + 25), ImVec2(pos.x + item_size.x, pos.y + 25 + item_size.y), ImColor(46, 46, 72), 13.f);
    }
}