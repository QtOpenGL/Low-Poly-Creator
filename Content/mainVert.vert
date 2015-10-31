/** *********************************************************************** **/
/** *********************************************************************** **/
/**     Created by:     Ruan (Baggins) Luies 23511354                       **/
/**     email:          23511354@nwu.ac.za                                  **/
/**     Project name:   Virtual Concierge Creator And Management System     **/
/**     File Name:      mainVert.vert                                       **/
/**     From Date:      2015/02/24                                          **/
/**     To Date:        2015/10/01                                          **/
/** **********************************************************************  **/
/** *********************************************************************** **/

// the vertex shader is used to calculate the geometry of the vertices relative
// to the camera projection and position
// this is all the public valiables for this vertexshader

// this is the real vertex position
attribute highp vec4 vertex;

// model view projection
uniform highp mat4 mvpMatrix;

// world view projection
uniform highp mat4 wvpMatrix;

// matrix used for rotation
uniform highp mat4 rotationMatrix;

// each vertices' normal vector
attribute vec3 normal;

// result of the normal vector relative to the projection (used in fragment shader)
varying vec3 normals;

// orientation vector
varying highp vec4 posi;

// position
varying highp vec4 position_eye;
// texture coordinate on each vertex
attribute vec2 textureCoordinate;

// resulting texture coordinates, (used in fragment shader)
varying vec2 varyingTextureCoordinate;

void main()
{
    // position of each pixel is the result of the world view projection,
    // modelview projection and the vertices position
  position_eye = wvpMatrix * mvpMatrix * vertex;
    gl_Position = position_eye;

    // only the model view projection needs to be used for rotational lighting
    posi = mvpMatrix * vertex;

    // the normal vectors are independent of the position of the model
    // only dependent on the orientation of the model
    vec4 norm = rotationMatrix * vec4(normal,1);

    // normal vector is only a 3-D vector. only x, y, z parameters are required
    normals = norm.xyz;

    // change the texture coordinates
    varyingTextureCoordinate = textureCoordinate;
}
