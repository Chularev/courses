#include "widget.h"
#include "simpleobject3d.h"
#include "transformational.h"
#include "group3d.h"
#include "camera3d.h"

#include <QOpenGLContext>
#include <QtMath>
#include <QKeyEvent>

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent),
      m_camera(new Camera3D())
{
    m_camera->translate({0.0f,0.0f,-5.0f});
}

Widget::~Widget()
{
    delete m_camera;
    for (int i = 0; i < m_objects.size(); i++)
    {
        delete m_objects[i];
    }

    for (int i = 0; i < m_groups.size(); i++)
    {
        delete m_groups[i];
    }

    for (int i = 0; i < m_transformObjects.size(); i++)
    {
        delete m_transformObjects[i];
    }
}

void Widget::initializeGL()
{
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    initShaders();

    int step = 2;
    m_groups.append(new Group3D);

    for (int x = -step; x <= step; x += step) {
        for (int y = -step; y <= step; y += step) {
            for (int z = -step; z <= step; z += step) {
                initCube(0.7f);
                m_objects[m_objects.size() - 1]->translate(QVector3D(x, y, z));
                m_groups[m_groups.size() - 1]->addObject(m_objects[m_objects.size() - 1]);
            }
        }
    }
    m_groups[0]->translate(QVector3D(-4.0f,0.0f,0.0f));

    m_groups.append(new Group3D);

    for (int x = -step; x <= step; x += step) {
        for (int y = -step; y <= step; y += step) {
            for (int z = -step; z <= step; z += step) {
                initCube(0.7f);
                m_objects[m_objects.size() - 1]->translate(QVector3D(x, y, z));
                m_groups[m_groups.size() - 1]->addObject(m_objects[m_objects.size() - 1]);
            }
        }
    }
    m_groups[1]->translate(QVector3D(4.0f,0.0f,0.0f));

    m_groups.append(new Group3D);
    m_groups[2]->addObject(m_groups[0]);
    m_groups[2]->addObject(m_groups[1]);

    m_transformObjects.append(m_groups[2]);

    m_groups[0]->addObject(m_camera);

    m_timer.start(30, this);
}

void Widget::resizeGL(int w, int h)
{
    float aspect = w / (h ? (float)h : 1);
    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(45, aspect, 0.01f, 100.f);
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    m_program.bind();
    m_program.setUniformValue("u_projectionMatrix", m_projectionMatrix);


    m_program.setUniformValue("u_lightPosition", QVector4D(0.0,0.0,0.0,1.0));
    m_program.setUniformValue("u_lightPower", 2.0f);

    m_camera->draw(&m_program);
    for (int i = 0;  i < m_transformObjects.size(); ++i ) {
        m_transformObjects[i]->draw(&m_program, context()->functions());

    }
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
        m_mousePosition = QVector2D(event->position());
    event->accept();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() != Qt::LeftButton)
        return;

    QVector2D diff = QVector2D(event->position()) - m_mousePosition;
    m_mousePosition = QVector2D(event->position());

    float angle = diff.length() / 2.0;
    QVector3D axis = QVector3D(diff.y(), diff.x(), 0);

    m_camera->rotate(QQuaternion::fromAxisAndAngle(axis, angle));

    update();
}

void Widget::wheelEvent(QWheelEvent *event)
{
    double delta = event->angleDelta().y();
    if (delta > 0)
    {
        m_camera->translate({0.0f,0.0f,0.25f});
    } else if (delta < 0) {
        m_camera->translate({0.0f,0.0f,-0.25f});
    }
    update();
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
        m_groups[0]->delObject(m_camera);
        m_groups[1]->addObject(m_camera);
        break;
    case Qt::Key_Right:
        m_groups[1]->delObject(m_camera);
        m_groups[0]->addObject(m_camera);
        break;
    case Qt::Key_Down:
        m_groups[1]->delObject(m_camera);
        m_groups[0]->delObject(m_camera);
        break;
    case Qt::Key_Up:
        m_groups[1]->delObject(m_camera);
        m_groups[0]->delObject(m_camera);

        QMatrix4x4 tmp;
        tmp.setToIdentity();
        m_camera->setGlobalTransform(tmp);

        break;
    }
    update();

}

