/**
 * @file texture.hh
 * @author kate
 * @brief Defines the texture class
 * @version 1.0
 * @date 2023-04-06
 */


#ifndef TEXTURE_HH
#define TEXTURE_HH

// C++ Standard Library
#include <vector>
#include <filesystem>
#include <stdexcept>
#include <cstddef>

// Third-Party Libraries
#include <GL/glew.h>
#include <stb_image.h>

namespace kate {
    class texture {
    public:
        explicit texture();
        /**
         * Creates a new texture object and fills it with the data
         * from texture file in path. If no data is provided it simply creates
         * a valid texture object with a valid id
         * @param path the path to the texture file
         * */
        explicit texture(const std::filesystem::path& path) noexcept;

        /**
         * Copy constructor. Marked as delete to avoid texture aliasing
         * Ensure one texture id is held by one texture instance
         * */
        texture(const texture& other) = delete;

        /**
         * Copy assigment. Marked as delete to avoid aliasing
         * Ensure one texture id is held by one texture instance
         * */
        texture& operator=(const texture& other) = delete;

        /**
         * Move constructor
         * */
        texture(texture&& other) noexcept;

        /**
         * Move constructor assignment
         * */
        texture& operator=(texture&& other) noexcept;

        /**
         * Mark this texture as current
         * */
        auto bind() const -> void;


        static auto unbind() -> void;

        /**
         * Returns the identifier of this texture
         * @return id of this object
         * */
        [[nodiscard]]
        auto get_id() const -> std::uint32_t;

        /**
         * Returns the total size in bytes of the contents of texture
         * @return total size in bytes of the texture
         * */
        [[nodiscard]]
        auto get_size() const -> std::size_t;

        /**
         * Returns the total count of vertices of this vertex buffer
         * @return count of vertices
         * */
        [[nodiscard]]
        auto get_count() const -> std::size_t;

        /**
         * Releases the currently bound vertex buffer object
         * */
        static auto unbind() -> void;

        /**
         * Creates a new texture object and fills it with the data
         * from texture file in path. If no data is provided it simply creates
         * a valid texture object with a valid id
         * @param path the path to the texture file
         * */
        auto load_data(const std::filesystem::path& path) -> void;
        
        /**
         * Releases resources from this vertex buffer
         * */
        ~texture();

    private:
        std::uint32_t m_id{};   // Identifier of this vertex buffer object
        std::vector<std::uint8_t> m_data{}; // Holds the texture data
    };


    // IMPLEMENTATION
    texture::texture(const std::filesystem::path& path) noexcept {
        glGenTextures(1, &this->m_id);
        load_data(path);
    }

    auto texture::load_data(const std::filesystem::path& path) -> void {
        bind();
    }

    auto texture::bind() const -> void {
        glBindTexture(GL_TEXTURE_2D, this->m_id);
    }

    auto texture::unbind() -> void {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
#endif