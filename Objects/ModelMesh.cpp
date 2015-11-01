/** *********************************************************************** **/
/** *********************************************************************** **/
/**     Created by:     Ruan (Baggins) Luies 23511354                       **/
/**     email:          23511354@nwu.ac.za                                  **/
/**     Project name:   Virtual Concierge Creator And Management System     **/
/**     File Name:      ModelMesh.cpp                                       **/
/**     From Date:      2015/02/24                                          **/
/**     To Date:        2015/10/01                                          **/
/** **********************************************************************  **/
/** *********************************************************************** **/
#include <QFile>
#include <QTextStream>
#include <QOpenGLFunctions>
#include <QVector2D>
#include <QVector3D>
#include <QString>
#include <QtMath>
#include "./Objects/ModelMesh.h"

bool ModelMesh::LoadOBJ(QString file) {
  vertices.clear();
  texture_coordinates.clear();
  normals.clear();
  vertex_indices.clear();
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
    vertices.push_back(vertex);
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
  vertices.clear();
  texture_coordinates.clear();
  normals.clear();
  vertex_indices.clear();

  const float PI_times_2 = 2 * 3.141592654f;
  const float PI_slices = PI_times_2 / slices;
  const float PI_stacks = PI_times_2 / stacks;
  for ( int sl = 0; sl < slices; sl++) {
    for ( int st = 0; st < stacks; st++) {
      vertices.push_back(QVector3D(radius * sin(PI_slices * sl) * sin(PI_stacks * st),
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

ModelMesh::~ModelMesh() {
   texture_coordinates.detach();
   vertices.detach();
   normals.detach();
   vertex_indices.detach();
   uv_indices.detach();
   normal_indices.detach();
}

ModelMesh::ModelMesh(QString file) {
   LoadOBJ(file);
}

void ModelMesh::Draw() {
  glPointSize(3.0f);
    // Draw the vertices as triangles, not linked triangles, each triangle is seperated from the other
  glDrawArrays(GL_TRIANGLES, 0, vertices.size());

  for(int i = 0; i < vertices.size(); i++){
    glBegin(GL_POINTS);
    //glColor3f(1.0,0.0,0.1);
    glVertex3f(vertices.value(i).x(), vertices.value(i).y(), vertices.value(i).z());
    glEnd();
  }
}
