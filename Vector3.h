#ifndef _VECTOR3_H_
#define _VECTOR3_H_

struct Vector3 {
	float X, Y, Z;
};
struct Vector3i16 {
	short X, Y, Z;
};
struct Vector3i8 {
	char X, Y, Z;
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
namespace Math {
#endif

	void __fastcall NormalizeInPlace(Vector3 *InVec3);

#ifdef __cplusplus
} // namespace
#endif


#ifdef __cplusplus
}
#endif


#endif