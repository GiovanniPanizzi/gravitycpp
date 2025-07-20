#include "../../../include/classes/generalPurpose/Draw.hpp"
#include <iostream>
#include <functional>
#include <vector>

struct Matrix4x4 {
    float m[16];
};

// function to create an identity matrix
Matrix4x4 identityMatrix() {
    Matrix4x4 mat = {};
    for (int i = 0; i < 16; i++) mat.m[i] = 0.0f;
    mat.m[0] = 1.0f;
    mat.m[5] = 1.0f;
    mat.m[10] = 1.0f;
    mat.m[15] = 1.0f;
    return mat;
}

//simple planet shaders
const char* vertexShaderSimplePlanet = R"glsl(
#version 330 core
layout(location = 0) in vec2 aPos;
out vec2 localPos;
uniform mat4 uModel;
uniform mat4 uProjection;

void main() {
    localPos = aPos;
    gl_Position = uProjection * uModel * vec4(aPos, 0.0, 1.0);
}
)glsl";

const char* fragmentShaderSimplePlanet = R"glsl(
#version 330 core
in vec2 localPos;
out vec4 aColor;

uniform vec4 uColor;

void main() {
    float r2 = (localPos.x - 0.5) * (localPos.x - 0.5) + ((localPos.y - 0.5) * (localPos.y - 0.5));
    if(r2 < 0.1){
        aColor = uColor;
    }
    else{
        aColor = vec4(0.0, 0.0, 0.0, 0.0);
    }
}
)glsl";

//rect shaders
const char* vertexShaderRect = R"glsl(
#version 330 core
layout(location = 0) in vec2 aPos;
out vec2 localPos;
uniform mat4 uModel;
uniform mat4 uProjection;

void main() {
    localPos = aPos;
    gl_Position = uProjection * uModel * vec4(aPos, 0.0, 1.0);
}
)glsl";

const char* fragmentShaderRect = R"glsl(
#version 330 core
in vec2 localPos;
out vec4 aColor;

uniform vec4 uColor;

void main() {
    aColor = uColor;
}
)glsl";

// function to create a scaled matrix
Matrix4x4 scaleMatrix(float sx, float sy, float sz = 1.0f) {
    Matrix4x4 mat = identityMatrix();
    mat.m[0] = sx;
    mat.m[5] = sy;
    mat.m[10] = sz;
    return mat;
}

Matrix4x4 translationMatrix(float tx, float ty) {
    Matrix4x4 mat = identityMatrix();
    mat.m[12] = tx;
    mat.m[13] = ty;
    return mat;
}

Matrix4x4 createOrthoTopLeft() {
    Matrix4x4 result = {};

    float left = 0.0f;
    float right = (float)screenWidth;
    float top = 0.0f;
    float bottom = (float)screenHeight;
    float near = -1.0f;
    float far = 1.0f;

    float rl = right - left;
    float tb = bottom - top;   // NOTA: bottom - top, NON top - bottom
    float fn = far - near;

    result.m[0]  = 2.0f / rl;
    result.m[5]  = -2.0f / tb;  // NOTA: negativo per invertire Y
    result.m[10] = -2.0f / fn;
    result.m[12] = -(right + left) / rl;
    result.m[13] = (bottom + top) / tb;  // positivo
    result.m[14] = -(far + near) / fn;
    result.m[15] = 1.0f;

    return result;
}

Matrix4x4 multiplyMatrix(const Matrix4x4& a, const Matrix4x4& b) {
    Matrix4x4 result = {};
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            result.m[col + row * 4] =
                a.m[0 + row * 4] * b.m[col + 0 * 4] +
                a.m[1 + row * 4] * b.m[col + 1 * 4] +
                a.m[2 + row * 4] * b.m[col + 2 * 4] +
                a.m[3 + row * 4] * b.m[col + 3 * 4];
        }
    }
    return result;
}

Matrix4x4 rotationMatrix(float angleRad) {
    Matrix4x4 mat = identityMatrix();
    float c = cos(angleRad);
    float s = sin(angleRad);
    mat.m[0] = c;
    mat.m[1] = -s;
    mat.m[4] = s;
    mat.m[5] = c;
    return mat;
}

Matrix4x4 createModelMatrixWithPivot(float width, float height, float angleRad, float pivotX, float pivotY) {
    Matrix4x4 T1 = translationMatrix(-pivotX, -pivotY); // trasla in origine pivot
    Matrix4x4 R = rotationMatrix(angleRad);             // ruota
    Matrix4x4 T2 = translationMatrix(pivotX, pivotY);  // trasla indietro
    Matrix4x4 S = scaleMatrix(width, height);           // scala

    // Model = T2 * R * T1 * S
    Matrix4x4 model = multiplyMatrix(T2, multiplyMatrix(R, multiplyMatrix(T1, S)));
    return model;
}

Matrix4x4 createModelMatrix(float width, float height) {
    return scaleMatrix(width, height, 1.0f);
}

GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // Controlla errori di compilazione
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Errore compilazione shader: " << infoLog << std::endl;
    }

    return shader;
}

GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Controlla errori di linking
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Errore linking shader program: " << infoLog << std::endl;
    }

    // Rimuove shader compilati (non più necessari dopo il linking)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

