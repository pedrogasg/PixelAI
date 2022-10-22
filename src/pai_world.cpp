#include "pai_world.hpp"

namespace pai
{
    PaiWorld::PaiWorld(int height, int width) : worldHeight{height},
                                                worldWidth{width}
    {
        worldState = std::vector<std::vector<int>>(worldWidth, std::vector<int>(worldHeight, 1));
    }

    PaiWorld::~PaiWorld() {}

    glm::vec4 PaiWorld::actions(glm::vec2 state)
    {
        int left = state.x <= 0 ? 0 : state.x - 1 ;
        int right = state.x >= worldWidth -1 ? worldWidth -1: state.x + 1 ;
        int top = state.y <= 0 ? 0 : state.y - 1 ;
        int bottom = state.y >= worldHeight -1 ? worldHeight -1: state.y + 1 ;
        return {
            left == state.x ? 0 : worldState[left][int(state.x)],
            right == state.x ? 0 : worldState[right][int(state.x)],
            top == state.y ? 0 : worldState[top][int(state.y)],
            bottom == state.y ? 0 : worldState[bottom][int(state.y)]
        };
    }

}