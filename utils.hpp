#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>

inline std::ostream &operator<<(std::ostream &a, const glm::vec3 &p) 
{
    return a << "(" << p.x << ", " << p.y << ", " << p.z << ")";
}

inline bool read_or_fail(const std::string &path, std::string &res)
{
    std::ifstream reader(path);
    if (!reader.good())
    {
        return false;
    }
    std::stringstream ss;
    ss << reader.rdbuf();
    res = ss.str();
    return true;
}

#endif // UTILS_HPP
