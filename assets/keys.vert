#version 450

layout (triangle_strip ) in ;
in vec2 buttons;

layout (line_strip) in ;
in vec2 path1;

layout (line_strip) in ;
in vec2 path2;
 


layout (triangle_strip ) in ;
out vec2 buttons;

layout (line_strip) in ;
out vec2 path1;

layout (line_strip) in ;
out vec2 path2;
 
 
void main() {
    gl_Position = vec4(0,0,0,1);
}