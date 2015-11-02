#include "renderstate.h"
#include <math.h>

RenderState::RenderState(QWidget *parent): QOpenGLWidget(parent),
    shader_program(0),
    mouse_x(0),
    mouse_y(0),
    type_of_view(0),
    viewport_width(1),
    viewport_height(1),
    mouse_right_clicked_x(0),
    mouse_right_clicked_y(0),
    mouse_relative_x_drag(0),
    mouse_relative_y_drag(0),
    prev_rotation_x(0),
    prev_rotation_y(0),
    mouse_zoom(1000.0f),
    view_angle(45.0),
    position_camera(QVector3D()),
    camera_previous(QVector3D()),
    raycast_direction(QVector3D()),
    mousedown_right(false),
    mousedown_left(false),
    edit_vertex_enable(false),
    remove_vertex_enable(false),
    add_vertex_enable(false)
{
    // enable antialiasing
    QSurfaceFormat format;
    format.setSamples(4);
    this->setFormat(format);

    clicked_position = new QVector3D(0, 0, 0);

    // set the current mouse position in 3D
    this->current_position = new QVector3D(0, 0, 0);

    // set the position to initial null vector
    position = new QVector3D(0, 0, 0);

    // set mouse tracking
    setMouseTracking(true);

    //connect(this, SLOT(update_frame_from_extern()), this, SIGNAL(update_frame()));
    change_camara(2);
}

void RenderState::enable_vertex_edit(bool value) {
  this->edit_vertex_enable = value;
}

void RenderState::enable_vertex_add(bool value) {
  this->add_vertex_enable = value;
}

void RenderState::enable_vertex_remove(bool value) {
  this->remove_vertex_enable = value;
}

void RenderState::initializeGL() {
    initializeOpenGLFunctions();
}

void RenderState::update_frame_from_extern() {
    update();
}

void RenderState::mouseMoveEvent(QMouseEvent *event) {
  emit this->update_frame();
  if ( this->edit_vertex_enable && this->mousedown_left) {
    for (int a = 0; a < CurrentScene::mesh_count(); a++) {
      QMatrix4x4 translation;
      translation.translate(*CurrentScene::get_position(a));

      if (CurrentScene::mesh_draw(a)->select_vertices_box(*this->clicked_position,
                                                          *this->current_position,
                                                          translation,
                                                          this->type_of_view) ) {
          qDebug() << CurrentScene::mesh_draw(a);
      }
    }
  }
  // alert mouse event's position (x)
  mouse_x = event->x();

  // alert mouse event's position (x)
  mouse_y = event->y();

  this->raycast_direction =
          this->mouse_raycast(mouse_x,
                              mouse_y,
                              this->view_matrix);
  if(mousedown_right) {
    this->position_camera.setX(this->clicked_position->x() -
                               this->current_position->x());
    this->position_camera.setY(this->clicked_position->y() -
                               this->current_position->y());
    this->position_camera.setZ(this->clicked_position->z() -
                               this->current_position->z());

    // pan view
    this->camera_previous.setX(this->camera_previous.x() -
                               this->position_camera.x());
    this->camera_previous.setY(this->camera_previous.y() -
                               this->position_camera.y());
    this->camera_previous.setZ(this->camera_previous.z() -
                               this->position_camera.z());
    this->position_camera = QVector3D();

    this->mouse_relative_y_drag = this->prev_rotation_y + (this->mouse_right_clicked_y - this->mouse_y);
    this->mouse_relative_x_drag = this->prev_rotation_x + (this->mouse_right_clicked_x - this->mouse_x);
  }
  // update openGL widget
  update();
}

int RenderState::sign(int x) {
  if ( x > 0 )
    return 1;
  else return -1;
}

void RenderState::mouseReleaseEvent(QMouseEvent *) {
  if ( this->mousedown_right ) {
    this->prev_rotation_x = this->mouse_relative_y_drag;
    this->prev_rotation_y = this->mouse_relative_x_drag;
  }
  this->mousedown_right = false;
  this->mousedown_left = false;
  update();

}

