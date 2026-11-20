#include <GL/glew.h>
#include <GL/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>

#include <GL/glm/glm.hpp>
#include <GL/glm/ext.hpp>
#include <GL/glm/gtc/matrix_transform.hpp>
#include <GL/glm/gtc/type_ptr.hpp>

#include <cstdlib>
#include <ctime>

#include <vector>
#include <cmath>

using namespace std;

#pragma region 정점들

const float axisVertices[18] = {
    -2.0f,  0.0f,  0.0f,
     2.0f,  0.0f,  0.0f,

     0.0f, -2.0f,  0.0f,
     0.0f,  2.0f,  0.0f,

     0.0f,  0.0f, -2.0f,
     0.0f,  0.0f,  2.0f
};
const float faceVertices[18] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,

    -0.5f, -0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f
};
const float pyramidSideVertices[9] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.0f,  0.5f,  0.0f
};

#pragma endregion
#pragma region 오픈지엘 변수들

// OpenGL 객체 이름
GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgramID;

GLuint axisVAO;
GLuint axisVBO;

GLuint faceVAO;
GLuint faceVBO;

GLuint pyramidSideVAO;
GLuint pyramidSideVBO;

GLuint cubeVAO;
GLuint cubeVBO;

GLuint coneVAO;
GLuint coneVBO;

GLuint cylinderVAO;
GLuint cylinderVBO;

GLuint sphereVAO;
GLuint sphereVBO;

GLuint circleVAO;
GLuint circleVBO;

GLuint pyramidVAO;
GLuint pyramidVBO;

std::vector<float> cubeVertices;
GLsizei cubeVertexCount = 0;

std::vector<float> coneVertices;
GLsizei coneVertexCount = 0;

std::vector<float> cylinderVertices;
GLsizei cylinderVertexCount = 0;

std::vector<float> sphereVertices;
GLsizei sphereVertexCount = 0;

std::vector<float> circleVertices;
GLsizei circleVertexCount = 0;

std::vector<float> pyramidVertices;
GLsizei pyramidVertexCount = 0;

#pragma endregion
#pragma region 각종 변수들

