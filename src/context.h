#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"
#include "vertex_layout.h"

CLASS_PTR(Context)
class Context {
public:
    static ContextUPtr Create();
    void CreateCircle(float radius, float SmallRadius, int segment, 
    float first_seta, float last_seta, float R, float G, float B);
    void Render();
private:
    Context() {}
    bool Init();
    ProgramUPtr m_program;

    VertexLayoutUPtr m_vertexLayout;
	BufferUPtr m_vertexBuffer;
    BufferUPtr m_indexBuffer;
    int m_indexCount { 6 };
};

#endif // __CONTEXT_H__