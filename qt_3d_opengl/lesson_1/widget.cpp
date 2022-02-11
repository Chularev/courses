#include "widget.h"

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent),
      m_texture(0),
      m_indexBuffer(QOpenGLBuffer::IndexBuffer)
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
    initCube(1.0f);
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

    QMatrix4x4 modelViewMatrix;
    modelViewMatrix.setToIdentity();
    modelViewMatrix.translate(0.0f,0.0f,-2.0f);
    modelViewMatrix.rotate(30,1.0,0.0,0.0);
    modelViewMatrix.rotate(30,0.0,1.0,0.0);

    m_texture->bind(0);

    m_program.bind();
    m_program.setUniformValue("qt_ModelViewProjectionMatrix",
                              m_projectionMatrix * modelViewMatrix);
    m_program.setUniformValue("qt_Texture0", 0);

    m_arrayBuffer.bind();


    int offset = 0;
    int vertLoc = m_program.attributeLocation("qt_Vertex");
    m_program.enableAttributeArray(vertLoc);
    m_program.setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3, sizeof (VertexData));

    offset += sizeof (VertexData);
    int textLoc = m_program.attributeLocation("qt_MultiTexCoord0");
    m_program.enableAttributeArray(textLoc);
    m_program.setAttributeBuffer(textLoc, GL_FLOAT, offset, 2, sizeof (VertexData));

    m_indexBuffer.bind();
    glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT,0);

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

    m_arrayBuffer.create();
    m_arrayBuffer.bind();
    m_arrayBuffer.allocate(vertexes.constData(), vertexes.size() * sizeof(VertexData));
    m_arrayBuffer.release();

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indexex.constData(), indexex.size() * sizeof (GLint));
    m_indexBuffer.release();

    m_texture = new QOpenGLTexture(QImage(":/cube.jpeg").mirrored());

    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_texture->setMinificationFilter(QOpenGLTexture::Linear);

    m_texture->setWrapMode(QOpenGLTexture::Repeat);
}

