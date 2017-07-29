// Definitions for SiNi Matrix class template
//
// This code has been inspired by the Matrix.hpp code in sfzCore,
// written by my friend Peter Hillerström (github.com/PetorSFZ).
// The quality of this code does not neccessarily represent the quality of
// sfzCore or any of his work.
//
// NOTE!
// Most functions are written with floating point values in mind, and some do
// therefore not always translate well to integers with undefined or undesired
// behaviour as a consquence. Until (if) this is fixed, consider casting to
// float/double when e.g. computing the inverse of a matrix (very few integer 
// matrices have integer matrix inverses!) or other more complicated operations.

#pragma once
namespace sini {
	// CONSTRUCTORS
	// =========================================================================
	// Initialization from value
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N>::Matrix(T init_val) {

		for (uint32_t i = 0; i < M; i++)
			row_vectors[i] = Vector<T, N>(init_val);
	}
	template<typename T, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,N,N>::Matrix(T init_val) {

		for (uint32_t i = 0; i < N; i++)
			row_vectors[i] = Vector<T, N>(init_val);
	}
	template<typename T>
	SINI_CUDA_COMPAT Matrix<T,2,2>::Matrix(T init_val) noexcept :
		a(init_val), b(init_val),
		c(init_val), d(init_val)
	{}
	template<typename T>
	SINI_CUDA_COMPAT Matrix<T,3,3>::Matrix(T init_val) noexcept :
		a(init_val), b(init_val), c(init_val),
		d(init_val), e(init_val), f(init_val),
		g(init_val), h(init_val), i(init_val)
	{}
	template<typename T>
	SINI_CUDA_COMPAT Matrix<T,4,4>::Matrix(T init_val) noexcept :
		e00(init_val), e01(init_val), e02(init_val), e03(init_val),
		e10(init_val), e11(init_val), e12(init_val), e13(init_val),
		e20(init_val), e21(init_val), e22(init_val), e23(init_val),
		e30(init_val), e31(init_val), e32(init_val), e33(init_val)
	{}

	// Initialization from (row-major ordered) array
	// -------------------------------------------------------------------------
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N>::Matrix(const T* init_arr) noexcept {

		T* elements = this->data();
		for (uint32_t i = 0; i < M*N; i++)
			elements[i] = init_arr[i];

		/* would this work as fast? slower?
		for (uint32_t i = 0; i < M; i++)
			for(uint32_t j = 0; j < N; j++)
				this->at(i,j) = init_arr[i*N + j];
		*/
	}
	template<typename T, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,N,N>::Matrix(const T* init_arr) noexcept {

		T* elements = this->data();
		for (uint32_t i = 0; i < N*N; i++)
			elements[i] = init_arr[i];
	}
	template<typename T>
	SINI_CUDA_COMPAT Matrix<T,2,2>::Matrix(const T* init_arr) noexcept {

		a = init_arr[0]; b = init_arr[1];
		c = init_arr[2]; d = init_arr[3];
	}
	template<typename T>
	SINI_CUDA_COMPAT Matrix<T,3,3>::Matrix(const T* init_arr) noexcept {

		a = init_arr[0]; b = init_arr[1]; c = init_arr[2];
		d = init_arr[3]; e = init_arr[4]; f = init_arr[5];
		g = init_arr[6]; h = init_arr[7]; i = init_arr[8];
	}
	template<typename T>
	SINI_CUDA_COMPAT Matrix<T,4,4>::Matrix(const T* init_arr) noexcept {

		e00 = init_arr[0]; // row 1
		e01 = init_arr[1];
		e02 = init_arr[2];
		e03 = init_arr[3];
		e10 = init_arr[4]; // row 2
		e11 = init_arr[5];
		e12 = init_arr[6];
		e13 = init_arr[7];
		e20 = init_arr[8]; // row 3
		e21 = init_arr[9];
		e22 = init_arr[10];
		e23 = init_arr[11];
		e30 = init_arr[12]; // row 4
		e31 = init_arr[13];
		e32 = init_arr[14];
		e33 = init_arr[15];
	}

