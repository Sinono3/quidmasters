#pragma once
#include <SFML/System.hpp>
#include <glm/vec2.hpp>

template <typename T>
struct Vector2 {
	T x, y;
	Vector2();
	Vector2(T x, T y);

	// Constructor from other libraries' types
	Vector2(sf::Vector2<T>);
	Vector2(glm::vec2);

	T dot(const Vector2& other) const;
	T norm() const;
	Vector2 normalized() const;
	Vector2 rotate(float angle) const;
	
	// Operators
	Vector2 operator+(const Vector2 &other) const;
	Vector2 operator-(const Vector2 &other) const;
	Vector2 operator-() const;
	Vector2 operator*(T scalar) const;
	// friend Vector2 operator*(T scalar, const Vector2 &vector);

	// Conversion
	sf::Vector2<T> toSFML() const;
	glm::vec2 toGLM() const;
	template<typename U>
	Vector2<U> to() const;
};

using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;
#include "Vector2.cpp"