// 카메라 위치
glm::vec3 cameraPos(1.0f, 4.0f, 7.0f);
glm::vec3 cameraDirection(-1.0f, -4.0f, -7.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
int viewmode{};

// 조명 위치
glm::vec3 lightPos(2.0f, 1.0f, -2.0f);

// 카메라 상태
int cameraRotationDirection = 0;
int cameraOrbitDirection = 0;

// 조명 상태
int lightRotationDirection = 0;
int lightOrbitDirection = 0;

bool lightEnabled = true;

glm::vec3 lightColor(1.0f);

// 초반 상태
bool shapeChange = false;
float lightRotationAngle = 0.0f;
float lightOrbitAngle = 0.0f;

// 상태 변수
bool usePerspective = false;
bool wireframeMode = false;

// 애니메이션 상태

// 입력처리
bool previousYPressed = false;
bool previousRPressed = false;
bool previousNPressed = false;
bool previousBPressed = false;
bool previousVPressed = false;
bool previousMPressed = false;
bool previousCPressed = false;


// 도형 종류 선택
const int SHAPE_CUBE = 0;
const int SHAPE_CONE = 1;
const int SHAPE_CYLINDER = 2;
const int SHAPE_SPHERE = 3;
const int SHAPE_CIRCLE = 4;
const int SHAPE_PYRAMID = 5;

// 선택의 종류
int selectedObject = 5;

// 은면제거
bool depthTestEnabled = true;

#pragma endregion
#pragma region 도형 만드는 함수들

// 정점 추가해주는 함수
void AddVertex(std::vector<float>& vertices, float x, float y, float z)
{
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
}

// 삼각형 정점들 + 법선벡터 저장
void AddTriNV(std::vector<float>& vertices, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2) {

    glm::vec3 edge1 = p1 - p0;
    glm::vec3 edge2 = p2 - p0;

    glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

    AddVertex(vertices, p0.x, p0.y, p0.z);
    AddVertex(vertices, normal.x, normal.y, normal.z);

    AddVertex(vertices, p1.x, p1.y, p1.z);
    AddVertex(vertices, normal.x, normal.y, normal.z);

    AddVertex(vertices, p2.x, p2.y, p2.z);
    AddVertex(vertices, normal.x, normal.y, normal.z);
}

// 사각형 면 정점 + 법선 벡터 저장하는 함수
void AddQuad(std::vector<float>& vertices, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
    // 첫 번째 삼각형
    AddTriNV(vertices, p0, p1, p2);
    // 두 번째 삼각형
    AddTriNV(vertices, p0, p2, p3);
}

// 육면체 만드는 함수 + 법선벡터 포함
void MakeCubeVertices()
{
    cubeVertices.clear();

    const float h = 0.5f;

    glm::vec3 p0(-h, -h, -h);
    glm::vec3 p1(h, -h, -h);
    glm::vec3 p2(h, h, -h);
    glm::vec3 p3(-h, h, -h);

    glm::vec3 p4(-h, -h, h);
    glm::vec3 p5(h, -h, h);
    glm::vec3 p6(h, h, h);
    glm::vec3 p7(-h, h, h);

    // 앞면: +Z
    AddQuad(cubeVertices, p4, p5, p6, p7);

    // 뒷면: -Z
    AddQuad(cubeVertices, p1, p0, p3, p2);

    // 오른쪽 면: +X
    AddQuad(cubeVertices, p5, p1, p2, p6);

    // 왼쪽 면: -X
    AddQuad(cubeVertices, p0, p4, p7, p3);

    // 윗면: +Y
    AddQuad(cubeVertices, p7, p6, p2, p3);

    // 밑면: -Y
    AddQuad(cubeVertices, p0, p1, p5, p4);

    cubeVertexCount =
        static_cast<GLsizei>(cubeVertices.size() / 6);
}

// 원뿔 만드는 함수
void MakeConeVertices()
{
    coneVertices.clear();

    const int sliceCount = 20;
    const float radius = 0.3f;
    const float topY = 0.3f;
    const float bottomY = -0.3f;
    const float PI = 3.141592f;

    for (int slice = 0; slice < sliceCount; ++slice)
    {
        float theta1 =
            2.0f * PI * slice / sliceCount;

        float theta2 =
            2.0f * PI * (slice + 1) / sliceCount;

        float x1 = radius * cos(theta1);
        float z1 = radius * sin(theta1);

        float x2 = radius * cos(theta2);
        float z2 = radius * sin(theta2);

        AddVertex(coneVertices, 0.0f, topY, 0.0f);
        AddVertex(coneVertices, x1, bottomY, z1);
        AddVertex(coneVertices, x2, bottomY, z2);
    }

    coneVertexCount =
        static_cast<GLsizei>(coneVertices.size() / 3);
}

// 실린더 만드는 함수 + 위치와 법선 벡터
void MakeCylinderVertices()
{
    cylinderVertices.clear();

    const int sliceCount = 20;
    const float radius = 0.3f;
    const float topY = 0.3f;
    const float bottomY = -0.3f;
    const float PI = 3.141592f;

    for (int slice = 0; slice < sliceCount; ++slice)
    {
        float theta1 = 2.0f * PI * slice / sliceCount;
        float theta2 = 2.0f * PI * (slice + 1) / sliceCount;

        float x1 = radius * cos(theta1);
        float z1 = radius * sin(theta1);
        float x2 = radius * cos(theta2);
        float z2 = radius * sin(theta2);

        glm::vec3 top1(x1, topY, z1);
        glm::vec3 bottom1(x1, bottomY, z1);
        glm::vec3 top2(x2, topY, z2);
        glm::vec3 bottom2(x2, bottomY, z2);
        glm::vec3 normal1 = glm::normalize(glm::vec3(x1, 0.0f, z1));
        glm::vec3 normal2 = glm::normalize(glm::vec3(x2, 0.0f, z2));

        // 옆면은 각 정점에서 바깥쪽을 향하는 법선을 사용한다.
        AddVertex(cylinderVertices, top1.x, top1.y, top1.z);
        AddVertex(cylinderVertices, normal1.x, normal1.y, normal1.z);
        AddVertex(cylinderVertices, bottom1.x, bottom1.y, bottom1.z);
        AddVertex(cylinderVertices, normal1.x, normal1.y, normal1.z);
        AddVertex(cylinderVertices, bottom2.x, bottom2.y, bottom2.z);
        AddVertex(cylinderVertices, normal2.x, normal2.y, normal2.z);

        AddVertex(cylinderVertices, top1.x, top1.y, top1.z);
        AddVertex(cylinderVertices, normal1.x, normal1.y, normal1.z);
        AddVertex(cylinderVertices, bottom2.x, bottom2.y, bottom2.z);
        AddVertex(cylinderVertices, normal2.x, normal2.y, normal2.z);
        AddVertex(cylinderVertices, top2.x, top2.y, top2.z);
        AddVertex(cylinderVertices, normal2.x, normal2.y, normal2.z);

        // 양쪽 면은 정점 순서로 각각 +Y, -Y 면 법선을 만든다.
        AddTriNV(cylinderVertices, glm::vec3(0.0f, topY, 0.0f), top2, top1);
        AddTriNV(cylinderVertices, glm::vec3(0.0f, bottomY, 0.0f), bottom1, bottom2);
    }

    cylinderVertexCount =
        static_cast<GLsizei>(cylinderVertices.size() / 6);
}

// 구 만드는 함수 + 법선 벡터
void MakeSphereVertices()
{
    sphereVertices.clear();

    const int stackCount = 10;
    const int sliceCount = 20;
    const float radius = 0.3f;
    const float PI = 3.141592f;

    for (int stack = 0; stack < stackCount; ++stack)
    {
        float phi1 =
            PI * stack / stackCount;

        float phi2 =
            PI * (stack + 1) / stackCount;

        for (int slice = 0; slice < sliceCount; ++slice)
        {
            float theta1 =
                2.0f * PI * slice / sliceCount;

            float theta2 =
                2.0f * PI * (slice + 1) / sliceCount;

            glm::vec3 p00(
                radius * sin(phi1) * cos(theta1),
                radius * cos(phi1),
                radius * sin(phi1) * sin(theta1)
            );

            glm::vec3 p01(
                radius * sin(phi1) * cos(theta2),
                radius * cos(phi1),
                radius * sin(phi1) * sin(theta2)
            );

            glm::vec3 p10(
                radius * sin(phi2) * cos(theta1),
                radius * cos(phi2),
                radius * sin(phi2) * sin(theta1)
            );

            glm::vec3 p11(
                radius * sin(phi2) * cos(theta2),
                radius * cos(phi2),
                radius * sin(phi2) * sin(theta2)
            );

            // 첫 번째 삼각형
            AddVertex(
                sphereVertices,
                p00.x, p00.y, p00.z
            );
            {
                glm::vec3 normal = glm::normalize(p00);
                AddVertex(sphereVertices, normal.x, normal.y, normal.z);
            }

            AddVertex(
                sphereVertices,
                p10.x, p10.y, p10.z
            );
            {
                glm::vec3 normal = glm::normalize(p10);
                AddVertex(sphereVertices, normal.x, normal.y, normal.z);
            }

            AddVertex(
                sphereVertices,
                p11.x, p11.y, p11.z
            );
            {
                glm::vec3 normal = glm::normalize(p11);
                AddVertex(sphereVertices, normal.x, normal.y, normal.z);
            }

            // 두 번째 삼각형
            AddVertex(
                sphereVertices,
                p00.x, p00.y, p00.z
            );
            {
                glm::vec3 normal = glm::normalize(p00);
                AddVertex(sphereVertices, normal.x, normal.y, normal.z);
            }

            AddVertex(
                sphereVertices,
                p11.x, p11.y, p11.z
            );
            {
                glm::vec3 normal = glm::normalize(p11);
                AddVertex(sphereVertices, normal.x, normal.y, normal.z);
            }

            AddVertex(
                sphereVertices,
                p01.x, p01.y, p01.z
            );
            {
                glm::vec3 normal = glm::normalize(p01);
                AddVertex(sphereVertices, normal.x, normal.y, normal.z);
            }
        }
    }

    sphereVertexCount =
        static_cast<GLsizei>(sphereVertices.size() / 6);
}

// 원 만드는 함수
void MakeCircleVertices() {
    circleVertices.clear();

    const int sliceCount = 20;
    const float radius = 1.0f;
    const float PI = 3.141592f;

    for (int slice = 0; slice < sliceCount; ++slice)
    {
        float theta1 =
            2.0f * PI * slice / sliceCount;

        float theta2 =
            2.0f * PI * (slice + 1) / sliceCount;

        float x1 = radius * cos(theta1);
        float z1 = radius * sin(theta1);

        float x2 = radius * cos(theta2);
        float z2 = radius * sin(theta2);

        AddVertex(circleVertices, x1, 0.0f, z1);
        AddVertex(circleVertices, x2, 0.0f, z2);
    }

    circleVertexCount =
        static_cast<GLsizei>(circleVertices.size() / 3);
}

// 사각뿔 만드는 함수 + 법선벡터 포함
void MakePyramidVertices() {
    pyramidVertices.clear();

    const float h = 0.5f;
    float topY = h;

    glm::vec3 top(0.0f, topY, 0.0f);

    glm::vec3 p0(-h, -h, -h);
    glm::vec3 p1(h, -h, -h);

    glm::vec3 p2(-h, -h, h);
    glm::vec3 p3(h, -h, h);

    // 앞면: +Z
    AddTriNV(pyramidVertices, top, p2, p3);

    // 뒷면: -Z p1 -> p0
    AddTriNV(pyramidVertices, top, p1, p0);

    // 오른쪽 면: +X p3 -> p1
    AddTriNV(pyramidVertices, top, p3, p1);

    // 왼쪽 면: -X p0 -> p2
    AddTriNV(pyramidVertices, top, p0, p2);

    // 밑면: -Y
    AddQuad(pyramidVertices, p2, p0, p1, p3);

    pyramidVertexCount =
        static_cast<GLsizei>(pyramidVertices.size() / 6);
}

# pragma endregion
#pragma region 잡다한 함수들

string filetobuf(const char* file)
{
    ifstream shaderFile(file);

    if (!shaderFile.is_open())
    {
        cerr << "파일 열기 실패: " << file << endl;
        return "";
    }

    string source(
        (istreambuf_iterator<char>(shaderFile)),
        istreambuf_iterator<char>()
    );

    shaderFile.close();

    return source;
}
void MakeVertexShader()
{
    string vertexSource = filetobuf("vertex.glsl");
    const char* source = vertexSource.c_str();

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &source, nullptr);
    glCompileShader(vertexShader);
}
void MakeFragmentShader()
{
    string fragmentSource = filetobuf("fragment.glsl");
    const char* source = fragmentSource.c_str();

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &source, nullptr);
    glCompileShader(fragmentShader);
}
GLuint MakeShaderProgram()
{
    GLuint program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    return program;
}

