//#include <iostream>
//#include <ctime>
//#include <Eigen/Eigen>
//
//using namespace std;
//using namespace Eigen;
//
//int main()
//{
//
//	MatrixXd A = MatrixXd::Random(5000,5000);
//	MatrixXd B = MatrixXd::Random(5000,2);
//
//	//cout << "Here is the invertible matrix A:" << endl << A << endl;
//	//cout << "Here is the matrix B:" << endl << B << endl;
//
//	clock_t clock_start=clock();
//
//	MatrixXd X = A.lu().solve(B);
//
//	double time_c=(double)(clock()- clock_start)/CLOCKS_PER_SEC;
//
//	//cout << "Here is the (unique) solution X to the equation AX=B:" << endl << X << endl;
//
//	cout << "Relative error: " << (A*X-B).norm() / B.norm() << endl;
//
//	cout<< "Elapsed Time is: "<< time_c<< "s" <<endl;
//
//	cin.get();
//}