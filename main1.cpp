
#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "shader.h"
#include "camera.h"

//bool readGLSLSources();
void printEnvInfo();
GLuint loadTextureFromFile(const char *);
void processInput(GLFWwindow *);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
float absf(float);

// square
float vertices[] = {
        //     ---- position ----       ---- color ----     ---- texture coordinate ----
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

// cube
float vertices_2[] = {
        //   ---- position ----           ---- texture coordinate ----
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
};

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

float cameraSpeed = 0.2f;
Camera *camera = nullptr;

bool firstMouse = true;
float deltaTime = 0.0f, lastTime = 0.0f;
float lastCursorX = 400, lastCursorY = 300;

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

    glEnable(GL_DEPTH_TEST);
    int screenWidth = 800;
    int screenHeight = 600;
    glViewport(0, 0, screenWidth, screenHeight);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // Initialise the shader program.
    Shader ourShader1("glsl/vertex_shader_1.glsl", "glsl/fragment_shader_1.glsl");
    Shader ourShader2("glsl/vertex_shader_2.glsl", "glsl/fragment_shader_2.glsl");

    // Configure and load textures.
    GLuint texture1, texture2;
    texture1 = loadTextureFromFile("texture/container.jpg");
    if (texture1 == -1)
        return -1;
    stbi_set_flip_vertically_on_load(true);
    texture2 = loadTextureFromFile("texture/awesomeface.jpg");
    if (texture2 == -1)
        return -1;
    stbi_set_flip_vertically_on_load(false);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    GLuint VAO1, VAO2;
    GLuint VBO1, VBO2;
    GLuint EBO1;

    {
        // 0. Initialise the vertex array object (VAO), the vertex buffer object (VBO)
        //    and the element buffer object (EBO).
        glGenVertexArrays(1, &VAO1);
        glGenBuffers(1, &VBO1);
        glGenBuffers(1, &EBO1);
        // 1. Bind the vertex array object (VAO).
        glBindVertexArray(VAO1);
        // 2. Copy the vertex array into the buffer for OpenGL to use.
        glBindBuffer(GL_ARRAY_BUFFER, VBO1);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // 3. Copy the element array into the buffer for OpenGL to use.
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        // 4. Set the vertex attribute pointer.
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *) 0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        // texture1 coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *) (6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        // 5. Unbind the VAO to avoid mistaken settings.
        glBindVertexArray(0);
    }

    {
        // 0. Initialise the vertex array object (VAO) and the vertex buffer object (VBO).
        glGenVertexArrays(1, &VAO2);
        glGenBuffers(1, &VBO2);
        // 1. Bind the vertex array object (VAO).
        glBindVertexArray(VAO2);
        // 2. Copy the vertex array into the buffer for OpenGL to use.
        glBindBuffer(GL_ARRAY_BUFFER, VBO2);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_2), vertices_2, GL_STATIC_DRAW);
        // 3. Set the vertex attribute pointer.
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *) 0);
        glEnableVertexAttribArray(0);
        // texture1 coordinate attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        // 4. Unbind the VAO to avoid mistaken settings.
        glBindVertexArray(0);
    }

    camera = new Camera;

    while(!glfwWindowShouldClose(window)) {
        // Input
        processInput(window);

        // Render
        std::cout << "pitch: " << camera->pitch << ", yaw: " << camera->yaw << std::endl;
        std::cout << "x: " << camera->pos.x << ", y: " << camera->pos.y << ", z: " << camera->pos.z << std::endl;
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view(1.0f); // convert to view space
        view = camera->getViewMatrix();
        glm::mat4 projection(1.0f); // convert to clip space
        projection = glm::perspective(glm::radians(45.0f), (float) screenWidth / (float) screenHeight, 0.1f, 100.0f);

        {
            ourShader1.use();
            ourShader1.setInt("texture1", 0);
            ourShader1.setInt("texture2", 1);

            glm::mat4 trans(1.0f);
            trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
            trans = glm::rotate(trans, (float) glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
            ourShader1.setMat4("transform", trans);
            glm::mat4 model(1.0f); // convert to world space
            model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            ourShader1.setMat4("model", model);
            ourShader1.setMat4("view", view);
            ourShader1.setMat4("projection", projection);

            glBindVertexArray(VAO1);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }

        for (unsigned int i = 0; i < 10; i++) {
            ourShader2.use();
            ourShader2.setInt("texture1", 0);
            ourShader2.setInt("texture2", 1);

            glm::mat4 model(1.0f); // convert to world space
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
//            model = glm::rotate(model, (float) glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
            ourShader2.setMat4("model", model);
            ourShader2.setMat4("view", view);
            ourShader2.setMat4("projection", projection);

            glBindVertexArray(VAO2);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        // Check, invoke events and swap buffers.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete camera;

    glfwTerminate();
    return 0;
}

void printEnvInfo()
{
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "GL_MAX_VERTEX_ATTRIBS: " << nrAttributes << std::endl;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &nrAttributes);
    std::cout << "GL_MAX_TEXTURE_IMAGE_UNITS: " << nrAttributes << std::endl;
}

GLuint loadTextureFromFile(const char *texturePath)
{
    int width, height, nrChannels;
    unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    } else {
        std::cout << "Failed to load texture: " << texturePath << std::endl;
        return -1;
    }
    return texture;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        return;
    }

    float currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    float fps = 1.0f / deltaTime;
    std::cout << "FPS: " << fps << std::endl;

    cameraSpeed = 5.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->moveFront(cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->moveBack(cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->moveLeft(cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->moveRight(cameraSpeed);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse) {
        lastCursorX = xpos;
        lastCursorY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastCursorX;
    // Note that the order gets reversed here since Y coordinate begins from bottom
    // within OpenGL's coordinate system.
    float yoffset = lastCursorY - ypos;
    lastCursorX = xpos;
    lastCursorY = ypos;

    float sensitivity = 0.2f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera->moveYaw(xoffset);
    camera->movePitch(yoffset);
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
