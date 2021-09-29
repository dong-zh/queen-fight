#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 textureCoordinates;

uniform mat4 transformMatrix;

out vec2 tc;

void main() {
    gl_Position = transformMatrix * position;
    tc = textureCoordinates;
}
