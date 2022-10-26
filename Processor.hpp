#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <filesystem>
#include <glad/glad.h>
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
    Processor(const std::string &path) : folder(path)
    {

    }

    int process(ProcessorExportMode export_mode) const
    {
        std::vector<std::string> files;

        for (const auto &entry : std::filesystem::directory_iterator(folder))
        {
            std::string path = entry.path().string();
            if (path.substr(path.length() - 4) != ".obj")
            {
                continue;
            }
            files.emplace_back(path);
        }
        
        std::cout << "Processing " << files.size() << " files." << std::endl;
        int processed = 0;

        Camera camera;
        Film film(128, 128);
        Shader shader("shader/normal.vert", "shader/normal.frag");

        for (const auto &entry : files)
        {
            std::cout << "Processing " << entry << "... ";

            Model model(entry);
            if (!model.valid)
            {
                std::cerr << "Cannot process " << entry << "." << std::endl;
                continue;
            }

            camera.align_to(model);
            camera.capture(model, shader, &film);
            switch (export_mode)
            {
                case PROCESSOR_JPEG:
                {
                    if (!film.save(entry + ".jpg"))
                    {
                        std::cerr << "Cannot save as " << entry << ".jpg." << std::endl;
                        continue;
                    }
                    std::cout << entry << ".jpg" << std::endl;
                    break;
                }

                case PROCESSOR_CSV:
                {
                    if (!film.save_csv(entry + ".csv"))
                    {
                        std::cerr << "Cannot save as " << entry << ".csv." << std::endl;
                        continue;
                    }
                    std::cout << entry << ".csv" << std::endl;
                    break;
                }
            }
        }

        return processed;
    }

private:
    std::string folder;
};

#endif // PROCESSOR_HPP
