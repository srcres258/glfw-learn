
#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

//bool readGLSLSources();
void printEnvInfo();
void processInput(GLFWwindow *);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
float absf(float);

static int main0();
static int main1();

//GLfloat vertices[] = {
//        // the 1st triangle
//        -0.5f, -0.5f, 0.0f,   // right-top
//        0.5f, -0.5f, 0.0f,    // right-bottom
//        0.0f,  0.5f, 0.0f,    // left-top
//        // the 2nd triangle
//        0.5f, -0.5f, 0.0f,   // right-bottom
//        -0.5f, -0.5f, 0.0f, // left-bottom
//        -0.5f, 0.5f, 0.0f   // left-top
//};
static float vertices[] = {
        //|          position          |           color               |
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,     // right-top
        0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   // right-bottom
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f,   // left-bottom
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f  // left-top
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
    return main1();
}

static int main0()
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

//    if (readGLSLSources()) {
//        std::cout << "vertexShaderSource:" << std::endl;
//        std::cout << vertexShaderSource <<std::endl;
//        std::cout << "fragmentShaderSource:" << std::endl;
//        std::cout << fragmentShaderSource <<std::endl;
//    } else {
//        std::cout << "Failed to read GLSL sources!" << std::endl;
//        return -1;
//    }
    std::cout << "vertexShaderSource:" << std::endl;
    std::cout << vertexShaderSource <<std::endl;
    std::cout << "fragmentShaderSource:" << std::endl;
    std::cout << fragmentShaderSource <<std::endl;

    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, reinterpret_cast<const GLchar *const *>(&vertexShaderSource), nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, reinterpret_cast<const GLchar *const *>(&fragmentShaderSource), nullptr);
    glCompileShader(fragmentShader);

    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog), nullptr, infoLog);
        std::cout << "Error occurred during linking program! infoLog: " << infoLog << std::endl;
        return -1;
    }
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *) 0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // 5. Unbind the VAO to avoid mistaken settings.
    glBindVertexArray(0);

    while(!glfwWindowShouldClose(window)) {
        // Input
        processInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // 6. Render objects.
//        float timeValue = glfwGetTime();
//        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
//        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUseProgram(shaderProgram);
//        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        glBindVertexArray(VAO);
//        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        // Check, invoke events and swap buffers.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

static int main1()
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
    Shader ourShader("glsl/vertex_shader.glsl", "glsl/fragment_shader.glsl");

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *) 0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // 5. Unbind the VAO to avoid mistaken settings.
    glBindVertexArray(0);

    while(!glfwWindowShouldClose(window)) {
        // Input
        processInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ourShader.use();
        float xOffset = 0.5f * sin(glfwGetTime() + M_PI / 2);
        float yOffset = 0.5f * sin(glfwGetTime());
        float rOffset = absf(1.0f * sin(glfwGetTime() + M_PI / 3));
        float gOffset = absf(1.0f * sin(glfwGetTime() + M_PI / 3 * 2));
        float bOffset = absf(1.0f * sin(glfwGetTime() + M_PI));
        std::cout << "xOffset: " << xOffset << ", yOffset: " << yOffset << ", rOffset: "
                << rOffset << ", gOffset: " << gOffset << ", bOffset: " << bOffset << std::endl;
        ourShader.setFloat("xOffset", xOffset);
        ourShader.setFloat("yOffset", yOffset);
        ourShader.setFloat("rOffset", rOffset);
        ourShader.setFloat("gOffset", gOffset);
        ourShader.setFloat("bOffset", bOffset);
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
