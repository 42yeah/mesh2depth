#ifndef BBOX_HPP
#define BBOX_HPP

#include <glm/glm.hpp>

struct BBox
{
    BBox() :
        min(glm::vec3(std::numeric_limits<float>::max())),
        max(glm::vec3(std::numeric_limits<float>::lowest()))
    {

    }

    void enclose(const glm::vec3 &p)
    {
        min = glm::min(p, min);
        max = glm::max(p, max);
    }

    bool is_inside(const glm::vec3 &p) const
    {
        return (p.x > min.x && p.y > min.y && p.z && min.z && p.x < max.x && p.y < max.y && p.z < max.z);
    }

    glm::vec3 center() const
    {
        return 0.5f * (min + max);
    }

    glm::vec3 min, max;
};

#endif // BBOX_HPP
