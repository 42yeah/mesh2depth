#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "include/Model.hpp"
#include "include/Shader.hpp"
#include "include/Camera.hpp"
#include "include/Film.hpp"
#include "include/Processor.hpp"


int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 3)
    {
        std::cout << "Usage: " << argv[0] << " <path to folder> [format]" << std::endl;
        std::cout << "Format description:" << std::endl
            << "1: CSV (default)" << std::endl
            << "2: JPEG" << std::endl;
        return 1;
    }
    const std::string path = argv[1];
    ProcessorExportMode format = PROCESSOR_CSV;
    if (argc == 3)
    {
        if (argv[2][0] == '1')
        {
            format = PROCESSOR_CSV;
        }
        else if (argv[2][0] == '2')
        {
            format = PROCESSOR_JPEG;
        }
        else
        {
            std::cout << "Unsupported format type: " << argv[2] << "?" << std::endl;
            return 1;
        }
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow *window = glfwCreateWindow(128, 128, "mesh2depth", nullptr, nullptr);

    if (!window)
    {
        std::cerr << "Failed to initialize GLFW window?" << std::endl;
        return 1;
    }

    glfwMakeContextCurrent(window);
    
    if (!gladLoadGL())
    {
        std::cerr << "Failed to initialize GLAD?" << std::endl;
        return 1;
    }

    glEnable(GL_DEPTH_TEST);

    // Make sure everything OpenGL is destroyed before the eventual termination of window
    {
        try
        {
            Processor processor(false, path);
            processor.process(format);
        }
        catch (const std::exception &)
        {
            std::cerr << "Cannot process directory " << path << "." << std::endl;
        }

        // Shader shader("shader/normal.vert", "shader/normal.frag");
        // Model model("obj/086075_mesh.obj");
        // Camera camera;
        // camera.align_to(model);
        // Film film(128, 128);

        // std::shared_ptr<Model> rect_model = Model::generate_rect_model();
        // Shader rect_shader("shader/rect_texture.vert", "shader/rect_texture.frag");

        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            // glClearColor(0.5f, 0.0f, 0.2f, 1.0f);
            // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // camera.capture(model, shader, &film);
            
            // film.draw(*rect_model, rect_shader);
            // assert(film.save("test.jpg"));
            // assert(film.save_csv("test.csv"));
            glfwSetWindowShouldClose(window, true);

            glfwSwapBuffers(window);
        }
    }

    glfwDestroyWindow(window);

    return 0;
}
