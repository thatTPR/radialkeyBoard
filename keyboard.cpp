

#include <android/native_window_jni.h>
#include <android/native_activity.h>

char* app_name_str = "radialKeybaord";

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



    template <typename charT=uint8_t>
    float cmpStr(charT* s1 ,uint8_t si1,charT* s2,uint8_t si2 ){
        float d = 0;
        size_t j=0;
        for(size_t i=0 ; (j<si2) || (i<si1);){
            if(s1[i]!=s2[j]){d+=1.0;if(j<si2-1){j++;};};
            else { for(j++;(s2[j] == s1[i]) &&(j<si2); j++){d+=0.1;};}
            if(i<si1-1){i++;}
        };
        return d; 
    };
    template <typenaem charT=uint8_t>
    struct languagedict {

        uint32_t size;
        charT* words;

        uint32_t wordsNum;
        uint32_t* offsets;
        uint8_t* sizeStr;

        
        
        struct fletter {
            uint8_t offset;
            uint8_t letter ; 
            std::vector<fletter> sc;
            uint8_t operator[](uint8_t t){

            };
            fletter(uint32_t _o,  uint16_t _l ) :Offset(_o) , letter(_l) {};
        }
        std::vector<fletter> flet;
        fletter operator[](uint8_t u){return flet[u-'a'] ;};
        void fletters(){
            flet.emplace_back(0 ,words[0] ) ;flet.push_back(0,words[1]) ;
            for(uint32_t i=1;i<wordsNum;i++){
                if(flet.back().sc.back().letter!=words[offsets[i]+1]){flet.back().sc.emplace_back(i,words[offset[i]+1]) ;};
            if(words[offsets[i]] != flet.back().letter ){flet.emplace_back(i,words[offset[i]] ) ;}
            }
        };  
        
        void match(uint8_t* strng , uint8_t& size) {
            for(fletter& a : flet){
                if(a.letter == strng[0]) {
                    for(fletter i : a.sc){
                        if(i.letter = strn[1]){
                            uint32_t ind = a.offset;
                            float dif = cmpStr(strng,size,words+ i.offset , sizeStr[i.offset] );
                            for( uint32_t j = i.offset+1 ; words[offset[j]] ==a.letter ; j++){
                                float d = cmpStr(strng,size,words+ offset[j] , sizeStr[j] )
                                if(d < dif){dif=d; ind = j;}
                            }; 
                            strng = words+offset[ind] ;size=sizeStr[ind]; 
                        }
                    }

                }
            }
        };
        languagedict(std::string& str){
            words = new uint8_t [str.size()] ; 
            size=0;wordsNum=0;
            for(char c : str){
                if(c=='\n'){wordsNum++;}
                else {size++;}
            }
            offsets=new uint32_t[wordsNum];
            sizeStr=new uint32_t[wordsNum];
            size_t i = 0;size_t it=0;offsets[i]=0;
            for(char c: str){
                if(c=='\n'){i++;offsets[i+1] =it+1;sizeStr=it-offsets[i]; }
                else {words[it]=c;}
                it++;
            }
            fletters();
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

template <typename T>
struct lybase {
    
    T botl[3] ={'\t',KC_LSHIFT,KC_CONTROL};
    T botr[2] ={KC_BACK,'\n'}
    T space= KC_SPACE ; 
    lybase() = default;
    lybase(T bl[3], T br[2] , T sp) : botl(bl) , botr(br) , space(sp) {};
};
template <typename T=char , typename BT=char>
struct Layout : lybase<BT>{ 
    T top[2][10] =    {{'1','2','3','4','5','6','7','8','9','0'},
    {'!','@','#','$','%','^','&','*','(',')'}} ;
    T topl[2] = {'`','~'};
    T topCenter[2] = {KC_INSERT , KC_CANCEL} ;

    T leftCenter[2] = {'q','Q'};
    T leftNear[2][6] = {{'w','e','r','t','y','u'},
                           {'W','E','R','T','Y','U'}} ;
    T leftFar[2][12] =      {{'i','o','p','a','s','d','f','g','h','i','j','l'}, 
                             {'I','O','P','A','S','D','F','G','H','I','J','L'}} ;
    T rightCenter = {'z','Z'};
    T rightNear[2][6]={{'x','c','v','b','n','m'}, 
                       {'X','C','V','B','N','M'}} ;
    T rightFar[2][12] ={{',','.','/',';','\'','[',']','\\','-','=',KC_DELETE,' '} ;
                        {'<','>','?',':','\"','{','}','|','_','+', KC_DELETE,' ' }} ;
    
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

    std::vector<T> getEach(size_t j){
        std::vector<T> r;
        for(size_t i=0;i<10;i++){r.push_back(top[j][i]);};
        r.push_back(topl[j]);
        r.push_back(leftCenter[j]);

        for(size_t i=0;i<6;i++){r.push_back(leftNear[j][i]);};
        for(size_t i=0;i<12;i++){r.push_back(leftFar[j][i]);};
        r.push_back(rightCenter[j]);
        for(size_t i=0;i<6;i++){r.push_back(rightNear[j][i]);};
        for(size_t i=0;i<12;i++){r.push_back(rightFar[j][i]);};
        if constexpr (std::is_same<T,BT>::value){
            for(size_t i=0;i<3;i++){r.push_back(botl[i]);}
            for(size_t i=0;i<2;i++){r.push_back(botr[i]);}
            r.push_back(space);
        };
        return r;
    };
    Layout () =default ;

    layout( ,T sTopn, T eTopN, T stopS)
    Layout(T _top[2][10] , T _topl[2], T _topc, T lc[2], T ln[2][6], T lf[2][12],
                                                     T rc[2], T rn[2][6], T rf[2][12] ) : 
                                                     top(_top) , topl(_topl) , topCenter(_topc), 
                                                     leftCenter(lc), leftNear(ln) , leftFar(lf), 
                                                     rightCenter(rc) , rightNear(rn),rightFar(rf) {lybase();};
};


template <typename charT>
struct keyBoard : grpipeline {

    languageDict dict;
    typedef glm::vec4 uv;
    
    
    Layout lyt ;
    
    fontAtlas<AtlasType::stripsOffsetT,charT> atlas;
    void fontLayout(Layout& _lyt , ttf::font& fnt , charT rangeStart,charT rangeEnd ) {
        lyt=_lyt;charT* start ; charT* end;*start=rangeStart;*end=rangeEnd;
        atlas =fontAtlas<AtlasType::stripsOffsetT,charT> fatls(fontAtlas<charT>::type::stripsOffsetT , fnt,1,start,end,false) ;
    };
    
    

    struct position {
        glm::vec2 center;
        charT ch;
        positon(glm::vec2&& _v, charT&& _ch )  :center(_v) , ch(_ch) {};
    };

        
        
    glm::vec2 uvPressed;
    void pressed(charT c,float x,float y){
        atlas.get_range(c)

    }   ; 
    const uint8_t numw = 10;
    const uint8_t numh = 6;
    const float kw = 1/numw;
    const float kh = 1/numh;
            const float rsd = kw/5;

    const glm::vec2 lc=  glm::vec2(kw*2.5,3.5*kh) ;
    const glm::vec2 rc=  glm::vec2(kw*7.5,3.5*kh) ;

    std::vector<position> getPositionLayout(size_t j){    
        std::vector<position> r;r.reserve(56) ;
        size_t i=0;
        for(glm::vec2 v=glm::vec2(kw/2,kh/2) ; v.x<kw*10;v.x+=1.0){r.emplace_back(v,lyt.top[j][i]);i++;};
        r.emplace_back(glm::vec2(kw*0.5,kh*1.5) ,lyt.topl[j] );
        r.emplace_back(glm::vec2(kw*5,kh*1.5) ,lyt.topCenter[j] );
        r.emlace_back(lc,lyt.leftCenter[j]);
        for(float i=1;i<=6;i+=1){r.emplace_back( lc+glm::vec2 (cos(i*M_PI/3) , sin(i*M_PI/3) ),lyt.top[j][i-1])  ; }
        for(float i=1;i<=12;i+=1){r.emplace_back( lc+glm::vec2 (cos(i*M_PI/6) , sin(i*M_PI/6) ),lyt.top[j][i-1])  ; }
        r.emlace_back(lc,lyt.rightCenter[j]);
        for(float i=1;i<=6;i+=1){r.emplace_back(rc+ glm::vec2 (cos(i*M_PI/3) , sin(i*M_PI/3) ) , lyt.top[j][i-1] ) ; }
        for(float i=1;i<=12;i+=1){r.emplace_back(rc+ glm::vec2 (cos(i*M_PI/6) , sin(i*M_PI/6) ) , lyt.top[j][i-1] ) ; }

        r.emplace_back(glm::vec2(kw*0.5,kh*6.5) , lyt.botl[0]);
        r.emplace_back(glm::vec2(kw*1.5,kh*6.5) , lyt.botl[1]);
        r.emplace_back(glm::vec2(kw*2.5,kh*6.5) , lyt.botl[2]);

        r.emplace_back(glm::vec2(kw*5,kh*6.5) , lyt.space);
        r.emplace_back(glm::vec2(kw*5,kh*8.5) , lyt.botr[0]);
        r.emplace_back(glm::vec2(kw*5,kh*9.5) , lyt.botr[1]);
        return r;
        // Layout<glm::vec2> res(  { {kw/2,kh/2} , {kw*1.5,kh/2} ,{kw*2.5,kh*0.5} ,{kw*3.5,kh/2} , {kw*4.5,kh/2} ,{kw*5.5,kh*0.5} ,{kw*6.5,kh/2} , {kw*7.5,kh/2} ,{kw*8.5,kh*0.5} ,{kw*9.5,kh/2} ,{} } ) 
        //                              {{kw/2,kh*1.5}} , {{kw*4.5 , kh*1.5} , {kw*5.5,kh*1.5} , 
        //                              lc , lcn,lcf,rc,rcn,rcf }    ;
    }
    uv space = glm::vec4(kw*4,kh*6,kw*6,kh*7);

    std::vector<position> stripsN;
    std::vector<position> stripsShift;

    std::vector<glm::vec2> stripsN;
    std::vector<glm::vec2> stripsShift;

    struct range { charT ch ;size_t start,end;
        range(charT _ch,size_t _start,size_t _end) : ch(_ch) start(_start) , end(_end) {}; 
    };
    std::vector<range> NRange;
    std::vector<range> shiftRange;

    std::vector<glm::vec2> pressed ;


    range getRange(std::vector<range>& rng,charT ch){
        size_t div=rng.size()/2;size_t i;
        for( i = div ;rng[i].ch!=ch; ){
            div/= div<2 ? 2 : 1;
            i+= rng[i].ch < ch ? -div : div;
        };
        return rng[i];
    };
    void setPressed(std::vector<range>& rng, charT ch){
        range r= getRange(rng,ch);
        pressed.resize(r.end-r.start+1) ;
        std::memcpy( pressed.data() , rng + r.start , (r.end-r.start+1)*sizeof(glm::vec2) );    
    };
    void unsetPressed(){pressed.clear();}
    void getStrips(std::vector<position> pos){

    };
    void getStrips(){
        poslN = getPositionLayout(0)
        std::vector<glm::vec2> vs;
        size_t i=0;
        auto lam = [&](glm::vec2& v , glm::vec2& c) ->glm::vec2 {return glm::vec2(v.x*kw/2 + c.x,v.y*kh/2 + c.y) ;};
        auto iterl = [&](std::vector<position>& poss,std::vector<range>& rng, std::vector<glm::vec2>& strps) ->glm::vec2 {        
            for(position p : poss){ vs=atlas.getRange(p.ch) ;rng.emplace_back(p.ch,i,i+vs.size() - 1) ;
                for(glm::vec2 v : vs ){strps.push_back(lam(v,p.center););};
            }
        };
        iterl(poslN,NRange,stripsN);
        poslShift = getPositionLayout(1);
        iterl(poslShift,shiftRange,stripsShift) ;
    };
    void swtch(bool shft){
        if(shft){

        }
        else {}
    };
    bool shift=false; void shiftPress(){shift!=shift;swtch(shift);}
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
    
    charT getPos(float x,float y){
        glm::vec2 v(x,y) ;
        

        if(y<kh){ size_t i = (size_t)(x/kw) ; lyt.atTop(i,shift)} ;
        if(y<kh*2 and (x<kw)){return shift?lyt.topl[1] : lyt.topl[0] ;}
        
        if(inRadial(x,y,lc)){lyt.atRLeft(getKeyRadial(x,y,lc) ,shift ) } ;
        if(inRadial(x,y,rc)){lyt.atRRight(getKeyRadial(x,y,rc) ,shift ) }; 
        
        size_t s = (size_t)(x/kw);
        if(y>kh*6) {if(s==1){shiftPress();}
            if(x<kw*2){return lyt.botl[s] ;} 
            if(x<kw*6){return lyt.space;}
                return botr[s-8]        
        }
    };    
  std::vector<charT> chr;bool i0,i1;
    bool cursor=false; float crs;bool cursed;
    
    void getChrs(uint8_t* c,size_t& s){
        std::vector<uint8_t> chrs;
        for(size_t i=1;i<chrs.size();i++){
            size_t j=0;for(j=i ; chrs[j]=chrs[i-1] ; j++){};
            chrs.erase(chrs.begin()+i, begin()+j ) ;
        };
        c=chrs.data();s=chrs.size();
        dict.match(c,s);
    };  
    void down(jint id,jfloat x,jfloat y){
        if((y>kh>6) and ((x>kw*4 ) && (x<kw*6))){crs=x;cursor=true;}
        charT c= getPos(x,y);chr.push_back(c);
        pressed(c,x,y);
        if(id==0){p0.push_back(glm::vec2(x,y));i0=true;};
        if(id==1){p1.push_back(glm::vec2(x,y));i1=true;};
    };
    char* up(jint id,jfloat x,jfloat y,char* c,bool* b){
        if((y>kh>6) and ((x>kw*4 ) && (x<kw*6))){if(!cursed){return " ";}}
        
        if(cursor){cursor=false;return;};
        if((y>kh>6) and ((x>kw*4 ) && (x<kw*6))){crs=x;cursor=true;}

        if( i0 ^ i1) {
            solve(chr.data(),chr.size());*b=true;i0=false;i1=false;
            return cs; }
        else {
            if(id==0){i0=false;};
            else {i1=false;};
            charT* cs ;charT c= getPos(x,y); cs={c,'\0'};return cs; }
    };
    jint _move(jint id,jfloat x,jfloat y){
        if(cursor==true){if(glm::abs(x-crs) > (kw/4)) ?  (x-crs )<0 ?-1:1 : 0  ;return;}
        glm::vec2 v = glm::vec2(x,y);
        if(glm::distance(v,lc)<=kw*2.5 || glm::distance(v,rc)<=kw*1.5 ){chr.push_back(getPos(x,y););}
        if(id==0){ glm::vec2 v(x,y);if(rsd<glm::distance(v,p0.back()) || !i0) {ps.emplace_back(0,p0.size());p0.emplace_back(x,y); } ;i0=true;};
        if(id==1){ glm::vec2 v(x,y);if(rsd<glm::distance(v,p1.back()) || !i1) {ps.emplace_back(1,p1.size());p1.emplace_back(x,y); } ;i1=true;};
    };
    jint move(jint id,jfloat x,jfloat y){
        jint s = _move(id,x,y);
        if(s!=0){cursed=true;}
        return s;
    }
    init(charT _start,charT _end){i0=false;i1=false;
        size_t size=_end - _start+1;
        positions.resize(size);

        glm::vec2 c = {kw/2,kh/2};size_t i=0;
        for(glm::vec2 c = {kw/2,kh/2} ;c.x<kw*10;c.x+=1.0){
            positions[]
        };
    };
    keyBoard(std::string& str,charT _start,charT _end, AAsetManager* asman){
        auto lsm = [&](gl::shader_type t, char* codep) ->shaderModule {

        AAsset* asset = AAsetManager_open(asman,codep,ASSET_MODE_BUFFER) ;
        size_t size = AAsset_getLength(asset);
        char* c ;size_t csize;
        AAset_read(asset,c,csize) ;
        shaderModule sm;sm.loadCode(c,csize);sm.sty=t; 
        AAset_close(asset);
    };
        shaderModule vert= lsm(gl::shader_type::vert , "key.vert.spv");
        shaderModule frag = lsm(gl::shader_type::frag, "key.frag.spv");

        grpipeline({&vert,&frag}) ;
        dict=languageDict(str);init(_start,_end);
    }


};

struct emojiPanel {

};

AAssetManager* gAssetManager;


struct widget : mod::grpipeline {
    keyBoard main ;
    keyBoard emoji;

    ttf::font ft;
    
    void _setFont(std::string& st){
        std::stringstream str(st);
        acqres<ft>::read(ft , str) ;};
    void read(char* str ,void (*ptr)(std::string&) ){
        AAsset* asset = AAssetManager_open(
        gAssetManager,
        str,
        AASSET_MODE_BUFFER
    );

    if (!asset) return "";

    size_t size = AAsset_getLength(asset);
    std::string buffer(size, '\0');

    AAsset_read(asset, buffer.data(), size);
    AAsset_close(asset);ptr(buffer);  
    }   ; 
    void _setdict(std::string& st){
        main= keyBoard<char>(st,0,0xFF);
    };
    void readdict(char* str  ) {
        read(str,_setdict);
    };
    void setFont(char* str){read(str,_setFont);};
    void init(){setFont("NotoSerif-Light.ttf") ;
        readdict("words.txt") ;

        main.fontLayout(Layout() , ft,0,0xFF , gAssetManager) ; 

        mod::grpipeline()
        };
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
