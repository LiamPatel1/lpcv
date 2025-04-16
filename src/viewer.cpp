#include "lpcv.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void lpcv::display(const lpcv::Image& image) {

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return;
    }

    // Configure OpenGL context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(
        700,
        600,
        image.getName().c_str(),
        nullptr, nullptr
    );

    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return;
    }

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwSetWindowPos(
        window,
        (mode->width - 800) / 2,  // Center horizontally
        (mode->height - 600) / 2   // Center vertically
    );

    
    float imageAspect = (float)image.getWidth() / (float)image.getHeight();

    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load OpenGL functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        glfwTerminate();
        return;
    }

    // Create and configure texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Handle bit depth conversion
    const auto& data = image.getData();
    const int depth = image.getBitDepth();
    std::vector<unsigned char> convertedData;
    const unsigned char* finalData = data.data();

    if (depth != 8) {
        const size_t pixelCount = image.getWidth() * image.getHeight() * 4;
        convertedData.resize(pixelCount);
        const size_t bytesPerChannel = depth / 8;
        const size_t totalElements = data.size() / bytesPerChannel;

        for (size_t i = 0; i < totalElements; ++i) {
            uint64_t value = 0;
            const size_t offset = i * bytesPerChannel;

            // Big-endian conversion
            for (int b = 0; b < bytesPerChannel; ++b) {
                value |= static_cast<uint64_t>(data[offset + b])
                    << (8 * (bytesPerChannel - 1 - b));
            }

            // Normalize to 8-bit
            convertedData[i] = static_cast<unsigned char>(
                (value * 255) / ((1ULL << depth) - 1)
                );
        }
        finalData = convertedData.data();
    }

    // Upload texture data
    int glType;
    if (image.getColourSpace() == lpcv::RGBA) glType = GL_RGBA;
    else if (image.getColourSpace() == lpcv::RGB) glType = GL_RGB;
    else {
        std::cerr << "Type not supported";
        return;
    }
    

    glTexImage2D(GL_TEXTURE_2D, 0, glType,
        image.getWidth(), image.getHeight(),
        0, glType, GL_UNSIGNED_BYTE,
        finalData);

    // Shader program
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec2 aTexCoord;
        out vec2 TexCoord;
        void main() {
            gl_Position = vec4(aPos, 0.0, 1.0);
            TexCoord = aTexCoord;
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec2 TexCoord;
        out vec4 FragColor;
        uniform sampler2D uTexture;
        void main() {
            FragColor = texture(uTexture, TexCoord);
        }
    )";

    // Compile and link shaders
    GLuint program = glCreateProgram();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Vertex data
 
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Configure VAO
    glBindVertexArray(VAO);

    // Configure VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Configure EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Main rendering loop
    while (!glfwWindowShouldClose(window)) {


        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        float windowAspect = (float)windowWidth / (float)windowHeight;
        float scaleX, scaleY;
        if (imageAspect > windowAspect) {
            // Image is wider than window → scale vertically
            scaleX = 1.0f;
            scaleY = windowAspect / imageAspect;
        }
        else {
            // Image is taller than window → scale horizontally
            scaleX = imageAspect / windowAspect;
            scaleY = 1.0f;
        }
        float vertices[] = {
            // Positions          // Texture Coords
             scaleX,  scaleY,     1.0f, 0.0f, // Top-right
             scaleX, -scaleY,     1.0f, 1.0f, // Bottom-right
            -scaleX, -scaleY,     0.0f, 1.0f, // Bottom-left
            -scaleX,  scaleY,     0.0f, 0.0f  // Top-left
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);




        // Clear screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw textured quad
        glUseProgram(program);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(program);
    glDeleteTextures(1, &texture);
    glfwTerminate();
}