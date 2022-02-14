#ifndef GROUP3D_H
#define GROUP3D_H

#include "transformational.h"

#include <QQuaternion>
#include <QMatrix4x4>
#include <QVector>

class SimpleObject3D;

class Group3D: public Transformational
{
public:
    Group3D();

    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions);

    void translate(const QVector3D &translateVector);

    void rotate(const QQuaternion &r);
    void scale(const float &s);
    void setGlobalTransform(QMatrix4x4 &s);

    void addObject(Transformational *object);

private:
    QQuaternion m_rotate;
    QVector3D m_translate;
    float m_scale;
    QMatrix4x4 m_globalTransform;

    QVector<Transformational *> m_objects;
};

#endif // GROUP3D_H