void RenderState::draw_grid() {
  const int max_lines = 32;
  for ( int x = -max_lines; x < max_lines + 1; x++ ) {
      switch ( type_of_view ) {
        case 0 :
          // draw horisontal lines
          draw_line(QVector3D(x ,0 ,-max_lines),
                    QVector3D(x ,0 ,max_lines),
                    this->view_matrix, QMatrix4x4(),
                    QVector3D(1.0, 1.0, 1.0));
          // draw vertical lines
          draw_line(QVector3D(-max_lines ,0 ,x),
                    QVector3D(max_lines ,0 ,x),
                    this->view_matrix, QMatrix4x4(),
                    QVector3D(1.0, 1.0 ,1.0));
        break;
        case 1 :
          // draw horisontal lines
          draw_line(QVector3D(0, x, -max_lines),
                    QVector3D(0, x, max_lines),
                    this->view_matrix, QMatrix4x4(),
                    QVector3D(1.0, 1.0, 1.0));
          // draw vertical lines
          draw_line(QVector3D(0, -max_lines, x),
                    QVector3D(0, max_lines, x),
                    this->view_matrix, QMatrix4x4(),
                    QVector3D(1.0, 1.0 ,1.0));
        break;
        case 2 :
          // draw horisontal lines
          draw_line(QVector3D(x, -max_lines, 0),
                    QVector3D(x, max_lines, 0),
                    this->view_matrix, QMatrix4x4(),
                    QVector3D(1.0, 1.0, 1.0));
          // draw vertical lines
          draw_line(QVector3D(-max_lines, x, 0),
                    QVector3D(max_lines, x, 0),
                    this->view_matrix, QMatrix4x4(),
                    QVector3D(1.0, 1.0 ,1.0));
        break;
      case 3 :
        // draw horisontal lines
        draw_line(QVector3D(x ,0 ,-max_lines),
                  QVector3D(x ,0 ,max_lines),
                  this->view_matrix, QMatrix4x4(),
                  QVector3D(1.0, 1.0, 1.0));
        // draw vertical lines
        draw_line(QVector3D(-max_lines ,0 ,x),
                  QVector3D(max_lines ,0 ,x),
                  this->view_matrix, QMatrix4x4(),
                  QVector3D(1.0, 1.0 ,1.0));
      break;
    }
  }
}

void RenderState::mousePressEvent(QMouseEvent *event) {
  if(event->button() == Qt::LeftButton) {
    this->mousedown_left = true; // set for dragging the left button
    add_vertices(); // this will only happen when the vertices are addable
    edit_vertices(); // this will only happen when vertices can be edit
    emit this->update_frame();
  } else if(event->button() == Qt::RightButton) {
    this->mousedown_right = true;
    this->clicked_position = new QVector3D(this->current_position->x(),
                                           this->current_position->y(),
                                           this->current_position->z());
    this->mouse_right_clicked_x = this->mouse_x;
    this->mouse_right_clicked_y = this->mouse_y;
  }
}

void RenderState::wheelEvent(QWheelEvent *event) {
  this->mouse_zoom -= (float)event->delta()/3.0f;
  if ( this->mouse_zoom < 5.0f )
    this->mouse_zoom = 5.0f;
  update();
}

void RenderState::change_camara(int type) {
  camera_transformation.setToIdentity();
  switch(type) {
    case 0:
      this->view_angle = 11.25 / 64.0;
      camera_transformation.rotate(-90, 1, 0, 0);
      break;
    case 1:
      this->view_angle = 11.25 / 64.0;
      camera_transformation.rotate(-90, 0, 1, 0);
      break;
    case 2:
      this->view_angle = 11.25 / 64.0;
      camera_transformation.rotate(-90, 0, 0, 1);
      break;
    case 3:
      this->view_angle = 45.0;
      this->projection_matrix.perspective(this->view_angle, (float) this->viewport_width / (float) this->viewport_height, 1.0f, 65536.0f);
      camera_transformation.rotate(0, 0, 1, 0);
      camera_transformation.rotate(-45, 1, 0, 0);

    break;
    case 4:
      camera_transformation.rotate(45, 0, 1, 0);
      camera_transformation.rotate(45, 1, 0, 0);
    break;
    case 5:
      camera_transformation.rotate(90, 0, 0, 1);
    break;
    default:
      camera_transformation.rotate(-90, 1, 0, 0);
      break;
  }
  this->type_of_view = type;
}

void RenderState::resizeGL(int w, int h) {
  this->viewport_width = w;
  this->viewport_height = h;

  // setup the viewport for opengl
  glViewport(0, 0, w, h);

  // initialize the projection matrix
  this->projection_matrix.setToIdentity();

    // set the projection matrix
 this->projection_matrix.perspective(view_angle, (float) w / (float) h, 1.0f, 65536.0f);
  //pMatrix.ortho(-(float)w/h, (float)w/h, -1.0, 1.0, 1.0, 200.0);
}

