#pragma once
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define MIN(a,b) (((a)<(b)) ? a : b)
#define MAX(a,b) (((a)>(b)) ? a : b)
#define CLAMP(x,lo,hi) (MIN ((hi), MAX ((lo), (x))))

////Compare two floats for equality
//bool cmpf(float a, float b) {
//	return (fabs(a - b) < 0.000001f); //NB using fixed epsilon not ideal. Prob ok for smallish a,b
//}
//bool cmpf_e(float a, float b, float eps) {
//	return (fabs(a - b) < eps);
//}

#pragma region Vectors




// data structures
union Vector2;
union Vector3;
union Vector4;

// vector functions
float length(const Vector2& v);
float length2(const Vector2& v);
Vector2 normalise(const Vector2& v);
float dot(const Vector2& a, const Vector2& b);
float get_squared_dist(Vector2 from, Vector2 to);

float length(const Vector3& v);
float length2(const Vector3& v);
Vector3 normalise(const Vector3& v);
float dot(const Vector3& a, const Vector3& b);
Vector3 cross(const Vector3& a, const Vector3& b);
float get_squared_dist(Vector3 from, Vector3 to);
float direction_to_heading(Vector2 d);
Vector2 heading_to_direction(float degrees);

// print functions
void print(const Vector2& v);
void print(const Vector3& v);
void print(const Vector4& v);

// const used to convert degrees into radians
#define TAU 2.0 * M_PI
#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444
#define ONE_RAD_IN_DEG 360.0 / (2.0 * M_PI) //57.2957795
#define DEG2RAD(a) ((a)*(M_PI/180.0))
#define RAD2DEG(a) ((a)*(180.0/M_PI))

//------------------------------------------------------------------------------
//Suppress anonymous struct warning
#ifdef __GNUC__
#pragma GCC diagnostic push
//This is awful but ignoring "-Wgnu-anonymous-struct" doesn't work on MinGW :(
#pragma GCC diagnostic ignored "-Wpedantic" 
#endif

#ifdef __clang__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wgnu-anonymous-struct"
#endif

#ifdef _MSC_VER
#pragma warning(push, disable: 4201)
#endif
//------------------------------------------------------------------------------

union Vector2 {
	struct {
		float v[2];
	};
	struct {
		float x, y;
	};

	Vector2() {}
	Vector2(float x, float y) {
		v[0] = x;
		v[1] = y;
	}
	Vector2(Vector3 v3); //fwd dec, can't define before vec3

	Vector2 operator+ (const Vector2& rhs) {
		Vector2 vc;
		vc.v[0] = v[0] + rhs.v[0];
		vc.v[1] = v[1] + rhs.v[1];
		return vc;
	}

	Vector2& operator+= (const Vector2& rhs) {
		v[0] += rhs.v[0];
		v[1] += rhs.v[1];
		return *this; // return self
	}

	Vector2 operator- (const Vector2& rhs) {
		Vector2 vc;
		vc.v[0] = v[0] - rhs.v[0];
		vc.v[1] = v[1] - rhs.v[1];
		return vc;
	}

	Vector2& operator-= (const Vector2& rhs) {
		v[0] -= rhs.v[0];
		v[1] -= rhs.v[1];
		return *this;
	}

	Vector2 operator+ (float rhs) {
		Vector2 vc;
		vc.v[0] = v[0] + rhs;
		vc.v[1] = v[1] + rhs;
		return vc;
	}

	Vector2 operator- (float rhs) {
		Vector2 vc;
		vc.v[0] = v[0] - rhs;
		vc.v[1] = v[1] - rhs;
		return vc;
	}

	Vector2 operator* (float rhs) {
		Vector2 vc;
		vc.v[0] = v[0] * rhs;
		vc.v[1] = v[1] * rhs;
		return vc;
	}

	Vector2 operator/ (float rhs) {
		Vector2 vc;
		vc.v[0] = v[0] / rhs;
		vc.v[1] = v[1] / rhs;
		return vc;
	}

	Vector2& operator*= (float rhs) {
		v[0] = v[0] * rhs;
		v[1] = v[1] * rhs;
		return *this;
	}

