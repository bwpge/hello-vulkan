#pragma once

#include <fstream>
#include <filesystem>
#include <unordered_map>

#include <vulkan/vulkan.hpp>

#include "hvk/core.hpp"
#include "hvk/vk_context.hpp"

namespace hvk {

class Shader {
public:
    static Shader load_spv(const std::filesystem::path& path) {
        std::ifstream file{path, std::ios::ate | std::ios::binary};
        Shader result{};

        if (!file.is_open()) {
            PANIC(spdlog::fmt_lib::format(
                "failed to open shader file '{}'", path.string()
            ));
        }

        auto size = file.tellg();
        if (size <= 0) {
            PANIC("shader contains no byte code");
        }

        result._buf.resize(size);
        file.seekg(0);
        file.read(result._buf.data(), size);

        return result;
    }

    [[nodiscard]]
    vk::UniqueShaderModule module() const;

private:
    std::vector<char> _buf{};
};

enum class ShaderType {
    Geometry,
    Vertex,
    Fragment,
};

class ShaderMap {
    using Key = std::string_view;
    using Map = std::unordered_map<Key, Shader>;

public:
    void load(Key key, ShaderType type, const std::filesystem::path& path);
    void remove(Key key, ShaderType type);
    [[nodiscard]]
    const Shader& get(Key key, ShaderType type) const;
    [[nodiscard]]
    vk::UniqueShaderModule module(Key key, ShaderType type) const;
    [[nodiscard]]
    const Shader& vertex(Key key) const;
    [[nodiscard]]
    const Shader& fragment(Key key) const;
    [[nodiscard]]
    const Shader& geometry(Key key) const;

private:
    [[nodiscard]]
    const Map& get_by_name(ShaderType type) const {
        switch (type) {
            case ShaderType::Geometry:
                return _geometry;
            case ShaderType::Vertex:
                return _vertex;
            case ShaderType::Fragment:
                return _fragment;
            default:
                PANIC("Unsupported shader type");
                break;
        }
    }

    Map& get_by_name(ShaderType type) {
        switch (type) {
            case ShaderType::Geometry:
                return _geometry;
            case ShaderType::Vertex:
                return _vertex;
            case ShaderType::Fragment:
                return _fragment;
            default:
                PANIC("Unsupported shader type");
                break;
        }
    }

    Map _geometry{};
    Map _vertex{};
    Map _fragment{};
};

}  // namespace hvk
