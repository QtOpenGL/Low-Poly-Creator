#include <QFile>
#include <QTextStream>
#include <QOpenGLFunctions>
#include <QVector2D>
#include <QVector3D>
#include <QString>
#include <QtMath>
#include "./Objects/ModelMesh.h"

ModelMesh::ModelMesh(QString file) {
   load_obj_file(file);
}

ModelMesh::ModelMesh() {

}

bool ModelMesh::load_obj_file(QString file) {
  original_vertices.clear();
  modified_vertices.clear();
  texture_coordinates.clear();
  normals.clear();
  vertex_indices.clear();
  selected_vertices.clear();
  QVector< QVector3D > temp_vertices;
  QVector< QVector2D > temp_uvs;
  QVector< QVector3D > temp_normals;
  QFile textfile(file);
  textfile.open(QIODevice::ReadOnly | QIODevice::Text);
  QTextStream ascread(&textfile);

  if(textfile.isOpen()) {
    QString line = ascread.readLine();
    while(!line.isNull())
        {
            QStringList list = line.split(" ");
            if(list[0]=="v")
            {
                float vertex[list.count()];
                for(int i = 0;i<list.count()-1;i++)
                     QTextStream(&list[i+1])>>vertex[i];
                temp_vertices.push_back(QVector3D(vertex[0],vertex[1],vertex[2]));
            }else
                if(list[0]=="vt")
                {
                    float uv[list.count()];
                    for(int i = 0;i<list.count()-1;i++)
                         QTextStream(&list[i+1])>>uv[i];

                    temp_uvs.push_back(QVector2D(uv[0],uv[1]));
                }else
                    if(list[0]=="vn")
                    {
                        float vertexnormal[list.count()];
                        for(int i = 0;i<list.count()-1;i++)
                             QTextStream(&list[i+1])>>vertexnormal[i];
                        temp_normals.push_back(QVector3D(vertexnormal[0],vertexnormal[1],vertexnormal[2]));
                    }else
                        if(list[0] == "f")
                        {
                            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                            for(int i = 0;i<list.count()-1;i++)
                            {
                                QStringList facelist = list[i+1].split("/");
                                QTextStream(&facelist[0])>>vertexIndex[i];
                                QTextStream(&facelist[1])>>uvIndex[i];
                                QTextStream(&facelist[2])>>normalIndex[i];
                            }
                            for(int j = 0;j<3;j++)
                            {
                            vertex_indices.push_back(vertexIndex[j]);
                            uv_indices.push_back(uvIndex[j]);
                            normal_indices.push_back(normalIndex[j]);
                            }
                        }
            line = ascread.readLine();
        }
    textfile.close();
  }
  // For each vertex of each triangle
  for( int i = 0; i < vertex_indices.size(); i++ ){
    int vertex_index = vertex_indices[i];
    QVector3D vertex = temp_vertices[ vertex_index - 1 ];
    modified_vertices.push_back(vertex);
    original_vertices.push_back(vertex);
  }
  // For each vertex of each triangle
  for( int i = 0; i < uv_indices.size(); i++ ){
    int uv_index = uv_indices[i];
    QVector2D uvs = temp_uvs[ uv_index - 1 ];
    texture_coordinates.push_back(uvs);
  }
  // For each vertex of each triangle
  for( int k = 0; k < normal_indices.size(); k++ ){
    int normal_index = normal_indices[k]-1;
    QVector3D normal = temp_normals[normal_index];
    normals.push_back(normal);
  }
  return true;
 }

bool ModelMesh::load_sphere(float radius, int stacks, int slices) {
  original_vertices.clear();
  modified_vertices.clear();
  texture_coordinates.clear();
  normals.clear();
  vertex_indices.clear();

  const float PI_times_2 = 2 * 3.141592654f;
  const float PI_slices = PI_times_2 / slices;
  const float PI_stacks = PI_times_2 / stacks;
  for ( int sl = 0; sl < slices; sl++) {
    for ( int st = 0; st < stacks; st++) {
      modified_vertices.push_back(QVector3D(radius * sin(PI_slices * sl) * sin(PI_stacks * st),
                                   radius * cos(PI_stacks * st),
                                   radius * cos(PI_slices * sl) * sin(PI_stacks * st)));
      original_vertices.push_back(QVector3D(radius * sin(PI_slices * sl) * sin(PI_stacks * st),
                                            radius * cos(PI_stacks * st),
                                            radius * cos(PI_slices * sl) * sin(PI_stacks * st)));
    }
  }
  /*// For each vertex of each triangle
  for( int i = 0; i < vertex_indices.size(); i++ ){
    vertices.push_back(vertex);
  }
  // For each vertex of each triangle
  for( int i = 0; i < uv_indices.size(); i++ ){
    texture_coordinates.push_back(uvs);
  }
  // For each vertex of each triangle
  for( int k = 0; k < normal_indices.size(); k++ ){
    normals.push_back(normal);
  }*/
  return true;
}

