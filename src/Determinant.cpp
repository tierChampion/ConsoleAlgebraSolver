#include "Matrix.h"

/// <summary>
/// Implementation of the determinants and inverse matrix.
/// </summary>

namespace als {

	/**
	* Calculate the determinant of the matrix.
	* @param A matrix to calculate the determinant of
	*/
	double Matrix::determinant(const Matrix A) {

		if (!A.isSquare()) return 0;

		double alpha = 0;

		Matrix reMat = Matrix::toRowEchelon(A, nullptr, &alpha);

		double det = 1;

		for (int i = 0; i < reMat.colCount(); i++) {
			det *= reMat(i, i);
		}

		return det * alpha;
	}

	/**
	* Calculate the determinant of the matrix.
	* @param A row-echelon matrix
	* @param alpha determinant factor between the original matrix and the row-echelon matrix
	*/
	double Matrix::determinant(const Matrix A, double alpha) {

		if (!A.isSquare()) return 0;

		double det = 1;

		for (int i = 0; i < A.colCount(); i++) {
			det *= A(i, i);
		}

		return det * alpha;
	}

	/**
	* Check if the matrix in invertible. A matrix is invertible if the determinant is non-zero
	*/
	bool Matrix::isInvertible() const {
		return Matrix::determinant((*this)) != 0;
	}

	/**
	* Calculate the inverse matrix if possible.
	* @param A matrix to invert
	* @return (A)^-1
	*/
	Matrix Matrix::inverse(const Matrix A) {

		if (!A.isSquare()) {
			std::cout << "ERROR: The matrix to invert is not square. Thus there is no inverse matrix.\n"
				<< std::endl;

			return Matrix::Null(1);
		}

		Matrix inv = Matrix::Identity(A.rowCount());

		double alpha = 0;

		Matrix reMat = Matrix::toReducedRowEchelon(A, &inv, &alpha);

		double det = Matrix::determinant(reMat, alpha);

		if (det == 0) {
			std::cout << "Warning: the determinant of the matrix is equal to 0. Thus it can't be inverted.\n"
				<< std::endl;
			return Identity(A.rowCount());
		}

		return inv;
	}

	/**
	* Calculate the matrix submatrix Aji. It is the A matrix without the row j and the column i.
	* @param A matrix to calculate the submatrix of
	* @param j row to remove
	* @param i column to remove
	* @return matrix Aji
	*/
	Matrix Matrix::subMatrix(const Matrix A, int j, int i) {

		Matrix subA = Matrix(A.rowCount() - 1, A.colCount() - 1);

		int index = 0;

		for (int y = 0; y < A.rowCount(); y++) {

			if (y == j) continue;

			for (int x = 0; x < A.colCount(); x++) {
				if (x == i) continue;

				subA(index) = A(y, x);
				index++;
			}
		}

		return subA;
	}

	/**
	* Calculate the adjugate matrix of A. It is the matrix of all the cofactors of A.
	* @param A matrix to calculate the adjugate of
	* @return adj(A)
	*/
	Matrix Matrix::adjugate(const Matrix A) {

		Matrix adjA = Matrix(A.rowCount(), A.colCount());

		for (int j = 0; j < adjA.rowCount(); j++) {
			for (int i = 0; i < adjA.colCount(); i++) {
				adjA(j, i) = A.cofactor(j, i);
			}
		}

		return adjA.transpose();
	}

	/**
	* Calculate the j-i-th cofactor of the matrix.
	* @param j row to remove of the matrix
	* @param i column to remove of the matrix
	*/
	double Matrix::cofactor(int j, int i) const {
		int sign = (((j + i) % 2) == 0) ? 1 : -1;

		Matrix Aji = Matrix::subMatrix((*this), j, i);

		return sign * determinant(Aji);
	}
}