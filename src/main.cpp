// C++17 doesn't have std::pi
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#include <cstdlib>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <chicken3421/chicken3421.hpp>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace helpers
{
    GLuint make_texture(const chicken3421::image_t &tex_img) {
        GLint format = tex_img.n_channels == 3 ? GL_RGB : GL_RGBA; // assuming that if not 3 channels then 4

        GLuint tex;
        glGenTextures(1, &tex);

        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, format, tex_img.width, tex_img.height, 0, format, GL_UNSIGNED_BYTE, tex_img.data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

        glBindTexture(GL_TEXTURE_2D, 0);

        return tex;
    }
} // namespace helpers

namespace shapes
{
    struct vertex_t {
        glm::vec4 position;
        glm::vec2 textureCoordinates;
    };
    struct rect_t {
        GLuint vao;
        GLuint vbo;
        std::vector<vertex_t> vertices;
    };


    // TODO: Modify this function so that texture coordinates are made with the rest of the mesh
    rect_t make_rect(float xmin, float ymin, float xmax, float ymax) {
        std::vector<vertex_t> verts = {
            {{ xmax, ymax, 0, 1 }, {1, 1}}, // top-right
            {{ xmax, ymin, 0, 1 }, {1, 0}}, // bottom-right
            {{ xmin, ymin, 0, 1 }, {0, 0}}, // bottom-left

            {{ xmax, ymax, 0, 1 }, {1, 1}}, // top-right
            {{ xmin, ymin, 0, 1 }, {0, 0}}, // bottom-left
            {{ xmin, ymax, 0, 1 }, {0, 1}}, // top-left
        };

        GLuint vao;
        glGenVertexArrays(1, &vao);

        GLuint vbo;
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, (GLintptr)(sizeof(vertex_t) * verts.size()), verts.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void *)(0 + offsetof(vertex_t, position)));
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void *)(0 + offsetof(vertex_t, textureCoordinates)));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        return rect_t{vao, vbo, verts};
    }

    struct circle_t {
        GLuint vao = 0;
        GLuint vbo = 0;
        GLsizeiptr n_verts = 0;

        std::vector<glm::vec4> positions;
        std::vector<glm::vec2> tcs;

        // TODO: Add transformational stuff here
        glm::mat4 translate = glm::mat4{1};
        glm::mat4 scale = glm::mat4{1};
        glm::mat4 rotate = glm::mat4{1};
    };

    /**
     *
     * A Circle is drawn as a triangle fan.
     * The first point is the center, and every point thereafter is on the perimeter of the circle.
     *
     * @param cx: center's x coordinate
     * @param cy: center's y coordinate
     * @param r: radius
     * @param detail: how many sides the circle approximation should have
     * @return: a circle mesh
     */
    circle_t make_circle(float cx, float cy, float r, int detail = 32) {
        circle_t c;

        c.positions.emplace_back(cx, cy, 0, 1);
        c.tcs.emplace_back(0.5f, 0.5f);
        c.n_verts++;

        float slice = 2 * M_PI/detail;
        for (int i = 0; i <= detail; ++i) {
            float theta = i * slice;
            float x = cx + r * std::cos(theta);
            float y = cy + r * std::sin(theta);

            float tx = (x/r + 1) * 0.5f;
            float ty = (y/r + 1) * 0.5f;

            c.positions.emplace_back(x, y, 0, 1);
            c.tcs.emplace_back(tx, ty);
            c.n_verts++;
        }

        glGenVertexArrays(1, &c.vao);
        glGenBuffers(1, &c.vbo);

        glBindVertexArray(c.vao);
        glBindBuffer(GL_ARRAY_BUFFER, c.vbo);

        glBufferData(GL_ARRAY_BUFFER, c.n_verts * (sizeof(glm::vec4) + sizeof(glm::vec2)), nullptr, GL_STATIC_DRAW);

        glBufferSubData(GL_ARRAY_BUFFER, 0, c.n_verts * sizeof(glm::vec4), c.positions.data());
        glBufferSubData(GL_ARRAY_BUFFER, c.n_verts * sizeof(glm::vec4), c.n_verts * sizeof(glm::vec2), c.tcs.data());

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)(c.n_verts * sizeof(glm::vec4)));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        return c;
    }
}


int main() {
    GLFWwindow *win = chicken3421::make_opengl_window(720, 720, "Assignment 1");
    std::cout << glGetString(GL_VERSION) << '\n';
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSwapInterval(1);

    // Make the shaders
    GLuint vertexShader = chicken3421::make_shader("res/shaders/vert.vert", GL_VERTEX_SHADER);
    GLuint fragShader = chicken3421::make_shader("res/shaders/frag.frag", GL_FRAGMENT_SHADER);

    GLuint program = chicken3421::make_program(vertexShader, fragShader);

    // car0 texture
    chicken3421::image_t car0Image = chicken3421::load_image("res/img/car0.png");
    auto car0ImageFormat = car0Image.n_channels == 3 ? GL_RGB : GL_RGBA;
    std::cout << car0Image.n_channels << " channels\n";
    GLuint car0Texture = chicken3421::make_texture();
    glBindTexture(GL_TEXTURE_2D, car0Texture);
    glTexImage2D(GL_TEXTURE_2D, 0, car0ImageFormat, car0Image.width, car0Image.height, 0, car0ImageFormat, GL_UNSIGNED_BYTE, car0Image.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);    // unbind
    auto location = glGetUniformLocation(program, "car0Texture");
    //

    shapes::rect_t rectange = shapes::make_rect(-.5, -.5, .5, .5);
    // glEnable( GL_BLEND );
    // glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glUseProgram(program);
    while (!glfwWindowShouldClose(win)) {
        using namespace std::chrono;
        long long now = time_point_cast<milliseconds>(system_clock::now()).time_since_epoch().count();

        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 1, 1);

        glBindTexture(GL_TEXTURE_2D, car0Texture);
        glUniform1i(location, 0);
        glBindVertexArray(rectange.vao);
        glBindBuffer(GL_ARRAY_BUFFER, rectange.vbo);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 6);






        // glClear(GL_COLOR_BUFFER_BIT);
        // glClearColor(std::cos(now/1000), std::sin(now/1000), std::cos(now/1000), 1);

        glfwSwapBuffers(win);
    }


    return EXIT_SUCCESS;
}
