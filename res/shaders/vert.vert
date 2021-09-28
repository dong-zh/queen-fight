#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 textureCoordinates;

out vec2 tc;

void main() {
    gl_Position = position;
    tc = textureCoordinates;
}
