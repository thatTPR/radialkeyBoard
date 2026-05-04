const float kw=  0.1;
const float kh = 0.1; 

const vec2 cl = vec2(kw*2.5 , kh*3.5);
const vec2 cr = vec2(kw*7.5 , kh*3.5);

layout(location=0)flat vec4 bgColor;
layout(location=2)flat vec4 fgColor;


layout (set =0 , binding=1) buffer paths {
    uint size;uint brk;
    vec2 paths[]; 
}ps;

layout (set =0 , binding=2) buffer buttons {
    uint size;
    uvec4 buttons[];
}ps;


layout(triangle_strip,max_vertices =3) out ;
out vec2 pos;
out vec4 color;



void main(){
    
    float auvf = 1/ps.size ; 
    vec4 auv=vec4(0,0,auv,1);
    vec4 uvl = vec4(auvf,0,auvf,0);
    
    vec4 uv=vec4(0,0,1,1);
    vec4 uvl = vec4(1,0,1,0);
    
    
    for(uint i=0;i<10;i++){

    };

};