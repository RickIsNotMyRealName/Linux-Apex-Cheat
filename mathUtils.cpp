#include "mathUtils.h"
#include "entity.h"
#include <random>


//got some of  this from misterY on UC 
void Math::ClampAngles(QAngle& angle)
{
	while (angle.x > 89.0f)
		angle.x -= 180.f;

	while (angle.x < -89.0f)
		angle.x += 180.f;

	while (angle.y > 180.f)
		angle.y -= 360.f;

	while (angle.y < -180.f)
		angle.y += 360.f;
}

QAngle Math::CalcAngle(const Vector& src, const Vector& dst)
{
	QAngle angle = QAngle();
	SVector delta = SVector((src.x - dst.x), (src.y - dst.y), (src.z - dst.z));

	double hyp = sqrt(delta.x*delta.x + delta.y * delta.y);

	angle.x = atan(delta.z / hyp) * (180.0f / M_PI);
	angle.y = atan(delta.y / delta.x) * (180.0f / M_PI);
	angle.z = 0;
	if (delta.x >= 0.0) angle.y += 180.0f;

	return angle;
}

double Math::GetFov(const QAngle& viewAngle, const QAngle& aimAngle)
{
	QAngle delta = aimAngle - viewAngle;
	ClampAngles(delta);

	return sqrt(pow(delta.x, 2.0f) + pow(delta.y, 2.0f));
}

double Math::DotProduct(const Vector& v1, const float* v2)
{
	return v1.x * v2[0] + v1.y * v2[1] + v1.z * v2[2];
}

// Returns a linearly interpolated vector between this vector and other
QAngle Math::interpolate(QAngle This, QAngle other, float t) 
{
	QAngle out;

	out.x = This.x + (other.x - This.x) * t;
    out.y = This.y + (other.y - This.y) * t;
	out.z = This.z + (other.z - This.z) * t;
	return out;
}

// Returns a vector scaled by s amount.
QAngle Math::scale(QAngle preScale, float s) 
{
	QAngle out;
	out.x = preScale.x * s;
	out.y = preScale.y * s;
	out.z = preScale.z * s;
    return out;
}
float Math::random(float min, float max) //range : [min, max]
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distr(min, max); // define the range

	return distr(gen);
}

void Math::normalize(QAngle &angle, float len)
{
	len = angle.Length();
	angle = angle/len;
}