	// Initialization from row vectors
	// -------------------------------------------------------------------------
	template<typename T>
	SINI_CUDA_COMPAT Matrix<T,2,2>::Matrix(Vector<T,2> row1, Vector<T,2> row2) noexcept {

		row_vectors[0] = row1;
		row_vectors[1] = row2;
	}
	template<typename T>
	SINI_CUDA_COMPAT Matrix<T,3,3>::Matrix(Vector<T,3> row1, Vector<T,3> row2,
		Vector<T,3> row3) noexcept {

		row_vectors[0] = row1;
		row_vectors[1] = row2;
		row_vectors[2] = row3;
	}
	template<typename T>
	SINI_CUDA_COMPAT Matrix<T,4,4>::Matrix(Vector<T,4> row1, Vector<T,4> row2,
		Vector<T,4> row3, Vector<T,4> row4) noexcept {

		row_vectors[0] = row1;
		row_vectors[1] = row2;
		row_vectors[2] = row3;
		row_vectors[3] = row4;
	}

	// Cast-constructor
	// -------------------------------------------------------------------------
	// General
	template<typename T, uint32_t M, uint32_t N>
	template<typename T2>
	SINI_CUDA_COMPAT Matrix<T,M,N>::Matrix(const Matrix<T2,M,N>& other) noexcept {

		T* data = this->data();
		const T2* o_data = other.data();
		for (uint32_t i = 0; i < M*N; i++)
			data[i] = static_cast<T>(o_data[i]);
	}
	// General square matrix
	template<typename T, uint32_t N>
	template<typename T2>
	SINI_CUDA_COMPAT Matrix<T,N,N>::Matrix(const Matrix<T2,N,N>& other) noexcept {

		T* data = this->data();
		const T2* o_data = other.data();
		for (uint32_t i = 0; i < N*N; i++)
			data[i] = static_cast<T>(o_data[i]);
	}
	// 2x2
	template<typename T>
	template<typename T2>
	SINI_CUDA_COMPAT Matrix<T,2,2>::Matrix(const Matrix<T2,2,2>& other) noexcept {

		row_vectors[0] = Vector<T, 2>(other.row_vectors[0]);
		row_vectors[1] = Vector<T, 2>(other.row_vectors[1]);
	}
	// 3x3
	template<typename T>
	template<typename T2>
	SINI_CUDA_COMPAT Matrix<T,3,3>::Matrix(const Matrix<T2,3,3>& other) noexcept {

		row_vectors[0] = Vector<T, 3>(other.row_vectors[0]);
		row_vectors[1] = Vector<T, 3>(other.row_vectors[1]);
		row_vectors[2] = Vector<T, 3>(other.row_vectors[2]);
	}
	// 4x4
	template<typename T>
	template<typename T2>
	SINI_CUDA_COMPAT Matrix<T,4,4>::Matrix(const Matrix<T2,4,4>& other) noexcept {

		row_vectors[0] = Vector<T, 4>(other.row_vectors[0]);
		row_vectors[1] = Vector<T, 4>(other.row_vectors[1]);
		row_vectors[2] = Vector<T, 4>(other.row_vectors[2]);
		row_vectors[3] = Vector<T, 4>(other.row_vectors[3]);
	}

