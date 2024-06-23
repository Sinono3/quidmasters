template <typename T> struct Vector2 {
	T x, y;
	Vector2(T x, T y) : x(x), y(y) {}
	Vector2 operator+(const Vector2 &other) {
		return {x + other.x, y + other.y};
	}
	Vector2 operator-(const Vector2 &other) {
		return {x - other.x, y - other.y};
	}
	Vector2 operator-() { return {-x, -y}; }
	T operator*(const Vector2 &other) { return {x * other.x, y * other.y}; }
};
