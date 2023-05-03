#include "../include/Texture.hh"

namespace  Kate {
    // IMPLEMENTATION
    Texture::Texture(TextureType type) noexcept
            :   m_Height{}, m_Width{}, m_Type{ type }
    {
        glGenTextures(1, &this->m_Id);
    }

    Texture::Texture(const std::filesystem::path& path, TextureType type) noexcept
            :   m_Height{}, m_Width{}, m_Type{ type }
    {
        glGenTextures(1, &this->m_Id);
        load(path);
    }

    auto Texture::load(const std::filesystem::path& path) -> void {
        std::array<char, 4096> fileDir{};
#ifdef WINDOWS
        // fileDir.size() will return the amount of elements of fileDir, since it contains char which are byte sized
        wcstombs_s(nullptr, fileDir.data(), fileDir.size(), path.c_str(), 4096);
#else

        std::copy(path.native().begin(), path.native().end(), fileDir.begin());
#endif
        // configure and enable blending
        enableBlending();
        setupBlendingProperties();

        stbi_set_flip_vertically_on_load(true);
        // cast to const char because on windows path.c_str() returns a const wchar_t
        std::uint8_t* imageData{ stbi_load(fileDir.data(), &m_Width, &m_Height, &m_Channels, 4) };

        if (imageData) {
            KATE_LOGGER_INFO("Texture data loaded successfully");

            bind();
            setupTexture(imageData);
            unbind();

            stbi_image_free(imageData);
        }
        else {
            throw std::runtime_error("Could not load Texture data");
        }

    }

    auto Texture::bind() const -> void { glBindTexture(GL_TEXTURE_2D, getId()); }

    auto Texture::unbind() -> void { glBindTexture(GL_TEXTURE_2D, 0); }

    auto Texture::getId() const -> std::uint32_t { return m_Id; }

    Texture::~Texture() { glDeleteTextures(1, &this->m_Id); }

    Texture::Texture(Texture &&other) noexcept { *this = std::move(other); }

    Texture &Texture::operator=(Texture &&other) noexcept {
        m_Id        = other.getId();
        m_Width     = other.getDimensions().first;
        m_Height    = other.getDimensions().second;
        m_Channels  = other.getChannels();
        m_Type      = other.getType();

        other.m_Id          = 0;
        other.m_Width       = 0;
        other.m_Height      = 0;
        other.m_Channels    = 0;
        m_Type              = TextureType::INVALID;

        return *this;
    }

    auto Texture::bindUnit(std::int32_t unit) -> void {
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

    auto Texture::fromFile(const std::filesystem::path& path, Kate::Texture::TextureType type) -> Kate::Texture  {
        Kate::Texture texture{ type };
        texture.load(path);
        return texture;
    }

    auto Texture::setupTexture(std::uint8_t* data) -> void {
        bind();
        // setup wrapping and filtering options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        unbind();
    }
}