	Vector2& operator= (const Vector2& rhs) {
		v[0] = rhs.v[0];
		v[1] = rhs.v[1];
		return *this;
	}
	Vector2 operator- () {
		Vector2 vc;
		vc.v[0] = v[0] * (-1);
		vc.v[1] = v[1] * (-1);
		return vc;
	}
	bool operator== (const Vector2& rhs) {
		return ((fabs(v[0] - rhs.v[0]) < 0.000001f) &&
			(fabs(v[1] - rhs.v[1]) < 0.000001f));
	}
};


union Vector3 {
	struct {
		float v[3];
	};
	struct {
		float x, y, z;
	};

	Vector3() {}
	Vector3(float x, float y, float z) {
		v[0] = x;
		v[1] = y;
		v[2] = z;
	}
	Vector3(const Vector2& vv, float z) {
		v[0] = vv.v[0];
		v[1] = vv.v[1];
		v[2] = z;
	}
	Vector3(const Vector4& vv); //fwd dec, can't define before vec4

	Vector3 operator+ (const Vector3& rhs) {
		Vector3 vc;
		vc.v[0] = v[0] + rhs.v[0];
		vc.v[1] = v[1] + rhs.v[1];
		vc.v[2] = v[2] + rhs.v[2];
		return vc;
	}
	Vector3& operator+= (const Vector3& rhs) {
		v[0] += rhs.v[0];
		v[1] += rhs.v[1];
		v[2] += rhs.v[2];
		return *this; // return self
	}
	Vector3 operator- (const Vector3& rhs) {
		Vector3 vc;
		vc.v[0] = v[0] - rhs.v[0];
		vc.v[1] = v[1] - rhs.v[1];
		vc.v[2] = v[2] - rhs.v[2];
		return vc;
	}
	Vector3& operator-= (const Vector3& rhs) {
		v[0] -= rhs.v[0];
		v[1] -= rhs.v[1];
		v[2] -= rhs.v[2];
		return *this;
	}
	Vector3 operator+ (float rhs) {
		Vector3 vc;
		vc.v[0] = v[0] + rhs;
		vc.v[1] = v[1] + rhs;
		vc.v[2] = v[2] + rhs;
		return vc;
	}
	Vector3 operator- (float rhs) {
		Vector3 vc;
		vc.v[0] = v[0] - rhs;
		vc.v[1] = v[1] - rhs;
		vc.v[2] = v[2] - rhs;
		return vc;
	}
	Vector3 operator* (float rhs) {
		Vector3 vc;
		vc.v[0] = v[0] * rhs;
		vc.v[1] = v[1] * rhs;
		vc.v[2] = v[2] * rhs;
		return vc;
	}

	Vector3 operator* (Vector3 rhs) {
		Vector3 vc;
		vc.v[0] = v[0] * rhs.v[0];
		vc.v[1] = v[1] * rhs.v[1];
		vc.v[2] = v[2] * rhs.v[2];
		return vc;
	}

	Vector3 operator/ (float rhs) {
		Vector3 vc;
		vc.v[0] = v[0] / rhs;
		vc.v[1] = v[1] / rhs;
		vc.v[2] = v[2] / rhs;
		return vc;
	}
	Vector3& operator*= (float rhs) {
		v[0] = v[0] * rhs;
		v[1] = v[1] * rhs;
		v[2] = v[2] * rhs;
		return *this;
	}
	Vector3& operator= (const Vector3& rhs) {
		v[0] = rhs.v[0];
		v[1] = rhs.v[1];
		v[2] = rhs.v[2];
		return *this;
	}
	//Negate
	Vector3 operator- () {
		Vector3 vc;
		vc.v[0] = v[0] * (-1);
		vc.v[1] = v[1] * (-1);
		vc.v[2] = v[2] * (-1);
		return vc;
	}
	bool operator== (const Vector3& rhs) {
		return ((fabs(v[0] - rhs.v[0]) < 0.000001f) &&
			(fabs(v[1] - rhs.v[1]) < 0.000001f) &&
			(fabs(v[2] - rhs.v[2]) < 0.000001f));
	}
};

union Vector4 {
	struct {
		float v[4];
	};
	struct {
		float x, y, z, w;
	};
	struct {
		float r, g, b, a;
	};

	Vector4() {}
	Vector4(float x, float y, float z, float w) {
		v[0] = x;
		v[1] = y;
		v[2] = z;
		v[3] = w;
	}
	Vector4(const Vector2& vv, float z, float w) {
		v[0] = vv.v[0];
		v[1] = vv.v[1];
		v[2] = z;
		v[3] = w;
	}
	Vector4(const Vector3& vv, float w) {
		v[0] = vv.v[0];
		v[1] = vv.v[1];
		v[2] = vv.v[2];
		v[3] = w;
	}
};


