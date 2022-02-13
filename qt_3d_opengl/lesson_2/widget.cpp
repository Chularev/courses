#include "widget.h"
#include "simpleobject3d.h"

#include <QOpenGLContext>

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

Widget::~Widget()
{
}

void Widget::initializeGL()
{
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    initShaders();
    initCube(0.7f);
    m_objects[0]->translate(QVector3D(-0.5, 0.0, 0.0));

    initCube(0.7f);
    m_objects[1]->translate(QVector3D(0.5, 0.0, 0.0));
}

void Widget::resizeGL(int w, int h)
{
    float aspect = w / (h ? (float)h : 1);
    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(45, aspect,0.1f,10.f);

}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 viewMatrix;
    viewMatrix.setToIdentity();
    viewMatrix.translate(0.0f,0.0f,-2.0f);
    viewMatrix.rotate(m_rotation);

    m_program.bind();
    m_program.setUniformValue("u_projectionMatrix", m_projectionMatrix);
    m_program.setUniformValue("u_viewMatrix", viewMatrix);

    m_program.setUniformValue("u_lightPosition", QVector4D(0.0,0.0,0.0,1.0));
    m_program.setUniformValue("u_lightPower", 2.0f);

    for (int i = 0;  i < m_objects.size(); ++i ) {
        m_objects[i]->draw(&m_program, context()->functions());

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

    m_rotation = QQuaternion::fromAxisAndAngle(axis, angle) * m_rotation;

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

