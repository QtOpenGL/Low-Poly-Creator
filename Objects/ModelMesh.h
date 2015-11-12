#ifndef MODELMESH_H
#define MODELMESH_H
#include <QVector>
#include <QMatrix4x4>

class QVector2D;
class QVector3D;
class QString;

class ModelMesh {
public:
    ModelMesh(QString);
    ModelMesh();
    ~ModelMesh();
    void draw_mesh();
    void draw_vertices(float point_size);
    void draw_vertices_selected(float point_size);
    void translate_selected(QVector3D translation);
    void commit_changes();
    QVector<QVector2D> texture_coordinates;
    QVector<QVector3D> modified_vertices, original_vertices;
    QVector<QVector3D> normals;
    QVector<unsigned int> vertex_indices, uv_indices, normal_indices;
    bool load_obj_file(QString file_name);
    bool load_sphere(float radius, int stacks, int slices);
    bool select_vertices_box(QVector3D position1,
                             QVector3D position2,
                             QMatrix4x4 modifier,
                             int view_type);
    QVector<int> selected_vertices;
};

#endif // MODELMESH_H
