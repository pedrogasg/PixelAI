#include <vector>
#include <glm/glm.hpp>

namespace pai
{
    class PaiWorld
    {
    private:
        int worldHeight;
        int worldWidth;
        std::vector<std::vector<int>> worldState;
    public:
        PaiWorld(int height, int width);
        ~PaiWorld();

        glm::vec4 actions(glm::vec2 state);
        void addwalls(std::vector<glm::vec2> walls);
    };

} // namespace pai
