#include <iostream>
#include "External/imgui/imgui.h"
#include <Pure2D/Engine/Game.h>
#include <Pure2D/Graphics.h>
#include <Pure2D/System/Logging.h>



struct ImGuiExample final : public pure::Game
{
    void load() override
    {
        showFPS = false;
        m_window.setTitle("ImGui Example");
    }

    void draw() override
    {
        static bool show_demo_window = false;
        static bool show_another_window = false;
        static float clear_color[4] = {};
        static float f = 0.0f;
        static int counter = 0;

        // ImGui calls from imgui documentation examples. We can just use
        // ImGui anywhere in our Game class and it should "just werk"

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&m_renderer.clearColor); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

         // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
    }
};


int main() 
{
    ImGuiExample g{};
    g.run();
    return 0;
}
