/** *********************************************************************** **/
/** *********************************************************************** **/
/**     Created by:     Ruan (Baggins) Luies 23511354                       **/
/**     email:          23511354@nwu.ac.za                                  **/
/**     Project name:   Virtual Concierge Creator And Management System     **/
/**     File Name:      mainFrag.frag                                       **/
/**     From Date:      2015/02/24                                          **/
/**     To Date:        2015/10/01                                          **/
/** **********************************************************************  **/
/** *********************************************************************** **/

// application time variable
uniform lowp float t;

// input from the vertex shader, this is the orientated position of the vertex
varying highp vec4 posi;
varying highp vec4 position_eye;

// these are texture samples. the way textures are rendered are determined by the
// samplers
// basic texture sampler
uniform sampler2D texture;

// normal vector sampler
/* uniform sampler2D normaltexture; */ // currently disabled

// constants
//uniform vec3 lightdir ;//= vec3(-0.707,-0.707,0);

// texture coordinates inherited from  vertex shader
varying vec2 varyingTextureCoordinate;

// normal vector for each pixel inherit from the vertex shader
varying vec3 normals;

const vec3 specular_color = vec3(1.0, 1.0, 1.0);
uniform vec3 ambient_color;// = vec3(0.3, 0.0, 0.0);
const vec3 diffuse_color = vec3(0.5, 0.0, 0.0);
const vec3 light_position = vec3(0.0, 0.0, 0.0);
void main() {
    // use a default light direction this can be changed later
    /* may change later */
    vec3 lightdir = normalize(light_position - posi.xyz);

  //  vec3 reflectDir = reflect(-lightdir, normalize(normals));
  //  vec3 viewDir = normalize(-position_eye.xyz);

    float lambertian = max(dot(lightdir, normalize(normals)), 0.0);
    float specular = 0.0;

    if(lambertian > 0.0) {
      // this is blinn phong
      //float specAngle = max(dot(reflectDir, viewDir), 0.0);
      //specular = pow(specAngle, 4.0);
    }

    // set the active colour with a ambiet colour
  gl_FragColor = vec4(ambient_color +
                 lambertian * diffuse_color +
                 specular * specular_color, 1.0);

}
