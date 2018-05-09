#ifndef MOPENGLWIDGET_H
#define MOPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>

class MOpenGLWidget : public QOpenGLWidget
{
public:
    MOpenGLWidget();
    virtual ~MOpenGLWidget() {
        delete [] vertices;
        delete [] indices;
    }

protected:
    QOpenGLShaderProgram simple;
    GLuint m_posAttr;
    GLuint m_matrixUniform;
    QMatrix4x4 mvpMatrix;
    int frameNr = 0;
    GLfloat *vertices;
    GLshort *indices;

    float zoom = 6.0;

    const unsigned int planeSegments = 2;
    const float planeSize = 1.0;

    void initializeGL() {
        QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
        f->glClearColor(0.0, 0.0, 0.0, 0.0);

        simple.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vsh");
        simple.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.fsh");
        simple.link();

        m_posAttr = simple.attributeLocation("posAttr");
        m_matrixUniform = simple.uniformLocation("matrix");


        vertices = new GLfloat[planeSegments * planeSegments * 2];
        for(unsigned int col = 0; col < planeSegments + 1; col++) {
            for(unsigned int row = 0; row < planeSegments + 1; row++) {
                unsigned int index = 2 * (row * (planeSegments + 1) + col);
                qDebug() << "X: " << col << " Y: " << row << " Index: " << index << " = (" << col / float(planeSegments) << "|"
                         << row / float(planeSegments) << ")";
                vertices[index + 0] = col / float(planeSegments);
                vertices[index + 1] = row / float(planeSegments);
                //vertices[index + 2] = 2.0f;
            }
        }

        indices = new GLshort[planeSegments * planeSegments * 6];
        unsigned int i = 0;
        for(unsigned int col = 0; col < planeSegments; col++) {
            for(unsigned int row = 0; row < planeSegments; row++) {
                // (col|row) is always the upper left point for the triangle
                // vertices has the position
                indices[i++] = row * (planeSegments + 1) + col;
                indices[i++] = row * (planeSegments + 1) + col + 1;
                indices[i++] = (row + 1) * (planeSegments + 1) + col + 1;

                indices[i++] = row * (planeSegments + 1) + col;
                indices[i++] = (row + 1) * (planeSegments + 1) + col;
                indices[i++] = (row + 1) * (planeSegments + 1) + col + 1;
                qDebug() << "Index: " << i;

            }
        }

        qDebug() << i;
        for(unsigned int i=0; i<24; i++) qDebug() << "Index=" << indices[i];
        qDebug() << vertices[0] << vertices[1];
        qDebug() << vertices[2] << vertices[3];
        qDebug() << vertices[4] << vertices[5];
        qDebug() << "-";
        qDebug() << vertices[6] << vertices[7];

        /*
        vertices = new GLfloat[6];
        vertices[0] = 0.0f;
        vertices[1] = 0.5f;

        vertices[2] = 0.0f;
        vertices[3] = -0.5f;

        vertices[4] = 0.5f;
        vertices[5] = 0.0f;
        */
    }

    void resizeGL(int w, int h) {
        QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
        f->glViewport(0, 0, w, h);

        mvpMatrix.setToIdentity();
        mvpMatrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
        mvpMatrix.translate(0, 0, -2);
    }

    void paintGL() {
        QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
        f->glClear(GL_COLOR_BUFFER_BIT);
        simple.bind();

        mvpMatrix.setToIdentity();
        mvpMatrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
        mvpMatrix.translate(-0.5, -0.5, -zoom);
        //mvpMatrix.rotate(10.0f * frameNr, 0, 1, 0);
        simple.setUniformValue(m_matrixUniform, mvpMatrix);

        f->glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
        f->glEnableVertexAttribArray(0);



        //f->glDrawArrays(GL_TRIANGLES, 0, 6);

        f->glDrawElements(GL_TRIANGLES, planeSegments * planeSegments * 6, GL_UNSIGNED_SHORT, indices);

        f->glDisableVertexAttribArray(0);

        ++frameNr;
        update();
    }
};

#endif // MOPENGLWIDGET_H
