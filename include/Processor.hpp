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
    MESH2DEPTH_API Processor(bool offline);
    MESH2DEPTH_API Processor(bool offline, const std::string &folder);

    MESH2DEPTH_API ~Processor();

    bool init_offline();

    void load_models(const std::string &folder);

    MESH2DEPTH_API int process(ProcessorExportMode export_mode) const;

    MESH2DEPTH_API void add(const std::string &name, std::shared_ptr<Model> model);

private:
    std::map<std::string, std::shared_ptr<Model> > models;

    // Offline functionalities
    bool offline;
    GLFWwindow *window;
};

#endif // PROCESSOR_HPP
