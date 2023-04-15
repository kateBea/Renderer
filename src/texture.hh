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
#include <iostream>

// Third-Party Libraries
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Project Libraries
#include <utils.hh>

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

        /**
         * Unbinds the currently bound texture object
        */
        static auto unbind() -> void;

        /**
         * Activates GL_TEXTUREX unit where goes from 0 to amount
         * of supported texture units
        */
       static auto bind_texture_unit(std::uint32_t unit);

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
        std::uint32_t m_id{};               // Identifier of this vertex buffer object
        std::int32_t m_height{};            // Texture height
        std::int32_t m_width{};             // Texture width
        std::int32_t m_channels{};          // imagne components in texture file
        kate::texture_t m_type{};
    };


    // IMPLEMENTATION
    texture::texture() 
        :   m_height{}, m_width{}
    {
        glGenTextures(1, &this->m_id);
    }

    texture::texture(const std::filesystem::path& path) noexcept 
        :   m_height{}, m_width{}
    {
        glGenTextures(1, &this->m_id);
        load_data(path);
    }

    auto texture::load_data(const std::filesystem::path& path) -> void {

        stbi_set_flip_vertically_on_load(true); 
        std::uint8_t* image_data{ stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0) };

        if (image_data) {
            bind();
            std::cerr << "Texture data loaded succesfully...\n";
            // setup wrapping and filtering options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
            glGenerateMipmap(GL_TEXTURE_2D);

            unbind();
            stbi_image_free(image_data);
        }  
        else {
            std::cerr << "Could not load texture data...\n";
        }
        
    }

    auto texture::bind() const -> void { glBindTexture(GL_TEXTURE_2D, get_id()); }

    auto texture::unbind() -> void { glBindTexture(GL_TEXTURE_2D, 0); }

    auto texture::get_id() const -> std::uint32_t { return m_id; }

    texture::~texture() { glDeleteTextures(1, &this->m_id); }

    static auto bind_texture_unit(std::uint32_t unit) { glActiveTexture(GL_TEXTURE0 + unit); }
}
#endif