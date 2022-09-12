#include "Matrix.h"
#include "StringHelper.h"

using namespace als;

/*
* TODO:
* - not hardcode some constant (option count, nilpotence level)
* - Maybe have a menu for calculations
* - Add further functionality
*/

/// Forward declarations ///
void propertyMenu();
void multiplicationMenu();
void SleMenu();
void determinantMenu();
void matrixInversionMenu();
void matrixAdjugateMenu();
Matrix matrixMenu();

int main()
{

	std::cout << "    _    _            _                 ____        _\n"
		<< "   / \\  | | __ _  ___| |__  _ __ __ _  / ___|  ___ | |_   _____ _ __\n"
		<< "  / _ \\ | |/ _` |/ _ \\ '_ \\| '__/ _` | \\___ \\ / _ \\| \\ \\ / / _ \\ '__|\n"
		<< " / ___ \\| | (_| |  __/ |_) | | | (_| |  ___) | (_) | |\\ V /  __/ |\n"
		<< "/_/   \\_\\_|\\__, |\\___|_.__/|_|  \\__,_| |____/ \\___/|_| \\_/ \\___|_|\n"
		<< "           |___/\n" << std::endl;

	bool continueOperations = true;

	do {

		std::cout
			<< "<=================================>\n"
			<< "  What kind of problem to solve ? \n"
			<< "<=================================>\n"
			<< "1. Quit\n"
			<< "2. Property identification\n"
			<< "3. Matrix multiplication\n"
			<< "4. System of linear equations\n"
			<< "5. Determinant calculation\n"
			<< "6. Invert matrix calculation\n"
			<< "7. Adjugate matrix calculation\n"
			<< std::endl
			<< "--> ";

		std::string input;
		int problem = 0;

		do {
			std::getline(std::cin, input);
			try {
				problem = std::stoi(input);
			}
			catch (std::invalid_argument) {}
			if (problem < 1 || problem > 7) {
				std::cout
					<< "This is not a valid option.\n" << std::endl
					<< "--> ";
			}
		} while (problem < 1 || problem > 7);

		switch (problem) {
		case 1: continueOperations = false; break;
		case 2: propertyMenu(); break;
		case 3: multiplicationMenu(); break;
		case 4: SleMenu(); break;
		case 5: determinantMenu(); break;
		case 6: matrixInversionMenu(); break;
		case 7: matrixAdjugateMenu(); break;
		default: {
			std::cerr << "FATAL ERROR: A proper menu was not selected\n";
			exit(-1);
		}
		}

	} while (continueOperations);

	std::cout << "\nGood luck.\n";
}

/**
* Menu when selecting the first option.
* Multiple options to check if certain properties of a matrix is true.
*/
void propertyMenu() {

	std::cout
		<< "<========================>\n"
		<< "  Property to check for: \n"
		<< "<========================>\n"
		<< "1. Square\n"
		<< "2. Upper triangular\n"
		<< "3. Lower triangular\n"
		<< "4. Diagonal\n"
		<< "5. Identity\n"
		<< "6. Null\n"
		<< "7. Symetric\n"
		<< "8. Antisymetric\n"
		<< "9. Idempotence\n"
		<< "10. Nilpotence\n"
		<< "11. Invertibility\n"
		<< "Esc. Quit\n" << std::endl
		<< "--> ";

	// read entry
	std::string input;
	int property = 0;

	do {
		std::getline(std::cin, input);
		try {
			property = std::stoi(input);
		}
		catch (std::invalid_argument) {}
		if (property < 1 || property > 11) {
			std::cout
				<< "This is not a valid option.\n" << std::endl
				<< "--> ";
		}
	} while (property < 1 || property > 11);

	std::cout
		<< "Matrix to check: \n" << std::endl;

	Matrix A = matrixMenu();

	bool check = false;

	switch (property) {
	case 1: check = A.isSquare(); break;
	case 2: check = A.isUpperTriangular(); break;
	case 3: check = A.isLowerTriangular(); break;
	case 4: check = A.isDiagonal(); break;
	case 5: check = A.isIdentity(); break;
	case 6: check = A.isNull(); break;
	case 7: check = A.isSymetric(); break;
	case 8: check = A.isAntisymetric(); break;
	case 9: check = A.isIdempotent(); break;
	case 10: {
		// ask for level
		int level = 2;

		check = A.isNilpotent(level); break;
	}
	case 11: check = A.isInvertible();
	default: {
		std::cerr << "FATAL ERROR: A proper menu was not selected\n";
		exit(-1); }
	}

	std::cout
		<< "~~~~~ Result ~~~~~\n" << std::endl
		<< "The check is " << (check ? "True" : "False") << "\n" << std::endl;
}

