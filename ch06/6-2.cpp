#include <iostream>
#include <vector>
using namespace std;

template <typename elemType> class Matrix {
    friend Matrix<elemType> operator+(const Matrix<elemType>&, const Matrix<elemType>&);
    friend Matrix<elemType> operator*(const Matrix<elemType>&, const Matrix<elemType>&);

public:
    Matrix(int rows, int cols): _rows(rows), _cols(cols) {
        int size = _rows * _cols;
        _matrix = new elemType[size];
        // 如果elemType不是一个内置的数据类型，new elemType()可能会出错
        for (int i = 0; i < size; i++) _matrix[i] = new elemType();
    }
    Matrix(const Matrix&);
    ~Matrix() { delete []_matrix; }
    Matrix& operator=(const Matrix&);
    void operator+=(const Matrix&);
    elemType& operator()(int row, int col) {
        return _matrix[row * _cols + col];
    }
    const elemType& operator()(int row, int col) const {
        return _matrix[row * _cols + col];
    }
    int rows() { return _rows; }
    int cols() { return _cols; }
    bool same_size(const Matrix &m) const {
        return rows() == m._rows && cols() == m._cols;
    }
    bool comfortable(const Matrix &m) const {
        return cols() == m.rows();
    }
    ostream& print(ostream &) const;

protected:
    int _rows;
    int _cols;
    elemType *_matrix;
};

template <typename elemType>
inline ostream& operator<<(ostream &os, const Matrix<elemType> &m) {
    return m.print(os);
}

template <typename elemType>
Matrix<elemType> operator+(const Matrix<elemType> &m1, const Matrix<elemType> &m2) {
    Matrix<elemType> res(m1);
    res += m2;
    return res;
}

template <typename elemType>
Matrix<elemType> operator*(const Matrix<elemType> &m1, const Matrix<elemType> &m2) {
    if (!m1.comfortable(m2)) {
        // exit
    }
    Matrix<elemType> res(m1.rows(), m2.cols());
    for (int i = 0; i < m1.row(); i++) {
        for (int j = 0; j < m1.cols(); j++) {
            res(i, j) = 0;
            for (int k = 0; k < m1.cols(); k++) {
                res(i, j) += m1(i, k) * m2(k, j);
            }
        }
    }
    return res;
}

template <typename elemType>
Matrix<elemType>::Matrix(const Matrix<elemType> &rhs):
_rows(rhs._rows), _cols(rhs._cols) {
    int size = _rows * _cols;
    _matrix = new elemType[size];
    for (int i = 0; i < size; i++) _matrix[i] = rhs._matrix[i];
}

template <typename elemType>
Matrix<elemType>& Matrix<elemType>::operator=(const Matrix<elemType> &rhs) {
    if (this != &rhs) {
        _rows = rhs._rows;
        _cols = rhs._cols;
        int size = _rows * _cols;
        delete []_matrix;
        _matrix = new elemType[size];
        for (int i = 0; i < size; i++) _matrix[i] = rhs._matrix[i];
    }
    return *this;
}

template <typename elemType>
void Matrix<elemType>::operator+=(const Matrix<elemType> &rhs) {
    int size = cols() * rows();
    for (int i = 0; i < size; i++) {
        *(_matrix + i) += *(rhs._matrix + i);
    }
}

template <typename elemType>
ostream& Matrix<elemType>::print(ostream &os) const {
    int size = cols() * rows();
    for (int i = 0; i < size; i++) {
        if (i % _cols == 0) os << endl;
        os << *(_matrix + i) << " ";
    }
    os << endl;
    return os;
}

