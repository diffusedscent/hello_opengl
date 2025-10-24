#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <spdlog/spdlog.h>

#include <cstdlib> // rand
#include <ctime>   // time
#include <cmath>   // sin, cos

#include "algorithm/hello_algorithm.h"

// 랜덤 색상 생성
void getRandomColor(float* r, float* g, float* b) {
    *r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    *g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    *b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// 스크린 크기
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int exersize1(GLFWwindow *window);

void test() {
    hello_algorithm::HelloAlgorithm ex;
    ex.test();
}

int main() {

    test();
    
    // spdlog 초기화
    spdlog::info("program start!");

    // glfw 초기화
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // window 생성
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "First Open GL", NULL, NULL);  // 제목 표시줄(title bar)에 사진이 있으면 좋겠다.
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        //spdlog::error("Failed to initialize GLAD");
        return -1;
    }

    exersize1(window);

    glfwTerminate();

    

    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        //spdlog::info("ESC pressed, window close");
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    //spdlog::info("width: {}, height: {}", width, height);
}

int exersize1(GLFWwindow *window) 
{
    //spdlog::info("exersize1 starts...");

    // build and compile our shader program
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
        //spdlog::error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{}", infoLog);
    }

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
        //spdlog::error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n{}", infoLog);
    }

    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
        //spdlog::error("ERROR::SHADER::PROGRAM::LINKING_FAILED\n{}", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    };

    /*
    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    */

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);  // VBO 객체를 생성하고, GPU 메모리를 "할당"

    // VAO와 VBO을 묶는다.???
    // 이 코드가  실행되는 순간 Opengl의 상태는 "VAO에 대한 모든 설정을 기록할 준비"가 된다.
    // 이때부터 모든 후속 설정들은 이 VAO에 저장된다.  언제까지 저장? (glBindVertexArray(0); 가 호출될 때까지)
    glBindVertexArray(VAO);

    // VAO가 활성화된 상태에서 VBO를 바인딩하면, VAO는 "내게 필요한 정점 데이터는 이 VBO에 있다" 라고 기억한다.
    // 생성된 VBO 객체를 GL_ARRAY_BUFFER라는 전역 상태(global state)에 연결합니다. 
    // 이때부터 GL_ARRAY_BUFFER에 영향을 주는 모든 OpenGL 함수(예: glBufferData)는 
    // 현재 바인딩된 VBO 객체에 대해 작동하게 된다.
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 

    // GL_ARRAY_BUFFER 상태에 바인딩된 버퍼에 데이터를 복사하고, 필요한 GPU 메모리도 이때 할당합니다.
    // 즉, glBindBuffer는 데이터 전송을 위한 **'준비 단계'**라고 볼 수 있다.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
/*
    비유로 이해하기
    VBO를 비어있는 '책장'이라고 생각해 보세요.
    • glGenBuffers(1, &VBO); : "새로운 책장을 하나 만들어 줘" (책장 생성)
    • glBindBuffer(GL_ARRAY_BUFFER, VBO); : "자, 이제부터 내가 '책장'이라고 말하면 방금 만든 그 책장을 가리키는 거야" (책장을 특정 위치에 바인딩)
    • glBufferData(...) : "그 책장에 이 책들을 채워 넣어" (데이터 전송 및 메모리 할당)
    
    glBindBuffer는 '어떤 객체를 사용할지'를 OpenGL에 알려주는 역할을 하며, 
    실제 메모리 할당은 glBufferData와 같은 다른 함수들에 의해 이루어집니다.
 */

 /*
    OpenGL의 렌더링 파이프라인에서 정점 데이터(위치, 색상, 텍스처 좌표 등)는 GPU의 메모리(VBO)에 저장됩니다. 
    쉐이더 프로그램은 이 데이터를 가져와서 정점을 처리합니다. 
    이때, 쉐이더는 각 데이터가 어떤 의미인지 알 수 있도록 변수를 선언하는데, 
    이 변수를 **"정점 속성(Vertex Attribute)"**이라고 부릅니다.

    비유로 이해하기:
    glVertexAttribPointer를 "이 주소에 있는 데이터는 '위치' 데이터입니다"라고 말하는 것이라고 비유할 수 있습니다.
    glEnableVertexAttribArray는 "네, 알겠어요. 이제부터 이 '위치' 데이터를 렌더링에 사용하겠습니다"라고 말하며, 
                            쉐이더가 해당 데이터를 사용할 수 있도록 스위치를 켜는 것과 같습니다.
 */

    // glVertexAttribPointer VBO에 있는 데이터가 어떻게 구성되어 있는지(3개씩 끊어서 읽기, 데이터 간 간격은 6개 float 등)를 
    // 정의하고 현재 바인딩된 VAO에 저장한다.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // 정의된 데이터 구조를 렌더링에 사용할 수 있도록 활성화
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO
    // as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO,
    // but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray anyways
    // so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 메인 루프
    while (!glfwWindowShouldClose(window))
    {
        // 입력 처리
        processInput(window);

        // 버퍼 초기화
#if 0
        float r, g, b;
        getRandomColor(&r, &g, &b);
        //spdlog::info("r: {}, g: {}, b: {}", r, g, b);
        glClearColor(r, g, b, 1.0f);
#else
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);        
#endif
        glClear(GL_COLOR_BUFFER_BIT);

        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue / 2.0f) + 0.5f);
        spdlog::info("time: {}, sin(timeValue): {}, sin(timeValue / 2.0f): {}, greenValue: {}", timeValue, sin(timeValue),  sin(timeValue / 2.0f), greenValue);
        
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    return 0;
}


