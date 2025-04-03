#include "pch.h"
#include "shenmue.h"
#include "Vector3.h"
#include <math.h>


// @datetime 2025-03-19 16:24:00.370564
// @exhash 596b0294c8a1c3a28aae9e9b5b537335d0ac6390af9d903899d55a3c3d9c76bc
// @confirmed
#pragma runtime_checks("", off)
#pragma optimize("gt", on) 
extern "C" FUNC void __fastcall Math::NormalizeInPlace(Vector3 * InVec3)
{
    const float x = InVec3->X;
    const float y = InVec3->Y;
    const float z = InVec3->Z;
    float magnitude = sqrtf((float)((float)(x * x) + (float)(y * y)) + (float)(z * z));
    if (magnitude > 0.0f)
    {
        InVec3->X = x * (float)(1.0f / magnitude);
        InVec3->Y = y * (float)(1.0f / magnitude);
        InVec3->Z = z * (float)(1.0f / magnitude);
    }
}

