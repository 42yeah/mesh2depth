#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <filesystem>
#include <glad/glad.h>
#include <vector>
#include <iostream>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Film.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

enum ProcessorExportMode
{
    PROCESSOR_JPEG,
    PROCESSOR_CSV
};

class Processor
{
public:
    Processor(bool offline) : offline(offline)
    {
        if (offline)
        {
            init_offline();
        }
    }

    Processor(bool offline, const std::string &folder) : offline(offline)
    {
        if (offline)
        {
            init_offline();
        }

        try
        {
            load_models(folder);    
        }
        catch (const std::exception &e)
        {
            std::cerr << "Invalid directory " << folder << "?" << std::endl;
        }
    }

    bool init_offline()
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        window = glfwCreateWindow(128, 128, "mesh2depth", nullptr, nullptr);

        if (!window)
        {
            std::cerr << "Failed to initialize GLFW window?" << std::endl;
            return false;
        }

        glfwMakeContextCurrent(window);
        
        if (!gladLoadGL())
        {
            std::cerr << "Failed to initialize GLAD?" << std::endl;
            return false;
        }

        glEnable(GL_DEPTH_TEST);

        return true;
    }

    ~Processor()
    {
        models.clear();

        if (offline)
        {
            while (!glfwWindowShouldClose(window))
            {
                glfwPollEvents();
                glfwSetWindowShouldClose(window, true);

                glfwSwapBuffers(window);
            }

            glfwDestroyWindow(window);
        }
    }

    void load_models(const std::string &folder)
    {
        std::vector<std::string> files;

        std::cout << "Reading models." << std::endl;

        for (const auto &entry : std::filesystem::directory_iterator(folder))
        {
            std::string path = entry.path().string();
            if (path.substr(path.length() - 4) != ".obj")
            {
                continue;
            }
            
            // files.emplace_back(path);
            models[path] = std::make_shared<Model>(path);
        }
    }

    int process(ProcessorExportMode export_mode) const
    {
        std::cout << "Processing " << models.size() << " files." << std::endl;
        int processed = 0;

        Camera camera;
        Film film(128, 128);
        Shader shader("shader/normal.vert", "shader/normal.frag");

        for (const auto &entry : models)
        {
            const auto &path = entry.first;
            const auto &model = entry.second;

            if (!model->valid)
            {
                std::cerr << "Cannot process " << path << "." << std::endl;
                continue;
            }

            camera.align_to(*model);
            camera.capture(*model, shader, &film);
            switch (export_mode)
            {
                case PROCESSOR_JPEG:
                {
                    if (!film.save(path + ".jpg"))
                    {
                        std::cerr << "Cannot save as " << path << ".jpg." << std::endl;
                        continue;
                    }
                    std::cout << path << ".jpg" << std::endl;
                    break;
                }

                case PROCESSOR_CSV:
                {
                    if (!film.save_csv(path + ".csv"))
                    {
                        std::cerr << "Cannot save as " << path << ".csv." << std::endl;
                        continue;
                    }
                    std::cout << path << ".csv" << std::endl;
                    break;
                }
            }

            processed++;
        }

        return processed;
    }

    std::map<std::string, std::shared_ptr<Model> > models;

private:
    // Offline functionalities
    bool offline;
    GLFWwindow *window;
};

#endif // PROCESSOR_HPP
