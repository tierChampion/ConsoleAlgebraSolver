#pragma once
#include <iostream>

namespace als {

	enum class sleSolution {
		NONE,
		ONE,
		INFINITE,
	};

	/**
	* Mathematical matrix class
	*/
	class Matrix {

		int _m, _n;
		std::shared_ptr<double[]> _A;

	public:

		Matrix(int m, int n);
		static Matrix Identity(int dim);
		static Matrix Null(int dim);
		void fill(double* B);
		void print() const;

		int rowCount() const { return _m; }
		int colCount() const { return _n; }

		int rank() const;
		double trace() const;

		bool operator==(Matrix B) const;

		double operator()(int a) const;
		double& operator()(int a);
		double operator()(int j, int i) const;
		double& operator()(int j, int i);
		Matrix transpose() const;

		Matrix operator+(Matrix B) const;

		Matrix operator*(double scalar) const;
		Matrix operator*(Matrix B) const;

		/*** Identities ***/
		bool isSquare() const;
		//todo
		bool isUpperTriangular() const;
		bool isLowerTriangular() const;
		bool isDiagonal() const;

		bool isNull() const;
		bool isIdentity() const;
		bool isSymetric() const;
		bool isAntisymetric() const;
		bool isIdempotent() const;
		bool isNilpotent(int level) const;

		/*** SLE ***/
		void scaleEquation(int equation, double scalar);
		void swapEquations(int equation1, int equation2);
		void addOtherEquation(int equation1, int equation2, double scalar);
		static Matrix toRowEchelon(const Matrix A, Matrix* b = nullptr, double* alpha = nullptr);
		static Matrix toReducedRowEchelon(const Matrix A, Matrix* b = nullptr, double* alpha = nullptr);
		static Matrix augmentedMatrix(const Matrix A, const Matrix b);
		static sleSolution solveSLE(const Matrix A, Matrix b, Matrix* x);

		/*** Determinant and inverse ***/
		static double determinant(const Matrix A);
		static double determinant(const Matrix A, double alpha);
		bool isInvertible() const;
		double cofactor(int j, int i) const;
		static Matrix inverse(Matrix A);
		static Matrix subMatrix(const Matrix A, int j, int i);
		static Matrix adjugate(const Matrix A);
	};
}