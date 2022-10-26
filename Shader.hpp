#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <iostream>
#include <map>
#include <fstream>
#include "utils.hpp"

class Shader 
{
public:
    Shader() : valid(false),
        program(GL_NONE)
    {

    }

    ~Shader()
    {
        if (valid) 
        {
            glDeleteProgram(program);
        }
    }

    static GLuint compile(GLuint type, const std::string &src, std::string &error)
    {
        GLuint shader = glCreateShader(type);
        const char *raw = src.c_str();
        glShaderSource(shader, 1, &raw, nullptr);
        glCompileShader(shader);

        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

        if (status == GL_FALSE)
        {
            char log[512] = {0};
            glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
            error = log;
            glDeleteShader(shader);
            return GL_NONE;
        }

        return shader;
    }

    Shader(const std::string &vertex_path, const std::string &fragment_path) : valid(false),
        program(GL_NONE)
    {
        std::string vertex_source, fragment_source;
        if (!read_or_fail(vertex_path, vertex_source) ||
            !read_or_fail(fragment_path, fragment_source))
        {
            std::cerr << "Failed to read " << vertex_path << " or " << fragment_path << "?" << std::endl;
            return;
        }

        std::string error;
        GLuint vertex_shader = compile(GL_VERTEX_SHADER, vertex_source, error);
        if (vertex_shader == GL_NONE)
        {
            std::cerr << "Failed to compile " << vertex_path << ": " << error << "?" << std::endl;
            return;
        }
        GLuint fragment_shader = compile(GL_FRAGMENT_SHADER, fragment_source, error);
        if (fragment_shader == GL_NONE)
        {
            std::cerr << "Failed to compile " << fragment_path << ": " << error << "?" << std::endl;
            return;
        }

        program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        GLint status;
        glGetProgramiv(program, GL_LINK_STATUS, &status);
        if (status == GL_FALSE)
        {
            char log[512] = {0};
            glGetProgramInfoLog(program, sizeof(log), nullptr, log);
            error = log;
            glDeleteProgram(program);
            std::cerr << "Failed to link program: " << log << "?" << std::endl;
            return;
        }

        valid = true;
    }

    GLuint at(const std::string &uniform_name)
    {
        if (uniforms.find(uniform_name) == uniforms.end())
        {
            GLint loc = glGetUniformLocation(program, uniform_name.c_str());

            if (loc < 0)
            {
                std::cerr << "Warning: cannot find " << uniform_name << "." << std::endl;
            }

            uniforms[uniform_name] = loc;
        }
        return uniforms[uniform_name];
    }

    void use() const
    {
        glUseProgram(program);
    }

    bool valid;

private:
    std::map<std::string, GLuint> uniforms;
    GLuint program;
};


#endif // SHADER_HPP