void InitBuffer()
{
#pragma region 좌표축
    glGenVertexArrays(1, &axisVAO);
    glGenBuffers(1, &axisVBO);

    glBindVertexArray(axisVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(axisVertices),
        axisVertices,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);
#pragma endregion

#pragma region 육면체
    MakeCubeVertices();

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        cubeVertices.size() * sizeof(float),
        cubeVertices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)(3 * sizeof(float))
    );

    glEnableVertexAttribArray(1);

#pragma endregion

#pragma region 원뿔
    MakeConeVertices();

    glGenVertexArrays(1, &coneVAO);
    glGenBuffers(1, &coneVBO);

    glBindVertexArray(coneVAO);
    glBindBuffer(GL_ARRAY_BUFFER, coneVBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        coneVertices.size() * sizeof(float),
        coneVertices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);
#pragma endregion

#pragma region 실린더
    MakeCylinderVertices();

    glGenVertexArrays(1, &cylinderVAO);
    glGenBuffers(1, &cylinderVBO);

    glBindVertexArray(cylinderVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        cylinderVertices.size() * sizeof(float),
        cylinderVertices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)(3 * sizeof(float))
    );

    glEnableVertexAttribArray(1);
#pragma endregion

#pragma region 구
    MakeSphereVertices();

    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);

    glBindVertexArray(sphereVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        sphereVertices.size() * sizeof(float),
        sphereVertices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)(3 * sizeof(float))
    );

    glEnableVertexAttribArray(1);

