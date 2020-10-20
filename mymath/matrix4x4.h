#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include <iostream>
#include <QOpenGLFunctions_4_1_Core>

class Matrix4x4
{
    static const unsigned int matrixSize = 4;

    float xy[matrixSize][matrixSize];
    //First is row, Second is column

    void initializeMatrix();

public:
    Matrix4x4();

    GLfloat *data()    {
        return (GLfloat*)xy;
    }

    void randomizeMatrix(); //For debugging purposes

    void setValueFrom2Dvec(int a, int b, float value){
        xy[a][b] = value;
    }
    float getValueFrom2Dvec(int a, int b){
        return xy[a][b];
    }

    friend std::ostream& operator<< (std::ostream&, const Matrix4x4&);

    void setToIdentity();
    Matrix4x4 operator = (Matrix4x4);
    Matrix4x4 operator * (float c);
    class Vector4D operator * (Vector4D v);
    Matrix4x4 operator * (Matrix4x4 m);
    void scale(float sX, float sY, float sZ);   //Her kan vi erstatte parameteret med en 3d/4d-vektor
    void rotate(class Vector3D v, float angle);
    void translate(float tX, float tY, float tZ);   //Her kan vi erstatte parameteret med en 3d/4d-vektor

//    void projeksjonsmatrise();
//    void gaussEliminasjon();
//    Matrix4x4 inverseMatrix();
};

inline std::ostream& operator<< (std::ostream& os, const Matrix4x4& m)   {
    for(int i = 0; i < m.matrixSize; i++)
    {
        os << std::endl;
        for(int j = 0; j < m.matrixSize; j++)
        {
            os << m.xy[i][j] << " ";
        }
    }

    return os;
}

#endif // MATRIX4X4_H
