#include <memory>

#include "hello_cube.hpp"

int main() {
    auto engine = std::make_unique<hc::Engine>(
        "Hello Cube", 1920, 1080, hc::BufferingMode::Double
    );

    try {
        engine->init();
        engine->run();
        engine->cleanup();
    } catch (const std::exception& e) {
        spdlog::critical("UNHANDLED EXCEPTION: {}", e.what());
    } catch (...) {
        spdlog::critical("UNHANDLED EXCEPTION");
    }
}
