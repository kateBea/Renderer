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
#include <vector>
#include <filesystem>
#include <stdexcept>
#include <cstddef>
#include <iostream>
#include <utility>
#include <cstdlib>
#include <cstring>

// Third-Party Libraries
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Project Libraries
#include <utils.hh>

namespace Kate {
    class Texture {
    public:
        /*
         * Represents image dimensions, first value is width and second is height
         * */
        using Dimensions = std::pair<std::int32_t, std::int32_t>;

        explicit Texture();
        /**
         * Creates a new Texture object and fills it with the data
         * from Texture file in path. If no data is provided it simply creates
         * a valid Texture object with a valid id
         * @param path the path to the Texture file
         * */
        explicit Texture(const std::filesystem::path& path) noexcept;

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
        */
        static auto unbind() -> void;

        [[nodiscard]]
        Dimensions getDimensions() const;


        /**
         * Activates a unit where goes from 0 to amount
         * of supported Texture N units. GL_TEXTURE0, GL_TEXTURE1 ... GL_TEXTUREN
        */
        auto bindUnit(std::int32_t unit) -> void ;

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
         * */
        auto load(const std::filesystem::path& path) -> void;
        
        /**
         * Releases resources from this Vertex buffer
         * */
        ~Texture();

    private:
        std::uint32_t m_id{};               // Identifier of this Vertex buffer object
        std::int32_t m_height{};            // Texture height
        std::int32_t m_width{};             // Texture width
        std::int32_t m_channels{};          // components in image file
        Kate::TextureType m_type{};         // type of Texture
    };


    // IMPLEMENTATION
    Texture::Texture()
        :   m_height{}, m_width{}
    {
        glGenTextures(1, &this->m_id);
    }

    Texture::Texture(const std::filesystem::path& path) noexcept
        :   m_height{}, m_width{}
    {
        glGenTextures(1, &this->m_id);
        load(path);
    }

    auto Texture::load(const std::filesystem::path& path) -> void {
        constexpr std::size_t maxPathLenght{ 4096 };
        char fileDir[maxPathLenght]{};

#ifdef WINDOWS
        std::wcstombs(fileDir, path.c_str(), maxPathLenght);
#else
        // reinterpret cast 'cause IDE issues
        std::memcpy(fileDir, path.c_str(), std::strlen(reinterpret_cast<const char *>(path.c_str())));
#endif

        stbi_set_flip_vertically_on_load(true);
        // cast to const char because on windows path.c_str() returns a const wchar_t
        std::uint8_t* image_data{ stbi_load(fileDir, &m_width, &m_height, &m_channels, 0) };

        if (image_data) {
            bind();
            std::cerr << "Texture data loaded successfully...\n";
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
            std::cerr << "Could not load Texture data...\n";
        }
        
    }

    auto Texture::bind() const -> void { glBindTexture(GL_TEXTURE_2D, getId()); }

    auto Texture::unbind() -> void { glBindTexture(GL_TEXTURE_2D, 0); }

    auto Texture::getId() const -> std::uint32_t { return m_id; }

    Texture::~Texture() { glDeleteTextures(1, &this->m_id); }

    Texture::Texture(Texture &&other) noexcept { *this = std::move(other); }

    Texture &Texture::operator=(Texture &&other) noexcept {
        m_id = other.getId();
        m_width = other.getDimensions().first;
        m_height = other.getDimensions().second;
        m_channels = other.getChannels();
        m_type = other.getType();

        other.m_id = 0;
        other.m_width = 0;
        other.m_height = 0;
        other.m_channels = 0;
        m_type = TextureType::INVALID;

        return *this;
    }

    auto Texture::bindUnit(std::int32_t unit) -> void {
        bind();
        glActiveTexture(GL_TEXTURE0 + unit);
    }

    Texture::Dimensions Texture::getDimensions() const {
        return std::make_pair(m_width, m_height);
    }

    auto Texture::getChannels() const -> std::int32_t {
        return m_channels;
    }

    auto Texture::getType() const -> TextureType {
        return m_type;
    }
}
#endif