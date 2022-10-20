#pragma once

#include "pai_descriptors.hpp"
#include "pai_device.hpp"
#include "pai_game_object.hpp"
#include "pai_renderer.hpp"
#include "pai_window.hpp"

// std
#include <memory>
#include <vector>

namespace pai
{
    class PaiAppBase
    {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        PaiAppBase();
        ~PaiAppBase();

        PaiAppBase(const PaiAppBase &) = delete;
        PaiAppBase &operator=(const PaiAppBase &) = delete;

        void run();

    private:
        void loadGameObjects();

        PaiWindow paiWindow{WIDTH, HEIGHT, "Vulkan Tutorial"};
        PaiDevice paiDevice{paiWindow};
        PaiRenderer paiRenderer{paiWindow, paiDevice};

        // note: order of declarations matters
        std::unique_ptr<PaiDescriptorPool> globalPool{};
        PaiGameObject::Map gameObjects;
    };
}