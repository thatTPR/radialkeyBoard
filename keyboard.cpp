#include <android/native_window_jni.h>
#include <android/native_activity.h>

#include <stratum/backend/keycode.hpp>
#include <stratum/backend/impl_android.hpp>
#include <stratum/backend/impl.hpp>
#include <stratum/backend/impl_vk.hpp>

#include <stratum/mod_pipeline.hpp>
#include <stratum/mod_font.hpp>

#include <stratum/acqres/fontft.hpp>
#include<stratum/acqres/acqres_util.hpp>


#include <jni.h>
#include <string>
#include <glm/glm.hpp> 
#include <cmath>


struct radialKeyBoard : mod::grpipeline { 
    
};  


    float cmpStr(uint8_t* s1 ,uint8_t si1,uint8_t* s2,uint8_t si2 ){
        float d = 0;
        size_t j=0;
        for(size_t i=0 ; (j<si2) || (i<si1);){
            if(s1[i]!=s2[j]){d+=1.0;if(j<si2-1){j++;};};
            else { for(j++;(s2[j] == s1[i]) &&(j<si2); j++){d+=0.1;};}
            if(i<si1-1){i++;}
        };
        return d; 
    };
    struct languagedict {

        uint32_t size;
        uint8_t* words;

        uint32_t wordsNum;
        uint32_t* offsets;
        uint8_t* sizeStr;


        
        struct fletter {
            uint8_t Offset;
            uint8_t letter ; 
            fletter(uint32_t _o,  uint16_t _l ) :Offset(_o) , letter(_l) {};
        }
        std::vector<fletter> flet;
        void fletters(){
            flet.push_back({0 ,words[0]}) ;
            for(uint32_t i=1;i<wordsNum;i++){
            if(words[offsets[i]] != flet.back().letter ){flet.emplace_back(i,words[offset[i]]) ;}
            }
        };  
        
        void match(uint8_t* strng , uint8_t& size) {
            for(fletter& a : flet){
                if(a.letter == strng[0]) {
                    uint32_t ind = a.offset+1;
                    float dif = cmpStr(strng,size,words+ offset[j] , sizeStr[j] );
                    for( uint32_t j = a.offset+1 ; words[offset[j]] ==a.letter ; j++){
                        float d = cmpStr(strng,size,words+ offset[j] , sizeStr[j] )
                        if(d < dif) ; ind = j;
                    }; 
                    strng = words+offset[ind] ;size=sizeStr[ind]; 
                }
            }
        };
    };
    struct dict  {
        languagedict ld;  
        dict(languagedict& ld , symbols ){};
    };
    struct layout {
        struct radial {
            uint8_t levelsNum;
            uint8_t firstSize;
        } ;
        uint8_t keysize;

        uint8_t radialNums;
        radial* radials;
        
        struct index {uint8_t size;};  
    };  


const uint8_t OuterKeys = 8 ; 
const uint8_t InnerKeys = 6  ;

template <typename T=uint8_t>
struct asciiLayout { 
    using type = uint8_t ; 
    T top[2][10] =    {{'1','2','3','4','5','6','7','8','9','0'},
    {'!','@','#','$','%','^','&','*','(',')'}} ;
    T topl[2] = {'`','~'};
    T topCenter[2] = {' ',' '} ;

    T leftCenter[2] = {'q','Q'}
    T leftNear[2][6] = {{'w','e','r','t','y','u'},
                           {'W','E','R','T','Y','U'}} ;
    T leftFar[2][12] =      {{'i','o','p','a','s','d','f','g','h','i','j','l'}, 
                             {'I','O','P','A','S','D','F','G','H','I','J','L'}} ;
    T rightCenter = {'z','Z'};
    T rightNear[2][6]={{'x','c','v','b','n','m'}, 
                       {'X','C','V','B','N','M'}} ;
    T rightFar[2][12] ={{',','.','/',';','\'','[',']','\\','-','=',KC_DELETE,' '} ;
                        {'<','>','?',':','\"','{','}','|','_','+', KC_DELETE,' ' }} ;
    // uint8_t leftBottomUp = '\t' ;
    // uint8_t leftBottom = KC_LSHIFT  ;
    // uint8_t centerBottom = ' ';
    // uint8_t rightBottom = '\n';
    // uint8_t rightBottomUp 
    uint8_t botl[3] ={'\t',KC_LSHIFT,KC_CONTROL};
    uint8_t botr[2] ={KC_BACK,'\n'}
    uint8_t space= KC_SPACE ; 
    T atTop(size_t s,bool shift){return shift? top[1][s]:top[0][s]  ;};
    T atBot(size_t s){return bot[s];};


