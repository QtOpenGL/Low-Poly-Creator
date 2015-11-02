/** *********************************************************************** **/
/** *********************************************************************** **/
/**     Created by:     Ruan (Baggins) Luies 23511354                       **/
/**     email:          23511354@nwu.ac.za                                  **/
/**     Project name:   Virtual Concierge Creator And Management System     **/
/**     File Name:      ModelMesh.h                                         **/
/**     From Date:      2015/02/24                                          **/
/**     To Date:        2015/10/01                                          **/
/** **********************************************************************  **/
/** *********************************************************************** **/

#ifndef MODELMESH_H
#define MODELMESH_H
#include <QVector>

class QVector2D;
class QVector3D;
class QString;

class ModelMesh {
public:
    ModelMesh(QString);
    ~ModelMesh();
    void draw_mesh();
    QVector<QVector2D> texture_coordinates;
    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    QVector<int> vertex_indices, uv_indices, normal_indices;
    bool load_obj_file(QString file_name);
    bool load_sphere(float radius, int stacks, int slices);
};

#endif // MODELMESH_H