#pragma endregion

#pragma region 원
    MakeCircleVertices();

    glGenVertexArrays(1, &circleVAO);
    glGenBuffers(1, &circleVBO);

    glBindVertexArray(circleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, circleVBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        circleVertices.size() * sizeof(float),
        circleVertices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);
#pragma endregion

#pragma region 면
    glGenVertexArrays(1, &faceVAO);
    glGenBuffers(1, &faceVBO);

    glBindVertexArray(faceVAO);
    glBindBuffer(GL_ARRAY_BUFFER, faceVBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(faceVertices),
        faceVertices,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    glGenVertexArrays(1, &pyramidSideVAO);
    glGenBuffers(1, &pyramidSideVBO);

    glBindVertexArray(pyramidSideVAO);
    glBindBuffer(GL_ARRAY_BUFFER, pyramidSideVBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(pyramidSideVertices),
        pyramidSideVertices,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);

#pragma endregion

#pragma region 사각뿔

    MakePyramidVertices();

    glGenVertexArrays(1, &pyramidVAO);
    glGenBuffers(1, &pyramidVBO);

    glBindVertexArray(pyramidVAO);
    glBindBuffer(GL_ARRAY_BUFFER, pyramidVBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        pyramidVertices.size() * sizeof(float),
        pyramidVertices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)(3 * sizeof(float))
    );

    glEnableVertexAttribArray(1);

