/**
 * @file VertexBuffer.hh
 * @author kT
 * @brief Defines the Vertex buffer object class
 * @version 1.0
 * @date 2023-03-21
 */


#ifndef VBO_HH
#define VBO_HH

// C++ Standard Library
#include <vector>
#include <cstdint>
#include <string>
#include <string_view>

// Third-Party Libraries
#include <GL/glew.h>

namespace kT {
    enum class ShaderDataType {
        NONE,
        FLOAT_TYPE,     // Represents a single float data type
        FLOAT2_TYPE,    // Represents a two float data type
        FLOAT3_TYPE,    // Represents a three float data type
        FLOAT4_TYPE,    // Represents a four float data type

        MAT3_TYPE,      // Represents 3x3 float matrix data type
        MAT4_TYPE,      // Represents 4x4 float matrix data type

        INT_TYPE,       // Represents a single int data type
        INT2_TYPE,      // Represents a two int data type
        INT3_TYPE,      // Represents a three int data type
        INT4_TYPE,      // Represents a four int data type
        BOOL_TYPE,      // Represents a single boolean data type
        COUNT,
    };

    /**
     * Describes an element contained within a buffer data
     * */
    class BufferElement {
    public:
        BufferElement(ShaderDataType type, std::string_view name, bool normalized = false)
                :   m_Name{ name }, m_Type{ type }, m_Size{ getSizeFromShaderType(type) }, m_Offset{ 0 }, m_Normalized{ normalized } {}

        auto getAttributeCount() const -> std::uint32_t { return getComponentCount(m_Type); }
        auto getOpenGLAttributeDataType() const -> std::uint32_t { return getOpenGLTypeFromShaderDataType(m_Type); }
        auto getAttributeSize() const -> std::uint32_t { return m_Size; }

        /*  Getters */
        auto getName() const -> const std::string& { return m_Name; }
        auto getType() const -> ShaderDataType { return m_Type; }
        auto getSize() const -> std::uint32_t { return m_Size; }
        auto getOffset() const -> std::uint32_t { return m_Offset; }
        auto isNormalized() const  { return m_Normalized; }

        /*  Setters */
        auto setName(std::string_view name) -> void { m_Name = name; }
        auto setType(ShaderDataType type) -> void { m_Type = type; }
        auto setSize(std::uint32_t size) -> void { m_Size = size; }
        auto setOffset(std::uint32_t offset) -> void { m_Offset = offset; }
        auto setNormalized() -> void { m_Normalized = true; }
        auto unsetNormalized() -> void { m_Normalized = false; }

    private:
        // Size in bytes for integer
        static constexpr std::uint32_t s_DefaultShaderIntSize{ 4 };
        // Size in bytes for float
        static constexpr std::uint32_t s_DefaultShaderFloatSize{ 4 };

        std::string     m_Name{};
        ShaderDataType  m_Type{};
        std::uint32_t        m_Size{};
        std::uint32_t        m_Offset{};
        bool            m_Normalized;

        /**
         * Returns the size of the shader data type
         * @returns the size in bytes of the data type
         * */
        static constexpr auto getSizeFromShaderType(ShaderDataType type) -> std::uint32_t {
            switch (type) {
                case ShaderDataType::FLOAT_TYPE:    return s_DefaultShaderFloatSize;
                case ShaderDataType::FLOAT2_TYPE:   return s_DefaultShaderFloatSize * 2;
                case ShaderDataType::FLOAT3_TYPE:   return s_DefaultShaderFloatSize * 3;
                case ShaderDataType::FLOAT4_TYPE:   return s_DefaultShaderFloatSize * 4;

                case ShaderDataType::MAT3_TYPE:     return s_DefaultShaderFloatSize * (3 * 3);
                case ShaderDataType::MAT4_TYPE:     return s_DefaultShaderFloatSize * (4 * 4);

                case ShaderDataType::INT_TYPE:      return s_DefaultShaderIntSize;
                case ShaderDataType::INT2_TYPE:     return s_DefaultShaderIntSize * 2;
                case ShaderDataType::INT3_TYPE:     return s_DefaultShaderIntSize * 3;
                case ShaderDataType::INT4_TYPE:     return s_DefaultShaderIntSize * 4;
                case ShaderDataType::BOOL_TYPE:     return 1;
            }

            return 0;
        }

        /**
         * Returns the number of components of the given type
         * @returns Count of elements of the data type
         * */
        static constexpr auto getComponentCount(ShaderDataType type) -> std::uint32_t {
            switch(type) {
                case ShaderDataType::FLOAT_TYPE:    return 1;
                case ShaderDataType::FLOAT2_TYPE:   return 2;
                case ShaderDataType::FLOAT3_TYPE:   return 3;
                case ShaderDataType::FLOAT4_TYPE:   return 4;

                case ShaderDataType::MAT3_TYPE:     return 3 * 3;
                case ShaderDataType::MAT4_TYPE:     return 4 * 4;

                case ShaderDataType::INT_TYPE:      return 1;
                case ShaderDataType::INT2_TYPE:     return 2;
                case ShaderDataType::INT3_TYPE:     return 3;
                case ShaderDataType::INT4_TYPE:     return 4;
                case ShaderDataType::BOOL_TYPE:     return 1;
            }

            return 0;
        }

