#pragma once

#include "pai_model.hpp"

// libs
#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>
#include <unordered_map>

namespace pai
{
    struct Transform2dComponent
    {
        glm::vec2 translation{};
        glm::vec2 scale{1.f, 1.f};
        float rotation;

        glm::mat2 mat2()
        {
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);
            glm::mat2 rotMatrix{{c, s}, {-s, c}};

            glm::mat2 scaleMat{{scale.x, .0f}, {.0f, scale.y}};
            return rotMatrix * scaleMat;
        }
    };

    struct TransformComponent
    {
        glm::vec3 translation{};
        glm::vec3 scale{1.f, 1.f, 1.f};
        glm::vec3 rotation{};

        // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
        // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
        // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
        glm::mat4 mat4();

        glm::mat3 normalMatrix();
    };

    struct UnitPointComponent
    {
        float lightIntensity = 1.0f;
    };

    class PaiGameObject
    {
    public:
        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, PaiGameObject>;

        static PaiGameObject createGameObject()
        {
            static id_t currentId = 0;
            return PaiGameObject{currentId++};
        }

        static PaiGameObject makeUnitPoint(
            float intensity = 10.f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.f));

        PaiGameObject(const PaiGameObject &) = delete;
        PaiGameObject &operator=(const PaiGameObject &) = delete;
        PaiGameObject(PaiGameObject &&) = default;
        PaiGameObject &operator=(PaiGameObject &&) = default;

        id_t getId() { return id; }

        glm::vec3 color{};
        Transform2dComponent transform2d{};
        TransformComponent transform{};
        std::shared_ptr<PaiModel> model{};
        std::unique_ptr<UnitPointComponent> unitPoint = nullptr;

    private:
        PaiGameObject(id_t objId) : id{objId} {}

        id_t id;
    };
}
