#pragma once

#include <cstdio>
#include "my_math.hpp"

namespace matrix
{

// Forward declaration of the matrix class.
template<int R, int C>
class Matrix;

// Shortcuts for square matrices.
typedef Matrix<2,2> Mat2;
typedef Matrix<3,3> Mat3;
typedef Matrix<4,4> Mat4;

// Matrix class.
template<int R, int C>
class Matrix
{
    public:
        // ------- Members ------ //
        float m[R][C];

        // ----- Constructors & Destructor ----- //
        Matrix(bool identity = false) 
        { 
            assert(R >= 2 && C >= 2);

            if (!identity)
            {
                for (int i = 0; i < R; i++)
                    for (int j = 0; j < C; j++)
                        m[i][j] = 0;
            }
            else
            {
                for (int i = 0; i < R; i++)
                    for (int j = 0; j < C; j++)
                        if (i == j)
                            m[i][j] = 1;
                        else
                            m[i][j] = 0;
            }
        }

        Matrix(const Matrix<R,C>& matrix)
        {
            assert(R >= 2 && C >= 2);
            for (int i = 0; i < R; i++)
                for (int j = 0; j < C; j++)
                    m[i][j] = matrix[i][j];
        }

        //? NOTE: Only for Matrix 2X2.
        Matrix(const float& a, const float& b, const float& c, const float& d)
        {
            assert(R >= 2 && C >= 2);
            m[0][0] = a; m[0][1] = b;
            m[1][0] = c; m[1][1] = d;
        }

        //? NOTE: Only for Matrix 3X3.
        Matrix(const float& a, const float& b, const float& c,
               const float& d, const float& e, const float& f,
               const float& g, const float& h, const float& i)
        {
            assert(R >= 3 && C >= 3);
            m[0][0] = a; m[0][1] = b; m[0][2] = c;
            m[1][0] = d; m[1][1] = e; m[1][2] = f;
            m[2][0] = g; m[2][1] = h; m[2][2] = i;
        }

        //? NOTE: Only for Matrix 4X4.
        Matrix(const float& a, const float& b, const float& c, const float& d,
                const float& e, const float& f, const float& g, const float& h,
                const float& i, const float& j, const float& k, const float& l,
                const float& M, const float& n, const float& o, const float& p)
        {
            assert(R >= 4 && C >= 4);
            m[0][0] = a; m[0][1] = b; m[0][2] = c; m[0][3] = d;
            m[1][0] = e; m[1][1] = f; m[1][2] = g; m[1][3] = h;
            m[2][0] = i; m[2][1] = j; m[2][2] = k; m[2][3] = l;
            m[3][0] = M; m[3][1] = n; m[3][2] = o; m[3][3] = p;
        }

        //? NOTE: Only for Matrix 4x4 (from 2x2 matrices).
        Matrix(const Mat2& a, const Mat2& b, const Mat2& c, const Mat2& d)
        {
            assert(R >= 4 && C >= 4);
            m[0][0] = a[0][0]; m[0][1] = a[0][1]; m[0][2] = b[0][0]; m[0][3] = b[0][1];
            m[1][0] = a[1][0]; m[1][1] = a[1][1]; m[1][2] = b[1][0]; m[1][3] = b[1][1];
            m[2][0] = c[0][0]; m[2][1] = c[0][1]; m[2][2] = d[0][0]; m[2][3] = d[0][1];
            m[3][0] = c[1][0]; m[3][1] = c[1][1]; m[3][2] = d[1][0]; m[3][3] = d[1][1];
        }
        
        //? NOTE: For > n  matrixes.
        Matrix(const float matrix[R][C])
        {
            assert(R > 4 && C > 4);
            for (int i = 0; i < R; i++)
                for (int j = 0; j < C; j++)
                    m[i][j] = matrix[i][j];
        }

        ~Matrix() {}

        // ----- Operators ----- //

