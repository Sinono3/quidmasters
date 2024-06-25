#pragma once
#include <SFML/System.hpp>
#include <glm/vec2.hpp>

template <typename T> struct Vector2 {
	T x, y;
	Vector2();
	Vector2(T x, T y);

	// Constructor from other libraries' types
	Vector2(sf::Vector2<T>);
	Vector2(glm::vec2);

	T dot(const Vector2& other);
	T norm();
	
	// Operators
	Vector2 operator+(const Vector2 &other);
	Vector2 operator-(const Vector2 &other);
	Vector2 operator-();
	Vector2 operator*(T scalar);
	// friend Vector2 operator*(T scalar, const Vector2 &vector);

	// Conversion
	sf::Vector2<T> toSFML();
	glm::vec2 toGLM();
	template<typename U>
	Vector2<U> to();
};

using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;
#include "Vector2.cpp"
