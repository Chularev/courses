#ifndef OBJECTENGINE3D_H
#define OBJECTENGINE3D_H

#include <QString>
#include <QVector>

#include "simpleobject3d.h"
#include "materiallibrary.h"
#include "transformational.h"

class ObjectEngine3D : public Transformational
{
public:
    ObjectEngine3D();
    void loadObjectFromFile(const QString &fileName);
    void addObject(SimpleObject3D *obj);
    SimpleObject3D* getObject(quint32 index);

    virtual void rotate(const QQuaternion &r);
    virtual void translate(const QVector3D &t);
    virtual void scale(const float &s);
    virtual void setGlobalTransform(QMatrix4x4 &s);
    virtual void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions);
private:
    QVector<SimpleObject3D *> m_objects;
    MaterialLibrary m_materialLibrary;
};

#endif // OBJECTENGINE3D_H