#pragma endregion

}
void DrawObject(int objectMode, const glm::mat4& model, GLint modelLocation)
{
    GLuint selectedVAO = 0;
    GLsizei selectedVertexCount = 0;

    if (objectMode == SHAPE_CUBE)
    {
        selectedVAO = cubeVAO;
        selectedVertexCount = cubeVertexCount;
    }
    else if (objectMode == SHAPE_CONE)
    {
        selectedVAO = coneVAO;
        selectedVertexCount = coneVertexCount;
    }
    else if (objectMode == SHAPE_CYLINDER)
    {
        selectedVAO = cylinderVAO;
        selectedVertexCount = cylinderVertexCount;
    }
    else if (objectMode == SHAPE_SPHERE)
    {
        selectedVAO = sphereVAO;
        selectedVertexCount = sphereVertexCount;
    }
    else if (objectMode == SHAPE_CIRCLE)
    {
        selectedVAO = circleVAO;
        selectedVertexCount = circleVertexCount;
    }
    else if (objectMode == SHAPE_PYRAMID)
    {
        selectedVAO = pyramidVAO;
        selectedVertexCount = pyramidVertexCount;
    }


    glUniformMatrix4fv(
        modelLocation,
        1,
        GL_FALSE,
        glm::value_ptr(model)
    );

    glBindVertexArray(selectedVAO);

    if (selectedVAO == circleVAO) {
        glDrawArrays(
            GL_LINES,
            0,
            selectedVertexCount
        );
    }
    else {
        glDrawArrays(
            GL_TRIANGLES,
            0,
            selectedVertexCount
        );
    }
}

#pragma endregion

