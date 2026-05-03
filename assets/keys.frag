
layout(triangle_strip,  ) in ;
in vec2 pos;
in vec4 color;

layout(set=0 ,binding =0) image2D imAtlas;

const float kw=  0.1;
const float kh = 0.1; 

const vec2 cl = vec2(kw*2.5 , kh*3.5);
const vec2 cr = vec2(kw*7.5 , kh*3.5);


void main(){

};