void RenderState::load_content() {
    // this initializes all the opengl functions
    initializeOpenGLFunctions();

    //load meshes
    this->current_mesh = new ModelMesh(":/Sphere");
    this->current_mesh->load_sphere(0.5, 12, 6);

    // load shaders
    this->shader_program = new QOpenGLShaderProgram();
    this->shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "://Vertex");
    this->shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "://Fragment");
    this->shader_program->link();
}

void RenderState::paintGL() {
  this->view_matrix.setToIdentity();
  // whenever content is not loaded, load the content
  if ( !this->shader_program ) { this->load_content(); }
  // enable the scene's depth mask
  glDepthMask(GL_TRUE);
  // clear the depth z = 0.0f -> 1.0f
  glClearDepth(1.0f);
  // enable the scene's depth test
  glEnable(GL_DEPTH_TEST);
  // enable cullmode CCW (counter clockwise)
  glEnable(GL_CULL_FACE);
  // clear the background color for rendering
  glClearColor(104.0/255.0, 104.0/255.0, 104.0/255.0, 1);
  // clear the color and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // transform the camera's position with respect to the rotation matrix
  QVector3D cameraPosition = camera_transformation * QVector3D(0, 0, this->mouse_zoom);

  // define the direction of the camera's up vector
  QVector3D cameraUpDirection = camera_transformation * QVector3D(0, 1.0, 0);

  // implement and transform the camera
  this->view_matrix.lookAt(cameraPosition, QVector3D(), cameraUpDirection);
  this->view_matrix.translate(this->camera_previous);

  // return the position of the ray intersection with the y-axis
  QVector3D camara_zoom = QVector3D(- this->camera_previous.x(),
                                    - this->camera_previous.y(),
                                    - this->camera_previous.z());
  //this->view_matrix.rotate(this->prev_rotation_y, 1, 0, 0);
  //this->view_matrix.rotate(this->prev_rotation_x, 0, 1, 0);
  switch ( type_of_view ) {
   case 0 : camara_zoom.setY( camara_zoom.y() + this->mouse_zoom );
   break;
   case 1 : camara_zoom.setX( camara_zoom.x() - this->mouse_zoom );
   break;
   case 2 : camara_zoom.setZ( camara_zoom.z() + this->mouse_zoom );
   break;
   case 3 : this->view_matrix.rotate(-this->mouse_relative_y_drag, 1.0, 0, 0);
            this->view_matrix.rotate(-this->mouse_relative_x_drag, 0, 1.0, 0);
           // this->view_matrix.rotate(-45-this->mouse_y, 1, 0, 0);

   break;
  }
  QVector3D Pos  = this->intersect_plane(this->raycast_direction, camara_zoom);

  // update current position
  this->current_position->setX(Pos.x());
  this->current_position->setZ(Pos.z());
  this->current_position->setY(Pos.y());

  // draw select box
  if ( (this->mousedown_left) && (this->edit_vertex_enable) ) {
    draw_flat_box(*this->clicked_position, *this->current_position);
  }

  for (int a = 0; a < CurrentScene::mesh_count(); a++) {
    QMatrix4x4 translation;
    translation.translate(*CurrentScene::get_position(a));
    draw_model(CurrentScene::mesh_draw(a),
               this->view_matrix,
               translation,
               QMatrix4x4(),
               QVector3D(0.3, 0.0, 0.0));
    draw_model_vertices(CurrentScene::mesh_draw(a),
                        this->view_matrix,
                        translation,
                        QMatrix4x4(),
                        QVector3D(1.0, 0.0, 0.0), true);

    draw_model_vertices(CurrentScene::mesh_draw(a),
                        this->view_matrix,
                        translation,
                        QMatrix4x4(),
                        QVector3D(1.0, 1.0, 1.0), false);

  }
  draw_grid();
  if ( this->add_vertex_enable ) {
    QMatrix4x4 translation;
    translation.translate(Pos);
    draw_model(this->current_mesh,this->view_matrix,
               translation,
               QMatrix4x4(),
               QVector3D());
  }
  // release the program for this frame
  this->shader_program->release();
  // disable the cullmode for the frame
  glDisable(GL_CULL_FACE);
  // disable the depthtest for the frame
  glDisable(GL_DEPTH_TEST);
  // finish up the opengl frame
  glFinish();
}

