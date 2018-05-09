#ifndef MOPENGLWIDGET_H
#define MOPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDragMoveEvent>

class MOpenGLWidget : public QOpenGLWidget
{
public:
    MOpenGLWidget();
    virtual ~MOpenGLWidget();

protected:
    QOpenGLShaderProgram simple;
    GLuint m_posAttr;
    GLuint m_matrixUniform;
    QMatrix4x4 mvpMatrix;
    int frameNr = 0;
    GLfloat *vertices;
    GLshort *indices;

    float zoom = 2.0;
    float rotX = 0.0, rotY = 0.0;

    float mouseStartX, mouseStartY;


    const unsigned int planeSegments = 2;
    const float planeSize = 1.0;

    void initializeGL();

    void resizeGL(int w, int h);

    void paintGL();

    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
};

#endif // MOPENGLWIDGET_H
