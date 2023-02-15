#pragma once
#include "..\SDK\VMatrix.h"

#define PI 3.14159265358979323846f

class Math {
public:

	void VectorAngles(const Vector &vecForward, Vector &vecAngles);

	void VectorAngles(const Vector& forward, Vector& up, Vector& angles);

	void VectorTransform(const Vector in1, const matrix3x4_t in2, Vector &out);

	void VectorTransform2(const Vector in1, const matrix3x4_t in2, Vector& out);

	Vector CalcAngle(const Vector& vecSource, const Vector& vecDestination);

	vec_t VectorNormalize(Vector& v);

	void AngleVectors(const Vector &angles, Vector *forward);

	void NormalizeAngles(Vector& angles);

	float NormalizeYaw(float yaw);

	float NormalizeVal(float val, float min, float max);

	void AngleVectors(const Vector& angles, Vector* forward, Vector* right, Vector* up);

	void RandomSeed(int seed);

	float RandomFloat(float min, float max);

	bool Clamp(Vector &angles);

	void ClampAngles(Vector &angles);

	float GRD_TO_BOG(float GRD);

	float GetFov(const Vector& viewAngle, const Vector& aimAngle);

	float degrees_to_radians(float deg);

	Vector get_rotated_position(Vector start, const float rotation, const float distance);

	float Distance(Vector2D point1, Vector2D point2);

	void AngleMatrix(const Vector angles, matrix3x4_t& matrix);

	void AngleMatrix2(const Vector &angles, const Vector &position, matrix3x4_t& matrix_out);

	void MatrixSetColumn(const Vector &in, int column, matrix3x4_t& out);

	void MatrixCopy(const matrix3x4_t& source, matrix3x4_t& target);

	void MatrixMultiply(matrix3x4_t& in1, const matrix3x4_t& in2);

	void VectorRotate(const float *in1, const matrix3x4_t& in2, float *out);

	void VectorRotate(const Vector& in1, const matrix3x4_t &in2, Vector &out);

	void VectorRotate(const Vector &in1, const Vector &in2, Vector &out);

	void RGBtoHSV(float& fR, float& fG, float& fB, float& fH, float& fS, float& fV);

	void HSVtoRGB(float& fR, float& fG, float& fB, float& fH, float& fS, float& fV);

	Vector ProjectPoint(Vector Origin, double Yaw, double Distance);

	bool InScreen(int x, int y, int screenwidth, int screenheight);
}; 
extern Math g_Math;