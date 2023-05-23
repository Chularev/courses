#include "skybox.h"
#include "simpleobject3d.h"

#include <QImage>

SkyBox::SkyBox(float width, const QImage &texture)
{
    float width_div_2 = width / 2.0f;
    QVector<VertexData> vertexes;

    //Позади нас
    QVector3D normal(0.0f,0.0f,-1.0f);
    vertexes.append({QVector3D(-width_div_2, width_div_2, width_div_2),QVector2D(1.0f, 2.0f/3.0f), normal});
    vertexes.append({QVector3D(-width_div_2, -width_div_2, width_div_2),QVector2D(1.0f, 1.0f/3.0f), normal});
    vertexes.append({QVector3D(width_div_2, width_div_2, width_div_2),QVector2D(3.0f/4.0f, 2.0f/3.0f), normal});
    vertexes.append({QVector3D(width_div_2, -width_div_2, width_div_2),QVector2D(3.0f/4.0f, 1.0f/3.0f), normal});

    //Cправа от нас
    normal = QVector3D(-1.0f,0.0f,0.0f);
    vertexes.append({QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(3.0f/4.0f, 2.0f/3.0f), normal});
    vertexes.append({QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(3.0f/4.0f, 1.0f/3.0f), normal});
    vertexes.append({QVector3D(width_div_2, width_div_2, -width_div_2),QVector2D(2.0f/4.0f, 2.0f/3.0f), normal});
    vertexes.append({QVector3D(width_div_2, -width_div_2, -width_div_2),QVector2D(2.0f/4.0f, 1.0f/3.0f),normal});

    //Над нами
    normal = QVector3D(0.0f,-1.0f,0.0f);
    vertexes.append({QVector3D(width_div_2, width_div_2, width_div_2),QVector2D(2.0f/4.0f, 1.0f),normal});
    vertexes.append({QVector3D(width_div_2, width_div_2, -width_div_2),QVector2D(2.0f/4.0f, 2.0f/3.0f), normal});
    vertexes.append({QVector3D(-width_div_2, width_div_2, width_div_2),QVector2D(1.0f/4.0f, 1.0f), normal});
    vertexes.append({QVector3D(-width_div_2, width_div_2, -width_div_2),QVector2D(1.0f/4.0f, 2.0f/3.0f), normal});

    //Перед нами
    normal = QVector3D(0.0f,0.0f,1.0f);
    vertexes.append({QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(2.0f/4.0f, 2.0f/3.0f), normal});
    vertexes.append({QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(2.0f/4.0f, 1.0f/3.0f), normal});
    vertexes.append({QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(1.0f/4.0f, 2.0f/3.0f), normal});
    vertexes.append({QVector3D(-width_div_2, -width_div_2, -width_div_2),QVector2D(1.0f/4.0f, 1.0f/3.0f ), normal});

    //Слева от нас
    normal = QVector3D(1.0f,0.0f,0.0f);
    vertexes.append({QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(0.0f, 2.0f/3.0f), normal});
    vertexes.append({QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(1.0f/4.0f, 2.0f/3.0f), normal});
    vertexes.append({QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(0.0f, 1.0f/3.0f), normal});
    vertexes.append({QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(1.0f/4.0f, 1.0f/3.0f), normal});

    //Под нами
    normal = QVector3D(0.0f,1.0f,0.0f);
    vertexes.append({QVector3D(-width_div_2, -width_div_2, width_div_2),QVector2D(1.0f/4.0f, 0.0f), normal});
    vertexes.append({QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(1.0f/4.0f, 1.0f/3.0f), normal});
    vertexes.append({QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(2.0f/4.0f, 0.0f), normal});
    vertexes.append({QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(2.0f/4.0f, 1.0f/3.0f), normal});
    //============================================
    QVector<GLuint> indexex;
    for (int i = 0; i < 24; i += 4 ) {

        indexex.append(i + 0);
        indexex.append(i + 2);
        indexex.append(i + 1);


        indexex.append(i + 2);
        indexex.append(i + 3);
        indexex.append(i + 1);
    }

    m_cube = new SimpleObject3D(vertexes,indexex, texture);
}

SkyBox::~SkyBox()
{
    delete m_cube;
}

void SkyBox::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    m_cube->draw(program,functions);
}

void SkyBox::translate(const QVector3D &translateVector)
{
    (void) translateVector;

}

void SkyBox::rotate(const QQuaternion &r)
{
    (void)r;
}

void SkyBox::scale(const float &s)
{
    (void)s;
}

void SkyBox::setGlobalTransform(QMatrix4x4 &s)
{
    (void)s;
}
