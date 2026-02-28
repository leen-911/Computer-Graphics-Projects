#include <iostream>
#include <cmath>
using namespace std;

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
//SHADERS 

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
    // --- 3. تهيئة GLFW وإعداد النافذة ---
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // إصدار OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // النمط الحديث

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // خاص بأجهزة ماك
#endif

    // إنشاء كائن النافذة
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Computer Graphics_First Project", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // جعل هذه النافذة هي سياق الرسم الحالي
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // ربط دالة تغيير الحجم

    // --- 4. تهيئة GLEW ---
    glewExperimental = GL_TRUE; // تفعيل التقنيات الحديثة
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // --- 5. بناء وتجميع برنامج الشيدر (Shader Program) ---

    // ==========================================
    // [ تفعيل قدرات OpenGL المتقدمة
    // المكان: يجب أن تضاف هنا (بعد glewInit وقبل حلقة الرسم).
    // ==========================================
    glEnable(GL_DEPTH_TEST); // تفعيل اختبار العمق (لمنع تداخل الرسم)
    glEnable(GL_BLEND);      // تفعيل الدمج (للشفافية)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // معادلة الدمج
    glDepthFunc(GL_LESS); //لرسم الشيء اذا كان اقرب من الذي تحته وليس حسب ترتيب الرسم

    // أ. تجميع Vertex Shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // فحص الأخطاء
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // ب. تجميع Fragment Shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // فحص الأخطاء
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // ج. ربط الشيدرز في برنامج واحد (Shader Program)
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);


    // فحص أخطاء الربط
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }


    int offsetLoc = glGetUniformLocation(shaderProgram, "offset");
    int colorLoc = glGetUniformLocation(shaderProgram, "ourColorUniform");


    // ---------------- SHAPES WITH Z ----------------

    // السماء 
    float skyVertices[] = {
        -1.0f,  1.0f, 0.9f, 0,0,1,
         1.0f,  1.0f, 0.9f, 0,0,1,
         1.0f, -1.0f, 0.9f, 0,0,1,

        -1.0f,  1.0f, 0.9f, 0,0,1,
         1.0f, -1.0f, 0.9f, 0,0,1,
        -1.0f, -1.0f, 0.9f, 0,0,1
    };

    // الطريق
    float roadVertices[] = {
        -1.0f, -0.3f, 0.5f, 0.2f,0.2f,0.2f,
         1.0f, -0.3f, 0.5f, 0.2f,0.2f,0.2f,
         1.0f, -1.0f, 0.5f, 0.2f,0.2f,0.2f,

        -1.0f, -0.3f, 0.5f, 0.2f,0.2f,0.2f,
         1.0f, -1.0f, 0.5f, 0.2f,0.2f,0.2f,
        -1.0f, -1.0f, 0.5f, 0.2f,0.2f,0.2f
    };

    // السيارة
    float carVertices[] = {
        -0.2f, -0.29f, 0.0f, 1,0,0,
         0.2f, -0.29f, 0.0f, 1,0,0,
         0.2f, -0.1f,  0.0f, 1,0,0,

        -0.2f, -0.29f, 0.0f, 1,0,0,
         0.2f, -0.1f,  0.0f, 1,0,0,
        -0.2f, -0.1f,  0.0f, 1,0,0
    };

    //النافذة
    float windowVertices[] = {
        0.07f, -0.23f, -0.1f, 1,1,1,
        0.18f, -0.23f, -0.1f, 1,1,1,
        0.18f, -0.13f, -0.1f, 1,1,1,

        0.07f, -0.23f, -0.1f, 1,1,1,
        0.18f, -0.13f, -0.1f, 1,1,1,
        0.07f, -0.13f, -0.1f, 1,1,1
    };


    const int segments = 100;

    // العجلة الأمامية
    float wheel1Vertices[(segments + 2) * 6];

    float wheelRadius = 0.05f;
    float wheel1CenterX = -0.10f;   // مكانها تحت السيارة
    float wheelCenterY = -0.30f;
    float wheelZ = -0.22f;

    int w1Index = 0;

    // المركز
    wheel1Vertices[w1Index++] = wheel1CenterX;
    wheel1Vertices[w1Index++] = wheelCenterY;
    wheel1Vertices[w1Index++] = wheelZ;
    wheel1Vertices[w1Index++] = 0.0f;
    wheel1Vertices[w1Index++] = 0.0f;
    wheel1Vertices[w1Index++] = 0.0f;

    // محيط الدائرة
    for (int i = 0; i <= segments; i++)
    {
        float angle = 2.0f * 3.1415926f * i / segments;
        float x = wheel1CenterX + wheelRadius * cos(angle);
        float y = wheelCenterY + wheelRadius * sin(angle);

        wheel1Vertices[w1Index++] = x;
        wheel1Vertices[w1Index++] = y;
        wheel1Vertices[w1Index++] = wheelZ;
        wheel1Vertices[w1Index++] = 0.0f;
        wheel1Vertices[w1Index++] = 0.0f;
        wheel1Vertices[w1Index++] = 0.0f;
    }

    // العجلة الخلفية
    float wheel2Vertices[(segments + 2) * 6];

    float wheel2CenterX = 0.10f;

    int w2Index = 0;

    // المركز
    wheel2Vertices[w2Index++] = wheel2CenterX;
    wheel2Vertices[w2Index++] = wheelCenterY;
    wheel2Vertices[w2Index++] = wheelZ;
    wheel2Vertices[w2Index++] = 0.0f;
    wheel2Vertices[w2Index++] = 0.0f;
    wheel2Vertices[w2Index++] = 0.0f;

    // محيط الدائرة
    for (int i = 0; i <= segments; i++)
    {
        float angle = 2.0f * 3.1415926f * i / segments;
        float x = wheel2CenterX + wheelRadius * cos(angle);
        float y = wheelCenterY + wheelRadius * sin(angle);

        wheel2Vertices[w2Index++] = x;
        wheel2Vertices[w2Index++] = y;
        wheel2Vertices[w2Index++] = wheelZ;
        wheel2Vertices[w2Index++] = 0.0f;
        wheel2Vertices[w2Index++] = 0.0f;
        wheel2Vertices[w2Index++] = 0.0f;
    }

    // الشمس 
    float sunVertices[(segments + 2) * 6]; // مركز + نقاط المحيط

    float radius = 0.08f;
    float centerX = 0.0f;
    float centerY = 0.85f;
    float z = 0.1f;

    int index = 0;

    // المركز
    sunVertices[index++] = centerX;
    sunVertices[index++] = centerY;
    sunVertices[index++] = z;
    sunVertices[index++] = 1.0f;
    sunVertices[index++] = 1.0f;
    sunVertices[index++] = 0.0f;

    // نقاط الدائرة
    for (int i = 0; i <= segments; i++)
    {
        float angle = 2.0f * 3.1415926f * i / segments;
        float x = centerX + radius * cos(angle);
        float y = centerY + radius * sin(angle);

        sunVertices[index++] = x;
        sunVertices[index++] = y;
        sunVertices[index++] = z;
        sunVertices[index++] = 1.0f;
        sunVertices[index++] = 1.0f;
        sunVertices[index++] = 0.0f;
    }

    // القمر
    float moonVertices[(segments + 2) * 6];

    float moonRadius = 0.06f;
    float moonCenterX = 0.0f;
    float moonCenterY = 0.8f;
    float moonZ = 0.1f;

    int mIndex = 0;

    // المركز
    moonVertices[mIndex++] = moonCenterX;
    moonVertices[mIndex++] = moonCenterY;
    moonVertices[mIndex++] = moonZ;
    moonVertices[mIndex++] = 0.9f;  // R
    moonVertices[mIndex++] = 0.9f;  // G
    moonVertices[mIndex++] = 0.9f;  // B

    // نقاط الدائرة
    for (int i = 0; i <= segments; i++)
    {
        float angle = 2.0f * 3.1415926f * i / segments;
        float x = moonCenterX + moonRadius * cos(angle);
        float y = moonCenterY + moonRadius * sin(angle);

        moonVertices[mIndex++] = x;
        moonVertices[mIndex++] = y;
        moonVertices[mIndex++] = moonZ;
        moonVertices[mIndex++] = 0.9f;
        moonVertices[mIndex++] = 0.9f;
        moonVertices[mIndex++] = 0.9f;
        }

    //  VAO/VBO SETUP 
    //we wrote a function so that we don't repeat the same process again and again
    auto setupVAO = [&](float* vertices, int size, unsigned int& VAO, unsigned int& VBO)
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

    unsigned int skyVAO, skyVBO;
    setupVAO(skyVertices, sizeof(skyVertices), skyVAO, skyVBO);

    unsigned int roadVAO, roadVBO;
    setupVAO(roadVertices, sizeof(roadVertices), roadVAO, roadVBO);

    unsigned int carVAO, carVBO;
    setupVAO(carVertices, sizeof(carVertices), carVAO, carVBO);

    unsigned int windowVAO, windowVBO;
    setupVAO(windowVertices, sizeof(windowVertices), windowVAO, windowVBO);

    unsigned int wheel1VAO, wheel1VBO;
    setupVAO(wheel1Vertices, sizeof(wheel1Vertices), wheel1VAO, wheel1VBO);

    unsigned int wheel2VAO, wheel2VBO;
    setupVAO(wheel2Vertices, sizeof(wheel2Vertices), wheel2VAO, wheel2VBO);

    unsigned int sunVAO, sunVBO;
    setupVAO(sunVertices, sizeof(sunVertices), sunVAO, sunVBO);

    unsigned int moonVAO, moonVBO;
    setupVAO(moonVertices, sizeof(moonVertices), moonVAO, moonVBO);

    //  MAIN LOOP 

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float timeValue = glfwGetTime();
        glUseProgram(shaderProgram);

        // (السماء (لون متغير باستخدام sin)
        float sky = (sin(timeValue * 0.3f) + 1.0f) / 2.0f;
        glUniform4f(colorLoc, 0.2f * sky, 0.4f * sky, 1.0f * sky, 1.0f);     
        glUniform2f(offsetLoc, 0, 0);
        glBindVertexArray(skyVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //  (الطريق (لون ثابت 
        glUniform4f(colorLoc, 0.2f, 0.2f, 0.2f, 1.0f);
        glBindVertexArray(roadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // ( السيارة (لون ثابت
        float carX = fmod(timeValue * 0.3f, 2.4f) - 1.2f;// fmod يعطي باقي القسمة
        glUniform4f(colorLoc, 1, 0, 0, 1);
        glUniform2f(offsetLoc, carX, 0);
        glBindVertexArray(carVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // ( العجلات (لون ثابت
        glUniform4f(colorLoc, 0, 0, 0, 1);
        glUniform2f(offsetLoc, carX, 0);

        glBindVertexArray(wheel1VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0,segments + 2);

        glBindVertexArray(wheel2VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, segments + 2);

        // الشمس والقمر
        float cycle = fmod(timeValue, 20.0f);

        if (cycle < 10.0f)
        {
            // الشمس
            float t = cycle / 10.0f;
            float sunX = -1.06f + 2.12f * t;

            float sunPulse = (sin(timeValue) + 1.0f) / 2.0f;
            glUniform4f(colorLoc, 1.0f, sunPulse, 0.0f, 1.0f);

            glUniform2f(offsetLoc, sunX, 0);
            glBindVertexArray(sunVAO);
            glDrawArrays(GL_TRIANGLE_FAN, 0,segments + 2);
        }
        else
        {
            // القمر
            float t = (cycle - 10.0f) / 10.0f;
            float moonX = 1.06f - 2.12f * t;

            float moonPulse = (cos(timeValue) + 1.0f) / 2.0f;
            glUniform4f(colorLoc, 0.6f * moonPulse, 0.6f * moonPulse, 1.0f, 1.0f);

            glUniform2f(offsetLoc, moonX, 0);
            glBindVertexArray(moonVAO);
            glDrawArrays(GL_TRIANGLE_FAN, 0 ,segments + 2);
        }

        //نافذة
        glDepthMask(GL_FALSE);   // لا تكتب في depth

        glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 0.4f);
        glUniform2f(offsetLoc, carX, 0);
        glBindVertexArray(windowVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDepthMask(GL_TRUE);    // إعادة التفعيل

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}
