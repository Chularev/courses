#include "objectengine3d.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>

ObjectEngine3D::ObjectEngine3D()
{

}

void ObjectEngine3D::loadObjectFromFile(const QString &fileName)
{
    QFile objFile(fileName);
    if (!objFile.exists()) {
        qDebug() << "File not found";
        return;
    }
    objFile.open(QIODevice::ReadOnly);
    QTextStream input(&objFile);

    QStringList myOptions;
    myOptions << "#" << "mtllib" << "v"<<"vt"<<"vn"<<"f";

    QVector<QVector3D> coords;
    QVector<QVector2D> texCoords;
    QVector<QVector3D> normals;

    QVector<VertexData> vertexes;
    QVector<GLuint> indexes;
    SimpleObject3D *object = nullptr;
    QString mtlName;


    while (!input.atEnd()) {
        QString str = input.readLine();
        QStringList list = str.split(" ");
        if (list[0] == "#") {
            qDebug() << "This is comment:" << str;
        } else if (list[0] == "mtllib") {
            QFileInfo fileInfo(fileName);
            QString path =fileInfo.absolutePath() + "/" +
                           list[1].replace('\\', '/');
            m_materialLibrary.loadMaterialsFromFile(path);
        } else if (list[0] == "v") {
            coords.append({list[1].toFloat(),list[2].toFloat(),list[3].toFloat()});
        } else if (list[0] == "vt") {
            texCoords.append({list[1].toFloat(),list[2].toFloat()});
        } else if (list[0] == "vn") {
           normals.append({list[1].toFloat(),list[2].toFloat(),list[3].toFloat()});
        } else if (list[0] == "f") {// (poligons)
            for (int i = 1; i <= 3; ++i){
                QStringList vert = list[i].split("/");


                vertexes.append({
                    coords[vert[0].toLong() - 1], texCoords[vert[1].toLong() - 1], normals[vert[2].toLong() - 1]
                });
                indexes.append(indexes.size());
            }
        } else if (list[0] == "usemtl") {
            if (object)
                object->init(vertexes,indexes, m_materialLibrary.getMaterial(mtlName));
            addObject(object);
            object = new SimpleObject3D;
            vertexes.clear();
            indexes.clear();
            mtlName = list[1];
        }
    }
    if (object)
        object->init(vertexes,indexes, m_materialLibrary.getMaterial(mtlName));

    addObject(object);

    objFile.close();


}

void ObjectEngine3D::addObject(SimpleObject3D *obj)
{
    if (!obj) return;

    foreach (SimpleObject3D *tmp, m_objects) {
        if (tmp == obj)
            return;
    }
    m_objects.append(obj);
}

SimpleObject3D* ObjectEngine3D::getObject(quint32 index)
{
    if (index < (quint32) m_objects.size())
        return m_objects[index];
    return nullptr;
}

void ObjectEngine3D::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    foreach (auto *obj, m_objects) {
        obj->draw(program, functions);
    }
}

void ObjectEngine3D::translate(const QVector3D &translateVector)
{
    foreach (auto *obj, m_objects) {
        obj->translate(translateVector);
    }
}

void ObjectEngine3D::rotate(const QQuaternion &r)
{
    foreach (auto *obj, m_objects) {
        obj->rotate(r);
    }
}

void ObjectEngine3D::scale(const float &s)
{
    foreach (auto *obj, m_objects) {
        obj->scale(s);
    }
}

void ObjectEngine3D::setGlobalTransform(QMatrix4x4 &g)
{
    foreach (auto *obj, m_objects) {
        obj->setGlobalTransform(g);
    }
}