    template <>
    T atRadial(size_t s,bool shift , T (&c)[2]  , T (&in)[2][6] , T (&far)[2][12]){
        if(s<1){return shift?c[1] : c[0];};
        if(s<7){return shift?in[1][s-1] : in[0][s-1];  };
        return shift?in[1][s-7] : in[0][s-7];  
    };
    T atRLeft(size_t s,bool shift){return atRadial(s,shift,leftCenter,leftNear,leftFar)  ;}; 
    T atRRight(size_t s,bool shift){return atRadial(s,shift,rightCenter,rightNear,rightFar)  ;}; 

    T atCenter(size_t s,bool shift){};
    asciiLayout () =default ;
    asciiLayout(T _top[2][10] , T _topl[2], T _topc, T lc[2], T ln[2][6], T lf[2][12],
                                                     T rc[2], T rn[2][6], T rf[2][12],
                                                     T bl[3] , T br[3] ,   ) : top(_top) , topl(_topl) , topCenter(_topc), 
                                                     leftCenter(lc), leftNear(ln) , leftFar(lf), rightCenter(rc) , rightNear(rn),
                                                        rightFar(rf), botl(bl) , botr(br) {};
};


struct keyBoard {

    template <typenmae charT>
    struct fontLayout {
        asciiLayout<charT> chr;

        
        layout(layout& lyt , ttf::font& fnt , charT rangeStart,charT rangeEnd ) {
            charT* start ; charT* end;*start=rangeStart;*end=rangeEnd;
            fontAtlas<charT> fatls(fontAtlas<charT>::type::stripsOffsetT , fnt,1,start,end,false) ;

        };
    };
    void strip(uint8_t& size,uint8_t* keys){
            for(size_t i=1;i<size;){if(keys[i+1] == keys[i-1]) {
                size_t j;for(j=i+1;j<size;j++){if(keys[j]!=keys[i-1]){break;} }
                std::memcpy(keys+i , keys+j ,size-j);
                i=j;
            }else {i++;}  };
        };
    
        languageDict dict;

    void solve( uint8_t& size, uint8_t* keys){strip(size,keys);dict.match(ev.str,ev.size);};
    

    

    
    std::vector<glm::vec2> fMove;
    std::vector<glm::vec2> rMove;

    typedef glm::vec4 uv;

    
    
    
    const uint8_t numw = 10;
    const uint8_t numh = 6;
    const float kw = 1/numw;
    const float kh = 1/numh;
    const glm::vec2 lc=  glm::vec2(kw*2.5,3.5*kh) ;
    const glm::vec2 rc=  glm::vec2(kw*7.5,3.5*kh) ;

    uv space = glm::vec4(kw*4,kh*6,kw*6,kh*7);


