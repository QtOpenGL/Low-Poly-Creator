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
#include "./Objects/ModelMesh.h"
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
    void draw_model(ModelMesh *mesh,
                    QMatrix4x4,
                    QMatrix4x4,
                    QMatrix4x4,
                    QVector3D color);
    void draw_model_vertices(ModelMesh *mesh,
                    QMatrix4x4,
                    QMatrix4x4,
                    QMatrix4x4,
                    QVector3D color,
                    bool selected);
    void draw_line(QVector3D point1,
                   QVector3D point2,
                   QMatrix4x4 world_view_projection,
                   QMatrix4x4 model_view_projection,
                   QVector3D color);
    void draw_shader(ModelMesh * mesh, int type);
    void draw_flat_box(QVector3D position1, QVector3D position2);
    void update_shaders(QMatrix4x4, QMatrix4x4, QMatrix4x4, QVector3D );
    void load_content();
    void draw_grid();
    void edit_vertices();
    void add_vertices();
    void translate_vertices();
    int sign(int x);

    // raycasting prototype
    QVector3D mouse_raycast(int, int, QMatrix4x4);

    // intersection with y = 0
    QVector3D intersect_plane(QVector3D direction,
                              QVector3D point);

    QMatrix4x4 projection_matrix,
               view_matrix,
               camera_transformation
               ;
    QOpenGLShaderProgram *shader_program;
    QVector3D *position,
              *clicked_position,
              *current_position;
    ModelMesh *current_mesh;
    int mouse_x,
        mouse_y,
        dmouse_x,
        dmouse_y,
        type_of_view,
        viewport_width,
        viewport_height,
        mouse_right_clicked_x,
        mouse_right_clicked_y,
        mouse_relative_x_drag,
        mouse_relative_y_drag,
        prev_rotation_x,
        prev_rotation_y;
    float mouse_zoom, view_angle;
    QVector3D position_camera,
              camera_previous,
              raycast_direction;
    bool mousedown_right,
         mousedown_left,
         edit_vertex_enable,
         remove_vertex_enable,
         add_vertex_enable,
         translate_enable;

  signals:
    void update_frame();

  private slots:
    void update_frame_from_extern();
    void change_camara(int type);
    void enable_vertex_edit(bool value);
    void enable_vertex_add(bool value);
    void enable_vertex_remove(bool value);
    void enable_translate(bool value);
};

#endif // RENDERSTATE_H
