#ifndef FILM_HPP
#define FILM_HPP

#include <iostream>
#include <iomanip>
#include <fstream>
#include <glad/glad.h>
#include "Shader.hpp"
#include "Model.hpp"
#include "stb_image_write.h"
#include "utils.hpp"

class MESH2DEPTH_API Film
{
public:
    Film(int width, int height) : valid(false),
        width(width), 
        height(height)
    {
        glGenFramebuffers(1, &framebuffer_gl);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_gl);

        // Create a texture and a framebuffer
        glGenTextures(1, &texture_gl);
        glBindTexture(GL_TEXTURE_2D, texture_gl);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_gl, 0);

        // Attach Renderbuffer and check for completion status
        glGenRenderbuffers(1, &renderbuffer_gl);
        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_gl);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer_gl);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cerr << "WARNING! Imcomplete framebuffer creation? OpenGL error is " << glGetError() << std::endl;
            glDeleteRenderbuffers(1, &renderbuffer_gl);
            glDeleteTextures(1, &texture_gl);
            glDeleteFramebuffers(1, &framebuffer_gl);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

        valid = true;
    }

    ~Film()
    {
        if (valid)
        {
            glDeleteRenderbuffers(1, &renderbuffer_gl);
            glDeleteTextures(1, &texture_gl);
            glDeleteFramebuffers(1, &framebuffer_gl);
        }
    }

    void bind() const
    {
        glViewport(0, 0, width, height);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_gl);
    }

    void unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
    }

    // Draw the texture to screen
    void draw(const Model &model, Shader &shader) const
    {
        glBindVertexArray(model.VAO);
        shader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_gl);
        glUniform1i(shader.at("tex"), 0);
        glDrawArrays(GL_TRIANGLES, 0, model.num_vertices);
    }

    bool save(const std::string &path) const;

    bool save_csv(const std::string &path) const
    {
        float *data = new float[width * height * 3];

        glBindTexture(GL_TEXTURE_2D, texture_gl);
        // glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, data);
        glGetTextureSubImage(texture_gl, 0, 0, 0, 0, width, height, 1, GL_RGB, GL_FLOAT, sizeof(float) * width * height * 3, data);
        
        std::ofstream writer(path);
        if (!writer.good())
        {
            return false;
        }
        writer << std::setprecision(std::numeric_limits<long double>::digits10 + 1);
        for (int y = height - 1; y >= 0; y--)
        {
            for (int x = 0; x < width; x++)
            {
                int offset = (y * width + x) * 3;
                float g = data[offset + 1];
                
                if (g == 0.0f)
                {
                    writer << "-1.0,";
                }
                else
                {
                    writer << data[offset + 0] << ",";
                }
            }
            writer << std::endl;
        }

        delete[] data;
        return true;
    }

    bool valid;
    int width, height;

private:
    GLuint texture_gl, framebuffer_gl, renderbuffer_gl;
};

#endif // FILM_HPP
