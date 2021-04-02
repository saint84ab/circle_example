#include "context.h"
#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void OnframebufferSizeChange(GLFWwindow* window, int width, int height) {
    SPDLOG_INFO("framebuffer size changed: ({} x {})", width, height);
    glViewport(0, 0, width, height);
}

void OnKeyEvent(GLFWwindow* window,
    int key, int scancode, int action, int mods) {
    SPDLOG_INFO("key: {}, scancode: {}, action: {}, mods: {}{}{}",
        key, scancode,
        action == GLFW_PRESS ? "Pressed" :
        action == GLFW_RELEASE ? "Released" :
        action == GLFW_REPEAT ? "Repeat" : "Undnown",
        mods & GLFW_MOD_CONTROL ? "C" : "-",
        mods & GLFW_MOD_SHIFT ? "S" : "-",
        mods & GLFW_MOD_ALT ? "A" : "-");
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
        }
    }

int main(int argc, const char** argv){
    SPDLOG_INFO("Start program");
    SPDLOG_INFO("arg count: {}", argc);
    for (int i = 0; i < argc; i++){
        SPDLOG_INFO("argv[{}]: {}", i, argv[i]);
    }
    
    float circleRadius = 0.75f;
    float circleSmallRadius = 0.5f;
    int circleSegmentCount = 32;
    float first_seta = 0.0f;
    float last_seta = 360.0f;
    float Red = 1.0f;
    float Green = 1.0f;
    float Blue = 1.0f;

    if (argc >= 2) {
        circleRadius = std::stof(argv[1]);
    }
    if (argc >= 3) {
        circleSmallRadius = std::stof(argv[2]);
    }
    if (argc >= 4) {
        circleSegmentCount = std::stoi(argv[3]);
    }
    if (argc >= 5) {
        first_seta = std::stof(argv[4]);
    }
    if (argc >= 6) {
        last_seta = std::stof(argv[5]);
    }
    if (argc >= 7) {
        Red = std::stof(argv[6]);
    }
    if (argc >= 8) {
        Green = std::stof(argv[7]);
    }
    if (argc >= 9) {
        Blue = std::stof(argv[8]);
    }

    // glfw 라이브러리 초기화, 실패하면 에러 출력 후 종료
    SPDLOG_INFO("Initialize glfw");
    if(!glfwInit()){
        const char* description = nullptr;
        glfwGetError(&description);
        SPDLOG_ERROR("failed to initialized glfw: {}", description);
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw 윈도우 생성, 실패하면 에러 출력 후 종료
    SPDLOG_INFO("Create glfw window");
    auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
    if (!window){
        SPDLOG_ERROR("failed to create glfw window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // glad 를 활용한 OpenGL 함수 로딩
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        SPDLOG_ERROR("failed to initialize glad");
        glfwTerminate();
        return -1;
    }
    auto glVersion = glGetString(GL_VERSION);
    SPDLOG_INFO("OpenGL context version: {}", glVersion);

    auto context = Context::Create();
    if (!context) {
        SPDLOG_ERROR("failed to create context");
        glfwTerminate();
        return -1;
    }

    context->CreateCircle(circleRadius, circleSmallRadius, circleSegmentCount,
        first_seta, last_seta, Red, Green, Blue);

    OnframebufferSizeChange(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, OnframebufferSizeChange);
    glfwSetKeyCallback(window, OnKeyEvent);

    // glfw 루프 실행, 윈도우 close 버튼을 누르면 정상 종료
    SPDLOG_INFO("Start main loop");
    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();
        context->Render();
        glfwSwapBuffers(window);
    }
    context.reset();

    glfwTerminate();
    return 0;
}