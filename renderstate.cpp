#include "renderstate.h"

RenderState::RenderState(QWidget *parent): QOpenGLWidget(parent),
    m_program(0),
    m_mouse_x(0),
    m_mouse_y(0),
    m_mouse_zoom(60.0f),
    m_position_camera(QVector3D()),
    m_camera_prev(QVector3D()),
    m_raycast(QVector3D()),
    m_mousedown_right(false)
{
    // enable antialiasing
    QSurfaceFormat format;
   // format.setDepthBufferSize(24);
    format.setSamples(4);
   // format.setStencilBufferSize(8);
  //  format.setVersion(3, 2);
    //format.setProfile(QSurfaceFormat::CoreProfile);
    this->setFormat(format);
    m_clicked_position = new QVector3D(0,0,0);
    // set the current mouse position in 3D
    m_current_position = new QVector3D(0,0,0);

    // clear the textures
    m_textures.clear();

    // set the position to initial null vector
    m_position = new QVector3D(0,0,0);

    // clear the nodes
    m_nodes.clear();

    // set mouse tracking
    setMouseTracking(true);
}

void RenderState::initializeGL()
{
    initializeOpenGLFunctions();

     // texture test
    QOpenGLTexture *texture = new QOpenGLTexture(QImage("://Texture0").mirrored());
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_textures.append(texture);
}

void RenderState::mouseMoveEvent(QMouseEvent *event)
{

    // alert mouse event's position (x)
    m_mouse_x = event->x();

    // alert mouse event's position (x)
    m_mouse_y = event->y();

    m_raycast = mouseRayCast(m_mouse_x, m_mouse_y, vMatrix);
    if(m_mousedown_right)
    {
        m_position_camera.setX(m_clicked_position->x()-m_current_position->x());
        m_position_camera.setY(m_clicked_position->y()-m_current_position->y());
        m_position_camera.setZ(m_clicked_position->z()-m_current_position->z());
        // pan view
        m_camera_prev.setX(m_camera_prev.x()-m_position_camera.x());
        m_camera_prev.setY(m_camera_prev.y()-m_position_camera.y());
        m_camera_prev.setZ(m_camera_prev.z()-m_position_camera.z());
    m_position_camera = QVector3D();

    }
    // update openGL widget
    update();
}
void RenderState::mouseReleaseEvent(QMouseEvent *)
{


    m_mousedown_right = false;
update();
}

void RenderState::mousePressEvent(QMouseEvent *event)
{


    if(event->button() == Qt::LeftButton)
    add_node(new QString("pewpew"+QString::number(m_nodes.count())));
    else
        if(event->button() == Qt::RightButton)
        {
            m_mousedown_right = true;

            m_clicked_position = new QVector3D(m_current_position->x(),m_current_position->y(),m_current_position->z());

        }
//    if(!m_mousedown_right)
//    update();

}

void RenderState::wheelEvent(QWheelEvent *event)
{
    m_mouse_zoom -= (float)event->delta()/120.0f;
    if(m_mouse_zoom<5.0f)
        m_mouse_zoom = 5.0f;
    update();
}

void RenderState::add_node(QString *name)
{
    // create new nodes
    Node *newnode = new Node(new QVector3D(m_current_position->x(),
                                           m_current_position->y(),
                                           m_current_position->z()),
                                           name);

    // add new node to vector
    m_nodes.push_back(newnode);
}

void RenderState::resizeGL(int w, int h)
{
    // setup the viewport for opengl
    glViewport(0, 0, w, h);

    // initialize the projection matrix
    pMatrix.setToIdentity();

    // set the projection matrix
    pMatrix.perspective(45, (float) w / (float) h, 1.0f, 1000.0f);
}
void RenderState::LoadContent()
{
    // this initializes all the opengl functions
    initializeOpenGLFunctions();
    //load meshes
    box = new ModelMesh(":/Sphere");
    node = new ModelMesh(":/Sphere");
    sky= new ModelMesh(":/Sky");
    wagen = new ModelMesh(":/Cube");
    m_plane = new ModelMesh(":/Plane");

    // load shaders
    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex,"://Vertex");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment,"://Fragment");
    m_program->link();
}