        // Matrix bracket operators.
        float* operator[](int index)             { return m[index]; }
        const float* operator[](int index) const { return m[index]; }

        // Matrix copy.
        Matrix<R,C> operator=(const Matrix<R,C>& matrix)
        {
            if (&matrix == this) return *this;
            
            // Matrix content copy
            for (int i = 0; i < R; i++) 
                for (int j = 0; j < C; j++)
                    m[i][j] = matrix[i][j];
            
            return *this;
        }

        Matrix<R,C> operator=(float** matrix)
        {
            assert(sizeof(matrix) / sizeof(float) == R * C);

            for (int i = 0; i < R; i++)
                for (int j = 0; j < C; j++)
                    m[i][j] = matrix[i][j];

            return *this;
        }

        // Matrix addition.
        Matrix<R,C> operator+(const float& val) const
        {
            Matrix<R,C> tmp;
            for (int i = 0; i < R; i++)
                for (int j = 0; j < C; j++)
                    tmp[i][j] = m[i][j] + val;
            return tmp;
        }

        template<int _R, int _C>
        Matrix<R,C> operator+(const Matrix<_R,_C>& matrix) const
        {
            assert(R == _R && C == _C); // Matrix must have the same dimension
            Matrix<_R,_C> tmp;
            for(int i = 0; i < R; i++)
                for(int j = 0; j < C; j++)
                    tmp[i][j] = m[i][j] + matrix[i][j];
            return tmp;
        }

        // Matrix substraction and inversion.
        Matrix<R,C> operator-() 
        {
            Matrix<R,C> tmp;
            for (int i = 0; i < R; i++)
                for (int j = 0; j < C; j++)
                    tmp[i][j] = -m[i][j];
            return tmp;
        }

        Matrix<R,C> operator-(const float& val) const
        {
            Matrix<R,C> tmp;
            for (int i = 0; i < R; i++)
                for (int j = 0; j < C; j++)
                    tmp[i][j] = m[i][j] - val;
            return tmp;
        }

        template<int _R, int _C>
        Matrix<R,C> operator-(const Matrix<_R,_C>& matrix) const
        {
            assert(R == _R && C == _C);
            Matrix<_R,_C> tmp;
            for (int i = 0; i < R; i++)
                for (int j = 0; j < C; j++)
                    tmp[i][j] = m[i][j] - matrix[i][j];
            return tmp;
        }


        // Matrix multiplication.
        Matrix<R,C> operator*(const float& val) const
        {
            Matrix<R,C> tmp;
            for (int i = 0; i < R; i++)
                for (int j = 0; j < C; j++)
                    tmp[i][j] = m[i][j] * val;
            return tmp;
        }

        template<int _R, int _C>
        Matrix<(R > _R ? R : _R),(C > _C ? C : _C)> operator*(const Matrix<_R,_C>& matrix) const
        {
            assert(C == _R); // Size condition to calculate

            Matrix<(R > _R ? R : _R),(C > _C ? C : _C)> result;
            for (int i = 0; i < R; i++)
                for (int j = 0; j < _C; j++)
                {
                    result[i][j] = 0;
                    for (int k = 0; k < _R; k++)
                        result[i][j] += m[i][k] * matrix[k][j];
                }
            return result;
        }

        // Matrix division by a scalar.
        Matrix<R,C> operator/(const float& val) const
        {
            Matrix<R,C> tmp;
            for (int i = 0; i < R; i++)
                for (int j = 0; j < C; j++)
                    tmp[i][j] = m[i][j] / val;
            return tmp;
        }

        // Matrix addition assignement.
        void operator+=(const float& val)
        {
            for (int i = 0; i < R; i++)
                for (int j = 0; j < C; j++)
                    m[i][j] += val;
        }

        template<int _R, int _C>
        void operator+=(const Matrix<_R,_C>& matrix)
        {
            assert(R == _R && C == _C);
            for (int i = 0; i < R; i++)
                for (int j = 0; j < C; j++)
                    m[i][j] += matrix[i][j];
        }

