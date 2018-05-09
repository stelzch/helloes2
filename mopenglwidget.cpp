#include "mopenglwidget.h"

MOpenGLWidget::MOpenGLWidget()
{

}

MOpenGLWidget::~MOpenGLWidget() {
    delete [] vertices;
    delete [] indices;
}

void MOpenGLWidget::initializeGL() {
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor(0.0, 0.0, 0.0, 0.0);

    simple.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vsh");
    simple.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.fsh");
    simple.link();

    if(!simple.isLinked()) {
        qDebug() << "Error: " << simple.log();
        return;
    }

    m_posAttr = simple.attributeLocation("posAttr");
    m_matrixUniform = simple.uniformLocation("matrix");


    vertices = new GLfloat[(planeSegments + 1) * (planeSegments + 1) * 3];
    for(unsigned int col = 0; col < planeSegments + 1; col++) {
        for(unsigned int row = 0; row < planeSegments + 1; row++) {
            unsigned int index = 3 * (row * (planeSegments + 1) + col);
            /*qDebug() << "X: " << col << " Y: " << row << " Index: " << index << " = (" << col / float(planeSegments) << "|"
                     << row / float(planeSegments) << ")"; */
            vertices[index + 0] = col / float(planeSegments);
            vertices[index + 1] = row / float(planeSegments);
            vertices[index + 2] = 0.0f;
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

void MOpenGLWidget::resizeGL(int w, int h) {
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glViewport(0, 0, w, h);

    mvpMatrix.setToIdentity();
    mvpMatrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    mvpMatrix.translate(0, 0, -2);
}

void MOpenGLWidget::paintGL() {
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT);
    simple.bind();

    mvpMatrix.setToIdentity();
    mvpMatrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    mvpMatrix.translate(0, 0, -zoom);
    mvpMatrix.rotate(rotY, 1, 0, 0);
    mvpMatrix.rotate(rotX, 0, 0, 1);
    mvpMatrix.translate(-0.5, -0.5, 0);

    simple.setUniformValue(m_matrixUniform, mvpMatrix);

    f->glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    f->glEnableVertexAttribArray(0);



    //f->glDrawArrays(GL_TRIANGLES, 0, 6);

    f->glDrawElements(GL_TRIANGLES, planeSegments * planeSegments * 6, GL_UNSIGNED_SHORT, indices);

    f->glDisableVertexAttribArray(0);

    ++frameNr;
    update();
}

void MOpenGLWidget::mousePressEvent(QMouseEvent *evt) {
    mouseStartX = evt->globalX();
    mouseStartY = evt->globalY();
}

void MOpenGLWidget::mouseMoveEvent(QMouseEvent *evt) {
    float deltaX = evt->globalX() - mouseStartX;
    float deltaY = evt->globalY() - mouseStartY;

    rotX += deltaX * 0.4;
    rotY += deltaY * 0.4;

    mouseStartX = evt->globalX();
    mouseStartY = evt->globalY();
}
void MOpenGLWidget::mouseReleaseEvent(QMouseEvent *evt) {

}

void MOpenGLWidget::wheelEvent(QWheelEvent *event) {
    zoom -= event->angleDelta().y() * 0.001f;
    qDebug() << "Zoom: " << zoom;
}
