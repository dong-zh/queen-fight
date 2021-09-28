#version 330 core

in vec2 tc;
out vec4 fs_color;

uniform sampler2D car0Texture;

void main() {
    vec4 texColour = texture(car0Texture, tc);
    // if (fs_color.a < 0.1) {
    //     discard;
    // }
    fs_color = texColour;
}