        // Matrix substraction assignement.
        void operator-=(const float &val)
        {
            for (int i = 0; i < R; i++)
                for (int j = 0; j < C; j++)
                    m[i][j] -= val;
        }

        template<int _R, int _C>
        void operator-=(const Matrix<_R,_C>& matrix)
        {
            assert(R == _R && C == _C);
            for (int i = 0; i < R; i++)
                for (int j = 0; j < C; j++)
                    m[i][j] -= matrix[i][j];
        }

        // Matrix multiplication assignement.
        void operator*=(const float &val)
        {
            for (int i = 0; i < R; i++)
                for (int j = 0; j < C; j++)
                    m[i][j] *= val;
        }

        // ----- Methods ----- //

        // Getters.
        int getRows() { return R; }
        int getColumns() { return C; }  
        float getMatrixValue(int i, int j) { return m[i][j]; }

        // Setters.

        // Arithmetic.
        bool isSquare() { return R == C; }

        bool isIdentity()
        {
            for (int i = 0; i < R; i++)
                for (int j = 0; j < C; j++)
                    if ((i != j && m[i][j] != 0) || (i == j && m[i][j] != 1))
                        return false;
            return true;
        }

        // Determinants.
        float det2() 
        { 
            return (m[0][0] * m[1][1]) - (m[0][1] * m[1][0]); 
        }

        float det3()
        {
            return m[0][0] * (Mat2){m[1][1], m[1][2], m[2][1], m[2][2]}.det2() - 
                   m[0][1] * (Mat2){m[1][0], m[1][2], m[2][0], m[2][2]}.det2() + 
                   m[0][2] * (Mat2){m[1][0], m[1][1], m[2][0], m[2][1]}.det2();
        }

        float det4()
        {
            Mat3 a(m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]);
            Mat3 b(m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]);
            Mat3 c(m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]);
            Mat3 d(m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]);

            return (a.det3() * m[0][0] - b.det3() * m[0][1] + c.det3() * m[0][2] - d.det3() * m[0][3]);
        }

        // Inverses.
        Mat2 inv2()
        {
            Mat2 val(m[1][1], -m[0][1], -m[1][0], m[0][0]);
            return val / val.det2();
        }

        Mat3 inv3()
        {
            Mat4 val(m[0][0], m[0][1], m[0][2], 0,
                     m[1][0], m[1][1], m[1][2], 0,
                     m[2][0], m[2][1], m[2][2], 0,
                     0,       0,       0,       1);
            val = val.inv4();
            Mat3 result(val.m[0][0], val.m[0][1], val.m[0][2],
                        val.m[1][0], val.m[1][1], val.m[1][2],
                        val.m[2][0], val.m[2][1], val.m[2][2]);
            return result;
        }

        Mat4 inv4()
        {
            Mat2 a(m[0][0], m[0][1], m[1][0], m[1][1]);
            Mat2 b(m[0][2], m[0][3], m[1][2], m[1][3]);
            Mat2 c(m[2][0], m[2][1], m[3][0], m[3][1]);
            Mat2 d(m[2][2], m[2][3], m[3][2], m[3][3]);
            
            Mat4 result =
            {
                (a - b * d.inv2() * c).inv2(), -(a - b * d.inv2() * c).inv2() * b * d.inv2(),
                -(d - c * a.inv2() * b).inv2() * c * a.inv2(), (d - c * a.inv2() * b).inv2()
            };

            return result;
        }


        void print()
        {
            // Print data
            printf("Matrix<%d,%d>\n", R, C);
            
            // Print content
            for (int i = 0; i < R; i++)
            {
                for (int j = 0; j < C; j++) printf("%.2f, ", m[i][j]);
                printf("\n");
            }
            printf("\n");
        }
};

}