#include "../inc/differentiator_lib.h"

int main(const int argc, const char** argv)
{
    if (argc != 2)
    {
        error_processing(differentiator_too_few_files);
        return 0;
    }

    Differentiator differentiator = {};

    differentiator_error error = differentiator_ctr(&differentiator);
    if (error != differentiator_ok)
    {
        error_processing(error);
        return 0;
    }
    error = load_data(&differentiator, argv[1]);
    if (error != differentiator_ok)
    {
        error_processing(error);
        return 0;
    } 
    differentiator_dtr(&differentiator);

    return 0;
}