void RenderState::paintGL()
{
    vMatrix.setToIdentity();
    // whenever content is not loaded, load the content
    if(!m_program){LoadContent();}
    // set the uniform value t for the shader (give it a new value)
    m_program->setUniformValue("t", (float) m_t);
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
    // setup camera
    QMatrix4x4 cameraTransformation;
    // rotation in the y - axis
   // cameraTransformation.rotate(0, 0, 1, 0);
    // rotation in the x - axis

    cameraTransformation.rotate(-90, 1, 0, 0);

    // transform the camera's position with respect to the rotation matrix
    QVector3D cameraPosition = cameraTransformation * QVector3D(0, 0, m_mouse_zoom) ;
    // define the direction of the camera's up vector
    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0);
    // implement and transform the camera
    vMatrix.lookAt(cameraPosition, QVector3D(), cameraUpDirection);
    vMatrix.translate(m_camera_prev);

    // return the position of the ray intersection with the y-axis
    QVector3D Pos  = intersectYnull(m_raycast, QVector3D(0, m_mouse_zoom, 0)-m_camera_prev );
    // update current position
    m_current_position->setX(Pos.x());
    m_current_position->setZ(Pos.z());
    m_current_position->setY(Pos.y());

    // draw line if right clicked
    if(m_mousedown_right)
    DrawLine(*m_clicked_position, *m_current_position, vMatrix, QMatrix4x4(), QMatrix4x4(), QVector3D(0,0,0));

    foreach(Node *n, m_nodes)
    {
        QMatrix4x4 translation;
        translation.translate(n->Position());

        DrawModel(node, vMatrix, translation,QMatrix4x4(),QVector3D());

    }

    QMatrix4x4 translation;
    translation.translate(Pos);
    DrawModel(node, vMatrix,translation, QMatrix4x4(),QVector3D());
    // release the program for this frame
    m_program->release();
    // disable the cullmode for the frame
    glDisable(GL_CULL_FACE);
    // disable the depthtest for the frame
    glDisable(GL_DEPTH_TEST);
    // finish up the opengl frame
    glFinish();
}

void RenderState::UpdateShaders(QMatrix4x4 wvp,QMatrix4x4 mvp, QMatrix4x4 rotate/*, GLuint texture*/,QVector3D color)
{
    // bind the current shader code
    m_program->bind();

    // bind the texture for the object
    m_textures.value(0)->bind();

    // update the colour of the object
    m_program->setUniformValue("col",color);

    // change the rotation of the object in the shader
    m_program->setUniformValue("rotationMatrix", rotate);

    // update model view projection
    m_program->setUniformValue("mvpMatrix", mvp * rotate);

    // update world view projection in the shader
    m_program->setUniformValue("wvpMatrix", pMatrix * wvp);

    // use GL_TEXTURE0
    m_program->setUniformValue("texture", 0);
}

void RenderState::ShaderDraw(ModelMesh *box)
{

    // convert the qstring to c-string for opengl purposes, this is the vertex variable in the shader files
    const char *vert ="vertex";//= vertex.toStdString().c_str();

    // convert the qstring to c-string for opengl purposes, this is the texture variable in the shader
    const char *textureCoordinate= "textureCoordinate";//= texCoord.toStdString().c_str();

    // convert the qstring to c-string for opengl, this is the normal variable in the shader code
    const char *normals = "normal";

    // load the vertices to the shaders
    m_program->setAttributeArray(vert, box->m_vertices.constData());

    // enable the shader attribute( vertices )
    m_program->enableAttributeArray(vert);

    // load the normals to the shaders
    m_program->setAttributeArray(normals, box->m_normals.constData());

    // enable the shader attribute( normals )
    m_program->enableAttributeArray(normals);

    // load the texture coordinates to the shaders
    m_program->setAttributeArray(textureCoordinate, box->m_textureCoordinates.constData());

    // enable the texture attribute
    m_program->enableAttributeArray(textureCoordinate);

    // draw the opengl vertices
    box->Draw();

    // disable the vertex attributes
    m_program->disableAttributeArray(vert);

    // disable the normal attributes
    m_program->disableAttributeArray(normals);

    // disable the Texture coordinates attributes
    m_program->disableAttributeArray(textureCoordinate);

    // release the current updated shader code (awaiting next frame)
    m_program->release();
   }

