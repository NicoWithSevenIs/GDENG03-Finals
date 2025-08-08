#pragma once
#include "Vector2D.h"
#include <cmath>

class Vector3D {

	public:
		float m_x, m_y, m_z;

	public:
		Vector3D(): m_x(0), m_y(0), m_z(0) {}
		Vector3D(float x, float  y, float  z) : m_x(x), m_y(y), m_z(z) {}
		Vector3D(const Vector3D& v): m_x(v.m_x), m_y(v.m_y), m_z(v.m_z){}
		~Vector3D(){}

		static Vector3D lerp(const Vector3D& start, const Vector3D& end, float t) 
		{
			Vector3D v;

			v.m_x = start.m_x * (1.f-t) + end.m_x * t; 
			v.m_y = start.m_y * (1.f - t) + end.m_y * t;
			v.m_z = start.m_z * (1.f - t) + end.m_z * t;


			return v;
		}

		Vector3D operator+(const Vector3D& other) {
			return Vector3D(m_x + other.m_x, m_y + other.m_y, m_z+other.m_z);
		}

		Vector3D operator-(const Vector3D& other) {
			return Vector3D(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z);
		}

		Vector3D operator*(const Vector3D& other) {
			return Vector3D(m_x * other.m_x, m_y * other.m_y, m_z * other.m_z);
		}

		Vector3D operator*(const float& scalar) {
			return Vector3D(m_x * scalar, m_y * scalar, m_z * scalar);
		}

		Vector3D operator/(const Vector3D& other) {
			return Vector3D(m_x / other.m_x, m_y / other.m_y, m_z / other.m_z);
		}

		void operator +=(const Vector3D& other) {
			m_x += other.m_x;
			m_y += other.m_y;
			m_z += other.m_z;
		}

		void operator *=(const Vector3D& other) {
			m_x *= other.m_x;
			m_y *= other.m_y;
			m_z *= other.m_z;
		}



		bool operator==(const Vector3D& other) const {
			return m_x == other.m_x && m_y == other.m_y && m_z == other.m_z;
		}

		bool operator!=(const Vector3D& other) const {
			return !(*this == other);
		}

		static Vector3D normalize(const Vector3D& v) {
			
			float mag = magnitude(v);

			if(mag == 0)
				return v;

			float d = 1/ mag;

			return Vector3D(v.m_x * d, v.m_y * d, v.m_z * d);

		}

		static float magnitude(const Vector3D& v) {
			return sqrt(v.m_x * v.m_x + v.m_y * v.m_y + v.m_z * v.m_z);
		}

		static float Dot(const Vector3D& a, const Vector3D& b) {
			return a.m_x * b.m_x + a.m_y * b.m_y + a.m_z * b.m_z;
		}

		static Vector3D Cross(const Vector3D& a, const Vector3D& b) {
			return Vector3D(
				a.m_y * b.m_z - a.m_z * b.m_y,
				a.m_z * b.m_x - a.m_x * b.m_z,
				a.m_x * b.m_y - a.m_y * b.m_x
			);
		}
};


struct vertex {
	Vector3D position;
	Vector2D m_texcoord;
	Vector3D color;
};