/*------------------------------VECTOR FUNCTIONS------------------------------*/
//---vec2---//
inline Vector2::Vector2(Vector3 v3) {//Truncation ctor
	v[0] = v3.v[0];
	v[1] = v3.v[1];
}
inline float length(const Vector2& v) {
	return sqrt(v.v[0] * v.v[0] + v.v[1] * v.v[1]);
}
// squared length
inline float length2(const Vector2& v) {
	return v.v[0] * v.v[0] + v.v[1] * v.v[1];
}
// returns unit vector in direction of v
inline Vector2 normalise(const Vector2& v) {
	Vector2 vb;
	float l = length(v);
	if (0.0f == l) {
		return Vector2(0.0f, 0.0f);
	}
	vb.v[0] = v.v[0] / l;
	vb.v[1] = v.v[1] / l;
	return vb;
}
inline float dot(const Vector2& a, const Vector2& b) {
	return a.v[0] * b.v[0] + a.v[1] * b.v[1];
}
inline float get_squared_dist(Vector2 from, Vector2 to) {
	float x = (to.v[0] - from.v[0]) * (to.v[0] - from.v[0]);
	float y = (to.v[1] - from.v[1]) * (to.v[1] - from.v[1]);
	return x + y;
}

//---vec3---//
inline Vector3::Vector3(const Vector4& vv) {
	v[0] = vv.v[0];
	v[1] = vv.v[1];
	v[2] = vv.v[2];
}
inline float length(const Vector3& v) {
	return sqrt(v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2]);
}
// squared length
inline float length2(const Vector3& v) {
	return v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2];
}
// returns unit vector in direction of v
inline Vector3 normalise(const Vector3& v) {
	Vector3 vb;
	float l = length(v);
	if (0.0f == l) {
		return Vector3(0.0f, 0.0f, 0.0f);
	}
	vb.v[0] = v.v[0] / l;
	vb.v[1] = v.v[1] / l;
	vb.v[2] = v.v[2] / l;
	return vb;
}
inline float dot(const Vector3& a, const Vector3& b) {
	return a.v[0] * b.v[0] + a.v[1] * b.v[1] + a.v[2] * b.v[2];
}
inline Vector3 cross(const Vector3& a, const Vector3& b) {
	float x = a.v[1] * b.v[2] - a.v[2] * b.v[1];
	float y = a.v[2] * b.v[0] - a.v[0] * b.v[2];
	float z = a.v[0] * b.v[1] - a.v[1] * b.v[0];
	return Vector3(x, y, z);
}
inline float get_squared_dist(Vector3 from, Vector3 to) {
	float x = (to.v[0] - from.v[0]) * (to.v[0] - from.v[0]);
	float y = (to.v[1] - from.v[1]) * (to.v[1] - from.v[1]);
	float z = (to.v[2] - from.v[2]) * (to.v[2] - from.v[2]);
	return x + y + z;
}
//converts a 2D direction vector into a heading in degrees
//angle vector makes with positive x-axis
inline float direction_to_heading(Vector2 dir) {
	return atan2(dir.v[1], dir.v[0]) * ONE_RAD_IN_DEG;
}
//converts an angle in degrees to a 2D vector which makes that angle with the positive x-axis
inline Vector2 heading_to_direction(float degrees) {
	float rad = degrees * ONE_DEG_IN_RAD;
	return Vector2(cos(rad), sin(rad));
	//return vec3 (-sinf (rad), 0.0f, -cosf (rad));
}

/*-----------------------------PRINT FUNCTIONS--------------------------------*/
inline void print(const Vector2& v) {
	printf("[%.2f, %.2f]\n", v.v[0], v.v[1]);
}

inline void print(const Vector3& v) {
	printf("[%.2f, %.2f, %.2f]\n", v.v[0], v.v[1], v.v[2]);
}

inline void print(const Vector4& v) {
	printf("[%.2f, %.2f, %.2f, %.2f]\n", v.v[0], v.v[1], v.v[2], v.v[3]);
}



#ifdef __clang__
#pragma GCC diagnostic pop
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#pragma endregion