void RenderState::update_shaders(QMatrix4x4 wvp, QMatrix4x4 mvp, QMatrix4x4 rotate, QVector3D color) {
    // bind the current shader code
    this->shader_program->bind();

    // update the colour of the object
    this->shader_program->setUniformValue("ambient_color", color);

    // change the rotation of the object in the shader
    this->shader_program->setUniformValue("rotationMatrix", rotate);

    // update model view projection
    this->shader_program->setUniformValue("mvpMatrix", mvp * rotate);

    // update world view projection in the shader
    this->shader_program->setUniformValue("wvpMatrix",this->projection_matrix * wvp);

    // use GL_TEXTURE0
    this->shader_program->setUniformValue("texture", 0);
}

void RenderState::draw_flat_box(QVector3D position1, QVector3D position2) {
      draw_line(QVector3D(position1.x(), position1.y(), position1.z()),
                QVector3D(position1.x(), position2.y(), position2.z()),
                this->view_matrix, QMatrix4x4(), QVector3D(1, 1, 1));
      draw_line(QVector3D(position1.x(), position1.y(), position1.z()),
                QVector3D(position2.x(), position1.y(), position2.z()),
                this->view_matrix, QMatrix4x4(), QVector3D(1, 1, 1));
      draw_line(QVector3D(position1.x(), position1.y(), position1.z()),
                QVector3D(position2.x(), position2.y(), position1.z()),
                this->view_matrix, QMatrix4x4(), QVector3D(1, 1, 1));

      draw_line(QVector3D(position2.x(), position1.y(), position1.z()),
                QVector3D(position2.x(), position2.y(), position2.z()),
                this->view_matrix, QMatrix4x4(), QVector3D(1, 1, 1));
      draw_line(QVector3D(position1.x(), position2.y(), position1.z()),
                QVector3D(position2.x(), position2.y(), position2.z()),
                this->view_matrix, QMatrix4x4(), QVector3D(1, 1, 1));
      draw_line(QVector3D(position1.x(), position1.y(), position2.z()),
                QVector3D(position2.x(), position2.y(), position2.z()),
                this->view_matrix, QMatrix4x4(), QVector3D(1, 1, 1));

}

void RenderState::draw_shader(ModelMesh *mesh, int type) {

    // convert the qstring to c-string for opengl purposes, this is the vertex variable in the shader files
    const char *vert ="vertex";//= vertex.toStdString().c_str();

    // convert the qstring to c-string for opengl purposes, this is the texture variable in the shader
    const char *textureCoordinate= "textureCoordinate";//= texCoord.toStdString().c_str();

    // convert the qstring to c-string for opengl, this is the normal variable in the shader code
    const char *normals = "normal";

    // load the vertices to the shaders
    this->shader_program->setAttributeArray(vert, mesh->vertices.constData());

    // enable the shader attribute( vertices )
    this->shader_program->enableAttributeArray(vert);

    // load the normals to the shaders
    this->shader_program->setAttributeArray(normals, mesh->normals.constData());

    // enable the shader attribute( normals )
    this->shader_program->enableAttributeArray(normals);

    // load the texture coordinates to the shaders
    this->shader_program->setAttributeArray(textureCoordinate, mesh->texture_coordinates.constData());

    // enable the texture attribute
    this->shader_program->enableAttributeArray(textureCoordinate);
    // draw the opengl vertices
    switch ( type ) {
      case 0: mesh->draw_mesh(); break;
      case 1: mesh->draw_vertices(3.0f); break;
      case 2: mesh->draw_vertices_selected(3.0f); break;
      default: this->current_mesh->draw_mesh(); break;
    }
    // disable the vertex attributes
    this->shader_program->disableAttributeArray(vert);

    // disable the normal attributes
    this->shader_program->disableAttributeArray(normals);

    // disable the Texture coordinates attributes
    this->shader_program->disableAttributeArray(textureCoordinate);

    // release the current updated shader code (awaiting next frame)
    this->shader_program->release();
   }

