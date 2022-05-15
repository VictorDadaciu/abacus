#include "pch.h"

#include "Application.h"

using namespace abc;

int main(int argc, char* argv[])
{
    try
    {
        APP->Start();
        while (APP->IsRunning())
        {
            APP->Update();
        }
        APP->Quit();
    } 
    catch (const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}