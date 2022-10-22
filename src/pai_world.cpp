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
            left == state.x ? 0 : worldState[int(state.y)][left],
            right == state.x ? 0 : worldState[int(state.y)][right],
            top == state.y ? 0 : worldState[top][int(state.x)],
            bottom == state.y ? 0 : worldState[bottom][int(state.x)]
        };
    }

    void PaiWorld::addwalls(std::vector<glm::vec2> walls)
    {
        for (int i = 0; i < walls.size(); i++)
        {
            auto wall = walls[i];
            worldState[wall.x][wall.y] = 0;
        }

    }

}