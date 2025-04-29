#ifndef RENDERGUISYSTEM_H
#define RENDERGUISYSTEM_H

#include <filesystem> 
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/HealthComponent.h"
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

// Documentation: https://github.com/ocornut/imgui/blob/master/docs/FONTS.md

class RenderGUISystem: public System {

    private:
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4(&colors)[ImGuiCol_COUNT] = ImGui::GetStyle().Colors;

    public:
        RenderGUISystem() 
        {
            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

            io.Fonts->AddFontDefault();

            // Add all .ttf in fonts folder
            //for (const auto& entry : std::filesystem::directory_iterator(RESOURCES_PATH"fonts")) {
            //    if (entry.path().extension() == ".ttf") {
            //        io.Fonts->AddFontFromFileTTF(entry.path().string().c_str(), 18);
            //    }
            //}

            // Load a font and set it as the default font
            ImFont* customFont = io.Fonts->AddFontFromFileTTF(RESOURCES_PATH"fonts/Cousine-Regular.ttf", 20);
            if (customFont) {
                io.FontDefault = customFont; // Set the loaded font as the default font
            }

            // Change style
            style.FrameRounding = 6.0f;
            style.GrabRounding = 12.0f;
            style.TabBorderSize = 1.0f;
            style.WindowMenuButtonPosition = ImGuiDir_None;

            colors[ImGuiCol_Text] = ImVec4{ 0.9f, 0.9f, 0.9f, 1.0f };

            colors[ImGuiCol_WindowBg] = ImVec4{ 0.141f, 0.161f, 0.18f, 1.0f };

            // Headers
            colors[ImGuiCol_Header] = ImVec4{ 0.267f, 0.302f, 0.337f, 1.0f };
            colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
            colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

            // Buttons
            colors[ImGuiCol_Button] = ImVec4{ 0.012f, 0.4f, 0.839f, 1.0f };
            colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
            colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

            // Frame BG
            colors[ImGuiCol_FrameBg] = ImVec4{ 0.114f, 0.129f, 0.145f, 1.0f };
            colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
            colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

            colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.141f, 0.161f, 0.18f, 1.0f };

            // Tabs
            colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
            colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
            colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
            colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
            colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

            // Title
            colors[ImGuiCol_TitleBg] = ImVec4{ 0.267f, 0.302f, 0.337f, 1.0f };
            colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
            colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        }
           
        void Update(const std::unique_ptr<Registry>& registry, const SDL_Rect& camera, SDL_Renderer* renderer) 
        {
            ImGui_ImplSDLRenderer3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();

            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable) 
            { 
                ImGui::DockSpaceOverViewport(
                    ImGui::GetMainViewport()->ID,
                    ImGui::GetMainViewport(),
                    ImGuiDockNodeFlags_PassthruCentralNode
                ); 
            }

            bool show_demo_window = true;
            ImGui::ShowDemoWindow(&show_demo_window);

            // Add a menu bar at the top of the screen
            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("New Scene")) { /* Handle action here */ }
                    if (ImGui::MenuItem("Open Scene", "Ctrl+O")) { /* Handle action here */ }
                    if (ImGui::MenuItem("Save Scene")) { /* Handle action here */ }
                    if (ImGui::MenuItem("Save Scene As", "Ctrl+S")) { /* Handle action here */ }
                    if (ImGui::MenuItem("New Project")) { /* Handle action here */ }
                    if (ImGui::MenuItem("Open Project")) { /* Handle action here */ }
                    if (ImGui::MenuItem("Save Project")) { /* Handle action here */ }
                    if (ImGui::MenuItem("Exit", "Alt+F4")) { /* Handle action here */ }
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Edit"))
                {
                    if (ImGui::MenuItem("Undo", "Ctrl+Z")) { /* Handle action here */ }
                    if (ImGui::MenuItem("Redo", "Ctrl+Y")) { /* Handle action here */ }
                    if (ImGui::MenuItem("Project Settings")) { /* Handle action here */ }
                    if (ImGui::MenuItem("Preferences")) { /* Handle action here */ }
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Help"))
                {
                    if (ImGui::MenuItem("Search Documentation")) { /* Handle action here */ }
                    if (ImGui::MenuItem("Check for Updates")) { /* Handle action here */ }
                    if (ImGui::MenuItem("About...")) { /* Handle action here */ }
                    ImGui::EndMenu();
                }

                ImGui::EndMainMenuBar();
            }
            
            // Set position and size for the toolbar
            ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight()), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 60), ImGuiCond_Always);

            ImGuiWindowFlags toolbarFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

            if (ImGui::Begin("Toolbar", nullptr, toolbarFlags))
            {
                // Calculate the center position for the buttons
                float toolbarWidth = ImGui::GetIO().DisplaySize.x;
                float buttonWidth = 32.0f; // Width of each button
                float buttonSpacing = ImGui::GetStyle().ItemSpacing.x; // Spacing between buttons
                float totalButtonWidth = (buttonWidth * 3) + (buttonSpacing * 2); // Total width of all buttons and spacing
                float offsetX = (toolbarWidth - totalButtonWidth) / 2.0f; // Center offset

                // Add spacing to center the buttons
                ImGui::SetCursorPosX(offsetX);

                // Render the buttons
                ImTextureID playIcon = io.Fonts->TexID;
                if (ImGui::ImageButton("play", playIcon, ImVec2(buttonWidth, buttonWidth))) { /* Handle Play action */ }
                ImGui::SameLine();
                ImTextureID pauseIcon = io.Fonts->TexID;
                if (ImGui::ImageButton("pause", pauseIcon, ImVec2(buttonWidth, buttonWidth))) { /* Handle Pause action */ }
                ImGui::SameLine();
                ImTextureID stopIcon = io.Fonts->TexID;
                if (ImGui::ImageButton("stop", stopIcon, ImVec2(buttonWidth, buttonWidth))) { /* Handle Stop action */ }


            }

            ImGui::End();

