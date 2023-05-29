#ifndef MATERIALLIBRARY_H
#define MATERIALLIBRARY_H

#include <QVector>

class Material;

class MaterialLibrary
{
public:
    MaterialLibrary();

    void addMaterial(Material *material);
    Material *getMaterial(qint32 index);
    Material *getMaterial(const QString& mtlName);
    qint32 getCountMaterials() const;
    void loadMaterialsFromFile(const QString& filename);
private:
    QVector<Material *> m_materials;
};

#endif // MATERIALLIBRARY_H
