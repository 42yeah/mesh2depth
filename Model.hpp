#ifndef MODEL_HPP
#define MODEL_HPP

#include <iostream>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "BBox.hpp"
#include "deps/tiny_obj_loader.h"

class Camera;
class Film;

class Model 
{
public:
    Model() : valid(false)
    {

    }
    
    Model(const std::string &path);
    Model(const Model &) = delete;
    Model(Model &&) = delete;

    ~Model();

    static std::shared_ptr<Model> generate_triangle_model();
    static std::shared_ptr<Model> generate_rect_model();

    bool valid;

private:

    bool init_model_from_data(const tinyobj::attrib_t &attrib,
        const std::vector<tinyobj::shape_t> &shapes);

    BBox bounding_box;
    GLuint VAO, VBO;
    int num_vertices;

    friend Camera;
    friend Film;
};

#endif // MODEL_HPP
