#pragma once

#include "lve_window.hpp"
#include "lve_model.hpp"
#include "lve_game_object.hpp"
#include "lve_renderer.hpp"

#include <memory>

namespace lve
{
    class LveAppBase
    {
    private:
        LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
        LveDevice lveDevice{lveWindow};
        LveRenderer lveRenderer{lveWindow, lveDevice};
        std::vector<LveGameObject> gameObjects;

        void loadGameObjects();

        void sierpinski(
            std::vector<LveModel::Vertex> &vertices,
            int depth,
            glm::vec2 left,
            glm::vec2 right,
            glm::vec2 top);

    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        LveAppBase();
        ~LveAppBase();

        LveAppBase(const LveAppBase &) = delete;
        LveAppBase &operator=(const LveAppBase &) = delete;

        void run();
    };
}