void ResetScene()
{
    // 초반 상태

    // 상태 변수

    // 애니메이션 상태

    // 카메라
    cameraPos = glm::vec3(3.0f, 2.0f, 4.0f);
    cameraDirection = glm::vec3(-3.0f, -2.0f, -4.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // 카메라 자전
    cameraRotationDirection = 0;

    // 카메라 공전
    cameraOrbitDirection = 0;
}
void InputProcess(GLFWwindow* window)
{
    bool shiftPressed =
        glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
    
    // 종료, 초기화
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        ResetScene();
    }

    // 카메라 시점 선택
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        cameraPos = glm::vec3(0.0f, 5.0f, 0.0f);
        cameraDirection = glm::vec3(0.0f, 0.5f - 5.0f, 0.0f);
        cameraUp = glm::vec3(0.0f, 0.0f, -1.0f);
        cameraRotationDirection = 0;
        cameraOrbitDirection = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        cameraPos = glm::vec3(0.0f, 0.5f, 3.0f);
        cameraDirection = glm::vec3(0.0f, 0.0f, -3.0f);
        cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        cameraRotationDirection = 0;
        cameraOrbitDirection = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        cameraPos = glm::vec3(3.0f, 0.5f, 0.0f);
        cameraDirection = glm::vec3(-3.0f, 0.0f, 0.0f);
        cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        cameraRotationDirection = 0;
        cameraOrbitDirection = 0;
    }

    // 투영, 출력모드 선택
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        if (shiftPressed) {
            usePerspective = false;
        }
        else {
            usePerspective = true;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        if (shiftPressed) {
            wireframeMode = true;
        }
        else {
            wireframeMode = false;
        }
    }


    // 카메라 이동
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        if (shiftPressed)
        {
            cameraPos.x -= 0.005f;
        }
        else
        {
            cameraPos.x += 0.005f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        if (shiftPressed)
        {
            cameraPos.z -= 0.005f;
        }
        else
        {
            cameraPos.z += 0.005f;
        }

    }
    
    bool currentYPressed = glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS;
    if (currentYPressed && !previousYPressed)
    {
        if (shiftPressed)
        {
            if (cameraRotationDirection == -1) {
                cameraRotationDirection = 0;
            }
            else {
                cameraRotationDirection = -1;
            }
        }
        else
        {
            if (cameraRotationDirection == 1) {
                cameraRotationDirection = 0;
            }
            else {
                cameraRotationDirection = 1;
            }
        }
    }
    previousYPressed = currentYPressed;

    bool currentRPressed = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;
    if (currentRPressed && !previousRPressed)
    {
        if (shiftPressed)
        {
            if (cameraOrbitDirection == -1) {
                cameraOrbitDirection = 0;
            }
            else {
                cameraOrbitDirection = -1;
            }
        }
        else
        {
            if (cameraOrbitDirection == 1) {
                cameraOrbitDirection = 0;
            }
            else {
                cameraOrbitDirection = 1;
            }
        }
    }
    previousRPressed = currentRPressed;

    bool currentMPressed = glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS;
    if (currentMPressed && !previousMPressed)
    {
        lightEnabled = !lightEnabled;
    }
    previousMPressed = currentMPressed;

}
void UpdateScene()
{
    // 카메라 시선 y축 자전 행렬
    if (cameraRotationDirection != 0)
    {
        glm::mat4 rotation = glm::rotate(
            glm::mat4(1.0f),
            glm::radians(0.003f * cameraRotationDirection),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        cameraDirection =
            glm::vec3(rotation * glm::vec4(cameraDirection, 0.0f));
    }

    // 카메라 위치 y축 공전 행렬
    if (cameraOrbitDirection != 0)
    {
        glm::mat4 orbitRotation = glm::rotate(
            glm::mat4(1.0f),
            glm::radians(0.003f * cameraOrbitDirection),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        cameraPos =
            glm::vec3(orbitRotation * glm::vec4(cameraPos, 1.0f));
    }

    // 투영 선택
    {
        if (usePerspective == true) {
            viewmode = 1;
        }
        else {
            viewmode = 0;
        }
    }

    // 출력 모드 선택
    {
        if (wireframeMode == true) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
}
void DrawScene(int viewmode)
{   
    #pragma region 뷰행렬, 투영행렬, 깊이 등...
    
    glEnable(GL_DEPTH_TEST);
    
    glUseProgram(shaderProgramID);

    // 조명 사용 여부
    int lightingLocation = glGetUniformLocation(shaderProgramID, "useLighting");
    glUniform1i(lightingLocation, GL_FALSE);

    int lightPosLocation = glGetUniformLocation(shaderProgramID, "lightPos");
    glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);
    int lightColorLocation = glGetUniformLocation(shaderProgramID, "lightColor");
    glUniform3f(lightColorLocation, lightColor.x, lightColor.y, lightColor.z);

    // 광원 사용 여부
    int lightEnabledLocation = glGetUniformLocation(shaderProgramID, "lightEnabled");
    glUniform1i(lightEnabledLocation, lightEnabled);

	// 광원 위치
    int viewPosLocation = glGetUniformLocation(shaderProgramID, "viewPos");
    glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);

    // 뷰행렬
    glm::mat4 view;
    if (viewmode == 0 || viewmode == 1) {
        glm::vec3 cameraTarget = cameraPos + cameraDirection;


        view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
    }
    int viewLocation = glGetUniformLocation(shaderProgramID, "view");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
    
    // 투영행렬
    glm::mat4 projection;
    if (viewmode == 0) {
        projection = glm::perspective(
            glm::radians(45.0f),
            1.0f,
            0.1f,
            100.0f
        );
    }
    if (viewmode == 1){
        projection = glm::ortho(
            -2.5f, 2.5f,
            -2.5f, 2.5f,
            0.1f, 20.0f
        );
    }
    int projectionLocation = glGetUniformLocation(shaderProgramID, "projection");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

    int modelLocation = glGetUniformLocation(shaderProgramID, "model");
    int colorLocation = glGetUniformLocation(shaderProgramID, "objectColor");


    #pragma endregion

    //좌표축, 양의방향 출력
    {
        glm::mat4 model(1.0f);
        glBindVertexArray(axisVAO);
        glUniformMatrix4fv(
            modelLocation,
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );
        // x축은 빨간색
        glUniform4f(colorLocation, 1.0f, 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_LINES, 0, 2);
        // y축은 초록색
        glUniform4f(colorLocation, 0.0f, 1.0f, 0.0f, 1.0f);
        glDrawArrays(GL_LINES, 2, 2);
        // z축은 파란색
        glUniform4f(colorLocation, 0.0f, 0.0f, 1.0f, 1.0f);
        glDrawArrays(GL_LINES, 4, 2);
        // 양의 방향 출력
        glPointSize(8.0f);
        glUniform4f(colorLocation, 1.0f, 1.0f, 0.0f, 1.0f);
        glDrawArrays(GL_POINTS, 5, 1);
        glDrawArrays(GL_POINTS, 1, 1);
        glDrawArrays(GL_POINTS, 3, 1);
    }

    // 바닥
    {
        glm::mat4 model = glm::mat4(1.0f);

        model = glm::rotate(
            model,
            glm::radians(90.0f),
            glm::vec3(1.0f, 0.0f, 0.0f)
        );

        model = glm::scale(
            model,
            glm::vec3(4.0f, 4.0f, 1.0f)
        );

        glUniformMatrix4fv(
            modelLocation,
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );

        glUniform4f(colorLocation, 0.5f, 0.5f, 0.5f, 1.0f);

        glBindVertexArray(faceVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    // 차량 아래 몸체
    {
        glUniform1i(lightingLocation, GL_TRUE);

		glm::mat4 model(1.0f);

        model = glm::translate(
            model,
            glm::vec3(0.0f, 0.5f, 0.0f)
        );

        model = glm::scale(
            model,
            glm::vec3(0.5f, 0.2f, 1.0f)
        );

		glUniform4f(colorLocation, 0.6f, 0.4f, 0.2f, 1.0f);
		DrawObject(SHAPE_CUBE, model, modelLocation);
    }

    // 차량 위 몸체
    {
        glm::mat4 model(1.0f);

        model = glm::translate(
            model,
            glm::vec3(0.0f, 0.5f + 0.1f + 0.1f, 0.0f)
        );

        model = glm::scale(
            model,
            glm::vec3(0.5f, 0.2f, 0.6f)
        );

        glUniform4f(colorLocation, 0.6f, 0.4f, 0.2f, 1.0f);
        DrawObject(SHAPE_CUBE, model, modelLocation);
    }

    // 바퀴들
    {
        // 왼쪽 앞 바퀴
        {
            glm::mat4 model(1.0f);

            model = glm::translate(
                model,
                glm::vec3(-0.25f - 0.06f, 0.5f - 0.1f, 0.5f - 0.18f)
            );

            model = glm::rotate(
                model,
                glm::radians(90.0f),
                glm::vec3(0.0f, 0.0f, 1.0f)
            );

            model = glm::scale(
                model,
                glm::vec3(0.5f, 0.2f, 0.5f)
            );

            glUniform4f(colorLocation, 0.15f, 0.15f, 0.15f, 1.0f);
            DrawObject(SHAPE_CYLINDER, model, modelLocation);
        }

        // 오른쪽 앞 바퀴
        {
            glm::mat4 model(1.0f);

            model = glm::translate(
                model,
                glm::vec3(0.25f + 0.06f, 0.5f - 0.1f, 0.5f - 0.18f)
            );

            model = glm::rotate(
                model,
                glm::radians(90.0f),
                glm::vec3(0.0f, 0.0f, 1.0f)
            );

            model = glm::scale(
                model,
                glm::vec3(0.5f, 0.2f, 0.5f)
            );

            glUniform4f(colorLocation, 0.15f, 0.15f, 0.15f, 1.0f);
            DrawObject(SHAPE_CYLINDER, model, modelLocation);
        }

        // 왼쪽 뒤 바퀴
        {
            glm::mat4 model(1.0f);

            model = glm::translate(
                model,
                glm::vec3(-0.25f - 0.06f, 0.5f - 0.1f, -0.5f + 0.18f)
            );

            model = glm::rotate(
                model,
                glm::radians(90.0f),
                glm::vec3(0.0f, 0.0f, 1.0f)
            );

            model = glm::scale(
                model,
                glm::vec3(0.5f, 0.2f, 0.5f)
            );

            glUniform4f(colorLocation, 0.15f, 0.15f, 0.15f, 1.0f);
            DrawObject(SHAPE_CYLINDER, model, modelLocation);
        }

        // 오른쪽 뒤 바퀴
        {
            glm::mat4 model(1.0f);

            model = glm::translate(
                model,
                glm::vec3(0.25f + 0.06f, 0.5f - 0.1f, -0.5f + 0.18f)
            );

            model = glm::rotate(
                model,
                glm::radians(90.0f),
                glm::vec3(0.0f, 0.0f, 1.0f)
            );

            model = glm::scale(
                model,
                glm::vec3(0.5f, 0.2f, 0.5f)
            );

            glUniform4f(colorLocation, 0.15f, 0.15f, 0.15f, 1.0f);
            DrawObject(SHAPE_CYLINDER, model, modelLocation);
        }
    }
}

int main()
{
    #pragma region 초기 설정
    if (!glfwInit())
    {
        cerr << "GLFW 초기화 실패!" << endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800,800,"OpenGL Practice 20",nullptr,nullptr);

    if (!window)
    {
        cerr << "윈도우 생성 실패!" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        cerr << "GLEW 초기화 실패!" << endl;

        glfwDestroyWindow(window);
        glfwTerminate();

        return -1;
    }

    MakeVertexShader();
    MakeFragmentShader();
    shaderProgramID = MakeShaderProgram();

    InitBuffer();

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    #pragma endregion

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        InputProcess(window);
        UpdateScene();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, 800, 800);
        DrawScene(viewmode);

        glfwSwapBuffers(window);
    }

    #pragma region 정리
    glDeleteProgram(shaderProgramID);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
    #pragma endregion
}