void multiplicationMenu() {
	std::cout
		<< "<========================>\n"
		<< "  Matrix multiplication: \n"
		<< "<========================>\n"
		<< "      [ C = A * B ]\n"
		<< "A matrix: \n";

	Matrix A = matrixMenu();

	std::cout << "B matrix: \n";

	Matrix B = matrixMenu();

	Matrix C = A * B;

	std::cout
		<< "~~~~~ Result ~~~~~\n" << std::endl;

	C.print();
}

void SleMenu() {

	std::cout
		<< "<========================>\n"
		<< "     SLE Resolution: \n"
		<< "<========================>\n"
		<< " [ S : { E1 E2 E3 ... } ]\n"
		<< "Number of variables (without resultant): \n"
		<< "--> ";

	std::string entry;

	std::getline(std::cin, entry);

	int varCount = std::stoi(entry);
	// read input
	std::cout
		<< "Number of equations: \n"
		<< "--> ";
	std::getline(std::cin, entry);
	int equationCount = std::stoi(entry);

	Matrix A(equationCount, varCount);
	Matrix b(equationCount, 1);
	Matrix x(1, varCount);

	for (int e = 0; e < equationCount; e++) {
		std::cout
			<< "Coefficients and resultant of equation #" << e + 1 << " :\n";

		bool acceptedEntry = false;

		do {

			std::cout << "--> ";

			std::getline(std::cin >> std::noskipws, entry);
			std::vector<std::string_view> elements = split(entry, ' ');

			if (elements.size() == varCount + 1) {

				for (int i = 0; i < varCount; i++) {
					std::from_chars(elements[i].data(),
						elements[i].data() + elements[i].size(),
						A(e, i));
				}

				std::from_chars(elements[varCount].data(),
					elements[varCount].data() + elements[varCount].size(),
					b(e - 1, 1));

				acceptedEntry = true;
			}
			else {
				std::cerr
					<< "ERROR: the quantity of values doesn't match the size of the equation.\n"
					<< "Expected " << varCount + 1 << " values.\n" << std::endl;
			}

		} while (!acceptedEntry);
	}

	A.print();
	b.print();

	sleSolution solution = Matrix::solveSLE(A, b, &x);
}

void determinantMenu() {

	std::cout
		<< "<==========================>\n"
		<< "  Determinant calculation: \n"
		<< "<==========================>\n"
		<< " det(A)\n" << std::endl;

	Matrix A = matrixMenu();

	double det = Matrix::determinant(A);

	std::cout << "Determinant(A) = " << det << "\n" << std::endl;

}

void matrixInversionMenu() {

	std::cout
		<< "<===================>\n"
		<< "  Matrix inversion: \n"
		<< "<===================>\n"
		<< " Inv(A)\n" << std::endl;

	Matrix A = matrixMenu();

	Matrix invA = Matrix::inverse(A);

	std::cout << "Inv(A) = \n";

	invA.print();
}

void matrixAdjugateMenu() {

	std::cout
		<< "<==================>\n"
		<< "  Matrix adjugate: \n"
		<< "<==================>\n"
		<< " Adj(A)\n" << std::endl;

	Matrix A = matrixMenu();

	Matrix adjA = Matrix::adjugate(A);

	std::cout << "Adj(A) = \n";

	adjA.print();
}

Matrix matrixMenu() {

	std::string entry;

	std::cout
		<< "--- Shape of the matrix --- \n"
		<< " width --> ";
	std::getline(std::cin, entry);
	// read entries
	int w = std::stoi(entry);

	std::cout
		<< " height --> ";
	std::getline(std::cin, entry);
	// read entries
	int h = std::stoi(entry);

	Matrix ret(h, w);

	bool acceptedEntry = false;

	std::cout
		<< "\nElements of the matrix from left to right, top to bottom: \n";

	do {

		std::cout << "--> ";

		std::getline(std::cin >> std::noskipws, entry);
		// read entries
		std::vector<std::string_view> elements = split(entry, ' ');

		if (elements.size() == w * h) {

			for (int i = 0; i < w * h; i++) {
				std::from_chars(elements[i].data(),
					elements[i].data() + elements[i].size(),
					ret(i));
			}

			acceptedEntry = true;
		}
		else {
			std::cerr
				<< "ERROR: the quantity of elements doesn't match the size of the matrix.\n"
				<< "Expected " << w * h << " elements.\n" << std::endl;
		}

	} while (!acceptedEntry);

	return ret;
}