// ---DrawClass Implementation---
Draw::Draw(Window& window) : window(window) {
    float vertices[] = {
        0.0f, 0.0f,  // top-left
        1.0f, 0.0f,  // top-right
        1.0f, 1.0f,  // bottom-right
        0.0f, 1.0f   // bottom-left
    };

    glGenVertexArrays(1, &rectVAO);
    glGenBuffers(1, &rectVBO);

    glBindVertexArray(rectVAO);

    glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glViewport(0, 0, screenWidth, screenHeight);

    simplePlanetShaderProgram = createShaderProgram(vertexShaderSimplePlanet, fragmentShaderSimplePlanet);
    rectShaderProgram = createShaderProgram(vertexShaderRect, fragmentShaderRect);
}

Draw::~Draw() {
    if (rectVBO != 0) {
        glDeleteBuffers(1, &rectVBO);
        rectVBO = 0;
    }
    if (rectVAO != 0) {
        glDeleteVertexArrays(1, &rectVAO);
        rectVAO = 0;
    }
    if (simplePlanetShaderProgram != 0) {
        glDeleteProgram(simplePlanetShaderProgram);
        simplePlanetShaderProgram = 0;
    }
}

void Draw::drawFilledRotatedRect(float x, float y, float width, float height,
                                 float angleRad, float pivotX, float pivotY,
                                 Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    glUseProgram(rectShaderProgram);

    // Colore
    glUniform4f(glGetUniformLocation(rectShaderProgram, "uColor"),
                r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);

    // Calcola pivot in coordinate reali (non relative)
    float pivotRealX = pivotX;
    float pivotRealY = pivotY;

    // Matrici: ordine giusto delle trasformazioni
    Matrix4x4 T1 = translationMatrix(-pivotRealX, -pivotRealY);       // Sposta origine sul pivot
    Matrix4x4 S = scaleMatrix(width, height);                        // Scala rettangolo (0-1 → width-height)
    Matrix4x4 R = rotationMatrix(angleRad);                          // Ruota attorno al pivot
    Matrix4x4 T2 = translationMatrix(pivotRealX, pivotRealY);         // Riporta il pivot dov’era
    Matrix4x4 T3 = translationMatrix(x, y);                           // Posiziona globalmente il tutto

    // Composizione finale: T3 * T2 * R * S * T1
    Matrix4x4 model = multiplyMatrix(multiplyMatrix(multiplyMatrix(multiplyMatrix(S, T1), R), T2), T3);

    glUniformMatrix4fv(glGetUniformLocation(rectShaderProgram, "uModel"), 1, GL_FALSE, model.m);

    // Projection
    Matrix4x4 proj = createOrthoTopLeft();
    glUniformMatrix4fv(glGetUniformLocation(rectShaderProgram, "uProjection"), 1, GL_FALSE, proj.m);

    // Draw
    glBindVertexArray(rectVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);

    glUseProgram(0);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error after drawFilledRotatedRect: " << err << std::endl;
    }
}

void Draw::drawPlanet(Galaxy& currentGalaxy, size_t planetIndex){
    if(currentGalaxy.planets.layers[planetIndex].empty()){
        glUseProgram(simplePlanetShaderProgram);
        Uint8 r = 50;
        Uint8 g = 50;
        Uint8 b = 50;
        Uint8 a = 255;
        glUniform4f(glGetUniformLocation(simplePlanetShaderProgram, "uColor"),
                r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);

    } else{

    }
}

void Draw::clearScreen(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    // Set the clear color
    glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
//present
void Draw::present() {
    SDL_GL_SwapWindow(window.getWindow());
}

//camera Position and angle
void Draw::adjustCameraPosition(Galaxy& currentGalaxy) {
    Vec2 playerPosition = currentGalaxy.humans.positions[0];

    float offsetDistance = 50.0f;
    float direction = currentGalaxy.humans.directions[0].value;
    float angleRad = currentGalaxy.humans.angles[0].rad;

    float offsetX = offsetDistance * direction * cos(angleRad);
    float offsetY = offsetDistance * direction * sin(angleRad);

    float targetX = playerPosition.x + offsetX;
    float targetY = playerPosition.y + offsetY;

    float dist = distance(Vec2{targetX, targetY}, cameraPosition);

    float t = 0.01f;

    cameraPosition.x += (targetX - cameraPosition.x) * t * dist / 30.0f;
    cameraPosition.y += (targetY - cameraPosition.y) * t * dist / 30.0f;
    
    float targetAngle = currentGalaxy.humans.angles[0].rad;
    float angleDiff = targetAngle - cameraAngle;

    // Porta angleDiff in [-π, π]
    if (angleDiff > M_PI) {
        angleDiff -= 2 * M_PI;
    } else if (angleDiff < -M_PI) {
        angleDiff += 2 * M_PI;
    }

    // Ignora piccole variazioni (anti-scattino)
    if (fabs(angleDiff) < 0.01f) angleDiff = 0.0f;

    // Applica un easing leggero (più fluido)
    float easingFactor = 30.0f * (fabs(angleDiff) / M_PI); // più basso = più smorzato
    float baseDivisor = (currentGalaxy.humans.planetIndexes[0] == -1) ? 500.0f : 120.0f;
    float interp = t * dist / baseDivisor;

    // Limita interpolazione e smorza con easingFactor
    interp = std::min(interp, 0.001f); // previene salti
    cameraAngle += angleDiff * interp * easingFactor;

    // Normalizza cameraAngle in [0, 2π]
    cameraAngle = fmod(cameraAngle, 2 * M_PI);
    if (cameraAngle < 0) cameraAngle += 2 * M_PI;
}
