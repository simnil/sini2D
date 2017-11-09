// SiNi vector class template definitions
//
// This code has been heavily inspired by the Vector.hpp code in sfzCore,
// written by my friend Peter Hillerström (github.com/PetorSFZ).
// The quality of this code does not neccessarily represent the quality of
// sfzCore or any of his work.

#pragma once
namespace sini {

	// CONSTRUCTORS
	// =========================================================================

	// Initialize with values
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT Vector<T,n>::Vector(const T* init_arr) noexcept {

		for (uint32_t i = 0; i < n; i++)
			components[i] = init_arr[i];
	}
	template<typename T, uint32_t n>
	template<typename T2>
	SINI_CUDA_COMPAT Vector<T,n>::Vector(const Vector<T2,n>& other) noexcept {

		for (uint32_t i = 0; i < n; i++)
			components[i] = static_cast<T>(other.components[i]);
	}
	template<typename T,uint32_t n>
	SINI_CUDA_COMPAT Vector<T,n>::Vector(T init_val) {

		for (uint32_t i = 0; i < n; i++)
			components[i] = init_val;
	}

	template<typename T>
	SINI_CUDA_COMPAT Vector<T,2>::Vector(const T* init_arr) noexcept
		: x(init_arr[0]),
		  y(init_arr[1])
	{}

	// 2D
	//----------------------------
	template<typename T>
	template<typename T2>
	SINI_CUDA_COMPAT Vector<T,2>::Vector(const Vector<T2,2>& other) noexcept
		: x(static_cast<T>(other.x)),
		  y(static_cast<T>(other.y))
	{}

	template<typename T>
	SINI_CUDA_COMPAT Vector<T,2>::Vector(T init_val) noexcept
		: x(init_val),
		  y(init_val)
	{}

	template<typename T>
	SINI_CUDA_COMPAT Vector<T,2>::Vector(T x, T y) noexcept
		: x(x),
		  y(y)
	{}


	// 3D
	//----------------------------
	template<typename T>
	SINI_CUDA_COMPAT Vector<T,3>::Vector(const T* init_arr) noexcept
		: x(init_arr[0]),
		  y(init_arr[1]),
		  z(init_arr[2])
	{}

	template<typename T>
	template<typename T2>
	SINI_CUDA_COMPAT Vector<T,3>::Vector(const Vector<T2,3>& other) noexcept
		: x(static_cast<T>(other.x)),
		  y(static_cast<T>(other.y)),
		  z(static_cast<T>(other.z))
	{}

	template<typename T>
	SINI_CUDA_COMPAT Vector<T,3>::Vector(T init_val) noexcept
		: x(init_val),
		  y(init_val),
		  z(init_val)
	{}

	template<typename T>
	SINI_CUDA_COMPAT Vector<T,3>::Vector(T x, T y, T z) noexcept
		: x(x),
		  y(y),
		  z(z)
	{}

	template<typename T>
	SINI_CUDA_COMPAT Vector<T,3>::Vector(Vector<T,2> xy, T z) noexcept
		: xy(xy),
		  z_(z)
	{}

	template<typename T>
	SINI_CUDA_COMPAT Vector<T,3>::Vector(T x, Vector<T,2> yz) noexcept
		: x_(x),
		  yz(yz)
	{}

	// 4D
	//----------------------------
	template<typename T>
	SINI_CUDA_COMPAT Vector<T,4>::Vector(const T* init_arr) noexcept
		: x(init_arr[0]),
		  y(init_arr[1]),
		  z(init_arr[2]),
		  w(init_arr[3])
	{}

	template<typename T>
	template<typename T2>
	SINI_CUDA_COMPAT Vector<T,4>::Vector(const Vector<T2,4>& other) noexcept
		: x(static_cast<T>(other.x)),
		  y(static_cast<T>(other.y)),
		  z(static_cast<T>(other.z)),
		  w(static_cast<T>(other.w))
	{}

	template<typename T>
	SINI_CUDA_COMPAT Vector<T,4>::Vector(T init_val) noexcept
		: x(init_val),
		  y(init_val),
		  z(init_val),
		  w(init_val)
	{}

