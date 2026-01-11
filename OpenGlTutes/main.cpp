#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <string>

// Function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
std::string loadShaderSrc(const char* filename);

int main() {

    // 1️⃣ Variables for shader compilation error checking
    int success;
    char infoLog[512];

    // 2️⃣ Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL x.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 3️⃣ Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // 4️⃣ Load OpenGL function pointers using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    // 5️⃣ Set viewport size
    glViewport(0, 0, 800, 600);

    // 6️⃣ Set framebuffer size callback for window resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //========================
    // 7️⃣ SHADERS
    //========================

    // ---- Vertex Shader ----
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);              // Create shader object
    std::string vertexShaderSrc = loadShaderSrc("asset/vertex_core.glsl");    // Load GLSL source
    const GLchar* vertShader = vertexShaderSrc.c_str();                        // Convert to C-string
    glShaderSource(vertexShader, 1, &vertShader, NULL);                        // Give shader code to GPU
    glCompileShader(vertexShader);                                             // Compile shader

    // Check for compilation errors
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // ---- Fragment Shader ----
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);          // Create shader object
    std::string fragmentShaderSrc = loadShaderSrc("asset/fragment_core.glsl");// Load GLSL source
    const GLchar* fragShader = fragmentShaderSrc.c_str();                      // Convert to C-string
    glShaderSource(fragmentShader, 1, &fragShader, NULL);                      // Give shader code to GPU
    glCompileShader(fragmentShader);                                           // Compile shader

    // Check for compilation errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // ---- Shader Program ----
    unsigned int shaderProgram = glCreateProgram();           // Create program object
    glAttachShader(shaderProgram, vertexShader);              // Attach vertex shader
    glAttachShader(shaderProgram, fragmentShader);            // Attach fragment shader
    glLinkProgram(shaderProgram);                             // Link program

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Shaders no longer needed after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //========================
    // 8️⃣ Vertex Data and Buffers
    //========================

    // Very simple stylized realistic dildo - vertices + indices
// Units are arbitrary - you can scale as you wish

    const float vertices[] = {
        // Format:   x,      y,      z

        // ===== Head (dome) - 9 vertices =====
         0.00f,  1.60f,  0.00f,   // 0  center top
         0.60f,  1.35f,  0.00f,   // 1
         0.85f,  1.00f,  0.00f,   // 2
         0.90f,  0.60f,  0.00f,   // 3  widest point of head

        -0.60f,  1.35f,  0.00f,   // 4   (symmetric left side)
        -0.85f,  1.00f,  0.00f,   // 5
        -0.90f,  0.60f,  0.00f,   // 6

         0.00f,  0.60f,  0.00f,   // 7  neck center (transition)

         // ===== Shaft =====
          0.75f,  0.60f,  0.00f,   // 8
          0.78f,  0.00f,  0.00f,   // 9
          0.75f, -0.60f,  0.00f,   // 10

         -0.75f,  0.60f,  0.00f,   // 11
         -0.78f,  0.00f,  0.00f,   // 12
         -0.75f, -0.60f,  0.00f,   // 13

         // ===== Balls / base =====
          0.00f, -0.90f,  0.00f,   // 14 center bottom
          1.10f, -1.10f,  0.00f,   // 15 right ball outer
          0.70f, -0.65f,  0.00f,   // 16 right ball connect

         -1.10f, -1.10f,  0.00f,   // 17 left ball outer
         -0.70f, -0.65f,  0.00f,   // 18 left ball connect
    };

    const unsigned int indices[] = {
        // Head - fan from center top
        0, 1, 2,
        0, 2, 3,
        0, 4, 5,
        0, 5, 6,
        // head sides connection
        3, 7, 2,
        6, 7, 5,

        // Shaft - two long quads (6 triangles)
        3, 8, 7,
        8, 7, 11,
        8, 9, 11,
        9, 11, 12,
        9, 10, 12,
        10, 12, 13,

        // Base transition
        10, 13, 16,
        13, 16, 18,
        16, 14, 18,

        // Right ball
        14, 16, 15,
        // Left ball
        14, 18, 17
    };

    // ---- Vertex Array Object (VAO) ----
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);   // Bind VAO first!

    // ---- Vertex Buffer Object (VBO) ----
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);                          // Bind VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Send vertex data to GPU


	//Element Buffer Object (EBO)
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
        GL_STATIC_DRAW);


    // ---- Vertex Attribute Pointer ----
    // Attribute 0 = position, 3 floats per vertex, tightly packed
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // Unbind VAO (optional)

    //========================
    // 9️⃣ Render Loop
    //========================
    while (!glfwWindowShouldClose(window)) {

        // Input
        processInput(window);

        // Clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw triangle
        glUseProgram(shaderProgram);    // Use shader program
        glBindVertexArray(VAO);         // Bind VAO

        // Inside render loop, before glDrawElements:
        float time = glfwGetTime();
        float scale = 0.55f + sin(time * 1.8f) * 0.04f; // gentle pulsing

        // Then (very dirty but funny for assignment):
        glUseProgram(shaderProgram);
        int scaleLoc = glGetUniformLocation(shaderProgram, "uScale");
        if (scaleLoc != -1)
            glUniform1f(scaleLoc, scale);


        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //========================
    // 10️⃣ Cleanup
    //========================
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

// Resize callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Process ESC key to close window
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Load shader from file
std::string loadShaderSrc(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Could not open file: " << filename << std::endl;
        return "";
    }
    std::stringstream buf;
    buf << file.rdbuf();
    return buf.str();
}
