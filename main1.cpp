
#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"
#include "shader.h"

//bool readGLSLSources();
void printEnvInfo();
void processInput(GLFWwindow *);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
float absf(float);

float vertices[] = {
        //     ---- position ----       ---- color ----     - texture coordinate -
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // right-top
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // right-bottom
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // left-bottom
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // left-top
};

static unsigned int indices[] = {
        // Note that indexes begin from 0
        // The indexes of this example (0,1,2,3) are exactly those of vertices,
        // so as to use these indexes representing vertexes to form a rectangle.

        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
};

//GLchar vertexShaderSource[10240];
//GLchar fragmentShaderSource[10240];
static const char *vertexShaderSource = "#version 330 core\n"
                                        "layout (location = 0) in vec3 aPos;\n"
                                        "layout (location = 1) in vec3 aColor;\n"
                                        "\n"
                                        "out vec3 ourColor;\n"
                                        "\n"
                                        "void main()\n"
                                        "{\n"
                                        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                        "   ourColor = aColor;\n"
                                        "}\n\0";
static const char *fragmentShaderSource = "#version 330 core\n"
                                          "out vec4 FragColor;\n"
                                          "in vec3 ourColor;\n"
                                          "\n"
                                          "void main()\n"
                                          "{\n"
                                          "   FragColor = vec4(ourColor, 1.0);\n"
                                          "}\n\0";
static GLchar infoLog[10240];

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD." << std::endl;
        return -1;
    }

    printEnvInfo();

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialise the shader program.
    Shader ourShader("glsl/vertex_shader_1.glsl", "glsl/fragment_shader_1.glsl");

    // Configure and load textures
    int width, height, nrChannels;
    unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    } else {
        std::cout << "Failed to load textures!" << std::endl;
        return -1;
    }

    // 0. Initialise the vertex array object (VAO), the vertex buffer object (VBO)
    //    and the element buffer object (EBO).
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    GLuint VBO;
    glGenBuffers(1, &VBO);
    GLuint EBO;
    glGenBuffers(1, &EBO);
    // 1. Bind the vertex array object (VAO).
    glBindVertexArray(VAO);
    // 2. Copy the vertex array into the buffer for OpenGL to use.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 2. Copy the element array into the buffer for OpenGL to use.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 4. Set the vertex attribute pointer.
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *) 0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // 5. Unbind the VAO to avoid mistaken settings.
    glBindVertexArray(0);

    while(!glfwWindowShouldClose(window)) {
        // Input
        processInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ourShader.use();
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        // Check, invoke events and swap buffers.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

//bool readGLSLSources()
//{
//    std::ifstream vertexShaderSourceF("glsl/vertex_shader.glsl", std::ios::in);
//    if (!vertexShaderSourceF)
//        return false;
//    vertexShaderSourceF.read(vertexShaderSource, sizeof(vertexShaderSource));
//    vertexShaderSourceF.close();
//
//    std::ifstream fragmentShaderSourceF("glsl/fragment_shader.glsl", std::ios::in);
//    if (!fragmentShaderSourceF)
//        return false;
//    fragmentShaderSourceF.read(fragmentShaderSource, sizeof(fragmentShaderSource));
//    fragmentShaderSourceF.close();
//
//    return true;
//}

void printEnvInfo()
{
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

float absf(float absf)
{
    if (absf < 0)
        return -absf;
    else
        return absf;
}
