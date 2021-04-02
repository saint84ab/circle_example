#include "context.h"

ContextUPtr Context::Create() {
    auto context = ContextUPtr(new Context());
    if (!context->Init())
        return nullptr;
    return std::move(context);
}

bool Context::Init() {

    float vertices[] = {
         0.5f, 0.5f, 0.0f, 0.0f, 0.7f, 0.3f, // top right, red
         0.5f, -0.5f, 0.0f, 0.6f, 0.0f, 0.4f, // bottom right, green
        -0.5f, -0.5f, 0.0f, 0.1f, 0.9f, 0.0f, // bottom left, blue
        -0.5f, 0.5f, 0.0f, 0.3f, 0.3f, 0.4f, // top left, yellow
    };

    uint32_t indices[] = { // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    m_vertexLayout =  VertexLayout::Create();
    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER,
        GL_STATIC_DRAW, vertices, sizeof(float) * 24);

    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
    m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, sizeof(float) * 3);

    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER,
        GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6);

    ShaderPtr vertShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);
    if (!vertShader || !fragShader)
        return false;
    SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragShader->Get());

    m_program = Program::Create({fragShader, vertShader});
    if (!m_program)
        return false;
    SPDLOG_INFO("program id: {}", m_program->Get());

    glClearColor(0.0f, 0.5f, 0.5f, 0.0f);

    return true;
}

void Context::Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    m_program->Use();
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}

// 그림 두 개를 출력하는게 불가능한 것 같아 호 자체를 삼각형으로 분할하여 그림.
void Context::CreateCircle(float radius, float SmallRadius, int segment,
    float first_seta, float last_seta, float R, float G, float B) {
   	std::vector<float> vertices;
    std::vector<uint32_t> indices;

    const float pi = 3.141592f;

    for (int i = 0; i <= segment; i++){ 
        float angle;     

        if (first_seta != 0){       // 그림을 그릴 각도. 최대각 - 최소각
            angle = (first_seta + (last_seta - first_seta) / segment * i) * pi / 180.0f;      // 최소각이 0이 아닐 때, 그림의 시작점의 위치를 변경
        }
        else if (first_seta == 0){
            angle = (last_seta - first_seta) / segment * i * pi / 180.0f;       // 최소각이 0일 때, 그림의 시작점을 그대로
        }

        float x1 = cosf(angle) * radius;        // 호의 바깥쪽 점들 좌표 계산
        float y1 = sinf(angle) * radius;
        float a1 = cosf(angle) * SmallRadius;   // 호의 안쪽 점들 좌표 계산
        float b1 = sinf(angle) * SmallRadius;
        

        vertices.push_back(x1);     // (x1, y1, 0)의 좌표를 가진 호의 바깥쪽 점 생성
        vertices.push_back(y1);
        vertices.push_back(0.0f);

        vertices.push_back(a1);     // (a1, b1, 0)의 좌표를 가진 호의 안쪽 점 생성
        vertices.push_back(b1);
        vertices.push_back(0.0f);

    }
    
    for (int i = 0; i <= segment * 2 - 1; i++){     // 호를 삼각형으로 만들기 때문에 입력받은 segment의 두 배만큼 삼각형을 그림. 원점은 사용하지 않으므로 시작점은 i = 0.
        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
    }

    m_vertexLayout =  VertexLayout::Create();
    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER,
        GL_STATIC_DRAW, vertices.data(), sizeof(float) * vertices.size());

    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER,
        GL_STATIC_DRAW, indices.data(), sizeof(uint32_t) * indices.size());

    m_indexCount = (int)indices.size();
    
    auto loc = glGetUniformLocation(m_program->Get(), "color"); // 색 초기화
    m_program->Use();
    glUniform4f(loc, R, G, B, 1.0f);
}