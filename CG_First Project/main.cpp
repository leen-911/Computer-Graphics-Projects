#include <iostream>
using namespace std;

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// ---------------- SHADERS ----------------

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"uniform vec2 offset;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x + offset.x, aPos.y + offset.y, aPos.z, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"in vec3 ourColor;\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColorUniform;\n"
"void main()\n"
"{\n"
"   FragColor = ourColorUniform;\n"
"}\n\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Graphics Project", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glewExperimental = GL_TRUE;
    glewInit();

    // -------- SHADER PROGRAM --------
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    int offsetLoc = glGetUniformLocation(shaderProgram, "offset");
    int colorLoc = glGetUniformLocation(shaderProgram, "ourColorUniform");

    // ---------------- SHAPES WITH Z ----------------

    // السماء (خلف كل شيء)
    float skyVertices[] = {
        -1.0f,  1.0f, -0.9f, 0,0,1,
         1.0f,  1.0f, -0.9f, 0,0,1,
         1.0f, -1.0f, -0.9f, 0,0,1,

        -1.0f,  1.0f, -0.9f, 0,0,1,
         1.0f, -1.0f, -0.9f, 0,0,1,
        -1.0f, -1.0f, -0.9f, 0,0,1
    };

    // الطريق
    float roadVertices[] = {
        -1.0f, -0.3f, -0.5f, 0.2f,0.2f,0.2f,
         1.0f, -0.3f, -0.5f, 0.2f,0.2f,0.2f,
         1.0f, -1.0f, -0.5f, 0.2f,0.2f,0.2f,

        -1.0f, -0.3f, -0.5f, 0.2f,0.2f,0.2f,
         1.0f, -1.0f, -0.5f, 0.2f,0.2f,0.2f,
        -1.0f, -1.0f, -0.5f, 0.2f,0.2f,0.2f
    };

    // السيارة
    float carVertices[] = {
        -0.2f, -0.29f, 0.0f, 1,0,0,
         0.2f, -0.29f, 0.0f, 1,0,0,
         0.2f, -0.1f, 0.0f, 1,0,0,

        -0.2f, -0.29f, 0.0f, 1,0,0,
         0.2f, -0.1f, 0.0f, 1,0,0,
        -0.2f, -0.1f, 0.0f, 1,0,0
    };

    // العجلة الأمامية
    float wheel1Vertices[] = {
        -0.15f, -0.35f, 0.0f, 0,0,0,
        -0.05f, -0.35f, 0.0f, 0,0,0,
        -0.05f, -0.25f, 0.0f, 0,0,0,

        -0.15f, -0.35f, 0.0f, 0,0,0,
        -0.05f, -0.25f, 0.0f, 0,0,0,
        -0.15f, -0.25f, 0.0f, 0,0,0
    };

    // العجلة الخلفية
    float wheel2Vertices[] = {
         0.05f, -0.35f, 0.0f, 0,0,0,
         0.15f, -0.35f, 0.0f, 0,0,0,
         0.15f, -0.25f, 0.0f, 0,0,0,

         0.05f, -0.35f, 0.0f, 0,0,0,
         0.15f, -0.25f, 0.0f, 0,0,0,
         0.05f, -0.25f, 0.0f, 0,0,0
    };

    // الشمس (أمام كل شيء)
    float sunVertices[] = {
        -0.05f, 0.8f, 0.2f, 1,1,0,
         0.05f, 0.8f, 0.2f, 1,1,0,
         0.05f, 0.9f, 0.2f, 1,1,0,

        -0.05f, 0.8f, 0.2f, 1,1,0,
         0.05f, 0.9f, 0.2f, 1,1,0,
        -0.05f, 0.9f, 0.2f, 1,1,0
    };

    // القمر
    float moonVertices[] = {
        -0.05f, 0.8f, 0.2f, 0.8f,0.8f,1,
         0.05f, 0.8f, 0.2f, 0.8f,0.8f,1,
         0.05f, 0.9f, 0.2f, 0.8f,0.8f,1,

        -0.05f, 0.8f, 0.2f, 0.8f,0.8f,1,
         0.05f, 0.9f, 0.2f, 0.8f,0.8f,1,
        -0.05f, 0.9f, 0.2f, 0.8f,0.8f,1
    };

    // -------- VAO/VBO SETUP --------
    auto setupVAO = [&](float* vertices, int size, GLuint& VAO, GLuint& VBO)
        {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
        };

    GLuint skyVAO, skyVBO;
    setupVAO(skyVertices, sizeof(skyVertices), skyVAO, skyVBO);

    GLuint roadVAO, roadVBO;
    setupVAO(roadVertices, sizeof(roadVertices), roadVAO, roadVBO);

    GLuint carVAO, carVBO;
    setupVAO(carVertices, sizeof(carVertices), carVAO, carVBO);

    GLuint wheel1VAO, wheel1VBO;
    setupVAO(wheel1Vertices, sizeof(wheel1Vertices), wheel1VAO, wheel1VBO);

    GLuint wheel2VAO, wheel2VBO;
    setupVAO(wheel2Vertices, sizeof(wheel2Vertices), wheel2VAO, wheel2VBO);

    GLuint sunVAO, sunVBO;
    setupVAO(sunVertices, sizeof(sunVertices), sunVAO, sunVBO);

    GLuint moonVAO, moonVBO;
    setupVAO(moonVertices, sizeof(moonVertices), moonVAO, moonVBO);

    // ---------------- MAIN LOOP ----------------

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        float timeValue = glfwGetTime();
        float cycle = fmod(timeValue, 20.0f);

        // -------- تغيير لون السماء حسب الوقت --------
        float r, g, b;

        if (cycle < 5.0f) { r = 0.4; g = 0.7; b = 1.0; }        // نهار
        else if (cycle < 10.0f) { r = 1.0; g = 0.6; b = 0.2; } // غروب
        else if (cycle < 15.0f) { r = 0.05; g = 0.05; b = 0.1; } // ليل
        else { r = 0.6; g = 0.4; b = 0.9; }                    // فجر

        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // السماء
        glUniform4f(colorLoc, r, g, b, 1.0f);
        glUniform2f(offsetLoc, 0, 0);
        glBindVertexArray(skyVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // الطريق
        glUniform4f(colorLoc, 0.2f, 0.2f, 0.2f, 1.0f);
        glBindVertexArray(roadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // السيارة
        float carX = fmod(timeValue * 0.3f, 2.4f) - 1.2f;
        glUniform4f(colorLoc, 1, 0, 0, 1);
        glUniform2f(offsetLoc, carX, 0);
        glBindVertexArray(carVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // العجلات
        glUniform4f(colorLoc, 0, 0, 0, 1);
        glBindVertexArray(wheel1VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(wheel2VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // الشمس والقمر
        if (cycle < 10.0f)
        {
            float t = cycle / 10.0f;
            float sunX = -1.2f + 2.4f * t;

            glUniform4f(colorLoc, 1, 1, 0, 1);
            glUniform2f(offsetLoc, sunX, 0);
            glBindVertexArray(sunVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        else
        {
            float t = (cycle - 10.0f) / 10.0f;
            float moonX = 1.2f - 2.4f * t;

            glUniform4f(colorLoc, 0.8f, 0.8f, 1, 1);
            glUniform2f(offsetLoc, moonX, 0);
            glBindVertexArray(moonVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