/*
            // Dock the window to the right of the screen.
            ImGui::SetNextWindowPos(
                ImVec2(ImGui::GetMainViewport()->WorkPos.x + ImGui::GetMainViewport()->WorkSize.x, ImGui::GetMainViewport()->WorkPos.y), 
                ImGuiCond_Once, 
                ImVec2(1.0f, 0.0f)
            );

            // Display a window to customize and create new enemies
            if (ImGui::Begin("Spawn enemies")) {

                // Static variables to hold input values
                static int posX = 0;
                static int posY = 0;
                static int scaleX = 1;
                static int scaleY = 1;
                static int velX = 0;
                static int velY = 0;
                static int health = 100;
                static float rotation = 0.0;
                static float projAngle = 0.0;
                static float projSpeed = 100.0;
                static int projRepeat = 10;
                static int projDuration = 10;
                const char* sprites[] = {"tank-image", "truck-image"};
                static int selectedSpriteIndex = 0;

                // Section to input enemy sprite texture id 
                if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::Combo("texture id", &selectedSpriteIndex, sprites, IM_ARRAYSIZE(sprites));
                }
                ImGui::Spacing();

                // Section to input enemy transform values
                if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::InputInt("position x", &posX);
                    ImGui::InputInt("position y", &posY);
                    ImGui::SliderInt("scale x", &scaleX, 1, 10);
                    ImGui::SliderInt("scale y", &scaleY, 1, 10);
                    ImGui::SliderAngle("rotation (deg)", &rotation, 0, 360);
                }
                ImGui::Spacing();

                // Section to input enemy rigid body values
                if (ImGui::CollapsingHeader("Rigid body", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::InputInt("velocity x", &velX);
                    ImGui::InputInt("velocity y", &velY);
                }
                ImGui::Spacing();

                // Section to input enemy projectile emitter values
                if (ImGui::CollapsingHeader("Projectile emitter", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::SliderAngle("angle (deg)", &projAngle, 0, 360);
                    ImGui::SliderFloat("speed (px/sec)", &projSpeed, 10, 500);
                    ImGui::InputInt("repeat (sec)", &projRepeat);
                    ImGui::InputInt("duration (sec)", &projDuration);
                }
                ImGui::Spacing();

                // Section to input enemy health values
                if (ImGui::CollapsingHeader("Health", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::SliderInt("%", &health, 0, 100);
                }

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                // Enemy creation button
                if (ImGui::Button("Spawn new enemy")) {
                    Entity enemy = registry->CreateEntity();
                    enemy.Group("enemies");
                    enemy.AddComponent<TransformComponent>(glm::vec2(posX, posY), glm::vec2(scaleX, scaleY), glm::degrees(rotation));
                    enemy.AddComponent<RigidBodyComponent>(glm::vec2(velX, velY));
                    enemy.AddComponent<SpriteComponent>(sprites[selectedSpriteIndex], 32, 32, 2);
                    enemy.AddComponent<BoxColliderComponent>(25, 20, glm::vec2(5, 5));
                    double projVelX = cos(projAngle) * projSpeed; // convert from angle-speed to x-value
                    double projVelY = sin(projAngle) * projSpeed; // convert from angle-speed to y-value
                    enemy.AddComponent<ProjectileEmitterComponent>(glm::vec2(projVelX, projVelY), projRepeat * 1000, projDuration * 1000, 10, false);
                    enemy.AddComponent<HealthComponent>(health);

                    // Reset all input values after we create a new enemy
                    posX = posY = rotation = projAngle = 0;
                    scaleX = scaleY = 1;
                    projRepeat = projDuration = 10;
                    projSpeed = 100;
                    health = 100;
                }
            }
            ImGui::End();

            // Display a small overlay window to display the map position using the mouse
            ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav;
            ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always, ImVec2(0, 0));
            ImGui::SetNextWindowBgAlpha(0.9f);
            if (ImGui::Begin("Map coordinates", NULL, windowFlags)) {
                ImGui::Text(
                    "Map coordinates (x=%.1f, y=%.1f)",
                    ImGui::GetIO().MousePos.x + camera.x,
                    ImGui::GetIO().MousePos.y + camera.y
                );
            }
            ImGui::End();
*/
            ImGui::Render();
            ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
            
        }

    
};

#endif
