// Python interoperate library

#include <iostream>
#include <cstdio>
#include <sys/stat.h>
#include "include/utils.hpp"
#include "include/Processor.hpp"

std::shared_ptr<Processor> processor = nullptr;

extern "C"
{
    MESH2DEPTH_API int processor_init()
    {
        // Offline processor
        processor = std::make_shared<Processor>(true);
        return 0;
    }

    MESH2DEPTH_API int processor_free()
    {
        processor = nullptr;
        return 0;
    }

    MESH2DEPTH_API int processor_add_model(const char *id, float *vertices, int num_elements, unsigned int *indices, int num_indices)
    {
        std::vector<float> vertices_vec;
        vertices_vec.assign(vertices, vertices + num_elements);

        std::vector<unsigned int> indices_vec;
        indices_vec.assign(indices, indices + num_indices);

        processor->add(id, std::make_shared<Model>(vertices_vec, indices_vec));

        return 0;
    }

    MESH2DEPTH_API int processor_process(int mode)
    {
        if (mode == 0)
        {
            return processor->process(PROCESSOR_JPEG);
        }
        else if (mode == 1)
        {
            return processor->process(PROCESSOR_CSV);
        }
        else
        {
            std::cerr << "Unknown processing mode: " << mode << "?" << std::endl;
            return 0;
        }
    }


    MESH2DEPTH_API int hello()
    {
        std::cout << "Hello!" << std::endl;
        return 0;
    }

    MESH2DEPTH_API int process_folder(const char *folder)
    {
        struct stat info;

        if (stat(folder, &info) != 0)
        {
            return 0;
        }

        Processor processor(true, folder);
        return processor.process(PROCESSOR_JPEG);
    }

    MESH2DEPTH_API int process_model(int mode, float *vertices, int num_elements, unsigned int *indices, int num_indices)
    {
        Processor processor(true);

        std::vector<float> vertices_vec;
        vertices_vec.assign(vertices, vertices + num_elements);

        std::vector<unsigned int> indices_vec;
        indices_vec.assign(indices, indices + num_indices);

        processor.add("result", std::make_shared<Model>(vertices_vec, indices_vec));

        if (mode == 0)
        {
            return processor.process(PROCESSOR_JPEG);
        }
        else if (mode == 1)
        {
            return processor.process(PROCESSOR_CSV);
        }
        else
        {
            std::cerr << "Unknown processing mode: " << mode << "?" << std::endl;
            return 0;
        }
    }
}