void RenderState::draw_line(QVector3D point1,
                            QVector3D point2,
                            QMatrix4x4 world_view_projection,
                            QMatrix4x4 model_view_projection,
                            QVector3D color) {
  QVector< QVector3D > temp_vertices;
  temp_vertices.push_back(point1);
  temp_vertices.push_back(point2);
  this->update_shaders(world_view_projection, model_view_projection, QMatrix4x4(), color);
  const char *vert ="vertex";// convert the qstring to c-string for opengl purposes
  const char *normals = "normal";// convert the qstring to c-string for opengl purposes
  this->shader_program->setAttributeArray(vert, temp_vertices.constData());//load the vertices to the shaders
  this->shader_program->enableAttributeArray(vert);//enable the shader attribute( vertices )
  this->shader_program->setAttributeArray(normals, temp_vertices.constData());//load the normals to the shaders
  this->shader_program->enableAttributeArray(normals);//enable the shader attribute( vertices )
  glLineWidth(1.0);
  glDrawArrays(GL_LINES, 0, temp_vertices.size());
  this->shader_program->disableAttributeArray(vert);// disable the vertex attributes
  this->shader_program->disableAttributeArray(normals);// disable the normal attributes
  this->shader_program->release(); // release the current updated shader code (awaiting next frame)
  temp_vertices.clear();
}

void RenderState::draw_model(ModelMesh *mesh, QMatrix4x4 wvp, QMatrix4x4 mvp, QMatrix4x4 rotate, QVector3D color) {
  this->update_shaders(wvp,
                       mvp,
                       rotate,
                       color);
  this->draw_shader(mesh, 0);
}

void RenderState::draw_model_vertices(ModelMesh *mesh,
                                      QMatrix4x4 wvp,
                                      QMatrix4x4 mvp,
                                      QMatrix4x4 rotate,
                                      QVector3D color, bool selected) {
  this->update_shaders(wvp,
                       mvp,
                       rotate,
                       color);
  if ( !selected )
   this->draw_shader(mesh, 1);
  else
   this->draw_shader(mesh, 2);
}

QVector3D RenderState::mouse_raycast(int mx,
                                     int my,
                                     QMatrix4x4 view_matrix) {
    float nx = (2.0f * mx) / this->width() - 1.0f; // normalize the x-mouse position
    float ny = 1.0f - (2.0f * my) / this->height();// normalize the y-mouse position

    QVector4D ray_clip = QVector4D(nx, ny, -1, 1.0); // clip the x,y,z values between -1:1
    QMatrix4x4 pInverse =this->projection_matrix.inverted(NULL);// invert the projection
    QMatrix4x4 vInverse = view_matrix.inverted(NULL);// invert the view
    QVector4D ray_eye = pInverse*ray_clip; // "convert" the normalized ray to the projection values
    ray_eye = QVector4D(ray_eye.x(),ray_eye.y(),-1.0,0);// change the w-value of the vector for matrix manipulation purposes
    QVector4D ray_wor4 = vInverse * ray_eye; // "convert" the new ray to the view values
    QVector3D ray_wor = ray_wor4.toVector3D(); // take the x,y,z values of the new position
    ray_wor.normalize();// make the ray a unit vector

    return ray_wor; // return the raycast of the 2D mouse in the 3D world view projection
}

QVector3D RenderState::intersect_plane(QVector3D u_dir,
                                       QVector3D r_point) {
    float t = 0.0;//t determines the point of intersection
    switch ( type_of_view ) {
      case 0 : if ( u_dir.y() != 0 )// (1/0) validation
                 t = -r_point.y() / u_dir.y(); // t = -r1.y / r (calculus)
      break;
      case 1 : if ( u_dir.x() != 0 )// (1/0) validation
                 t = -r_point.x() / u_dir.x(); // t = -r1.x / r (calculus)
      break;
      case 2 : if ( u_dir.z() != 0 )// (1/0) validation
                 t = - r_point.z() / u_dir.z(); // t = -r1.x / r (calculus)
      break;
    }
    return r_point + t * u_dir;// v(t) = r + t * r1
}

void RenderState::add_vertices() {
  if ( this->add_vertex_enable ) {
      ModelMesh *mesh = new ModelMesh();
      *mesh = *this->current_mesh;
    CurrentScene::add_mesh(mesh,
                           new QVector3D(this->current_position->x(),
                                         this->current_position->y(),
                                         this->current_position->z()));
  }
}

void RenderState::edit_vertices() {
  if ( this->edit_vertex_enable ) {
    this->clicked_position = new QVector3D(this->current_position->x(),
                                           this->current_position->y(),
                                           this->current_position->z());
  }
}

RenderState::~RenderState() {
  delete this->shader_program;
  delete this->position;
  delete this->clicked_position;
  delete this->current_mesh;
  delete this->current_position;
}