    bool shift=false;
    constexpr std::array<glm::mat4x2,12> getTopCexpr(){  
        std::array<glm::mat4x2> res;
        glm::mat4x2 v ={{0,0} ,{kw,0},{kw,kh},{0,kh} } ;
        res[0]=v;
        for(size_t i=1;i<10;i++){res[i]=v+glm::vec2((float)i*kw,0);};
        res[10]= v + glm::vec2{kw,kh};
        res[11]= v + glm::vec2{kw*2,kh};
    };  
    constexpr std::array<uv,5> getBotL(float fl=0){  
        std::array<uv> res;
        res[0] = glm::vec4(kw*9,kh*5,kw*10,kh*6);
        res[1] = glm::vec4(kw*8,kh*6,kw*9,kh*7);
        res[0] = glm::vec4(kw*9,kh*6,kw*10,kh*7);
        return res;
    };
    constexpr std::array<uv,5> getBotR(float fl=0){  
        std::array<uv> res;
        res[0] = glm::vec4(kw*9,kh*5,kw*10,kh*6);
        res[1] = glm::vec4(kw*8,kh*6,kw*9,kh*7);
        res[0] = glm::vec4(kw*9,kh*6,kw*10,kh*7);
        return res;
    };
    bool isInside(float x,float y, mat4x2& m){
        glm::vec2 v(x,y);
        glm::vec2 i = m[0] ;
        glm::vec2 i1 = m[1];
        glm::vec2 i2 = m[2];
        glm::vec2 i3 = m[3] ;
        v= v-i;
        i1=i1-i;i3=i3-i;
        float va;float v;float v3;float di , dv;
    auto lam=[&](){va=atan2(v.y,v.x);
                    v=atan2(i1.y,i2.x);
                    v3=atan2(i3.y,i1.x); ;di=v3-v; dv=va-v;}
        if(!(glm::abs(dv) < glm::abs(di))) {return false ; }
        float f= glm::abs( (i3 - va) / (va-i1)) ;

        if(glm::length(v)< ( f* glm::length(i3)  + (1-f) * glm::length(i1))){return true;}
        v=v-i1;
        i2=i2-i-i1;
        i3=i3-i1;  
        lam();
        if(!(glm::abs(dv) < glm::abs(di))) {return false ; }
        float f= glm::abs( (i3 - va) / (va-i1)) ;
        if(glm::length(v)< ( f* glm::length(i3)  + (1-f) * glm::length(i1))){return true;}
    };
    size_t getKeyRadial(float x,float y , glm::vec2 c){
        glm::vec2 v = glm::vec2(x,y);
        glm::vec2 vec=  v - c;
        float d = sqrt(vec.x*vec.x + vec.y*vec.y)
        if(d<(kw*0.5)) {return 0;};
        float ang =atan2(vec.y,vec.x) ;

        if(d<(kw*1.5)){ return 1+ ang / (M_PI/3); };
        if(d<(kw*2.5)){return 7+ ang / (M_PI/6);};
    };
    bool inRadial(float x, float y glm::vec2 c){
        return (abs(x -c.x ) < kw*2.5 ) &&  (abs(y-c.y) < kh*2.5 ) ;
    };
    uint8_t getPos(float x,float y){
        glm::vec2 v(x,y) ;
        

        if(y<kh){ size_t i = (size_t)(x/kw) ; lyt.atTop(i,shift)} ;
        if(isInside(x,y,topl[10])){return shift?lyt.topl[1] : lyt.topl[0] ;}
        
        if(inRadial(x,y,lc)){lyt.atRLeft(getKeyRadial(x,y,lc) ,shift ) } ;
        if(inRadial(x,y,rc)){lyt.atRRight(getKeyRadial(x,y,rc) ,shift ) }; 
        
        size_t s = (size_t)(x/kw);
        if(x<kw*2){return shift?lyt.botl[s]: lyt.botl[0][s] ;} 
        if(x<kw*6){return lyt.space;}
            return botr[s-6]        
        
        
    };
    std::vector<glm::vec2> p; 
    std::vector<uint8_t> chr;bool i0,i1;
    void down(jint id,jfloat x,jfloat y){
        chr.push_back(getPos(x,y));
        if(id==0){p0.push_back(glm::vec2(x,y));i0=true;};
        if(id==1){p1.push_back(glm::vec2(x,y));i1=true;};
    };
    void up(jint id,jfloat x,jfloat y,char* c,bool* b){
        if(i0==0){i0=false;};
        if(i1==0){i1=false;};
        if(! ( i0 || i1)) {
            uint8_t* c = chr.data();
            size_t s=chr.size();
            solve(cs,s);*b=true;
            return cs; }
    };
    void move(jint id,jfloat x,jfloat y){
        if(id==0){p.push_back(glm::vec2(x,y));i0=true;};
        if(id==1){p.push_back(glm::vec2(x,y));i1=true;};
    };
    init(){i0=false;i1=false;}
    
};

struct emojiPanel {

};

AAssetManager* gAssetManager;


struct widget  {
    keyBoard main ;
    keyBoard Secondary;
    keyBoard emoji;

    ttf::font ft;
    
    void setFont(char* str){
        AAsset* asset = AAssetManager_open(
        gAssetManager,
        filename,
        AASSET_MODE_BUFFER
    );

    if (!asset) return "";

    size_t size = AAsset_getLength(asset);
    std::string buffer(size, '\0');

    AAsset_read(asset, buffer.data(), size);
    AAsset_close(asset);
    
    std::stringstrema str(buffer); 

    acqres<ft>::read(ft , str) ;

}
    void init(){setFont("NotoSerif-Light.ttf") ;
        };
};


keyyBoardLoadInfo get_ascii_load_info(){
    return keyBoardLoadInfo()
};
widget w;
JNIEnv* env;
extern "C" JNIEXPORT void JNICALL
android_view_im_nativeInit(JNIEnv* e, jobject thiz ,jobject surface , jobject assetManager) {env=e; 
    sys.window = ANativeWindow_fromSurface(e,surface) ;

gAssetManager = AAssetManager_fromJava(env, assetManager);

    
    pl.initialize();  w.init() ;
};


extern "C"
JNIEXPORT void JNICALL
android_view_im_nativedown(jint id,
    jfloat x,
    jfloat y)
{w.down(id,x,y);}

extern "C"
JNIEXPORT void JNICALL
jstring android_view_im_nativeup(jint id,
    jfloat x,
    jfloat y)
{
    bool b;
        char* c;
        w.up(id,x,y,c,&b);
    if(b){return env->newStringUTF(c);}
    else return env;

};

extern "C"
JNIEXPORT void JNICALL
jint android_view_im_nativemove(jint id,
    jfloat x,
    jfloat y)
{w.move(id,x,y);};
