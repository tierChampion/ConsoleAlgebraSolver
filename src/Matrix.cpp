#include "Matrix.h"

/// <summary>
/// Implementation of the basic matrix operations.
/// </summary>

namespace als {

	/**
	* Initialisation and allocation of the matrix
	* @param height of matrix (m)
	* @param width of matrix (n)
	*/
	Matrix::Matrix(int m, int n) : _m(m), _n(n) {

		_A = std::shared_ptr<double[]>(new double[_m * _n]);
	}

	/**
	* Construct an identity matrix of a desired size.
	* @param dim size of the matrix
	*/
	Matrix Matrix::Identity(int dim) {

		Matrix I = Matrix::Null(dim);

		for (int d = 0; d < dim; d++) {
			I(d, d) = 1;
		}

		return I;
	}

	/**
	* Construct a null matrix of the desired size.
	* @param dim size of the matrix
	*/
	Matrix Matrix::Null(int dim) {
		Matrix O(dim, dim);

		for (int a = 0; a < dim * dim; a++) {
			O(a) = 0;
		}

		return O;
	}

	/**
	* Setting of the elements in the matrix.
	* The data must be organized left to right, top to bottom
	*
	* @param data to put in the matrix
	*/
	void Matrix::fill(double* B) {

		for (int a = 0; a < _m * _n; a++) {
			_A.get()[a] = B[a];
		}
	}

	/**
	* Show the matrix in the console
	*/
	void Matrix::print() const {
		std::cout << "[ ";
		for (int j = 0; j < _m; j++) {
			for (int i = 0; i < _n; i++) {
				std::cout << (*this)(j, i) << " ";
			}

			if (j == _m - 1) std::cout << "] ";
			std::cout << "\n  ";
		}
		std::cout << std::endl;
	}

	/**
	* Rank of the matrix. Number of non-null rows.
	*/
	int Matrix::rank() const {

		int rank = 0;

		for (int j = 0; j < _m; j++) {

			double absSum = 0;

			for (int i = 0; i < _n; i++) {
				absSum += abs((*this)(j, i));
			}

			if (absSum != 0) rank++;
		}

		return rank;
	}

	/**
	* Trace of the matrix. Sum of the diagonal of a square matrix.
	*/
	double Matrix::trace() const {

		if (!isSquare()) {
			std::cerr << "ERROR: The matrix is not square, the trace is not defined. \n" << std::endl;
			return 0;
		}

		double trace = 0;

		for (int i = 0; i < _m; i++) {
			trace += (*this)(i, i);
		}

		return trace;
	}

	/**
	* Matrix transposition. Swap the rows with the columns.
	*/
	Matrix Matrix::transpose() const {

		Matrix res(_n, _m);

		for (int j = 0; j < _m; j++) {
			for (int i = 0; i < _n; i++) {
				res(i, j) = (*this)(j, i);
			}
		}
		return res;
	}

	/**
	* Get the element from the matrix.
	* @param linear position (a)
	*/
	double Matrix::operator()(int a) const {
		if (a > _m * _n) {
			std::cerr << "ERROR: the element requested is outside of the matrix.\n";
			return 0;
		}

		return _A[a];
	}

	/**
	* Get the element from the matrix.
	* @param linear position (a)
	*/
	double& Matrix::operator()(int a) {
		if (a > _m * _n) {
			std::cerr << "FATAL ERROR: the element requested is outside of the matrix.\n";
			exit(-1);
		}

		return _A[a];
	}

	/**
	* Get the element from the matrix.
	* @param row (j)
	* @param column (i)
	*/
	double Matrix::operator()(int j, int i) const {
		if (j > _m || i > _n) {
			std::cerr << "ERROR: the element requested is outside of the matrix.\n";
			return 0;
		}

		return _A[j * _n + i];
	}

	/**
	* Get the element from the matrix.
	* @param row (j)
	* @param column (i)
	*/
	double& Matrix::operator()(int j, int i) {
		if (j > _m || i > _n) {
			std::cerr << "FATAL ERROR: the element requested is outside of the matrix.\n";
			exit(-1);
		}

		return _A[j * _n + i];
	}

	/**
	* Check if two matrix are equal
	* @param matrix to check for equality
	*/
	bool Matrix::operator==(Matrix B) const {

		if (_m != B.rowCount() || _n != B.colCount()) return false;

		for (int a = 0; a < _m * _n; a++) {
			if ((*this)(a) != B(a)) return false;
		}

		return true;
	}

	/**
	* Matrix addition
	* @param matrix to add
	*/
	Matrix Matrix::operator+(Matrix B) const {

		if (B.rowCount() != _m || B.colCount() != _n) {
			std::cerr << "ERROR: Sizes aren't equal. Matrix addition is not defined\n";
			return Matrix(1, 1);
		}

		Matrix res(_m, _n);

		for (int a = 0; a < _m * _n; a++) {
			res(a) = (*this)(a) + B(a);
		}

		return res;
	}

	/**
	* Product by scalar. Multiply all elements by the scalar.
	* @param scalar to multiply the matrix by
	*/
	Matrix Matrix::operator*(double scalar) const {

		Matrix res(_m, _n);

		for (int a = 0; a < _m * _n; a++) {
			res(a) = (*this)(a) * scalar;
		}

		return res;
	}

	/**
	* Matrix multiplication.
	* @param B matrix to multplity by
	* @return new matrix
	*/
	Matrix Matrix::operator*(Matrix B) const {

		if (_n != B.rowCount()) {
			std::cerr << "ERROR: Sizes don't match, matrix multiplication is not defined.\n";
			return Matrix(1, 1);
		}

		const int newSize = _m * B.colCount();

		Matrix res(_m, B.colCount());

		for (int a = 0; a < newSize; a++) {

			int j = a / _m;
			int i = a % B.colCount();

			double elem = 0;

			for (int x = 0; x < _n; x++) {
				elem += (*this)(j, x) * B(x, i);
			}

			res(a) = elem;
		}

		return res;
	}
}