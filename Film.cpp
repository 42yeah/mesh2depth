#include "include/Film.hpp"


bool Film::save(const std::string &path) const
{
    stbi_flip_vertically_on_write(true);

    unsigned char *data = new unsigned char[width * height * 3];

    glBindTexture(GL_TEXTURE_2D, texture_gl);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    bool success = stbi_write_jpg(path.c_str(), width, height, 3, data, 100);

    delete[] data;
    return success;
}
