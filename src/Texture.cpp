#include "../include/Texture.hh"

namespace  Kate {
    // IMPLEMENTATION
    Texture::Texture() noexcept
            :   m_Height{}, m_Width{}
    {
        glGenTextures(1, &this->m_Id);
    }

    Texture::Texture(const std::filesystem::path& path) noexcept
            :   m_Height{}, m_Width{}
    {
        glGenTextures(1, &this->m_Id);
        load(path);
    }

    auto Texture::load(const std::filesystem::path& path) -> void {
        char fileDir[4096]{};
#ifdef WINDOWS
        std::wcstombs(fileDir, path.c_str(), 4096);
#else
        // reinterpret cast 'cause IDE issues
        std::memcpy(fileDir, path.c_str(), std::strlen(reinterpret_cast<const char *>(path.c_str())));
#endif
        // configure and enable blending
        enableBlending();
        setupBlendingProperties();

        stbi_set_flip_vertically_on_load(true);
        // cast to const char because on windows path.c_str() returns a const wchar_t
        std::uint8_t* imageData{ stbi_load(fileDir, &m_Width, &m_Height, &m_Channels, 4) };

        if (imageData) {
            bind();
            std::cout << "Texture data loaded successfully..." << std::endl;
            // setup wrapping and filtering options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
            glGenerateMipmap(GL_TEXTURE_2D);

            unbind();
            stbi_image_free(imageData);
        }
        else {
            std::cerr << "Could not load Texture data...\n";
        }

    }

    auto Texture::bind() const -> void { glBindTexture(GL_TEXTURE_2D, getId()); }

    auto Texture::unbind() -> void { glBindTexture(GL_TEXTURE_2D, 0); }

    auto Texture::getId() const -> std::uint32_t { return m_Id; }

    Texture::~Texture() { glDeleteTextures(1, &this->m_Id); }

    Texture::Texture(Texture &&other) noexcept { *this = std::move(other); }

    Texture &Texture::operator=(Texture &&other) noexcept {
        m_Id = other.getId();
        m_Width = other.getDimensions().first;
        m_Height = other.getDimensions().second;
        m_Channels = other.getChannels();
        m_Type = other.getType();

        other.m_Id = 0;
        other.m_Width = 0;
        other.m_Height = 0;
        other.m_Channels = 0;
        m_Type = TextureType::INVALID;

        return *this;
    }

    auto Texture::bindUnit(std::int32_t unit) const -> void {
        bind();
        glActiveTexture(GL_TEXTURE0 + unit);
    }

    Texture::Dimensions Texture::getDimensions() const {
        return std::make_pair(m_Width, m_Height);
    }

    auto Texture::getChannels() const -> std::int32_t {
        return m_Channels;
    }

    auto Texture::getType() const -> TextureType {
        return m_Type;
    }

    auto Texture::enableBlending() -> void {
        glEnable(GL_BLEND);
    }

    auto Texture::setupBlendingProperties() -> void {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
}