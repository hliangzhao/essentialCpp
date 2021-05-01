#include <iostream>
using namespace std;

// 一个n*n的矩阵（难点在于二维数组的分配）
class Matrix {
    friend Matrix operator+(const Matrix &, const Matrix &);
    friend Matrix operator*(const Matrix &, const Matrix &);

public:
    Matrix(int n, const double *);
    Matrix(int n):_size(n) {
        _matrix = new double*[_max_size];
        for (int i = 0; i < _max_size; i++) {
            _matrix[i] = new double[_max_size];
        }
        for (int i = 0; i < _size; i++) {
            for (int j = 0; j < _size; j++) {
                _matrix[i][j] = 0;
            }
        }
    }
    // copy constructor
    Matrix(const Matrix &m):_size(m._size) {
        _matrix = new double*[_max_size];
        for (int i = 0; i < _max_size; i++) {
            _matrix[i] = new double[_max_size];
        }
        for (int i = 0; i < _size; i++) {
            for (int j = 0; j < _size; j++) {
                _matrix[i][j] = m._matrix[i][j];
            }
        }
    }
    int rows() const { return _size; }
    int cols() const { return _size; }

    ostream& print(ostream&) const;
    void operator+=(const Matrix &);
    double operator()(int row, int column) const {
        return _matrix[row][column];
    }
    double& operator()(int row, int column) {
        return _matrix[row][column];
    }

private:
    const static int _max_size = 100;
    int _size;
    double **_matrix;
};

Matrix::Matrix(int n, const double *arr):_size(n) {
    _matrix = new double*[_max_size];
    for (int i = 0; i < _max_size; i++) {
        _matrix[i] = new double[_max_size];
    }
    int idx = 0;
    for (int i = 0; i < _size; i++) {
        for (int j = 0; j < _size; j++)
            _matrix[i][j] = arr[idx++];
    }
}

ostream& Matrix::print(ostream &os) const {
    int cnt = 1;
    for (int i = 0; i < _size; i++) {
        for (int j = 0; j < _size; j++) {
            os << _matrix[i][j] << " ";
            if (cnt % 4 == 0) cout << endl;
            cnt++;
        }
    }
    os << endl;
    return os;
}

void Matrix::operator+=(const Matrix &rhs) {
    for (int i = 0; i < _size; i++) {
        for (int j = 0; j < _size; j++) {
            _matrix[i][j] += rhs._matrix[i][j];
        }
    }
}

inline ostream& operator<<(ostream &os, const Matrix &m) {
    return m.print(os);
}

Matrix operator+(const Matrix &m1, const Matrix &m2) {
    Matrix res(m1);
    res += m2;
    return res;
}

Matrix operator*(const Matrix &m1, const Matrix &m2) {
    Matrix res(m1._size);
    for (int i = 0; i < m1.rows(); i++) {
        for (int j = 0; j < m1.cols(); j++) {
            res(i, j) = 0;
            for (int k = 0; k < m1.cols(); k++) {
                res(i, j) += m1(i, k) * m2(k, j);
            }
        }
    }
    return res;
}


int main() {
    Matrix m(4);
    cout << m;

    double *arr = new double[16];
    for (int i = 0; i < 16; i++) {
        arr[i] = 1;
    }
    Matrix m1(4, arr);
    cout << m1;

    Matrix m2(m1);
    cout << m2;

    Matrix m3(4);
    m3 = m2 + m2;
    cout << m3;

    Matrix m4(4);
    m4 = m3 * m3;
    cout << m4;
}