	template<typename T>
	SINI_CUDA_COMPAT Vector<T,4>::Vector(T x, T y, T z, T w) noexcept
		: x(x),
		  y(y),
		  z(z),
		  w(w)
	{}

	template<typename T>
	SINI_CUDA_COMPAT Vector<T,4>::Vector(Vector<T,2> xy, T z, T w) noexcept
		: xy(xy),
		  zw(Vector<T,2>(z, w))
	{}

	template<typename T>
	SINI_CUDA_COMPAT Vector<T,4>::Vector(T x, T y, Vector<T,2> zw) noexcept
		: xy(Vector<T,2>(x,y)),
		  zw(zw)
	{}

	template<typename T>
	SINI_CUDA_COMPAT Vector<T,4>::Vector(Vector<T,2> xy, Vector<T,2> zw) noexcept
		: xy(xy),
		  zw(zw)
	{}

	template<typename T>
	SINI_CUDA_COMPAT Vector<T,4>::Vector(Vector<T,3> xyz, T w) noexcept
		: xyz(xyz),
		  w_(w)
	{}

	template<typename T>
	SINI_CUDA_COMPAT Vector<T,4>::Vector(T x, Vector<T,3> yzw) noexcept
		: x_(x),
		  yzw(yzw)
	{}

	template<typename T>
	SINI_CUDA_COMPAT Vector<T,4>::Vector(T x, Vector<T,2> yz, T w) noexcept
		: x__(x),
		  yz(yz),
		  w__(w)
	{}


	// Math functions
	// =========================================================================

	// Dot/scalar product
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT T dot(const Vector<T,n>& v1, const Vector<T,n>& v2) noexcept {

		T sum = T(0);
		for (uint32_t i = 0; i < n; i++)
			sum += v1.components[i] * v2.components[i];
		return sum;
	}

	// Cross product
	template<typename T>
	SINI_CUDA_COMPAT Vector<T,3> cross(Vector<T,3> v1, Vector<T,3> v2) noexcept {

		return Vector<T, 3>(v1.y*v2.z - v1.z*v2.y,
							v1.z*v2.x - v1.x*v2.z,
							v1.x*v2.y - v1.y*v2.x);
	}

	// Vector norm
	template<uint32_t n>
	SINI_CUDA_COMPAT float norm(const Vector<int32_t,n>& v, int32_t N) noexcept {

		if (N == 2) return length(v);
		else {
			float sum = 0.0f;
			for (uint32_t i = 0; i < n; i++)
				sum += std::pow(static_cast<float>(v.components[i]), N);

			return std::pow(sum, 1.0f / static_cast<float>(N));
		}
	}
	template<uint32_t n>
	SINI_CUDA_COMPAT float norm(const Vector<float,n>& v, int32_t N) noexcept {

		if (N == 2) return length(v);
		else {
			float sum = 0.0f;
			for (uint32_t i = 0; i < n; i++)
				sum += std::pow(v.components[i], N);
			
			return std::pow(sum, 1.0f / static_cast<float>(N));
		}
	}
	template<uint32_t n>
	SINI_CUDA_COMPAT double norm(const Vector<double,n>& v, int32_t N) noexcept {

		if (N == 2) return length(v);
		else {
			double sum = 0.0;
			for (uint32_t i = 0; i < n; i++)
				sum += std::pow(v.components[i], N);

			return std::pow(sum, 1.0 / static_cast<double>(N));
		}
	}
	// The Euclidean length is the same as the 2-norm
	template<uint32_t n>
	SINI_CUDA_COMPAT float length(const Vector<int32_t,n>& v) noexcept {

		return std::sqrt(static_cast<float>(dot(v, v)));
	}
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT T length(const Vector<T,n>& v) noexcept {

		return T(std::sqrt(dot(v, v)));
	}

	// Squared norm
	// By default normPowered is the same as lengthSquared, but not when
	// choosing an arbitrary norm (p-norm, p != 2)
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT T normPowered(const Vector<T,n>& v, int32_t N) noexcept {

		T sum = T(0);
		for (uint32_t i = 0; i < n; i++)
			sum += static_cast<T>(std::pow(v.components[i], N));
		return sum;
	}
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT T lengthSquared(const Vector<T,n>& v) noexcept {

		return T(dot(v, v));
	}


