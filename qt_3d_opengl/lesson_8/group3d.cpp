#include "group3d.h"
#include "simpleobject3d.h"

Group3D::Group3D()
    : m_scale(1.0f)
{

}

void Group3D::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    for (int i = 0; i < m_objects.size(); ++i) {
        m_objects[i]->draw(program, functions);
    }

}

void Group3D::translate(const QVector3D &translateVector)
{
    m_translate += translateVector;

    QMatrix4x4 localMatrix;
    localMatrix.setToIdentity();
    localMatrix.translate(m_translate);
    localMatrix.rotate(m_rotate);
    localMatrix.scale(m_scale);
    localMatrix = m_globalTransform * localMatrix;

    for (int i = 0; i < m_objects.size(); ++i)
        m_objects[i]->setGlobalTransform(localMatrix);
}

void Group3D::rotate(const QQuaternion &r)
{
    m_rotate = r * m_rotate;

    QMatrix4x4 localMatrix;
    localMatrix.setToIdentity();
    localMatrix.translate(m_translate);
    localMatrix.rotate(m_rotate);
    localMatrix.scale(m_scale);
    localMatrix = m_globalTransform * localMatrix;

    for (int i = 0; i < m_objects.size(); ++i)
        m_objects[i]->setGlobalTransform(localMatrix);
}

void Group3D::scale(const float &s)
{
    m_scale *= s;

    QMatrix4x4 localMatrix;
    localMatrix.setToIdentity();
    localMatrix.translate(m_translate);
    localMatrix.rotate(m_rotate);
    localMatrix.scale(m_scale);
    localMatrix = m_globalTransform * localMatrix;

    for (int i = 0; i < m_objects.size(); ++i)
        m_objects[i]->setGlobalTransform(localMatrix);
}

void Group3D::addObject(Transformational *object)
{
    m_objects.append(object);

    QMatrix4x4 localMatrix;
    localMatrix.setToIdentity();
    localMatrix.translate(m_translate);
    localMatrix.rotate(m_rotate);
    localMatrix.scale(m_scale);
    localMatrix = m_globalTransform * localMatrix;

    m_objects[m_objects.size() - 1]->setGlobalTransform(localMatrix);
}

void Group3D::delObject(Transformational *object)
{
    m_objects.removeAll(object);
}

void Group3D::delObject(const int &index)
{
    m_objects.remove(index);
}

void Group3D::setGlobalTransform(QMatrix4x4 &g)
{
    m_globalTransform = g;

    QMatrix4x4 localMatrix;
    localMatrix.setToIdentity();
    localMatrix.translate(m_translate);
    localMatrix.rotate(m_rotate);
    localMatrix.scale(m_scale);
    localMatrix = m_globalTransform * localMatrix;

    for (int i = 0; i < m_objects.size(); ++i)
        m_objects[i]->setGlobalTransform(localMatrix);
}
