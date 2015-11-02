#include "./currentscene.h"
QVector<ModelMesh *> CurrentScene::meshes;
QVector<QVector3D *> CurrentScene::positions;

CurrentScene::CurrentScene() {

}

CurrentScene::~CurrentScene() {

}

void CurrentScene::add_mesh(ModelMesh *model_mesh, QVector3D *model_position) {
  CurrentScene::meshes.append(model_mesh);
  CurrentScene::positions.append(model_position);
}

int CurrentScene::mesh_count() {
  return CurrentScene::meshes.count();
}

ModelMesh * CurrentScene::mesh_draw(int index) {
  return CurrentScene::meshes.value(index);
}

QVector3D * CurrentScene::get_position(int index) {
  return CurrentScene::positions.value(index);
}