	// ELEMENT ACCESS
	// =========================================================================
	// General
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT T& Matrix<T,M,N>::at(uint32_t i, uint32_t j) noexcept {

		assert(i < M);
		return row_vectors[i][j];
	}
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT T Matrix<T,M,N>::at(uint32_t i, uint32_t j) const noexcept {

		assert(i < M);
		return row_vectors[i][j];
	}
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Vector<T,M> Matrix<T,M,N>::column(uint32_t j) const noexcept {
	
		Vector<T, M> column;
		for (uint32_t i = 0; i < M; i++)
			column[i] = this->at(i, j);
		return column;
	}
	// General square matrix
	template<typename T, uint32_t N>
	SINI_CUDA_COMPAT T& Matrix<T,N,N>::at(uint32_t i, uint32_t j) noexcept {
		
		assert(i < N);
		return row_vectors[i][j];
	}
	template<typename T, uint32_t N>
	SINI_CUDA_COMPAT T Matrix<T,N,N>::at(uint32_t i, uint32_t j) const noexcept {
		
		assert(i < N);
		return row_vectors[i][j];
	}
	template<typename T, uint32_t N>
	SINI_CUDA_COMPAT Vector<T,N> Matrix<T,N,N>::column(uint32_t j) const noexcept {
		
		Vector<T, N> column;
		for (uint32_t i = 0; i < N; i++)
			column[i] = this->at(i, j);
		return column;
	}
	// 2x2
	template<typename T>
	SINI_CUDA_COMPAT T& Matrix<T,2,2>::at(uint32_t i, uint32_t j) noexcept {

		assert(i < 2);
		return row_vectors[i][j];
	}
	template<typename T>
	SINI_CUDA_COMPAT T Matrix<T,2,2>::at(uint32_t i, uint32_t j) const noexcept {
		
		assert(i < 2);
		return row_vectors[i][j];
	}
	template<typename T>
	SINI_CUDA_COMPAT Vector<T,2> Matrix<T,2,2>::column(uint32_t j) const noexcept {
	
		return Vector<T, 2>(this->at(0, j), this->at(1, j));
	}
	// 3x3
	template<typename T>
	SINI_CUDA_COMPAT T& Matrix<T,3,3>::at(uint32_t i, uint32_t j) noexcept {

		assert(i < 3);
		return row_vectors[i][j];
	}
	template<typename T>
	SINI_CUDA_COMPAT T Matrix<T,3,3>::at(uint32_t i, uint32_t j) const noexcept {
	
		assert(i < 3);
		return row_vectors[i][j];
	}
	template<typename T>
	SINI_CUDA_COMPAT Vector<T,3> Matrix<T,3,3>::column(uint32_t j) const noexcept {
	
		return Vector<T, 3>(
			this->at(0, j),
			this->at(1, j),
			this->at(2, j)
			);
	}
	// 4x4
	template<typename T>
	SINI_CUDA_COMPAT T& Matrix<T,4,4>::at(uint32_t i, uint32_t j) noexcept {
	
		assert(i < 4);
		return row_vectors[i][j];
	}
	template<typename T>
	SINI_CUDA_COMPAT T Matrix<T,4,4>::at(uint32_t i, uint32_t j) const noexcept {
	
		assert(i < 4);
		return row_vectors[i][j];
	}
	template<typename T>
	SINI_CUDA_COMPAT Vector<T,4> Matrix<T,4,4>::column(uint32_t j) const noexcept {
	
		return Vector<T, 4>(
			this->at(0, j),
			this->at(1, j),
			this->at(2, j),
			this->at(3, j)
			);
	}

	// SETTERS
	// =========================================================================
	// General
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT void Matrix<T,M,N>::set(uint32_t i, uint32_t j, T val) noexcept {
	
		this->at(i, j) = val;
	}
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT void Matrix<T,M,N>::setColumn(uint32_t j, const Vector<T,M>& col) noexcept {
	
		for (uint32_t i = 0; i < M; i++)
			this->at(i, j) = col.components[i];
	}
	// General square matrix
	template<typename T, uint32_t N>
	SINI_CUDA_COMPAT void Matrix<T,N,N>::set(uint32_t i, uint32_t j, T val) noexcept {

		this->at(i, j) = val;
	}
	template<typename T, uint32_t N>
	SINI_CUDA_COMPAT void Matrix<T,N,N>::setColumn(uint32_t j, const Vector<T,N>& col) noexcept {
		
		for (uint32_t i = 0; i < N; i++)
			this->at(i, j) = col.components[i];
	}
	// 2x2
	template<typename T>
	SINI_CUDA_COMPAT void Matrix<T,2,2>::set(uint32_t i, uint32_t j, T val) noexcept {
	
		this->at(i, j) = val;
	}
	template <typename T>
	SINI_CUDA_COMPAT void Matrix<T,2,2>::setColumn(uint32_t j, const Vector<T,2>& col) noexcept {
		
		this->at(0, j) = col.x;
		this->at(1, j) = col.y;
	}
	// 3x3
	template<typename T>
	SINI_CUDA_COMPAT void Matrix<T,3,3>::set(uint32_t i, uint32_t j, T val) noexcept {

		this->at(i, j) = val;
	}
	template<typename T>
	SINI_CUDA_COMPAT void Matrix<T,3,3>::setColumn(uint32_t j, const Vector<T,3>& col) noexcept {
		
		this->at(0, j) = col.x;
		this->at(1, j) = col.y;
		this->at(2, j) = col.z;
	}
	// 4x4
	template<typename T>
	SINI_CUDA_COMPAT void Matrix<T,4,4>::set(uint32_t i, uint32_t j, T val) noexcept {
		
		this->at(i, j) = val;
	}
	template<typename T>
	SINI_CUDA_COMPAT void Matrix<T,4,4>::setColumn(uint32_t j, const Vector<T,4>& col) noexcept {
		
		this->at(0, j) = col.x;
		this->at(1, j) = col.y;
		this->at(2, j) = col.z;
		this->at(3, j) = col.w;
	}

