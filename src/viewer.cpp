#include "lpcv.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void lpcv::display(const lpcv::Image& originalImage, std::string title) {

    lpcv::Image image = originalImage;

    if (title == "_") {
        title = image.getName();
    }


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
        title.c_str(),
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



    int glFormat;
    int glInternalFormat;
    switch (image.getColourType()) {
    case RGB8:
        glFormat = GL_RGB;
        glInternalFormat = GL_RGB8;
        break;

    case RGBA8:
        glFormat = GL_RGBA;
        glInternalFormat = GL_RGBA8;
        break;
    case G8:
        image = image.expand_G_RGB__GA_RGBA();
        glFormat = GL_RGB;
        glInternalFormat = GL_RGB8;
        break;
    case GA8:
        image = image.expand_G_RGB__GA_RGBA();
        glFormat = GL_RGBA;
        glInternalFormat = GL_RGBA8;
        break;
    case GF:
        image = image.expand_G_RGB__GA_RGBA();
        image = image.toUINT8();
        glFormat = GL_RGB;
        glInternalFormat = GL_RGB8;
        break;
    case GAF:
        image = image.expand_G_RGB__GA_RGBA();
        image = image.toUINT8();
        glFormat = GL_RGBA;
        glInternalFormat = GL_RGBA8;
        break;

    case RGBAF:
        image = image.toUINT8();
        glFormat = GL_RGBA;
        glInternalFormat = GL_RGBA8;
        break;

    default:
        throw std::invalid_argument("Unsupported colour type");
    }

    const unsigned char* imageData = image.getData().data();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // SET BYTE ALIGNMENT OF EACH ROW TO 1 BYTE
    glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat,
        image.getWidth(), image.getHeight(),
        0, glFormat, GL_UNSIGNED_BYTE,
        imageData
    );

    int e = glGetError();
    if (e != GL_NO_ERROR) {
        std::cerr << "Error glTexImage2D :" << (GLenum)e << "\n";
        return;
    }

   
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
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex shader compilation failed:\n" << infoLog << std::endl;
    }

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation failed:\n" << infoLog << std::endl;
    }


    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Program linking failed:\n" << infoLog << std::endl;
    }


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

 
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3  
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

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

             scaleX,  scaleY,     1.0f, 0.0f, // Top-right
             scaleX, -scaleY,     1.0f, 1.0f, // Bottom-right
            -scaleX, -scaleY,     0.0f, 1.0f, // Bottom-left
            -scaleX,  scaleY,     0.0f, 0.0f  // Top-left
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(program);
    glDeleteTextures(1, &texture);
    glfwTerminate();
}