// SiNi vector class template definitions

namespace sini {

// CONSTRUCTORS
// =============================================================================

// Initialize with values
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n>::Vector(const T* data_ptr) noexcept
{
    for (int i = 0; i < static_cast<int>(n); i++)
        components[i] = data_ptr[i];
}
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n>::Vector(const T (&init_arr)[n]) noexcept
{
    for (int i = 0; i < static_cast<int>(n); i++)
        components[i] = init_arr[i];
}
template<typename T, uint32_t n>
template<typename T2>
SINI_CUDA_COMPAT Vector<T,n>::Vector(const Vector<T2,n>& other) noexcept
{
    for (int i = 0; i < static_cast<int>(n); i++)
        components[i] = static_cast<T>(other.components[i]);
}
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n>::Vector(T init_val)
{
    for (int i = 0; i < static_cast<int>(n); i++)
        components[i] = init_val;
}

// 2D
//------------------------------------------------------------------------------
template<typename T>
SINI_CUDA_COMPAT Vector<T,2>::Vector(const T* data_ptr) noexcept
    : x(data_ptr[0]),
      y(data_ptr[1])
{}
template<typename T>
SINI_CUDA_COMPAT Vector<T,2>::Vector(const T (&init_arr)[2]) noexcept
    : x(init_arr[0]),
      y(init_arr[1])
{}
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
//------------------------------------------------------------------------------
template<typename T>
SINI_CUDA_COMPAT Vector<T,3>::Vector(const T* data_ptr) noexcept
    : x(data_ptr[0]),
      y(data_ptr[1]),
      z(data_ptr[2])
{}
template<typename T>
SINI_CUDA_COMPAT Vector<T,3>::Vector(const T (&init_arr)[3]) noexcept
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
//------------------------------------------------------------------------------
template<typename T>
SINI_CUDA_COMPAT Vector<T,4>::Vector(const T* data_ptr) noexcept
    : x(data_ptr[0]),
      y(data_ptr[1]),
      z(data_ptr[2]),
      w(data_ptr[3])
{}
template<typename T>
SINI_CUDA_COMPAT Vector<T,4>::Vector(const T (&init_arr)[4]) noexcept
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


// Printing and to string
// =============================================================================
namespace {
template<typename T, uint32_t n>
std::string formatString(const Vector<T,n>& vec, const char* format)
{
    char buf[20];
    std::sprintf(buf, format, vec[0]);
    std::string str = "(";
    str += buf;
    for (int i = 1; i < static_cast<int>(n); i++) {
        str += ", ";
        std::sprintf(buf, format, vec[i]);
        str += buf;
    }
    str += ")";
    return str;
}
}

template<uint32_t n>
std::string toString(const Vector<int32_t,n>& vec)
{
    return formatString(vec, "%d");
}

template<uint32_t n>
std::string toString(const Vector<float,n>& vec)
{
    return formatString(vec, "%g");
}

template<uint32_t n>
std::string toString(const Vector<double,n>& vec)
{
    return formatString(vec, "%g");
}

template<typename T, uint32_t n>
std::string toFormattedString(const Vector<T,n>& vec, const char* format)
{
    return formatString(vec, format);
}

template<typename T, uint32_t n>
std::ostream& operator<< (std::ostream& ostream, const Vector<T,n>& vec)
{
    return ostream << toString(vec);
}


// Math functions
// =============================================================================

// Dot/scalar product
template<typename T, uint32_t n>
SINI_CUDA_COMPAT T dot(const Vector<T,n>& v1, const Vector<T,n>& v2) noexcept
{
    T sum = T(0);
    for (int i = 0; i < static_cast<int>(n); i++)
        sum += v1.components[i] * v2.components[i];
    return sum;
}

// Cross product
template<typename T>
SINI_CUDA_COMPAT Vector<T,3> cross(Vector<T,3> v1, Vector<T,3> v2) noexcept
{
    return Vector<T, 3>(v1.y*v2.z - v1.z*v2.y,
                        v1.z*v2.x - v1.x*v2.z,
                        v1.x*v2.y - v1.y*v2.x);
}

// Vector norm
template<uint32_t n>
SINI_CUDA_COMPAT float norm(const Vector<int32_t,n>& v, int p) noexcept
{
    assert(p > 0);
    if (p == 2) return length(v);
    else return norm(v, static_cast<float>(p));
}
template<uint32_t n>
SINI_CUDA_COMPAT float norm(const Vector<int32_t,n>& v, float p) noexcept
{
    assert(p > 0.0f);
    float sum = 0.0f;
    for (int i = 0; i < static_cast<int>(n); i++)
        sum += std::pow(std::abs(static_cast<float>(v.components[i])), p);

    return std::pow(sum, 1.0f / p);
}

template<uint32_t n>
SINI_CUDA_COMPAT float norm(const Vector<float,n>& v, int p) noexcept
{
    assert(p > 0);
    if (p == 2) return length(v);
    else return norm(v, static_cast<float>(p));
}
template<uint32_t n>
SINI_CUDA_COMPAT float norm(const Vector<float,n>& v, float p) noexcept
{
    assert(p > 0.0f);
    float sum = 0.0f;
    for (int i = 0; i < static_cast<int>(n); i++)
        sum += std::pow(std::abs(v.components[i]), p);

    return std::pow(sum, 1.0f / p);

}

template<uint32_t n>
SINI_CUDA_COMPAT double norm(const Vector<double,n>& v, int p) noexcept
{
    assert(p > 0);
    if (p == 2) return length(v);
    else return norm(v, static_cast<double>(p));
}
template<uint32_t n>
SINI_CUDA_COMPAT double norm(const Vector<double,n>& v, double p) noexcept
{
    assert(p > 0.0);
    double sum = 0.0;
    for (int i = 0; i < static_cast<int>(n); i++)
        sum += std::pow(std::abs(v.components[i]), p);

    return std::pow(sum, 1.0 / p);
}

// The Euclidean length is the same as the 2-norm
template<uint32_t n>
SINI_CUDA_COMPAT float length(const Vector<int32_t,n>& v) noexcept
{
    return std::sqrt(static_cast<float>(dot(v, v)));
}
template<typename T, uint32_t n>
SINI_CUDA_COMPAT T length(const Vector<T,n>& v) noexcept
{
    return T(std::sqrt(dot(v, v)));
}

// Squared norm
// By default normPowered is the same as lengthSquared, but not when
// choosing an arbitrary norm (p-norm, p != 2)
template<typename T, uint32_t n>
SINI_CUDA_COMPAT T normPowered(const Vector<T,n>& v, int p) noexcept
{
    assert(p > 0);
    T sum = T(0);
    for (int i = 0; i < static_cast<int>(n); i++) {
        T abs_elem = (v.components[i] > T(0)) ? v.components[i] : -v.components[i],
              temp = abs_elem;
        for (int k = 1; k < p; k++)
            temp *= abs_elem;
        sum += temp;
    }
    return sum;
}

template<uint32_t n>
SINI_CUDA_COMPAT float normPowered(const Vector<int32_t,n>& v, float p) noexcept
{
    assert(p > 0.0f);
    float sum = 0.0f;
    for (int i = 0; i < static_cast<int>(n); i++)
        sum += std::pow(std::abs(static_cast<float>(v.components[i])), p);
    return sum;
}
template<uint32_t n>
SINI_CUDA_COMPAT float normPowered(const Vector<float,n>& v, float p) noexcept
{
    assert(p > 0.0f);
    float sum = 0.0f;
    for (int i = 0; i < static_cast<int>(n); i++)
        sum += std::pow(std::abs(v.components[i]), p);
    return sum;
}
template<uint32_t n>
SINI_CUDA_COMPAT double normPowered(const Vector<double,n>& v, double p) noexcept
{
    assert(p > 0.0);
    double sum = 0.0;
    for (int i = 0; i < static_cast<int>(n); i++)
        sum += std::pow(std::abs(v.components[i]), p);
    return sum;
}

template<typename T, uint32_t n>
SINI_CUDA_COMPAT T lengthSquared(const Vector<T,n>& v) noexcept
{
    return T(dot(v, v));
}

// Abs
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n> abs(const Vector<T,n>& v) noexcept
{
    Vector<T, n> temp = v;
    for (int i = 0; i < static_cast<int>(n); i++) {
        if (temp.components[i] < 0) temp.components[i] *= T(-1);
    }
    return temp;
}

// Normalization
template<uint32_t n>
SINI_CUDA_COMPAT Vector<float,n> normalize(const Vector<int32_t,n>& v)
{
    float norm_2 = length(v);
    assert(norm_2 > 0);

    Vector<float, n> temp = v;
    return temp /= norm_2;
}
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n> normalize(const Vector<T,n>& v)
{
    T norm_2 = length(v);
    assert(norm_2 > 0);

    Vector<T, n> temp = v;
    return temp /= norm_2;
}

// Max and min element
template<typename T, uint32_t n>
SINI_CUDA_COMPAT T maxElement(const Vector<T,n>& vec) noexcept
{
    T max_found = std::numeric_limits<T>::lowest();
    for (int i = 0; i < static_cast<int>(n); i++)
        if (vec[i] > max_found) max_found = vec[i];
    return max_found;
}

template<typename T, uint32_t n>
SINI_CUDA_COMPAT T minElement(const Vector<T,n>& vec) noexcept
{
    T min_found = std::numeric_limits<T>::max();
    for (int i = 0; i < static_cast<int>(n); i++)
        if (vec[i] < min_found) min_found = vec[i];
    return min_found;
}

// Hash function taken from sfzCore: Vector.inl, hashing algorithm is the
// same as hash_combine from boost
template<typename T, uint32_t n>
SINI_CUDA_COMPAT size_t hash(const Vector<T,n>& v) noexcept
{
    std::hash<T> hasher;
    size_t hash = 0;
    for (int i = 0; i < static_cast<int>(n); i++)
        hash ^= hasher(v.components[i]) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    return hash;
}


// Operators
// =============================================================================

// Equality
template<typename T, uint32_t n>
SINI_CUDA_COMPAT bool operator== (const Vector<T,n>& left, const Vector<T,n>& right) noexcept
{
    for (int i = 0; i < static_cast<int>(n); i++)
        if (left.components[i] != right.components[i]) return false;
    return true;
}

// Inequality
template<typename T, uint32_t n>
SINI_CUDA_COMPAT bool operator!= (const Vector<T,n>& left, const Vector<T,n>& right) noexcept
{
    return !(left == right);
}

// Addition
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n>& operator+= (Vector<T,n>& left, const Vector<T,n>& right) noexcept
{
    for (int i = 0; i < static_cast<int>(n); i++)
        left.components[i] += right.components[i];
    return left;
}
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n> operator+ (const Vector<T,n>& left, const Vector<T,n>& right) noexcept
{
    Vector<T, n> temp = left;
    return temp += right;
}

// Subtraction
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n>& operator-= (Vector<T,n>& left, const Vector<T,n>& right) noexcept
{
    for (int i = 0; i < static_cast<int>(n); i++)
        left.components[i] -= right.components[i];
    return left;
}
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n> operator- (const Vector<T,n>& left, const Vector<T,n>& right) noexcept
{
    Vector<T, n> temp = left;
    return temp -= right;
}

// Negation
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n> operator- (const Vector<T,n>& vector) noexcept
{
    Vector<T, n> temp;
    for (int i = 0; i < static_cast<int>(n); i++)
        temp[i] = -vector[i];
    return temp;
}

// Multiplication with scalar
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n>& operator*= (Vector<T,n>& vector, T scalar) noexcept
{
    for (int i = 0; i < static_cast<int>(n); i++)
        vector.components[i] *= scalar;
    return vector;
}
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n> operator* (const Vector<T,n>& vector, T scalar) noexcept
{
    Vector<T, n> temp = vector;
    return temp *= scalar;
}
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n> operator* (T scalar, const Vector<T,n>& vector) noexcept
{
    return vector*scalar;
}

// Element-wise multiplication
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n>& operator*= (Vector<T,n>& left, const Vector<T,n>& right) noexcept
{
    for (int i = 0; i < static_cast<int>(n); i++)
        left.components[i] *= right.components[i];
    return left;
}
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n> operator* (const Vector<T,n>& left, const Vector<T,n>& right) noexcept
{
    Vector<T, n> temp = left;
    return temp *= right;
}

// Division with scalar
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n>& operator/= (Vector<T,n>& vector, T scalar) noexcept
{
    assert(scalar != T(0));
    for (int i = 0; i < static_cast<int>(n); i++)
        vector.components[i] /= scalar;
    return vector;
}
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n> operator/ (const Vector<T,n>& vector, T scalar) noexcept
{
    Vector<T, n> temp = vector;
    return temp /= scalar;
}

// Element-wise division
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n>& operator/= (Vector<T,n>& left, const Vector<T,n>& right) noexcept
{
    for (int i = 0; i < static_cast<int>(n); i++)
        left.components[i] /= right.components[i];
    return left;
}
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n> operator/ (const Vector<T,n>& left, const Vector<T,n>& right) noexcept
{
    Vector<T, n> temp = left;
    return temp /= right;
}

// Element-wise modulus
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n>& operator%= (Vector<T,n>& left, const Vector<T,n>& right) noexcept
{
    for (int i = 0; i < static_cast<int>(n); i++)
        left[i] %= right[i];
    return left;
}
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n> operator% (const Vector<T,n>& left, const Vector<T,n>& right) noexcept
{
    Vector<T, n> temp = left;
    return temp %= right;
}
// Modulus with scalar
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n>& operator%= (Vector<T,n>& vector, T scalar) noexcept
{
    for (int i = 0; i < static_cast<int>(n); i++)
        vector[i] %= scalar;
    return vector;
}
template<typename T, uint32_t n>
SINI_CUDA_COMPAT Vector<T,n> operator% (const Vector<T,n>& vector, T scalar) noexcept
{
    Vector<T, n> temp = vector;
    return temp %= scalar;
}

// Indexing (access to vector components)
// General vector
template<typename T, uint32_t n>
SINI_CUDA_COMPAT T& Vector<T,n>::operator[] (int index) noexcept
{
    assert(index >= 0);
    assert(index < static_cast<int>(n));
    return components[index];
}
template<typename T, uint32_t n>
SINI_CUDA_COMPAT const T& Vector<T,n>::operator[] (int index) const noexcept
{
    assert(index >= 0);
    assert(index < static_cast<int>(n));
    return components[index];
}
// 2D vector
template<typename T>
SINI_CUDA_COMPAT T& Vector<T,2>::operator[] (int index) noexcept
{
    assert(index >= 0);
    assert(index < 2);
    return components[index];
}
template<typename T>
SINI_CUDA_COMPAT const T& Vector<T,2>::operator[] (int index) const noexcept
{
    assert(index >= 0);
    assert(index < 2);
    return components[index];
}
// 3D vector
template<typename T>
SINI_CUDA_COMPAT T& Vector<T,3>::operator[] (int index) noexcept
{
    assert(index >= 0);
    assert(index < 3);
    return components[index];
}
template<typename T>
SINI_CUDA_COMPAT const T& Vector<T,3>::operator[] (int index) const noexcept
{
    assert(index >= 0);
    assert(index < 3);
    return components[index];
}
// 4D vector
template<typename T>
SINI_CUDA_COMPAT T& Vector<T,4>::operator[] (int index) noexcept
{
    assert(index >= 0);
    assert(index < 4);
    return components[index];
}
template<typename T>
SINI_CUDA_COMPAT const T& Vector<T,4>::operator[] (int index) const noexcept
{
    assert(index >= 0);
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
    bool operator() (const sini::Vector<T,n>& v1, const sini::Vector<T,n>& v2) const
    {
        for (int i = 0; i < static_cast<int>(n); i++) {
            if (v1.components[i] == v2.components[i]) continue;
            return v1.components[i] < v2.components[i];
        }
        return false;
    }
};

} // namespace std
