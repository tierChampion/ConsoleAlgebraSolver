#include "Matrix.h"

/// <summary>
/// Implementation of the system of linear equation 
/// matrix functionalities.
/// </summary>

namespace als {

	/**
	* Scale a row by a factor.
	* @param equation row to scale
	* @param scalar factor
	*/
	void Matrix::scaleEquation(int equation, double scalar) {
		for (int i = 0; i < _n; i++) {
			(*this)(equation, i) *= scalar;
		}
	}

	/**
	* Swap two rows.
	* @param equation1 first row
	* @param equation2 second row
	*/
	void Matrix::swapEquations(int equation1, int equation2) {
		for (int i = 0; i < _n; i++) {
			double temp = (*this)(equation1, i);
			(*this)(equation1, i) = (*this)(equation2, i);
			(*this)(equation2, i) = temp;
		}
	}

	/**
	* Add a scaled row to another.
	* @param equation1 row to add the second row to
	* @param equation2 row to scale
	* @param scalar factor to apply to the second row
	*/
	void Matrix::addOtherEquation(int equation1, int equation2, double scalar) {
		for (int i = 0; i < _n; i++) {
			(*this)(equation1, i) += (*this)(equation2, i) * scalar;
		}
	}

	/**
	* Transform the SLE to an equivalent row echelon matrix.
	* Uses the Gauss reduction algorithm.
	* @param b optional resultant vector
	* @return factor between the determinant of the original matrix and its row echelon equivalent
	*/
	Matrix Matrix::toRowEchelon(const Matrix A, Matrix* b, double* alpha) {

		int equation = 0;

		double k = 1;

		Matrix ret = Matrix(A.rowCount(), A.colCount());
		ret.fill(A._A.get());

		// Gauss Reduction
		while (equation < ret.rowCount()) {

			if (ret(equation, equation) != 0) {
				double scalar = 1 / ret(equation, equation);
				ret.scaleEquation(equation, scalar);
				if (b) b->scaleEquation(equation, scalar);
				k /= scalar;

				for (int a = equation + 1; a < ret.colCount(); a++) {
					double s = -ret(a, equation);
					ret.addOtherEquation(a, equation, s);
					if (b) b->addOtherEquation(a, equation, s);
				}

				equation++;
			}
			else {
				if (equation + 1 != ret.rowCount()) {
					if (ret(equation + 1, equation) != 0) {
						ret.swapEquations(equation, equation + 1);
						if (b) b->swapEquations(equation, equation + 1);
						k *= -1;
					}
					else {
						equation++;
					}
				}
				else {
					equation++;
				}
			}
		}

		if (alpha) *alpha = k;

		return ret;
	}

	/**
	* Transform the SLE to an equivalent reduced row echelon matrix.
	* Uses the Gauss-Jordan reduction algorithm.
	* @param b optional resultant vector
	*/
	Matrix Matrix::toReducedRowEchelon(const Matrix A, Matrix* b, double* alpha) {

		Matrix ret = Matrix::toRowEchelon(A, b, alpha);

		// Gauss-Jordan Reduction
		for (int equation = ret.rowCount() - 2; equation >= 0; equation--) {

			for (int j = equation + 1; j < ret.rowCount(); j++) {

				double scalar = -ret(equation, j);
				ret.addOtherEquation(equation, j, scalar);

				if (b) b->addOtherEquation(equation, j, scalar);
			}
		}

		return ret;
	}

	/**
	* Construct an augmented matrix from an SLE.
	* @param A factor part of the SLE
	* @param b resultant part of the SLE
	* @return augmented matrix
	*/
	Matrix Matrix::augmentedMatrix(const Matrix A, const Matrix b) {

		Matrix Ab(A.rowCount(), A.colCount() + 1);

		for (int j = 0; j < A.rowCount(); j++) {
			Ab(j, A.colCount()) = b(j, 0);

			for (int i = 0; i < A.colCount(); i++) {
				Ab(j, i) = A(j, i);
			}
		}

		return Ab;
	}

	/**
	* Solve the system of linear equations. If the system has a single solution,
	* the value of the variables will be in the x matrix.
	* @param A factors of the equations.
	* @param b resultants of the equations.
	* @param x solution to the system if there is a single solution
	* @return number of solutions (0, 1 or infinite)
	*/
	sleSolution Matrix::solveSLE(const Matrix A, Matrix b, Matrix* x) {

		if (A.rowCount() != b.rowCount() || A.colCount() != x->colCount()) {
			std::cerr << "ERROR: The given SLE doesn't have proper sizes." << std::endl;
			return sleSolution::NONE;
		}

		sleSolution ret;

		Matrix::toReducedRowEchelon(A, &b);

		Matrix Ab = augmentedMatrix(A, b);

		int abRank = Ab.rank();

		if (abRank > A.rank()) {
			ret = sleSolution::NONE;

			std::cout << "No solution to the SLE, it is incompatible." << std::endl;
			Ab.print();
		}
		else {

			if (abRank < A.colCount()) {
				ret = sleSolution::INFINITE;

				std::cout << "Infinite solutions to the SLE: " << std::endl;

				std::cout << "[A|b] = " << std::endl;
				Ab.print();
			}
			else {
				ret = sleSolution::ONE;

				for (int resultant = 0; resultant < Ab.colCount(); resultant++) {
					(*x)(0, resultant) = Ab(resultant, Ab.colCount() - 1);
				}

				std::cout << "Single solution to the SLE:" << std::endl;
				x->print();
			}
		}

		return ret;
	}
}