void ModelMesh::translate_selected(QVector3D translation) {
  for(int i = 0; i < this->selected_vertices.count(); i++){
      QVector3D temp_prev = original_vertices.value(this->selected_vertices.value(i));
      modified_vertices.removeAt(this->selected_vertices.value(i));
      modified_vertices.insert(this->selected_vertices.value(i), translation + temp_prev);
    //vertices.value(this->selected_vertices.value(i)).setX(translation.x());
    qDebug() << modified_vertices.value(this->selected_vertices.value(i)).x() << translation.x();
  }
}

bool ModelMesh::select_vertices_box(QVector3D position1, QVector3D position2, QMatrix4x4 modifier, int view_type) {
  bool vertex_selected = false;
  this->selected_vertices.clear();
  float max_x = position1.x() > position2.x() ? position1.x() : position2.x();
  float min_x = position1.x() < position2.x() ? position1.x() : position2.x();
  float max_y = position1.y() > position2.y() ? position1.y() : position2.y();
  float min_y = position1.y() < position2.y() ? position1.y() : position2.y();
  float max_z = position1.z() > position2.z() ? position1.z() : position2.z();
  float min_z = position1.z() < position2.z() ? position1.z() : position2.z();
  for ( int l = 0; l < modified_vertices.count(); l++ ) {
    QVector3D transformed = modifier * this->modified_vertices.value(l);
    switch (view_type) {
      case 2 :
      if ((min_x < transformed.x() && transformed.x() < max_x) &&
          (min_y < transformed.y() && transformed.y() < max_y)) {
          this->selected_vertices.append(l);
          vertex_selected = true;
      } break;
      case 0 :
      if ( (min_x < transformed.x() && transformed.x() < max_x) &&
        (min_z < transformed.z() && transformed.z() < max_z)) {
        this->selected_vertices.append(l);
        vertex_selected = true;
      } break;
      case 1 :
      if ((min_y < transformed.y() && transformed.y() < max_y) &&
          (min_z < transformed.z() && transformed.z() < max_z)) {
        this->selected_vertices.append(l);
        vertex_selected = true;
      } break;
    }
  }
   if (vertex_selected)
  qDebug() << this->selected_vertices;
  return vertex_selected;
}

void ModelMesh::draw_vertices_selected(float point_size) {
  glPointSize(point_size + 4.0);
  for(int i = 0; i < this->selected_vertices.count(); i++){
    glBegin(GL_POINTS);
    glVertex3f(modified_vertices.value(this->selected_vertices.value(i)).x(),
               modified_vertices.value(this->selected_vertices.value(i)).y(),
               modified_vertices.value(this->selected_vertices.value(i)).z());
    glEnd();
  }
}

void ModelMesh::draw_vertices(float point_size) {
  glPointSize(point_size);
  for(int i = 0; i < modified_vertices.size(); i++){
    glBegin(GL_POINTS);
    glVertex3f(modified_vertices.value(i).x(),
               modified_vertices.value(i).y(),
               modified_vertices.value(i).z());
    glEnd();
  }
}

void ModelMesh::draw_mesh() {
    // Draw the vertices as triangles, not linked triangles, each triangle is seperated from the other
  glDrawArrays(GL_TRIANGLES, 0, modified_vertices.size());
}

void ModelMesh::commit_changes() {
 this->original_vertices = this->modified_vertices;
}

ModelMesh::~ModelMesh() {
   texture_coordinates.detach();
   modified_vertices.detach();
   original_vertices.detach();
   normals.detach();
   vertex_indices.detach();
   uv_indices.detach();
   normal_indices.detach();
}
