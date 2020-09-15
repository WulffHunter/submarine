varying vec3 N;  
varying vec3 p;
varying vec2 texcoordVarying;

void main(void) {
   // eye coords
   p = vec3(gl_ModelViewMatrix * gl_Vertex);       
   N = normalize(gl_NormalMatrix * gl_Normal);

   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   texcoordVarying = gl_TexCoord[0].st;
}