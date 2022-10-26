#include "Model.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "deps/tiny_obj_loader.h"
#include <vector>
#include <cassert>
#include "utils.hpp"


Model::Model(const std::string &path) : 
    valid(false),
    VAO(GL_NONE),
    VBO(GL_NONE),
    num_vertices(0)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warning, error;
    valid = tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &error, path.c_str());

    // std::cout << "Loading model " << path << "." << std::endl;
    
    if (!warning.empty())
    {
        // std::cerr << "Model finished loading with warning: " << warning << std::endl;
    }

    if (!error.empty())
    {
        // std::cerr << "Model finished loading with error: " << error << "?" << std::endl;
        return;
    }

    if (!init_model_from_data(attrib, shapes))
    {
        // std::cerr << "Failed to initialize model from data?" << std::endl;
        valid = false;
    }

    // std::cout << "Model loading report:" << std::endl;
    // std::cout << "Bounding box: " << bounding_box.min << " - " << bounding_box.max << std::endl;
    // std::cout << "Center: " << bounding_box.center() << std::endl;
    // std::cout << "#vertices: " << num_vertices << std::endl;
    // std::cout << "OpenGL error? " << glGetError() << std::endl;
}

Model::~Model()
{
    if (valid)
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
}

bool Model::init_model_from_data(const tinyobj::attrib_t &attrib, const std::vector<tinyobj::shape_t> &shapes)
{
    // Only position data. Normal is not suppported.
    std::vector<glm::vec3> data;

    for (size_t s = 0; s < shapes.size(); s++)
    {
        const auto &mesh = shapes[s].mesh;
        for (int f = 0; f < mesh.num_face_vertices.size(); f++)
        {
            assert(mesh.num_face_vertices[f] == 3 && "Only triangle meshes are supported");
            for (int v = 0; v < 3; v++)
            {
                tinyobj::index_t index = mesh.indices[3 * f + v];

                glm::vec3 p = glm::vec3(
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                );
                data.push_back(p);
                bounding_box.enclose(p);
                num_vertices++;
            }
        }
    }

    // Initialize OpenGL stuffs
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec3), data.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

    return true;
}

std::shared_ptr<Model> Model::generate_triangle_model() 
{
    std::vector<glm::vec3> data = 
    {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.5f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.5f, 0.0f)
    };

    std::shared_ptr<Model> model = std::make_shared<Model>();

    // Initialize OpenGL stuffs
    glGenVertexArrays(1, &model->VAO);
    glGenBuffers(1, &model->VBO);
    glBindVertexArray(model->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, model->VBO);

    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec3), data.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

    model->num_vertices = 3;
    for (const auto &p : data)
    {
        model->bounding_box.enclose(p);
    }
    return model;
}

std::shared_ptr<Model> Model::generate_rect_model() 
{
    std::vector<glm::vec3> data = 
    {
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(-1.0f, 1.0f, 0.0f),
        glm::vec3(-1.0f, -1.0f, 0.0f)
    };

    std::shared_ptr<Model> model = std::make_shared<Model>();

    // Initialize OpenGL stuffs
    glGenVertexArrays(1, &model->VAO);
    glGenBuffers(1, &model->VBO);
    glBindVertexArray(model->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, model->VBO);

    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec3), data.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

    model->num_vertices = 6;
    for (const auto &p : data)
    {
        model->bounding_box.enclose(p);
    }
    return model;
}
