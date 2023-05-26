#include "materiallibrary.h"
#include "material.h"

#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QFileInfo>
#include <QDir>

MaterialLibrary::MaterialLibrary()
{

}

void MaterialLibrary::addMaterial(Material *material)
{
    if (!material) return;

    for (const Material *mtl : m_materials) {
        if (mtl == material)
            return;
    }
    m_materials.append(material);
}

Material *MaterialLibrary::getMaterial(qint32 index)
{
    if (index < m_materials.size())
        return m_materials[index];
    return nullptr;
}

Material *MaterialLibrary::getMaterial(const QString &mtlName)
{
    for (Material *mtl : m_materials) {
        if (mtl->mtlName() == mtlName)
            return mtl;
    }
    return nullptr;
}

qint32 MaterialLibrary::getCountMaterials() const
{
    return m_materials.size();
}

void MaterialLibrary::loadMaterialsFromFile(const QString &filename)
{
    QFile m_mtlFile(filename);
    if (!m_mtlFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Can not open mtl file " << filename;
        return;
    }


    for (Material *m : m_materials) {
        delete m;
    }
    m_materials.clear();


    QTextStream m_inputStream(&m_mtlFile);

    Material *newMtl = nullptr;

    while (!m_inputStream.atEnd()) {
        QString str = m_inputStream.readLine();
        QStringList list = str.split(" ");
        if (list[0] == "#") {
            qDebug() << "This is comment:" << str;
        } else if (list[0] == "newmtl") {
            addMaterial(newMtl);
            newMtl = new Material();
            newMtl->setMtlName(list[1]);
        }  if (list[0] == "Ns") {
            if (newMtl) newMtl->setShinnes(list[1].toFloat());
        }  if (list[0] == "Ka") {
            newMtl->setAmbienceColor({list[1].toFloat(),list[2].toFloat(),list[3].toFloat()});
        }   if (list[0] == "Kd") {
            if (newMtl) newMtl->setDiffuseColor({list[1].toFloat(),list[2].toFloat(),list[3].toFloat()});
        }   if (list[0] == "Ks") {
            newMtl->setSpecularColor({list[1].toFloat(),list[2].toFloat(),list[3].toFloat()});
        } if (list[0] == "map_Kd") {
            QFileInfo fileInfo(filename);
            QString path =fileInfo.absoluteFilePath() + QDir::separator() +
                           list[1];
            if (newMtl) newMtl->setDiffuseMap(path);
        }
    }

    addMaterial(newMtl);

    m_mtlFile.close();
}