	// Abs
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT Vector<T,n> abs(const Vector<T,n>& v) noexcept {

		Vector<T, n> temp = v;
		for (uint32_t i = 0; i < n; i++) {
			if (temp.components[i] < 0) temp.components[i] *= T(-1);
		}
		return temp;
	}

	// Normalization
	template<uint32_t n>
	SINI_CUDA_COMPAT Vector<float,n> normalize(const Vector<int32_t,n>& v) {

		float norm_2 = length(v);
		assert(norm_2 > 0);

		Vector<float, n> temp = v;
		return temp /= norm_2;
	}
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT Vector<T,n> normalize(const Vector<T,n>& v) {

		T norm_2 = length(v);
		assert(norm_2 > 0);

		Vector<T, n> temp = v;
		return temp /= norm_2;
	}

	// Max and min element
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT T maxElement(const Vector<T,n>& vec) noexcept {
	
		T max_found = std::numeric_limits<T>::lowest();
		for (uint32_t i = 0; i < n; i++)
			if (vec[i] >= max_found) max_found = vec[i];
		return max_found;
	}
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT T minElement(const Vector<T,n>& vec) noexcept {
	
		T min_found = std::numeric_limits<T>::max();
		for (uint32_t i = 0; i < n; i++)
			if (vec[i] <= min_found) min_found = vec[i];
		return min_found;
	}

	// Hash function taken from sfzCore: Vector.inl, hashing algorithm is the
	// same as hash_combine from boost
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT size_t hash(const Vector<T,n>& v) noexcept {

		std::hash<T> hasher;
		size_t hash = 0;
		for (uint32_t i = 0; i < n; i++)
			hash ^= hasher(v.components[i]) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
		return hash;
	}


	// Operators
	// =========================================================================

	// Equality
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT bool operator== (const Vector<T,n>& left, const Vector<T,n>& right) noexcept {

		for (uint32_t i = 0; i < n; i++)
			if (left.components[i] != right.components[i]) return false;
		return true;
	}

	// Inequality
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT bool operator!= (const Vector<T,n>& left, const Vector<T,n>& right) noexcept {
		return !(left == right);
	}

	// Addition
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT Vector<T,n>& operator+= (Vector<T,n>& left, const Vector<T,n>& right) noexcept {

		for (uint32_t i = 0; i < n; i++)
			left.components[i] += right.components[i];
		return left;
	}
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT Vector<T,n> operator+ (const Vector<T,n>& left, const Vector<T,n>& right) noexcept {

		Vector<T, n> temp = left;
		return temp += right;
	}

	// Subtraction
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT Vector<T,n>& operator-= (Vector<T,n>& left, const Vector<T,n>& right) noexcept {

		for (uint32_t i = 0; i < n; i++)
			left.components[i] -= right.components[i];
		return left;
	}
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT Vector<T,n> operator- (const Vector<T,n>& left, const Vector<T,n>& right) noexcept {

		Vector<T, n> temp = left;
		return temp -= right;
	}

	// Negation
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT Vector<T,n> operator- (const Vector<T,n>& vector) noexcept {
		
		Vector<T, n> temp;
		for (uint32_t i = 0; i < n; i++)
			temp[i] = -vector[i];
		return temp;
	}

	// Multiplication with scalar
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT Vector<T,n>& operator*= (Vector<T,n>& vector, T scalar) noexcept {

		for (uint32_t i = 0; i < n; i++)
			vector.components[i] *= scalar;
		return vector;
	}
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT Vector<T,n> operator* (const Vector<T,n>& vector, T scalar) noexcept {

		Vector<T, n> temp = vector;
		return temp *= scalar;
	}
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT Vector<T,n> operator* (T scalar, const Vector<T,n>& vector) noexcept {

		return vector*scalar;
	}

	// Element-wise multiplication
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT Vector<T,n>& operator*= (Vector<T,n>& left, const Vector<T,n>& right) noexcept {

		for (uint32_t i = 0; i < n; i++)
			left.components[i] *= right.components[i];
		return left;
	}
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT Vector<T,n> operator* (const Vector<T,n>& left, const Vector<T,n>& right) noexcept {

		Vector<T, n> temp = left;
		return temp *= right;
	}

