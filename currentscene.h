#ifndef CURRENTSCENE_H
#define CURRENTSCENE_H

#include <QVector>
#include <QVector3D>
#include "./Objects/ModelMesh.h"

class CurrentScene {
public:
    CurrentScene();
    ~CurrentScene();
    static void add_mesh(ModelMesh *model_mesh, QVector3D *model_position);
    static ModelMesh * mesh_draw(int index);
    static int mesh_count();
    static QVector3D * get_position(int index);
private:
    static QVector<ModelMesh *> meshes;
    static QVector<QVector3D *> positions;
};

#endif // CURRENTSCENE_H
