#version 330 core

in vec2 tc;
out vec4 fs_color;

uniform sampler2D tex;

void main() {
    vec4 texColour = texture(tex, tc);
    // if (fs_color.a < 0.1) {
    //     discard;
    // }
    fs_color = texColour;
}
