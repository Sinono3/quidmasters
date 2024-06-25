#pragma once
#include <SFML/System.hpp>
#include <glm/vec3.hpp>

template <typename T> struct Vector3 {
	T x, y, z;
	Vector3();
	Vector3(T x, T y, T z);

	// Constructor from other libraries' types
	Vector3(sf::Vector3<T>);
	Vector3(glm::vec3);

	T dot(const Vector3& other);
	T norm();
	
	// Operators
	Vector3 operator+(const Vector3 &other);
	Vector3 operator-(const Vector3 &other);
	Vector3 operator-();
	Vector3 operator*(float scalar);
	// friend Vector3 operator*(float scalar, const Vector3 &vector);

	// Conversion
	sf::Vector3<T> toSFML();
	glm::vec3 toGLM();
	template<typename U>
	Vector3<U> to();
};

using Vector3f = Vector3<float>;
using Vector3i = Vector3<int>;
#include "Vector3.cpp"
