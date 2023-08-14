#pragma once

#include <algorithm>
#include <limits>
#include <set>
#include <string_view>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "debug_utils.hpp"
#include "types.hpp"

struct GLFWwindow;

namespace hc {

class Engine {
public:
    // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
    explicit Engine(std::string_view title, i32 width, i32 height)
        : _title{title},
          _width{width},
          _height{height} {}

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    void init();
    void run();
    void render();
    void cleanup();

private:
    void init_vulkan();
    void init_swapchain();
    void init_commands();
    void create_instance();
    void create_surface();
    void create_device();

    bool _is_init{false};
    i32 _frame_number{0};
    std::string _title{};
    i32 _width{1600};
    i32 _height{800};
    GLFWwindow* _window{nullptr};

    vk::UniqueInstance _instance{};
    vk::UniqueDebugUtilsMessengerEXT _messenger;
    vk::PhysicalDevice _gpu{};
    vk::UniqueDevice _device{};
    vk::UniqueSurfaceKHR _surface{};

    struct {
        u32 graphics;
        u32 present;
    } _queue_indices{};

    vk::Format _swapchain_format{};
    vk::UniqueSwapchainKHR _swapchain{};
    std::vector<vk::Image> _swapchain_images{};
    std::vector<vk::UniqueImageView> _swapchain_image_views;
};

}  // namespace hc