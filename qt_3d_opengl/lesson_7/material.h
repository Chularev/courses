#ifndef MATERIAL_H
#define MATERIAL_H

#include <QString>
#include <QVector2D>
#include <QImage>

class Material
{
public:
    Material();

    const QString &mtlName() const;
    void setMtlName(const QString &newMtlName);

    const QVector3D &diffuseColor() const;
    void setDiffuseColor(const QVector3D &newDiffuseColor);

    const QVector3D &ambienceColor() const;
    void setAmbienceColor(const QVector3D &newAmbienceColor);


    const qreal &shinnes() const;
    void setShinnes(qreal newShinnes);

    const QImage diffuseMap() const;
    void setDiffuseMap(const QString &path);

    QVector3D specularColor() const;
    void setSpecularColor(const QVector3D &newSpecularColor);

private:
    QString m_mtlName;
    QVector3D m_diffuseColor;
    QVector3D m_ambienceColor;
    QVector3D m_specularColor;
    qreal m_shinnes;
    QImage m_diffuseMap;
};

#endif // MATERIAL_H
