#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "Model.hpp"
#include "Shader.hpp"
#include "Film.hpp"

class Camera 
{
public:
    Camera()
    {
        view = glm::mat4(1.0f);
        perspective = glm::mat4(1.0f);
    }

    void capture(const Model &model, Shader &shader, Film *film = nullptr) const
    {
        if (film)
        {
            assert(film->valid);
            film->bind();
            glClearColor(0.2f, 0.0f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        shader.use();
        glUniformMatrix4fv(shader.at("model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
        glUniformMatrix4fv(shader.at("view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(shader.at("perspective"), 1, GL_FALSE, glm::value_ptr(perspective));
        glUniform2f(shader.at("zNearFar"), 0.1f, 300.0f);
        // Orthogonal depth map
        glUniform1i(shader.at("pDepthMap"), 0); 
        glBindVertexArray(model.VAO);
        glDrawArrays(GL_TRIANGLES, 0, model.num_vertices);
        if (film)
        {
            film->unbind();
            // Maybe revert glViewport?
        }
    }

    void align_to(const Model &model)
    {
        constexpr float full = 128.0f;
        constexpr float half = full / 2.0f;

        // Camera should be looking from negative direction to positive direction
        glm::vec3 eye = glm::vec3(half, half, model.bounding_box.max.z);
        eye.z += 1.0f;

        view = glm::lookAt(eye, eye - glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        perspective = glm::ortho(-half, half, -half, half, 0.1f, 300.0f);
        // perspective = glm::perspective(glm::radians(45.0f), 1.0f, 1.0f, 1000.0f);
    }

    glm::mat4 view, perspective;
};

#endif // CAMERA_HPP
