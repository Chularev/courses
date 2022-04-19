#ifndef TRANSFORMATIONAL_H
#define TRANSFORMATIONAL_H

class QQuaternion;
class QVector3D;
class QMatrix4x4;
class QOpenGLShaderProgram;
class QOpenGLFunctions;

class Transformational
{
public:
    virtual void rotate(const QQuaternion &r) = 0;
    virtual void translate(const QVector3D &t) = 0;
    virtual void scale(const float &s) = 0;
    virtual void setGlobalTransform(QMatrix4x4 &s) = 0;
    virtual void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions) = 0;
    virtual ~Transformational(){}
};

#endif // TRANSFORMATIONAL_H