void RenderState::DrawLine(QVector3D point1, QVector3D point2,QMatrix4x4 wvp,QMatrix4x4 mvp, QMatrix4x4 rotate/*, GLuint texture*/,QVector3D color)
   {
     QVector< QVector3D > temp_vertices;
     temp_vertices.push_back(point1);
     temp_vertices.push_back(point2);
    UpdateShaders(wvp, mvp, rotate/*, texture*/, color);
    const char *vert ="vertex";//= vertex.toStdString().c_str();// convert the qstring to c-string for opengl purposes
    //const char *textureCoordinate= "textureCoordinate";//= texCoord.toStdString().c_str();// convert the qstring to c-string for opengl purposes
    const char *normals = "normal";// convert the qstring to c-string for opengl purposes
    m_program->setAttributeArray(vert, temp_vertices.constData());//load the vertices to the shaders
    m_program->enableAttributeArray(vert);//enable the shader attribute( vertices )
    m_program->setAttributeArray(normals, temp_vertices.constData());//load the normals to the shaders
    m_program->enableAttributeArray(normals);//enable the shader attribute( vertices )
    //m_program->setAttributeArray(textureCoordinate, box->textureCoordinates.constData());//load the texture coordinates to the shaders
   // m_program->enableAttributeArray(textureCoordinate);//enable the shader attribute( texture coordinates )
    glLineWidth(2.0);
    glDrawArrays(GL_LINES, 0, temp_vertices.size());
    m_program->disableAttributeArray(vert);// disable the vertex attributes
    m_program->disableAttributeArray(normals);// disable the normal attributes
    m_program->release(); // release the current updated shader code (awaiting next frame)
    temp_vertices.clear();
}

void RenderState::DrawModel(ModelMesh *box,QMatrix4x4 wvp,QMatrix4x4 mvp, QMatrix4x4 rotate/*, GLuint texture*/,QVector3D color)
 {
     UpdateShaders(wvp, mvp, rotate/*, texture*/, color);
     ShaderDraw(box);
 }

QVector3D RenderState::mouseRayCast(int mx,
                                  int my,
                                  QMatrix4x4 view_matrix)
{
    float nx = (2.0f * mx) / this->width() - 1.0f; // normalize the x-mouse position
    float ny = 1.0f - (2.0f * my) / this->height();// normalize the y-mouse position

    QVector4D ray_clip = QVector4D(nx,ny,-1,1.0); // clip the x,y,z values between -1:1
    QMatrix4x4 pInverse = pMatrix.inverted(NULL);// invert the projection
    QMatrix4x4 vInverse = view_matrix.inverted(NULL);// invert the view
    QVector4D ray_eye = pInverse*ray_clip; // "convert" the normalized ray to the projection values
    ray_eye = QVector4D(ray_eye.x(),ray_eye.y(),-1.0,0);// change the w-value of the vector for matrix manipulation purposes
    QVector4D ray_wor4 = vInverse*ray_eye; // "convert" the new ray to the view values
    QVector3D ray_wor = ray_wor4.toVector3D(); // take the x,y,z values of the new position
    ray_wor.normalize();// make the ray a unit vector

    return ray_wor; // return the raycast of the 2D mouse in the 3D world view projection
}

QVector3D RenderState::intersectYnull(QVector3D u_dir, QVector3D r_point)
{
    float t =0.0;//t determines the point of intersection
    if(u_dir.y() != 0)// (1/0) validation
    t = -r_point.y()/u_dir.y(); // t=-r1.y/r (calculus)
    return r_point+t*u_dir;// v(t)=r+t*r1
}

RenderState::~RenderState()
{

}

