#include "Matrix.h"

/// <summary>
/// Implementation of the property checking.
/// </summary>

namespace als {

	/**
	* Check if the matrix has the same height and width.
	*/
	bool Matrix::isSquare() const {
		return _m == _n;
	}

	/**
	* Check if the half lower than the diagonal is all zeros.
	*/
	bool Matrix::isUpperTriangular() const {

		if (!isSquare()) return false;

		for (int j = 1; j < _m; j++) {
			for (int i = 0; i < j; i++) {
				if ((*this)(j, i) != 0) return false;
			}
		}

		return true;
	}

	/**
	* Check if the half higher than the diagonal is all zeros.
	*/
	bool Matrix::isLowerTriangular() const {

		if (!isSquare()) return false;

		for (int j = 0; j < _m; j++) {
			for (int i = _n; i > j; i--) {
				if ((*this)(j, i) != 0) return false;
			}
		}
		return true;
	}
	bool Matrix::isDiagonal() const { return isUpperTriangular() && isLowerTriangular(); }

	/**
	* Check if the matrix is filled with zeros.
	*/
	bool Matrix::isNull() const { return rank() == 0; }

	/**
	* Check if the matrix is an identity matrix.
	*/
	bool Matrix::isIdentity() const {

		Matrix one(_m, _n);

		for (int a = 0; a < _m * _n; a++) {
			one(a) = 1;
		}

		return ((*this) * one) == one;
	}

	/**
	* Check if the matrix is identical when transposed.
	*/
	bool Matrix::isSymetric() const {
		return (transpose() == (*this));
	}

	/**
	* Check if the matrix is its opposite when transposed.
	*/
	bool Matrix::isAntisymetric() const {
		return (transpose() == ((*this) * -1));
	}

	/**
	* Check if the matrix is identical when squared.
	*/
	bool Matrix::isIdempotent() const {
		return (((*this) * (*this)) == (*this));
	}

	/**
	* Check if the matrix becomes null when raised to a certain power.
	* @param level power to test for nilpotence
	*/
	bool Matrix::isNilpotent(int level) const {
		if (!isSquare()) return false;

		Matrix nil(_m, _n);

		nil.fill(_A.get());

		for (int l = 0; l < level - 1; l++) {
			nil = nil * nil;
			if (nil.isNull()) return true;
		}

		return false;
	}
}