        static GLenum getOpenGLTypeFromShaderDataType(ShaderDataType type) {
            switch(type) {
                case ShaderDataType::FLOAT_TYPE: return GL_FLOAT;
                case ShaderDataType::FLOAT2_TYPE: return GL_FLOAT;
                case ShaderDataType::FLOAT3_TYPE: return GL_FLOAT;
                case ShaderDataType::FLOAT4_TYPE: return GL_FLOAT;

                case ShaderDataType::MAT3_TYPE: return GL_FLOAT;
                case ShaderDataType::MAT4_TYPE: return GL_FLOAT;

                case ShaderDataType::INT_TYPE: return GL_INT;
                case ShaderDataType::INT2_TYPE: return GL_INT;
                case ShaderDataType::INT3_TYPE: return GL_INT;
                case ShaderDataType::INT4_TYPE: return GL_INT;
                case ShaderDataType::BOOL_TYPE: return GL_BOOL;
            }

            return GL_NONE;
        }
    };

    class BufferLayout {
    public:
        explicit BufferLayout(std::initializer_list<BufferElement>&& items)
                :    m_Stride{ 0 }, m_Items(std::forward<std::initializer_list<BufferElement>>(items))
        {
            computeOffsetAndStride();
        }

        auto getElements() const -> const std::vector<BufferElement>& { return m_Items; }
        auto getStride() const { return m_Stride; }

        auto begin() -> std::vector<BufferElement>::iterator { return m_Items.begin(); }
        auto end() -> std::vector<BufferElement>::iterator { return m_Items.end(); }

        auto begin() const -> std::vector<BufferElement>::const_iterator { return m_Items.begin(); }
        auto end() const -> std::vector<BufferElement>::const_iterator { return m_Items.end(); }

        auto rbegin() -> std::vector<BufferElement>::reverse_iterator { return m_Items.rbegin(); }
        auto rend() -> std::vector<BufferElement>::reverse_iterator { return m_Items.rend(); }

        auto rbegin() const -> std::vector<BufferElement>::const_reverse_iterator { return m_Items.rbegin(); }
        auto rend() const -> std::vector<BufferElement>::const_reverse_iterator { return m_Items.rend(); }
    private:
        // Helpers
        auto computeOffsetAndStride() -> void {
            std::uint32_t offset{ 0 };
            for (auto& item : m_Items) {
                item.setOffset(offset);
                offset += item.getSize();
                m_Stride += item.getSize();
            }
        }

        std::uint32_t m_Stride{};
        std::vector<BufferElement> m_Items{};
    };

    /**
     * Default layout. It assumes all the data in a vertex buffer are just positions
     * */
     inline static BufferLayout s_DefaultLayout{ {ShaderDataType::FLOAT3_TYPE, "Position_Attribute"} };

    /**
     * General interface for Vertex buffers
     * */
    class VertexBuffer {
    public:
        explicit VertexBuffer() = default;
        VertexBuffer(const std::vector<float>& vertices, const BufferLayout& bufferLayout = s_DefaultLayout, GLenum usage = GL_STATIC_DRAW);
        VertexBuffer(VertexBuffer && other) noexcept;

        auto getId() const -> std::uint32_t { return m_Id; }
        auto getSize() const -> std::size_t { return m_Size; }
        auto isEmpty() const -> bool { return m_Size == 0; }
        // assumes the data is in float
        auto getCount() const -> std::size_t { return m_Size / m_Layout.getStride();  }

        auto operator=(VertexBuffer && other) noexcept -> VertexBuffer&;

        auto bind() const -> void { glBindBuffer(GL_ARRAY_BUFFER, getId()); }
        auto unbind() const -> void { glBindBuffer(GL_ARRAY_BUFFER, 0); }

        auto load(const std::vector<float>& vertices, GLenum usage = GL_STATIC_DRAW) -> void;

        auto setBufferLayout(const BufferLayout& layout) -> void { m_Layout = layout; }
        auto getBufferLayout() const -> const BufferLayout& { return m_Layout; }

        ~VertexBuffer() { glDeleteBuffers(1, &m_Id); }
    private:
        // Forbidden operations
        VertexBuffer(const VertexBuffer & other) = delete;
        auto operator=(const VertexBuffer & other) -> VertexBuffer & = delete;
    private:
        std::uint32_t m_Id{};
        std::uint32_t m_Size{};
        BufferLayout m_Layout{};
        bool m_ValidId{};
    };
}

#endif	// END VBO_HH