#pragma once

#include <algorithm>
#include <limits>
#include <set>
#include <stdexcept>
#include <string_view>
#include <vector>

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

#include "allocator.hpp"
#include "camera.hpp"
#include "core.hpp"
#include "debug_utils.hpp"
#include "depth_buffer.hpp"
#include "shader.hpp"
#include "timer.hpp"
#include "scene.hpp"
#include "pipeline_builder.hpp"

struct GLFWwindow;

namespace hc {

struct PushConstants {
    glm::mat4 projection{};
    glm::mat4 view{};
    glm::mat4 model{};
    glm::vec3 light_pos{};
};

struct SwapchainData {
    vk::Format format{};
    vk::Extent2D extent{};
    vk::UniqueSwapchainKHR handle{};
    std::vector<vk::Image> images{};
    std::vector<vk::UniqueImageView> image_views{};
};

struct QueueFamily {
    u32 graphics;
    u32 present;
};

struct WindowData {
    std::string title{};
    i32 width{1600};
    i32 height{800};
    GLFWwindow* handle{nullptr};
};

class Engine {
public:
    // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
    explicit Engine(std::string_view title, i32 width, i32 height)
        : _window{std::string{title}, width, height} {}

    Engine() = default;
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    Engine(Engine&&) = delete;
    Engine& operator=(Engine&&) = delete;
    ~Engine() = default;

    void init();
    void run();
    void update(double dt);
    void render();
    void cleanup();

    void cycle_pipeline();
    void on_resize();
    void on_focus(bool focused);
    void on_mouse_move(glm::dvec2 pos);
    void on_scroll(double dx, double dy);
    void on_key_press(i32 keycode);

private:
    [[nodiscard]]
    float aspect_ratio() const noexcept;
    void init_vulkan();
    void init_allocator();
    void create_swapchain();
    void create_scene();
    void init_commands();
    void init_renderpass();
    void create_framebuffers();
    void create_pipelines();
    void init_sync_obj();
    void create_instance();
    void create_surface();
    void create_device();
    void load_shaders();
    void recreate_swapchain();
    void destroy_swapchain();

    bool _is_init{};
    bool _focused{};
    bool _resized{};
    usize _frame_number{};
    WindowData _window{};
    Timer _timer{};
    Camera _camera{};
    glm::dvec2 _cursor{};
    Scene _scene{};

    vk::UniqueInstance _instance{};
    vk::UniqueDebugUtilsMessengerEXT _messenger{};
    vk::PhysicalDevice _gpu{};
    vk::UniqueDevice _device{};
    VmaAllocator _allocator{};
    ShaderMap _shaders{};
    vk::Queue _graphics_queue{};
    vk::UniqueSurfaceKHR _surface{};
    QueueFamily _queue_family{};
    SwapchainData _swapchain{};
    DepthBuffer _depth_buffer{};
    vk::UniqueBuffer _vertex_buffer{}, _index_buffer{};
    vk::UniqueDeviceMemory _vertex_buffer_mem{}, _index_buffer_mem{};
    vk::UniqueCommandPool _cmd_pool{};
    vk::UniqueCommandBuffer _cmd_buffer{};
    vk::UniqueRenderPass _render_pass{};
    std::vector<vk::UniqueFramebuffer> _framebuffers{};
    vk::UniqueSemaphore _present_semaphore{}, _render_semaphore{};
    vk::UniqueFence _render_fence{};
    usize _pipeline_idx{};
    GraphicsPipeline _gfx_pipelines{};
};

}  // namespace hc
