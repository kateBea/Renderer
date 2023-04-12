#version 330 core

out vec4 fragColor;
out vec3 color;
in vec2 textCoord;

uniform sampler2D tex;

void main() {
    color = vec3(0.4f, 0.2f, 0.3f);
    fragColor = texture(tex, textCoord);
}