	// SUB-MATRICES
	// =========================================================================
	// 3x3
	template<typename T>
	SINI_CUDA_COMPAT Matrix<T,2,2> Matrix<T,3,3>::submatrix(uint32_t i, uint32_t j) const noexcept {
	
		assert(i < 3);
		assert(j < 3);
		Matrix<T, 2, 2> mat;
		// Add elements
		for (uint32_t k = 0; k < i; k++) {
			for (uint32_t l = 0; l < j; l++)
				mat.at(k, l) = this->at(k, l);
			// Skip column j
			for (uint32_t l = j + 1; l < 3; l++)
				mat.at(k, l - 1) = this->at(k, l);
		}
		// Skip row i
		for (uint32_t k = i + 1; k < 3; k++) {
			for (uint32_t l = 0; l < j; l++)
				mat.at(k - 1, l) = this->at(k, l);
			// Skip column j
			for (uint32_t l = j + 1; l < 3; l++)
				mat.at(k - 1, l - 1) = this->at(k, l);
		}
		return mat;
	}
	// 4x4
	template<typename T>
	SINI_CUDA_COMPAT Matrix<T,3,3> Matrix<T,4,4>::submatrix(uint32_t i, uint32_t j) const noexcept {
	
		assert(i < 4);
		assert(j < 4);
		Matrix<T, 3, 3> mat;
		for (uint32_t k = 0; k < i; k++) {
			for (uint32_t l = 0; l < j; l++)
				mat.at(k, l) = this->at(k, l);
			// Skip column j
			for (uint32_t l = j + 1; l < 4; l++)
				mat.at(k, l - 1) = this->at(k, l);
		}
		// Skip row i
		for (uint32_t k = i + 1; k < 4; k++) {
			for (uint32_t l = 0; l < j; l++)
				mat.at(k - 1, l) = this->at(k, l);
			// Skip column j
			for (uint32_t l = j + 1; l < 4; l++)
				mat.at(k - 1, l - 1) = this->at(k, l);
		}
		return mat;
	}
	// General
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M-1,N-1> Matrix<T,M,N>::submatrix(uint32_t i, uint32_t j) const noexcept {
	
		static_assert(M > 1,
			"Cannot get submatrix for matrices with singleton dimensions");
		static_assert(N > 1,
			"Cannot get submatrix for matrices with singleton dimensions");
		assert(i < M);
		assert(j < N);
		Matrix<T, M - 1, N - 1> mat;
		for (uint32_t k = 0; k < i; k++) {
			for (uint32_t l = 0; l < j; l++)
				mat.at(k, l) = this->at(k, l);
			// Skip column j
			for (uint32_t l = j + 1; l < N; l++)
				mat.at(k, l - 1) = this->at(k, l);
		}
		// Skip row i
		for (uint32_t k = i + 1; k < M; k++) {
			for (uint32_t l = 0; l < j; l++)
				mat.at(k - 1, l) = this->at(k, l);
			// Skip column j
			for (uint32_t l = j + 1; l < N; l++)
				mat.at(k - 1, l - 1) = this->at(k, l);
		}
	}

	// SPECIAL MATRICES
	// =========================================================================
	// Identity matrix
	// 2x2
	template<typename T>
	SINI_CUDA_COMPAT Matrix<T,2,2> Matrix<T,2,2>::identity() noexcept  {

		return Matrix<T, 2, 2> {
			{T(1), T(0)},
			{T(0), T(1)}
		};
	}
	// 3x3
	template<typename T>
	SINI_CUDA_COMPAT Matrix<T,3,3> Matrix<T,3,3>::identity() noexcept  {
	
		return Matrix<T, 3, 3> {
			{T(1), T(0), T(0)},
			{T(0), T(1), T(0)},
			{T(0), T(0), T(1)}
		};
	}
	// 4x4
	template<typename T>
	SINI_CUDA_COMPAT Matrix<T,4,4> Matrix<T,4,4>::identity() noexcept  {
	
		return Matrix<T, 4, 4> {
			{T(1), T(0), T(0), T(0)},
			{T(0), T(1), T(0), T(0)},
			{T(0), T(0), T(1), T(0)},
			{T(0), T(0), T(0), T(1)}
		};
	}
	// General square matrix
	template<typename T, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,N,N> Matrix<T,N,N>::identity() noexcept  {
	
		Matrix<T, N, N> id{ T(0) };
		for (uint32_t i = 0; i < N; i++)
			id.at(i, i) = T(1);
		return id;
	}

