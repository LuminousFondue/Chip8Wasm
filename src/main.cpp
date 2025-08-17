#include <spdlog/spdlog.h>

#include "Chip8.h"

using namespace std;

int main()
{
    spdlog::set_level(spdlog::level::trace);
    spdlog::info("Application Started");

    Chip8 chip8;

    spdlog::info("Application Ended");
    return 0;
}