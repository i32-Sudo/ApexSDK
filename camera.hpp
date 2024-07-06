#include "../Driver/API/Driver.h"
#include "../utils/vector.h"
#include "structs.h"
#include "../utils/pimraryXor.h"
#include "offsets.hpp"

int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);

struct Matrix {
    float matrix[16];
};

uint64_t get_view_render() {
    uint64_t renderer = Driver->RPM<uint64_t>(Driver->BaseAddress + OFF_VIEWRENDER);
    return renderer;
}

Matrix get_view_matrix(uint64_t renderer) {
    Matrix matrix = { };
    if (renderer == NULL) return matrix;
    uint64_t matrixx = Driver->RPM<uint64_t>(renderer + OFF_VIEWMATRIX);
    if (matrixx == NULL) return matrix;

    matrix = Driver->RPM<Matrix>(matrixx);
    return matrix;
}

static struct Vector3 WorldToScreen(const struct Vector3 pos, struct Matrix matrix) {
    struct Vector3 out;
    Vector3 None;
    float x = matrix.matrix[0] * pos.x + matrix.matrix[1] * pos.y + matrix.matrix[2] * pos.z + matrix.matrix[3];
    float y = matrix.matrix[4] * pos.x + matrix.matrix[5] * pos.y + matrix.matrix[6] * pos.z + matrix.matrix[7];
    out.z = matrix.matrix[12] * pos.x + matrix.matrix[13] * pos.y + matrix.matrix[14] * pos.z + matrix.matrix[15];

    x *= 1.f / out.z;
    y *= 1.f / out.z;

    int width = screenWidth;
    int height = screenHeight;

    out.x = width * .5f;
    out.y = height * .5f;

    out.x += 0.5f * x * width + 0.5f;
    out.y -= 0.5f * y * height + 0.5f;

    return out;
};