void Widget::timerEvent(QTimerEvent *event)
{
    for (int i = 0; i < m_objects.size(); ++i) {
        if (i % 2 == 0) {
            m_objects[i]->rotate(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, qSin(angleObject)));
            m_objects[i]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, qCos(angleObject)));
        } else {
            m_objects[i]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, qSin(angleObject)));
            m_objects[i]->rotate(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, qCos(angleObject)));
        }
    }

    m_groups[0]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 0.0f, 1.0f, qSin(angleGroup1)));
    m_groups[0]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, -qSin(angleGroup1)));

    m_groups[1]->rotate(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, qCos(angleGroup2)));
    m_groups[1]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, -qCos(angleGroup2)));


    m_groups[2]->rotate(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, qSin(angleMain)));
    m_groups[2]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, qCos(angleMain)));

    angleObject += M_PI / 100.0f;
    angleGroup1 += M_PI / 360.0f;
    angleGroup2 += M_PI / 360.0f;
    angleMain += M_PI / 720.0f;

    update();
}

void Widget::initShaders()
{
    if(!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                      ":/vshader.vsh"))
        close();

    if(!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                          ":/fshader.fsh"))
        close();

    if(!m_program.link())
        close();
}

void Widget::initCube(float width)
{
    float width_div_2 = width / 2.0f;
    QVector<VertexData> vertexes;

    //========      1       ===============================
    vertexes.append(
        VertexData(
                QVector3D(-width_div_2, width_div_2, width_div_2),
                QVector2D(0.0f, 1.0f),
                QVector3D(0.0f,0.0f,1.0f)
        )
    );

    vertexes.append(
        VertexData(
                QVector3D(-width_div_2, -width_div_2, width_div_2),
                QVector2D(0.0f, 0.0f),
                QVector3D(0.0f,0.0f,1.0f)
        )
    );

    vertexes.append(
        VertexData(
                QVector3D(width_div_2, width_div_2, width_div_2),
                QVector2D(1.0f, 1.0f),
                QVector3D(0.0f,0.0f,1.0f)
        )
    );

    vertexes.append(
        VertexData(
                QVector3D(width_div_2, -width_div_2, width_div_2),
                QVector2D(1.0f, 0.0f),
                QVector3D(0.0f,0.0f,1.0f)
        )
    );
    //========      2       ===============================
    vertexes.append(
        VertexData(
                QVector3D(width_div_2, width_div_2, width_div_2),
                QVector2D(0.0f, 1.0f),
                QVector3D(1.0f,0.0f,0.0f)
        )
    );

    vertexes.append(
        VertexData(
                QVector3D(width_div_2, -width_div_2, width_div_2),
                QVector2D(0.0f, 0.0f),
                QVector3D(1.0f,0.0f,0.0f)
        )
    );

    vertexes.append(
        VertexData(
                QVector3D(width_div_2, width_div_2, -width_div_2),
                QVector2D(1.0f, 1.0f),
                QVector3D(1.0f,0.0f,0.0f)
        )
    );

    vertexes.append(
        VertexData(
                QVector3D(width_div_2, -width_div_2, -width_div_2),
                QVector2D(1.0f, 0.0f),
                QVector3D(1.0f,0.0f,0.0f)
        )
    );

    //========      3       ===============================
    vertexes.append(
        VertexData(
                QVector3D(width_div_2, width_div_2, width_div_2),
                QVector2D(0.0f, 1.0f),
                QVector3D(0.0f,1.0f,0.0f)
        )
    );

    vertexes.append(
        VertexData(
                QVector3D(width_div_2, width_div_2, -width_div_2),
                QVector2D(0.0f, 0.0f),
                QVector3D(0.0f,1.0f,0.0f)
        )
    );

    vertexes.append(
        VertexData(
                QVector3D(-width_div_2, width_div_2, width_div_2),
                QVector2D(1.0f, 1.0f),
                QVector3D(0.0f,1.0f,0.0f)
        )
    );

    vertexes.append(
        VertexData(
                QVector3D(-width_div_2, width_div_2, -width_div_2),
                QVector2D(1.0f, 0.0f),
                QVector3D(0.0f,1.0f,0.0f)
        )
    );

    //========      4       ===============================
    vertexes.append(
        VertexData(
                QVector3D(width_div_2, width_div_2, -width_div_2),
                QVector2D(0.0f, 1.0f),
                QVector3D(0.0f,0.0f,-1.0f)
        )
    );

    vertexes.append(
        VertexData(
                QVector3D(width_div_2, -width_div_2, -width_div_2),
                QVector2D(0.0f, 0.0f),
                QVector3D(0.0f,0.0f,-1.0f)
        )
    );

    vertexes.append(
        VertexData(
                QVector3D(-width_div_2, width_div_2, -width_div_2),
                QVector2D(1.0f, 1.0f),
                QVector3D(0.0f,0.0f,-1.0f)
        )
    );

    vertexes.append(
        VertexData(
                QVector3D(-width_div_2, -width_div_2, -width_div_2),
                QVector2D(1.0f, 0.0f),
                QVector3D(0.0f,0.0f,-1.0f)
        )
    );

    //========      5       ===============================
    vertexes.append(
        VertexData(
                QVector3D(-width_div_2, width_div_2, width_div_2),
                QVector2D(0.0f, 1.0f),
                QVector3D(-1.0f,0.0f,0.0f)
        )
    );

    vertexes.append(
        VertexData(
                QVector3D(-width_div_2, width_div_2, -width_div_2),
                QVector2D(0.0f, 0.0f),
                QVector3D(-1.0f,0.0f,0.0f)
        )
    );

    vertexes.append(
        VertexData(
                QVector3D(-width_div_2, -width_div_2, width_div_2),
                QVector2D(1.0f, 1.0f),
                QVector3D(-1.0f,0.0f,0.0f)
        )
    );

    vertexes.append(
        VertexData(
                QVector3D(-width_div_2, -width_div_2, -width_div_2),
                QVector2D(1.0f, 0.0f),
                QVector3D(-1.0f,0.0f,0.0f)
        )
    );

    //========      6       ===============================
    vertexes.append(
        VertexData(
                QVector3D(-width_div_2, -width_div_2, width_div_2),
                QVector2D(0.0f, 1.0f),
                QVector3D(0.0f,-1.0f,0.0f)
        )
    );

    vertexes.append(
        VertexData(
                QVector3D(-width_div_2, -width_div_2, -width_div_2),
                QVector2D(0.0f, 0.0f),
                QVector3D(0.0f,-1.0f,0.0f)
        )
    );

    vertexes.append(
        VertexData(
                QVector3D(width_div_2, -width_div_2, width_div_2),
                QVector2D(1.0f, 1.0f),
                QVector3D(0.0f,-1.0f,0.0f)
        )
    );

    vertexes.append(
        VertexData(
                QVector3D(width_div_2, -width_div_2, -width_div_2),
                QVector2D(1.0f, 0.0f),
                QVector3D(0.0f,-1.0f,0.0f)
        )
    );

    //============================================
    QVector<GLuint> indexex;
    for (int i = 0; i < 24; i += 4 ) {

        indexex.append(i + 0);
        indexex.append(i + 1);
        indexex.append(i + 2);

        indexex.append(i + 2);
        indexex.append(i + 1);
        indexex.append(i + 3);
    }

    m_objects.append(new SimpleObject3D(vertexes,indexex,QImage(":/cube.jpeg")));
}

