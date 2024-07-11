#pragma once
#include <SFML/System/Vector2.hpp>

template <typename T>
struct Vector2 {
	T x, y;
	constexpr Vector2();
	constexpr Vector2(T x, T y);

	// Constructor from other libraries' types
	constexpr Vector2(sf::Vector2<T>);

	constexpr T dot(const Vector2& other) const;
	constexpr T norm() const;
	constexpr T norm_sq() const;
	constexpr T angle() const;
	constexpr Vector2 normalized() const;
	constexpr Vector2 rotate(float angle) const;
	constexpr Vector2 clamp(float minX, float minY, float maxX, float maxY) const;
	
	// Operators
	constexpr Vector2 operator+(const Vector2 &other) const;
	constexpr Vector2 operator-(const Vector2 &other) const;
	constexpr Vector2 operator-() const;
	constexpr Vector2 operator*(T scalar) const;
	// friend Vector2 operator*(T scalar, const Vector2 &vector);

	// Conversion
	sf::Vector2<T> toSFML() const;
	template<typename U>
	constexpr Vector2<U> to() const;
};

using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;
#include "Vector2.cpp"