	// Division with scalar
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT Vector<T,n>& operator/= (Vector<T,n>& vector, T scalar) noexcept {

		assert(scalar != T(0));
		for (uint32_t i = 0; i < n; i++)
			vector.components[i] /= scalar;
		return vector;
	}
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT Vector<T,n> operator/ (const Vector<T,n>& vector, T scalar) noexcept {

		Vector<T, n> temp = vector;
		return temp /= scalar;
	}

	// Element-wise division
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT Vector<T,n>& operator/= (Vector<T,n>& left, const Vector<T,n>& right) noexcept {
		
		for (uint32_t i = 0; i < n; i++)
			left.components[i] /= right.components[i];
		return left;
	}
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT Vector<T,n>& operator/ (const Vector<T,n>& left, const Vector<T,n>& right) noexcept {
		
		Vector<T, n> temp = left;
		return temp /= right;
	}

	// Element-wise modulus
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT Vector<T,n>& operator%= (Vector<T,n>& left, const Vector<T,n>& right) noexcept {
		for (int i = 0; i < n; i++)
			left[i] %= right[i];
		return left;
	}
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT Vector<T,n> operator% (const Vector<T,n>& left, const Vector<T,n>& right) noexcept {
		Vector<T, n> temp = left;
		return temp %= right;
	}
	// Modulus with scalar
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT Vector<T,n>& operator%= (Vector<T,n>& vector, T scalar) noexcept {
		for (int i = 0; i < n; i++)
			vector[i] %= scalar;
		return vector;
	}
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT Vector<T,n> operator% (const Vector<T,n>& vector, T scalar) noexcept {
		Vector<T, n> temp = vector;
		return temp %= scalar;
	}

	// Indexation (access to vector components)
	// General vector
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT T& Vector<T,n>::operator[] (uint32_t index) noexcept {

		assert(index < n);
		return components[index];
	}
	template<typename T, uint32_t n>
	SINI_CUDA_COMPAT const T& Vector<T,n>::operator[] (uint32_t index) const noexcept {

		assert(index < n);
		return components[index];
	}
	// 2D vector
	template<typename T>
	SINI_CUDA_COMPAT T& Vector<T,2>::operator[] (uint32_t index) noexcept {

		assert(index < 2);
		return components[index];
	}
	template<typename T>
	SINI_CUDA_COMPAT const T& Vector<T,2>::operator[] (uint32_t index) const noexcept {

		assert(index < 2);
		return components[index];
	}
	// 3D vector
	template<typename T>
	SINI_CUDA_COMPAT T& Vector<T,3>::operator[] (uint32_t index) noexcept {

		assert(index < 3);
		return components[index];
	}
	template<typename T>
	SINI_CUDA_COMPAT const T& Vector<T,3>::operator[] (uint32_t index) const noexcept {

		assert(index < 3);
		return components[index];
	}
	// 4D vector
	template<typename T>
	SINI_CUDA_COMPAT T& Vector<T,4>::operator[] (uint32_t index) noexcept {

		assert(index < 4);
		return components[index];
	}
	template<typename T>
	SINI_CUDA_COMPAT const T& Vector<T,4>::operator[] (uint32_t index) const noexcept {

		assert(index < 4);
		return components[index];
	}

} // namespace sini

namespace std {

	// Hasher specialization in std
	template<typename T, uint32_t n>
	struct hash<sini::Vector<T,n>> {
		size_t operator() (const sini::Vector<T,n>& vector) const { return sini::hash(vector); }
	};

	// Ordering specialization in std (e.g. for use as 'key' in std::map)
	template<typename T, uint32_t n>
	struct less<sini::Vector<T,n>> {
		bool operator() (const sini::Vector<T,n>& v1, const sini::Vector<T,n>& v2) const {
			for (int i = 0; i < n; i++) {
				if (v1.components[i] == v2.components[i]) continue;
				return v1.components[i] < v2.components[i];
			}
			return false;
		}
	};
}
