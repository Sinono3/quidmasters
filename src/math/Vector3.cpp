template<typename T>
Vector3<T>::Vector3() : x(0), y(0), z(0) {}

template<typename T>
Vector3<T>::Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

template<typename T>
Vector3<T>::Vector3(sf::Vector3<T> v) : x(v.x), y(v.y), z(v.z) { }
template<typename T>
Vector3<T>::Vector3(glm::vec3 v) : x(v.x), y(v.y), z(v.z) { }

template<typename T>
T Vector3<T>::dot(const Vector3& other) { return x * other.x + y * other.y + z * other.z; }

template<typename T>
T Vector3<T>::norm() { return sqrt(dot(*this)); }

// Operators
template<typename T>
Vector3<T> Vector3<T>::operator+(const Vector3 &other) {
	return Vector3(x + other.x, y + other.y, z + other.z);
}
template<typename T>
Vector3<T> Vector3<T>::operator-(const Vector3 &other) {
	return Vector3(x - other.x, y - other.y, z - other.z);
}
template<typename T>
Vector3<T> Vector3<T>::operator-() { return Vector3(-x, -y, -z); }

template<typename T>
Vector3<T> Vector3<T>::operator*(float scalar) {
	return Vector3(scalar * x, scalar * y, scalar * z);
}

template<typename T>
Vector3<T> operator*(float scalar, const Vector3<T> &vector) {
	return Vector3(scalar * vector.x, scalar * vector.y, scalar * vector.z);
}

// Conversion
template<typename T>
sf::Vector3<T> Vector3<T>::toSFML() {
	return sf::Vector3(x, y, z);
}
template<typename T>
glm::vec3 Vector3<T>::toGLM() {
	return glm::vec3((float)x, (float)y, (float)z);
}

template<typename T>
template<typename U>
Vector3<U> Vector3<T>::to() {
	return { (U)x, (U)y, (U)z };
}

template struct Vector3<int>;
template struct Vector3<unsigned int>;
template struct Vector3<float>;
template struct Vector3<double>;
