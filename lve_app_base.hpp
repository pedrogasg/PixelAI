#pragma once

#include "lve_descriptors.hpp"
#include "lve_device.hpp"
#include "lve_game_object.hpp"
#include "lve_renderer.hpp"
#include "lve_window.hpp"

// std
#include <memory>
#include <vector>

namespace lve
{
    class LveAppBase
    {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        LveAppBase();
        ~LveAppBase();

        LveAppBase(const LveAppBase &) = delete;
        LveAppBase &operator=(const LveAppBase &) = delete;

        void run();

    private:
        void loadGameObjects();

        LveWindow lveWindow{WIDTH, HEIGHT, "Vulkan Tutorial"};
        LveDevice lveDevice{lveWindow};
        LveRenderer lveRenderer{lveWindow, lveDevice};

        // note: order of declarations matters
        std::unique_ptr<LveDescriptorPool> globalPool{};
        std::vector<LveGameObject> gameObjects;
    };
}