	// MATH FUNCTIONS (AND OTHER UTILITIES)
	// =========================================================================

	// Conversion between column vectors (sini::Vector) and row vectors
	// (sini::Matrix<T,1,N>)
	// Separated from transpose since they are separate structs
	// Might be changed in the future
	template<typename T, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,1,N> toRowVector(const Vector<T,N>& vec) noexcept {
	
		Matrix<T, 1, N> mat;
		T* data = mat.data();
		for (uint32_t i = 0; i < N; i++)
			data[i] = vec.components[i];
		return mat;
	}
	template<typename T, uint32_t N>
	SINI_CUDA_COMPAT Vector<T,N> toColumnVector(const Matrix<T,1,N>& mat) noexcept {

		return Vector<T, N>(mat.data());
	}

	// Element-wise matrix multiplication
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> eqElemMult(Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept {

		for (uint32_t i = 0; i < M; i++)
			left.row_vectors[i] *= right.row_vectors[i];
		return left;
	}
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> elemMult(const Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept {
	
		Matrix<T, M, N> mat = left;
		return eqElemMult(mat, right);
	}

	// Element-wise division
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> eqElemDiv(Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept {
	
		for (uint32_t i = 0; i < M; i++)
			left.row_vectors[i] /= right.row_vectors[i];
		return left;
	}
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> elemDiv(const Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept {
	
		Matrix<T, M, N> mat = left;
		return eqElemDiv(mat, right);
	}

	// Element-wise exponentiation
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> eqElemPow(Matrix<T,M,N>& mat, T exponent) noexcept {

		T* data = mat.data();
		for (uint32_t i = 0; i < M*N; i++)
			data[i] = std::pow(data[i], exponent);
		return mat;
	}
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> elemPow(const Matrix<T,M,N>& mat, T exponent) noexcept {

		Matrix<T, M, N> temp = mat;
		return eqElemPow(temp, exponent);
	}
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> eqElemPow(Matrix<T,M,N>& mat, uint32_t exponent) noexcept {

		T* data = mat.data();
		for (uint32_t i = 0; i < M*N; i++) {
			T temp = T(1);
			for (uint32_t j = 0; j < exponent; j++) temp *= data[i];
			data[i] = temp;
		}
		return mat;
	}
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> elemPow(const Matrix<T,M,N>& mat, uint32_t exponent) noexcept {
		
		Matrix<T, M, N> temp = mat;
		return eqElemPow(mat, exponent);
	}
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> eqElemPow(Matrix<T,M,N>& mat, const Matrix<T,M,N>& exp_mat) noexcept {

		T* mat_data = mat.data();
		const T* exp_data = exp_mat.data();
		for (uint32_t i = 0; i < M*N; i++)
			mat_data[i] = std::pow(mat_data[i], exp_data[i]);
		return mat;
	}
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> elemPow(const Matrix<T,M,N>& mat, const Matrix<T,M,N>& exp_mat) noexcept {
		
		Matrix<T, M, N> temp = mat;
		return eqElemPow(temp, exp_mat);
	}
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> eqElemPow(Matrix<T,M,N>& mat, const Matrix<uint32_t,M,N>& exp_mat) noexcept {
		
		T* mat_data = mat.data();
		const uint32_t* exp_data = exp_mat.data();
		for (uint32_t i = 0; i < M*N; i++) {
			T temp = T(1);
			for (uint32_t j = 0; j < exp_data[i]; j++) temp *= mat_data[i];
			mat_data[i] = temp;
		}
		return mat;
	}
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> elemPow(const Matrix<T,M,N>& mat, const Matrix<uint32_t,M,N>& exp_mat) noexcept {
	
		Matrix<T, M, N> temp = mat;
		return eqElemPow(temp, exp_mat);
	}

	// Transpose
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,N,M> transpose(const Matrix<T,M,N>& mat) noexcept {
	
		Matrix<T, N, M> transp;
		for (uint32_t i = 0; i < N; i++)
			for (uint32_t j = 0; j < M; j++)
				transp.at(i, j) = mat.at(j, i);
		return transp;
	}

	// Power
	template<typename T, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,N,N> pow(const Matrix<T,N,N>& mat, uint32_t exponent) noexcept {
	
		Matrix<T, N, N> temp = Matrix<T,N,N>::identity();
		for (uint32_t i = 0; i < exponent; i++)
			temp *= mat;
		return temp;
	}
	// This version feels rather redundant, and might be removed in the future
	template<typename T, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,N,N> eqPow(Matrix<T,N,N>& mat, uint32_t exponent) noexcept {

		if (exponent == 0)
			return mat = Matrix<T, N, N>::identity();
		Matrix<T, N, N> temp = mat;
		for (uint32_t i = 1; i < exponent; i++)
			mat *= temp;
		return mat;
	}

	// Hasher
	// Same as hash_combine from boost
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT size_t hash(const Matrix<T,M,N>& m) noexcept {
	
		std::hash<Vector<T,N>> hasher;
		size_t hash = 0;
		for (uint32_t i = 0; i < M; i++)
			hash ^= hasher(m.row_vectors) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
		return hash;
	}

	// Determinant
	template<typename T>
	SINI_CUDA_COMPAT T det(const Matrix<T,2,2>& mat) noexcept {
	
		return mat.a * mat.d - mat.b * mat.c;
	}
	template<typename T>
	SINI_CUDA_COMPAT T det(const Matrix<T,3,3>& mat) noexcept {
	
		return mat.a * mat.e * mat.i
			+ mat.b * mat.f * mat.g
			+ mat.c * mat.d * mat.h
			- mat.g * mat.e * mat.c
			- mat.h * mat.f * mat.a
			- mat.i * mat.d * mat.b;
	}
	template<typename T>
	SINI_CUDA_COMPAT T det(const Matrix<T,4,4>& mat) noexcept {
		
		// Compute determinant using sub-determinant expansion
		T subdet00 = mat.e11*mat.e22*mat.e33
			+ mat.e12*mat.e23*mat.e31
			+ mat.e13*mat.e21*mat.e32
			- mat.e31*mat.e22*mat.e13
			- mat.e32*mat.e23*mat.e11
			- mat.e33*mat.e21*mat.e12;
		T subdet01 = mat.e10*mat.e22*mat.e33
			+ mat.e12*mat.e23*mat.e30
			+ mat.e13*mat.e20*mat.e32
			- mat.e30*mat.e22*mat.e13
			- mat.e32*mat.e23*mat.e10
			- mat.e33*mat.e20*mat.e12;
		T subdet02 = mat.e10*mat.e21*mat.e33
			+ mat.e11*mat.e23*mat.e30
			+ mat.e13*mat.e20*mat.e31
			- mat.e30*mat.e21*mat.e13
			- mat.e31*mat.e23*mat.e10
			- mat.e33*mat.e20*mat.e11;
		T subdet03 = mat.e10*mat.e21*mat.e32
			+ mat.e11*mat.e22*mat.e30
			+ mat.e12*mat.e20*mat.e31
			- mat.e30*mat.e21*mat.e12
			- mat.e31*mat.e22*mat.e10
			- mat.e32*mat.e20*mat.e11;

		return mat.e00*subdet00 - mat.e01*subdet01 + mat.e02*subdet02
			- mat.e03*subdet03;
	}
	// For computing the determinant of an arbitrary matrix size include
	// "sini/math/MatrixMath.h", which adds more advanced matrix operations
	// like LU decomposition etc. (LU decomp. is used for computing larger
	// determinants.)

	// Submatrix determinant (a.k.a. minor)
	template<typename T>
	SINI_CUDA_COMPAT T minor(const Matrix<T,3,3>& mat, uint32_t i, uint32_t j) noexcept {
		
		assert(i < 3);
		assert(j < 3);
		// Find what elements are part of the submatrix
		uint32_t t_row = (i == 0) ? 1 : 0,
			b_row = (i == 2) ? 1 : 2,
			l_col = (j == 0) ? 1 : 0,
			r_col = (j == 2) ? 1 : 2;
		// and return its determinant
		return mat.at(t_row, l_col)*mat.at(b_row, r_col)
			- mat.at(b_row, l_col)*mat.at(t_row, r_col);
	}
	template<typename T>
	SINI_CUDA_COMPAT T minor(const Matrix<T,4,4>& mat, uint32_t i, uint32_t j) noexcept {
		
		assert(i < 4);
		assert(j < 4);
		//TODO optimize?
		return det(mat.submatrix(i, j));
	}

	// Adjugate matrix
	template<typename T>
	SINI_CUDA_COMPAT Matrix<T,2,2> adj(const Matrix<T,2,2>& mat) noexcept {
		
		return Matrix<T, 2, 2>	{{mat.d, -mat.c},
								{-mat.b, mat.a}};
	}
	template<typename T>
	SINI_CUDA_COMPAT Matrix<T,3,3> adj(const Matrix<T,3,3>& mat) noexcept {
		
		Matrix<T, 3, 3> adj;
		for (uint32_t i = 0; i < 3; i++) {
			for (uint32_t j = 0; j < 3; j++) {
				T sign = (i + j) % 2 == 0 ? T(1) : T(-1);
				adj.at(i, j) = sign*minor(mat, j, i);
			}
		}
		return adj;
	}
	template<typename T>
	SINI_CUDA_COMPAT Matrix<T,4,4> adj(const Matrix<T,4,4>& mat) noexcept {
		
		Matrix<T, 4, 4> adj;
		for (uint32_t i = 0; i < 4; i++) {
			for (uint32_t j = 0; j < 4; j++) {
				T sign = (i + j) % 2 == 0 ? T(1) : T(-1);
				adj.at(i, j) = sign*minor(mat, j, i);
			}
		}
		return adj;
	}

	// Element-wise abs
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> abs(const Matrix<T,M,N>& mat) noexcept {
	
		Matrix<T, M, N> abs_mat;
		T* abs_data = abs_mat.data(),
			data = mat.data();
		for (uint32_t i = 0; i < M*N; i++)
			abs_data[i] = (data[i] < T(0)) ? T(-1)*data[i] : data[i];
		return abs_mat;
	}

	// Inverse
	// Returns the inverse of a matrix, if it is invertible, otherwise returns
	// a matrix with zeros. Computes it using Laplace's adjugate formula, which
	// can lead to rounding errors for matrices with small determinants.
	// "sini/math/MatrixMath.h" provides better tools and for more general
	// matrices.
	template<typename T>
	SINI_CUDA_COMPAT Matrix<T,2,2> inverse(const Matrix<T,2,2>& mat) noexcept {

		T det_ = det(mat);
		if (det_ == T(0)) return Matrix<T, 2, 2>(T(0));
		Matrix<T, 2, 2> inv	{
			{mat.d, -mat.b},
			{-mat.c, mat.a}
		};
		return inv /= det_;
	}
	template<typename T>
	SINI_CUDA_COMPAT Matrix<T,3,3> inverse(const Matrix<T,3,3>& mat) noexcept {

		T det_ = det(mat);
		if (det_ == T(0)) return Matrix<T, 3, 3>(T(0));

		// Compute the adjugate matrix
		T minor00 = mat.e11*mat.e22 - mat.e21*mat.e21;
		T minor01 = mat.e10*mat.e22 - mat.e20*mat.e12;
		T minor02 = mat.e10*mat.e21 - mat.e20*mat.e11;
		T minor10 = mat.e01*mat.e22 - mat.e21*mat.e02;
		T minor11 = mat.e00*mat.e22 - mat.e20*mat.e02;
		T minor12 = mat.e00*mat.e21 - mat.e20*mat.e01;
		T minor20 = mat.e01*mat.e12 - mat.e11*mat.e02;
		T minor21 = mat.e00*mat.e12 - mat.e10*mat.e02;
		T minor22 = mat.e00*mat.e11 - mat.e10*mat.e01;

		Matrix<T, 3, 3> adj {	
			{minor00, -minor10, minor20},
			{-minor01, minor11, -minor21},
			{minor02, -minor12, minor22}
		};
		return adj /= det_;
	}
	template<typename T>
	SINI_CUDA_COMPAT Matrix<T,4,4> inverse(const Matrix<T,4,4>& mat) noexcept {
		
		T det_ = det(mat);
		if (det_ == 0) return Matrix<T, 4, 4>(T(0));

		// Compute adjugate matrix
		Matrix<T, 4, 4> adj;
		for (uint32_t i = 0; i < 4; i++) {
			for (uint32_t j = 0; j < 4; j++) {
				T sign = (i + j) % 2 == 0 ? T(1) : T(-1);
				adj.at(i, j) = sign*minor(mat, j, i);
			}
		}
		return adj /= det_;
	}


	// OPERATORS
	// =========================================================================

	// Equality and inequality
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT bool operator== (const Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept {

		for (uint32_t i = 0; i < M; i++)
			if (left.row_vectors[i] != right.row_vectors[i]) return false;
		return true;
	}
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT bool operator!= (const Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept {
	
		return !(left == right);
	}

	// Addition
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> operator+= (Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept {
	
		for (uint32_t i = 0; i < M; i++)
			left.row_vectors[i] += right.row_vectors[i];
		return left;
	}
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> operator+ (const Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept {
	
		Matrix<T, M, N> mat = left;
		return mat += right;
	}

	// Subtraction
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> operator-= (Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept {
	
		for (uint32_t i = 0; i < M; i++)
			left.row_vectors[i] -= right.row_vectors[i];
		return left;
	}
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> operator- (const Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept {
	
		Matrix<T, M, N> mat = left;
		return mat -= right;
	}

	// Negation
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> operator- (const Matrix<T,M,N>& mat) noexcept {
	
		Matrix<T, M, N> neg_mat;
		T* neg_data = neg_mat.data();
		const T* data = mat.data();
		for (uint32_t i = 0; i < M*N; i++)
			neg_data[i] = -data[i];
		return neg_mat;
	}

	// Multiplication for general matrices
	template<typename T, uint32_t M, uint32_t N, uint32_t O>
	SINI_CUDA_COMPAT Matrix<T,M,O> operator* (const Matrix<T,M,N>& left, const Matrix<T,N,O>& right) noexcept {
	
		Matrix<T, M, O> mat;
		for (uint32_t i = 0; i < M; i++)
			for (uint32_t j = 0; j < O; j++)
				mat.at(i, j) = dot(left.row_vectors[i], right.column(j));
		return mat;
	}
	// Multiplication for square matrices
	template<typename T, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,N,N> operator*= (Matrix<T,N,N>& left, const Matrix<T,N,N>& right) noexcept {
	
		return left = left*right;
	}

	// Multiplication with vector (matrix*vector)
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Vector<T,M> operator* (const Matrix<T,M,N>& mat, const Vector<T,N>& vec) noexcept {
	
		Vector<T, M> result;
		for (uint32_t i = 0; i < M; i++)
			result[i] = dot(mat.row_vectors[i], vec);
		return result;
	}
	// vector*matrix multiplication must be done by converting the vector
	// to a row vector, i.e. a 1xN matrix. The reason for this design is
	// to only allow vector*matrix multiplication when you really want it
	// (and hopefully know what you're doing)

	// Multiplication with scalar
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> operator*= (Matrix<T,M,N>& mat, T scalar) noexcept {
	
		T* data = mat.data();
		for (uint32_t i = 0; i < M*N; i++)
			data[i] = data[i] * scalar;
		return mat;
	}
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> operator* (const Matrix<T,M,N>& mat, T scalar) noexcept {
	
		Matrix<T, M, N> temp = mat;
		return temp *= scalar;
	}
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> operator* (T scalar, const Matrix<T,M,N>& mat) noexcept {
	
		return mat*scalar;
	}

	// Division with scalar
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> operator/= (Matrix<T,M,N>& mat, T scalar) noexcept {
	
		T* data = mat.data();
		// Return a matrix with zeroes if division with 0 is attempted
		// Or is there a better way?
		if (scalar != T(0)) {
			for (uint32_t i = 0; i < M*N; i++)
				data[i] /= scalar;
		} else {
			for (uint32_t i = 0; i < M*N; i++)
				data[i] = 0;
		}
		return mat;
	}
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> operator/ (const Matrix<T,M,N>& mat, T scalar) noexcept {
	
		Matrix<T, M, N> temp = mat;
		return temp /= scalar;
	}

	//TODO Keep this?
	// Operator version of pow
	template<typename T, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,N,N> operator^ (const Matrix<T,N,N>& mat, uint32_t exponent) {

		return pow(mat, exponent);
	}
} // namespace sini

namespace std {
	// Hasher specialization in std
	template<typename T, uint32_t M, uint32_t N>
	struct hash<sini::Matrix<T,M,N>> {
		size_t operator() (const sini::Matrix<T,M,N>& mat) { return sini::hash(mat); }
	};
}