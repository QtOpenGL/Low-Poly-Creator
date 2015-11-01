#ifndef RENDERSTATE_H
#define RENDERSTATE_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QMouseEvent>
#include <QVector3D>
#include <QTimer>
#include "Objects/ModelMesh.h"
#include "./currentscene.h"

class RenderState : public QOpenGLWidget, protected QOpenGLFunctions {
     Q_OBJECT
public:
    explicit RenderState(QWidget *parent = 0);
    ~RenderState();
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
    void mouseReleaseEvent(QMouseEvent *);
  private:
    QMatrix4x4 pMatrix; // dynamic memory control not needed
    void DrawModel(ModelMesh *, QMatrix4x4 , QMatrix4x4 , QMatrix4x4 /*, GLuint texture*/, QVector3D );
    void DrawLine(QVector3D point1,
                  QVector3D point2,
                  QMatrix4x4 world_view_projection,
                  QMatrix4x4 model_view_projection,
                  QVector3D color);
    void ShaderDraw(ModelMesh *);
    void UpdateShaders(QMatrix4x4 ,QMatrix4x4 , QMatrix4x4 /*, GLuint texture*/, QVector3D );
    void LoadContent();
    void draw_grid();
    QSize m_viewportSize;
    QOpenGLShaderProgram *m_program;
    qreal m_t;
    QVector3D *m_position, *m_clicked_position;
    QVector<QOpenGLTexture *> m_textures;
    ModelMesh *box,
              *sky,
              *wagen,
              *node,
              *m_plane;
    int m_mouse_x, m_mouse_y, m_dmouse_x,m_dmouse_y;
    float m_mouse_zoom;
    // define a view matrix
    QMatrix4x4 vMatrix;
    QVector3D *m_current_position;
    QVector3D m_position_camera, m_camera_prev, m_raycast;
    // raycasting prototype
    QVector3D mouseRayCast(int, int, QMatrix4x4);

    // intersection with y=0
    QVector3D intersectYnull(QVector3D,QVector3D);
    QMatrix4x4 camera_transformation;
    bool m_mousedown_right;

  signals:
    void update_frame();

  private slots:
    void update_frame_from_extern();
    void change_camara(int type);
};

#endif // RENDERSTATE_H
