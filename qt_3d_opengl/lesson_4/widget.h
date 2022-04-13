#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMatrix4x4>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QtOpenGL/QOpenGLShaderProgram>
#include <QtOpenGL/QOpenGLTexture>
#include <QtOpenGL/QOpenGLBuffer>
#include <QMouseEvent>
#include <QBasicTimer>

class SimpleObject3D;
class Group3D;
class Camera3D;
class Transformational;

class Widget : public QOpenGLWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

    void initShaders();
private:
    void initCube(float width);

    QMatrix4x4 m_projectionMatrix;
    QOpenGLShaderProgram m_program;
    QVector2D m_mousePosition;
    QVector<SimpleObject3D *> m_objects;
    QVector<Group3D *> m_groups;
    Camera3D *m_camera;
    QVector<Transformational *> m_transformObjects;

    QBasicTimer m_timer;

    float angleObject;
    float angleGroup1;
    float angleGroup2;
    float angleMain;
};
#endif // WIDGET_H
