/**
 * @file Texture.hh
 * @author Kate
 * @brief Defines the Texture class
 * @version 1.0
 * @date 2023-04-06
 */


#ifndef TEXTURE_HH
#define TEXTURE_HH

// C++ Standard Library
#include <array>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <utility>
#include <string_view>

// Third-Party Libraries
#include <GL/glew.h>

#include <stb_image.h>

// Project Libraries
#include "Utils.hh"
#include "Logger.hh"

namespace Kate {
    class Texture {
    public:
        /**
         * Defines a type of Texture
         * */
        enum class TextureType {
            DIFFUSE,
            SPECULAR,
            NORMAL,
            INVALID,
        };

        /**
         * Represents image dimensions, first value is width and second is height
         * */
        using Dimensions = std::pair<std::int32_t, std::int32_t>;

        /**
         * Initializes this Texture creating a new
         * Texture object
         * */
        explicit Texture(TextureType type = TextureType::DIFFUSE) noexcept;

        /**
         * Creates a new Texture object and fills it with the data
         * from Texture file in path. If no data is provided it simply creates
         * a valid Texture object with a valid id
         * @param path the path to the Texture file
         * */
        explicit Texture(const std::filesystem::path& path, TextureType type = TextureType::DIFFUSE) noexcept;

        /**
         * Copy constructor. Marked as delete to avoid Texture aliasing
         * Ensure one Texture id is held by one Texture instance
         * */
        Texture(const Texture& other) = delete;

        /**
         * Copy assigment. Marked as delete to avoid aliasing
         * Ensure one Texture id is held by one Texture instance
         * */
        Texture& operator=(const Texture& other) = delete;

        /**
         * Move constructor
         * @param other move from Texture
         * */
        Texture(Texture&& other) noexcept;

        /**
         * Move assignment
         * @return *this
         * */
        Texture& operator=(Texture&& other) noexcept;

        /**
         * Mark this Texture as current
         * */
        auto bind() const -> void;

        /**
         * Unbinds the currently bound Texture object
         * */
        static auto unbind() -> void;

        /**
         * Returns the dimensions of the image file for this Texture
         * @return image dimensions
         * */
        [[nodiscard]]
        Dimensions getDimensions() const;

        /**
         * Activates a unit where goes from 0 to amount
         * of supported Texture N units. GL_TEXTURE0, GL_TEXTURE1 ... GL_TEXTUREN.
         * This function ensures to mark this Texture as current one, so a previous
         * call to <b>Texture::bind()</b> is unnecessary
         * */
        static auto bindUnit(std::int32_t unit) -> void ;

        /**
         * Returns the identifier of this Texture
         * @return id of this object
         * */
        [[nodiscard]]
        auto getId() const -> std::uint32_t;

        /**
         * Returns the number of image components in loaded file
         * @return channels of this Texture
         * */
        [[nodiscard]]
        auto getChannels() const -> std::int32_t ;

        /**
         * Returns the type of this Texture
         * @return type of this Texture
         * */
        [[nodiscard]]
        auto getType() const -> TextureType;

        /**
         * Creates a new Texture object and fills it with the data
         * from Texture file in path. If no data is provided it simply creates
         * a valid Texture object with a valid id
         * @param path the path to the Texture file
         * @throws std::runtime_error if the file does not exist
         * */
        auto load(const std::filesystem::path& path) -> void;

        /**
         * Similar behaviour to Kate::Texture::load() but we instead provide the path and type
         * to create a new Texture
         * @param path path to the texture file
         * @param type type of texture
         * @return newly created Texture
         * */
        static auto fromFile(const std::filesystem::path& path, Kate::Texture::TextureType type) -> Kate::Texture;

        /**
         * Returns a C-String representing the type of
         * texture specified
         * */
        static constexpr auto getStrType(Texture::TextureType type) -> std::string_view {
            switch (type) {
                case TextureType::SPECULAR: return "texture_specular";
                case TextureType::DIFFUSE: return "texture_diffuse";
                case TextureType::NORMAL: return "texture_normal";
                default: return "invalid";
            }
        }
        
        /**
         * Releases resources of this Texture
         * */
        ~Texture();

    private:
        /**
         * Enables blending
         * */
        static auto enableBlending() -> void;


        /**
         * Setup this bound texture
         * */
        auto setupTexture(std::uint8_t* data) -> void;

        /**
         * Defines blending properties, can be used to customize
         * how blending is performed
         * */
        static auto setupBlendingProperties() -> void;

        std::uint32_t   m_Id{};         // Identifier of this Vertex buffer object
        std::int32_t    m_Height{};     // Texture height
        std::int32_t    m_Width{};      // Texture width
        std::int32_t    m_Channels{};   // components in image file
        TextureType     m_Type{};       // type of Texture
    };
}
#endif