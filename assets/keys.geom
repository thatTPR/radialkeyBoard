const float kw=  0.1;
const float kh = 0.1; 

const vec2 cl = vec2(kw*2.5 , kh*3.5);
const vec2 cr = vec2(kw*7.5 , kh*3.5);

const vec4 bgColor;
const vec4 fgColor;



layout (set=0,binding =0 ) uniform uvec2 nshiftSize;
layout (set=0,binding =1 ) uniform ns{vec2 v[];}n;
layout (set=0,binding =2 ) uniform shifts{vec2 v[];}s ;


layout (set=1,binding =0 ) uniform uvec2 pressed ;
layout(set=1,binding =1 ) uniform bool shift;
layout (set=1,binding=2 ) uniform uint lsize;
layout (set=1,binding=3 ) uniform lines {vec2 v[];}l;



layout(triangle_strip,max_vertices =10000) out ;
flat out vec4 color;

#define PI

const float sw = 0.01
void circle(vec2 c,float r){
    float a=0;
    vec2 p = c + vec2(cos(a)*r, sin(a)*r) ;EmitVertex();
    gl_Positon =p + vec2(cos(a)*sw , sin(a)*r); EmitVertex();
    for(a+=0.005;a<2 * PI;a+= 0.005){
        vec2 p = c + vec2(cos(a)*r, sin(a)*r) ;EmitVertex();
        gl_Positon =p + vec2(cos(a)*sw , sin(a)*r); EmitVertex();
    };
};
void circleLine(vec2 c,float r , float sep){
    float a=0;
    vec2 p = c + vec2(cos(a)*r, sin(a)*r) ;EmitVertex();

    gl_Position =p + vec2(cos(a)*sw , sin(a)*r); EmitVertex();
    for(a+=0.005;a<2 * PI;a+= 0.003){
        vec2 p = c + vec2(cos(a)*r, sin(a)*r) ;gl_Position=p;EmitVertex();
        p+=p + vec2(cos(a)*sw , sin(a)*r);gl_Positon =p; EmitVertex();
        if( a % (PI*2 / sep) < (sw * r/PI)) {
            p += vec2(kw*cos(a),kw*sin(a)) ;gl_Position=p;EmitVertex();
            p+=vec2(sw*cos(a+PI/2) ,sw*sin(PI)/2) ;gl_Position = p ;EmitVertex();
            p+=vec2(-kw*cos(a),-kw*sin(a));gl_Position=p;EmitVertex();
            a+= sw * r / PI ;
        }
    };
};

void radial(vec2 c){
    circleLine(c,kw/2,6)
    circleLine(c,kw*1.5,12);
    circle(c,kw*2.5);
    EndPrimitive();
};


void shiftPress(){for(uint i=pressed.x ; i<=pressed.y;i++){gl_Position = s.v[i]+vec2(0,kh); EmitVertex() ;}; EndPrimitive();}
void nPress(){for(uint i=pressed.x ; i<=pressed.y;i++){gl_Position = n.v[i]+vec2(0,kh) ; EmitVertex();}; EndPrimitive();}

void n(){for(uint i=pressed.x ; i<=nshiftSize.x;i++){gl_Position = ns.v[i] ; EmitVertex();}; EndPrimitive()}
void shift(){for(uint i=0 ; i<=nshiftSize.y;i++){gl_Position = shifts.v[i] ; EmitVertex();}; EndPrimitive();}

void main(){
    radial(cl);
    radial(cr);    
    if(presbool ) {if (shift){shiftPress();};
        else nPress();
    }
    if(shift){shift();}
    else n();

    for(uint i =0;i<=lsize;i++){
        gl_Position=l.v[i];EmitVertex();
    };
    EndPrimitive();
    for(uint i=0;i<lsize;i++){
        if(l.v[i]<vec2(0,0)){color=vec4(0,0,1,0.7) ;EndPrimitive();continue;}
        gl_Position =l.v[i];EmitVertex(); 
    };

};