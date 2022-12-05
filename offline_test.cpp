#include "include/Processor.hpp"


int main()
{
    Processor processor(true, "obj");
    int num_processed = processor.process(PROCESSOR_JPEG);

    std::cout << "Processed " << num_processed << " files." << std::endl;

    return 0;
}
