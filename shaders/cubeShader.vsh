uniform mat4 u_mWVP;

in vec4 a_position;

void main(){
    gl_Position = u_mWVP * a_position; 
}