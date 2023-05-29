#include "material.h"

Material::Material()
{

}

const QVector3D &Material::ambienceColor() const
{
    return m_ambienceColor;
}

void Material::setAmbienceColor(const QVector3D &newAmbienceColor)
{
    m_ambienceColor = newAmbienceColor;
}

float Material::shinnes() const
{
    return m_shinnes;
}

void Material::setShinnes(float newShinnes)
{
    m_shinnes = newShinnes;
}

const QImage Material::diffuseMap() const
{
    return m_diffuseMap;
}

void Material::setDiffuseMap(const QString &path)
{
    isUsingDiffuseMap = true;
    m_diffuseMap = QImage(path);
}

QVector3D Material::specularColor() const
{
    return m_specularColor;
}

void Material::setSpecularColor(const QVector3D &newSpecularColor)
{
    m_specularColor = newSpecularColor;
}

bool Material::getIsUsingDiffuseMap() const
{
    return isUsingDiffuseMap;
}

const QVector3D &Material::diffuseColor() const
{
    return m_diffuseColor;
}

void Material::setDiffuseColor(const QVector3D &newDiffuseColor)
{
    m_diffuseColor = newDiffuseColor;
}

const QString &Material::mtlName() const
{
    return m_mtlName;
}

void Material::setMtlName(const QString &newMtlName)
{
    m_mtlName = newMtlName;
}