/*
질문) 
shader 구조를 보면 main 함수가 있어요. 이게 GPU에서 실행되는 프로그램인가요? 
통상 c언어 프로그램을 만들고 실행하면 프로세스가 생성되는데.. 
glUseProgram 하면 실행되어서 프로세스가 뜨는건지 모르겠네요.

대답) 
결론부터 말씀드리면, 쉐이더의 main 함수는 GPU에서 실행되는 프로그램이 맞지만, 
우리가 흔히 아는 독립적인 "프로세스"로 실행되는 것은 아닙니다.

GPU와 CPU의 근본적인 차이
이 개념을 이해하려면 CPU와 GPU의 작동 방식을 알아야 합니다.
• CPU (Central Processing Unit):
    • 소수의 강력한 코어를 가지고 있습니다.
    • 순차적이고 복잡한 작업을 처리하는 데 특화되어 있습니다.
    • **하나의 프로그램(프로세스)**을 순서대로 실행하며, 운영체제가 이 프로세스들을 관리하고 자원을 할당합니다. 
        우리가 흔히 아는 C 언어 프로그램의 main 함수는 이 프로세스의 시작점입니다.

• GPU (Graphics Processing Unit):
    • 수천 개의 작고 단순한 코어를 가지고 있습니다.
    • 수많은 계산을 동시에, 병렬적으로 처리하는 데 특화되어 있습니다. 
        그래픽 렌더링은 수백만 개의 픽셀과 정점을 동시에 계산해야 하기 때문에 이런 구조가 적합합니다.

쉐이더의 main 함수는 어떻게 실행되나요?
glUseProgram은 C 언어의 main 함수를 실행하듯 독립적인 프로세스를 만드는 명령이 아닙니다. 
대신, OpenGL에게 **"이 쉐이더 프로그램을 사용해서 앞으로의 렌더링 작업을 수행해라"**라고 알려주는 설정 명령에 가깝습니다.

실제 실행은 렌더링 명령(glDrawArrays 또는 glDrawElements)이 내려질 때 일어납니다. 
이때, GPU는 다음과 같이 쉐이더를 실행합니다.

1. 정점 쉐이더 (Vertex Shader):
    • GPU는 렌더링할 정점의 개수만큼 정점 쉐이더의 main 함수를 동시에 실행합니다. 
        예를 들어, 삼각형 하나를 그리면 정점이 3개이므로 main 함수가 3번 병렬적으로 실행됩니다.
    • 각 실행은 해당 정점 하나에 대한 위치, 색상 등을 계산합니다.
2. 프래그먼트 쉐이더 (Fragment Shader):
    • 정점 쉐이더가 계산을 마치고 나면, GPU는 이 정점들로 만들어지는 삼각형을 채우기 위해 
        수많은 **프래그먼트(픽셀 후보)**를 생성합니다.
    • GPU는 이 프래그먼트의 개수만큼 프래그먼트 쉐이더의 main 함수를 동시에 실행합니다.
    • 각 실행은 해당 픽셀의 최종 색상을 계산합니다.


요약
쉐이더의 main 함수: CPU처럼 독립된 프로세스로 실행되는 것이 아니라, 렌더링할 정점 또는 픽셀의 개수만큼 수천 번, 수만 번 동시에, 병렬적으로 실행됩니다.
*/