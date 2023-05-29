#ifndef SKYBOX_H
#define SKYBOX_H

#include "transformational.h"
#include "material.h"


class SimpleObject3D;
class QImage;

class SkyBox : public Transformational
{
public:
    SkyBox(float width, Material *mtl);
    ~SkyBox();

    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions);

    void translate(const QVector3D &translateVector);

    void rotate(const QQuaternion &r);
    void scale(const float &s);
    void setGlobalTransform(QMatrix4x4 &s);
private:
    SimpleObject3D *m_cube;
};

#endif // SKYBOX_H
