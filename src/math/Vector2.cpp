#include <cmath>
#include <algorithm>
template<typename T>
constexpr Vector2<T>::Vector2() : x(0), y(0) {}

template<typename T>
constexpr Vector2<T>::Vector2(T x, T y) : x(x), y(y) {}

template<typename T>
constexpr Vector2<T>::Vector2(sf::Vector2<T> v) : x(v.x), y(v.y) { }

template<typename T>
constexpr T Vector2<T>::dot(const Vector2& other) const { return x * other.x + y * other.y; }

template<typename T>
constexpr T Vector2<T>::norm() const { return std::sqrt(norm_sq()); }

template<typename T>
constexpr T Vector2<T>::norm_sq() const { return dot(*this); }

template<typename T>
constexpr T Vector2<T>::angle() const { return std::atan2(y, x); }

template<typename T>
constexpr Vector2<T> Vector2<T>::normalized() const { return *this * (1.0f / norm()); }

template <typename T>
constexpr Vector2<T> Vector2<T>::rotate(float angle) const {
	return Vector2f(x * std::cos(angle) - y * std::sin(angle),
					x * std::sin(angle) + y * std::cos(angle));
}

template <typename T>
constexpr Vector2<T> Vector2<T>::clamp(float minX, float minY, float maxX, float maxY) const {
	return Vector2(std::clamp(x, minX, maxX), std::clamp(y, minY, maxY));
}

// Operators
template<typename T>
constexpr Vector2<T> Vector2<T>::operator+(const Vector2 &other) const {
	return Vector2(x + other.x, y + other.y);
}
template<typename T>
constexpr Vector2<T> Vector2<T>::operator-(const Vector2 &other) const {
	return Vector2(x - other.x, y - other.y);
}
template<typename T>
constexpr Vector2<T> Vector2<T>::operator-() const {
	 return Vector2(-x, -y); }

template<typename T>
constexpr Vector2<T> Vector2<T>::operator*(T scalar) const {
	return Vector2(scalar * x, scalar * y);
}

// template<typename T>
// Vector2<T> operator*(T scalar, const Vector2<T> &vector) const {
// 	return vector * scalar;
// }

// Conversion
template<typename T>
sf::Vector2<T> Vector2<T>::toSFML() const {
	return sf::Vector2(x, y);
}

template<typename T>
template<typename U>
constexpr Vector2<U> Vector2<T>::to() const {
	return { (U)x, (U)y };
}

// template struct Vector2<int>;
// template struct Vector2<unsigned int>;
// template struct Vector2<float>;
// template struct Vector2<double>;
