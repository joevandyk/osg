#include <stdio.h>
#include <string.h>
// reading a design workshop file utility
// (c) GW Michel, 2001.
// Design Workshop format files can be downloaded from www.artifice.com
// Design Workshop editor can be downloaded from www.artifice.com = Mac & Win95/98/NT versions are available.
// DW Lite is completely free, produces textured 3D models
// aimed mostly at the architectural world.  Flat polygons are generally produced
// No ability to produce smooth shading, unfortunately.  
// But it is the best bangs per buck. (Anything/nothing = infinite value, and this is quite a lot/nothing)

#include <osg/CullFace>
#include <osg/Geode>
#include <osg/Group>
#include <osg/GeoSet>
#include <osg/Light>
#include <osg/LightSource>
#include <osg/Material>
#include <osg/Texture>
#include <osg/Transparency>
#include <osg/TexEnv>
#include <osg/StateSet>
#include <osg/Notify>

#include <osgDB/FileNameUtils>
#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <GL/glu.h>

using namespace osg;

#ifndef WIN32
    #define CALLBACK 
#endif

class _dwobj; // predefine for later call
int dwfgets(char *clin, int max, FILE *fin); // , end of line= 13 as well as Creturn=10

class dwmaterial {// design workshop material, to be translated to OGL
public:
    typedef enum {Properties,TiledTexture,FullFace, SpotLight,PointLight} mttype;
    dwmaterial() { type=Properties; 
        opacity=1; specular=0; specexp=0; fname="";TextureWidth=1; TextureHeight=1;
        ctx=NULL; tx=NULL; id=0; dstate=NULL;colour[0]=colour[1]=colour[2]=colour[3]=1;
        bright=halfIn=halfOut=falloff=0;atyp=NONE;
    }
    ~dwmaterial() { }
    void settexture() {
        if (!dstate) dstate = new StateSet;
        if (isTextured()) { // shares common textures
            if (!ctx || !tx) { // new texture needed
                if (fname.length()>0) {                
                    ctx=osgDB::readImageFile(fname.c_str());
                    if (ctx) {
                        ctx->setFileName(fname);
                        tx=new Texture;
                        tx->setImage(ctx);
                        tx->setWrap(Texture::WRAP_S, Texture::REPEAT);
                        tx->setWrap(Texture::WRAP_T, Texture::REPEAT);
                    }
                    osg::TexEnv* texenv = new osg::TexEnv;
                    texenv->setMode(osg::TexEnv::MODULATE);
                    dstate->setAttribute( texenv );
                }
            }
            if (ctx && tx) { // texture exists
                dstate->setAttributeAndModes(tx,osg::StateAttribute::ON);
                dstate->setMode(GL_TEXTURE_2D,StateAttribute::ON);
            }
        }
    }
    StateSet *make() { // returns the OSG material
        if (!dstate) { // if it does not exist, then make it
            dstate = new StateSet;
            osg::Material* osgMaterial = new osg::Material;
            dstate->setAttribute(osgMaterial);
            if (opacity<0.99) {
                osgMaterial->setTransparency(Material::FRONT_AND_BACK, opacity);
                dstate->setMode(GL_BLEND,StateAttribute::ON);
                dstate->setRenderingHint(StateSet::TRANSPARENT_BIN);
                colour[3]=opacity;
            }
            osgMaterial->setAmbient(Material::FRONT_AND_BACK,colour);
            osgMaterial->setDiffuse(Material::FRONT_AND_BACK,colour);

            Vec4 colspec=colour*specular;
            colspec[3]=colour[3];
            osgMaterial->setSpecular(Material::FRONT_AND_BACK,colspec);
            osgMaterial->setShininess(Material::FRONT_AND_BACK,specexp);

            dstate->setMode( GL_LIGHTING, StateAttribute::ON );
            dstate->setMode( GL_CULL_FACE, StateAttribute::ON );

            osg::CullFace *cf = new osg::CullFace; // to define non-default culling
            cf->setMode(osg::CullFace::BACK);
            dstate->setAttribute(cf);

            dstate->setMode(GL_TEXTURE_2D,StateAttribute::OFF);
            settexture();
        }
        return dstate;
    }
    inline int isType(mttype t1) const {return     (type==t1 ); }
    inline int isTextured() {return     (type==TiledTexture || type==FullFace ); }
    void setcolour(const float rgb[3]) {
        colour[0]=rgb[0]; colour[1]=rgb[1]; colour[2]=rgb[2];
    }
    void settxrep(const float repx, const float repy) {
        TextureWidth=repx;
        TextureHeight=repy;
    }
    inline float getRepWid() const { return TextureWidth;} 
    inline float getRepHt() const { return TextureHeight;}
    inline int isFullFace() const { return  type==FullFace;}
    inline int getid() const { return  id;}
    inline void setid(const int i) { id=i;}
    inline void setopacity(float o) { opacity=o;}
    inline void setspecular(float o) { specular=o;}
    inline void setspecexp(float o) { specexp=o;}
    void setType(const char *buff) {
        if (strncmp(buff,"Tiled_Texture",13)==0) 
            type=dwmaterial::TiledTexture;
        else if (strncmp(buff,"Spot_Light",11)==0) 
            type=dwmaterial::SpotLight;
        else if (strncmp(buff,"Point_Light",11)==0) 
            type=dwmaterial::PointLight;
        else if (strncmp(buff,"Properties",11)==0) 
            type=dwmaterial::Properties;
        else if (strncmp(buff,"Full_Face_Texture",16)==0) 
            type=dwmaterial::FullFace;
    }
    void setfname(const char *buff) {
        fname=new char[strlen(buff+13)+5];
        fname= (buff+13);
        fname+= ".tga";
    }
    LightSource *makeLight(const Vec4 pos) {
        Light *lt= new Light;
        Vec4 cdef;
        cdef[0]=cdef[1]=cdef[2]=0.0f; cdef[3]=0.0f;
        lt->setSpecular(colour*bright/2.0f);
        lt->setDiffuse(colour*bright/4.0f);
        lt->setAmbient(cdef);
        lt->on();
        if (atyp==NONE) ;
        else if (atyp==INVERSE_DIST) {
            lt->setLinearAttenuation(1.0f);
            lt->setConstantAttenuation(0.01f);
        }
        lt->setPosition(pos);
        LightSource *ls=new LightSource();
        ls->setLight(lt);
        return ls;
    }
    void setAtten(const char *buff) {
        if (strstr(buff,"kQ3AttenuationTypeNone")) atyp=NONE;
        else if (strstr(buff,"kQ3AttenuationTypeInverseDistance")) atyp=INVERSE_DIST;
        //    else if (strstr(buff,"kQ3AttenuationTypeNone")) ;
    }
    void setBright(const float br) { bright=br;}
    void setHalfAngleIn(const float ha) { halfIn=ha;}
    void setHalfAngleOut(const float ha) { halfOut=ha;}
    void setFallOff(const float fo) { falloff=fo;}
    const Vec4 getcolour() { return colour;}
private:
    int id;
    Vec4 colour; // the ambient/diffuse+alpha colour
    mttype type;
    float opacity, specular, specexp; // transp, specularity properties
    float TextureWidth, TextureHeight;
    std::string fname; // picture file
    enum atten {NONE, INVERSE_DIST, INVERSE_SQUARE} atyp;
    float bright,halfIn,halfOut,falloff; // light brightness
    Image *ctx;
    Texture *tx;
    StateSet *dstate; // used to represent the dw material in OSG
};
// structure to use as data for tesselation

typedef struct {
    double pos[3]; // must be double for the tessellator to detect vertices
    Vec2 uv; // texture coordainte - may not be used?
    Vec3 nrmv; // surface normal
    const dwmaterial *themat;
    int idx; // index in the verts[] array
} avertex;

class _face {
public:
    _face() { opening=NULL; idx=NULL; nv=0; nop=0; nset=0; nrm[0]=nrm[1]=nrm[2]=0;}
    ~_face() { delete [] idx;}
    void setnv(const int n){ nv=n; idx=new int[n];}
    void addvtx(const int n){ 
        if (nset < nv) {
            idx[nset]=n;
            nset++;
        }
    }
    void addholevtx(const int nvtot) {
        if (opening) {
            opening[nop-1].addvtx(nvtot);
        }
    }
    void norm(Vec3 &n, const Vec3 side, const Vec3 s2) const {
        n=s2^side; // perpendicular
        n.normalize(); // unit norm
    }
    const Vec3 getnorm(void) const { return nrm; } // use the predefined normal
    void getside12(Vec3 &s1, Vec3 &s2, const Vec3 verts[]) const {
        int ic=1; // counter for later vertices to ensure not coincident
        int i1=idx[0]; // first vertex of face
        int i2=idx[1]; // second, must be non-coincident
        while (i2==i1) {
            ic++;
            i2=idx[ic]; 
        }
        int i3=idx[ic]; // second, must be non-coincident
        while (i3==i2 || i3==i1) {
            ic++;
            i3=idx[ic]; 
        }
        s1=(verts[i2]-verts[i1]); // side 1 of face
        s2=(verts[i3]-verts[i2]); // side 2 of face
    }
    void getnorm(const Vec3 verts[]) {
        Vec3 side, s2; // used in cross product to find normal
        getside12(side,s2, verts);
        norm(nrm, s2, side);
    }
    void getsides(float *wid, float *ht, const Vec3 verts[]) const
    {
        Vec3 side, s2; // used in cross product to find normal
        getside12(side,s2, verts);
        *wid=side.length();
        *ht=s2.length();
    }
    void settrans(Matrix &mx, const Vec3 nrm, const Vec3 verts[], const dwmaterial *mat) const { 
        // define the matrix perpendcular to normal for mapping textures
        float wid=mat->getRepWid(); 
        float ht=mat->getRepHt();
        Vec3 r1, r2,r3; // 3 rows of rotation matrix
        if (mat->isFullFace()) { // set wid, ht from polygon
            Vec3 s2; // want transformed u coordinate parallel to 'r1'
            getside12(r1,s2, verts); // r1 = edge of first side
            getsides(&ht, &wid, verts); // get the lengths of sides for transformation of xyz to uv
            r3=nrm;
            r1.normalize();
            r2=r3^r1;
        } else {
            // mat.nrm= (0,0,1)  AND mat (0,1,0) => (0,a,b)
            // the transformation is unitary - preserves lengths; and
            // converts points on a plane into (s,t, constant) coords for use with texturing
            // Rinv.(0,0,1) = (nrm) implies R since Rinv=R(transpose)
            r3=nrm; // already a unit vector
            // mat.(010) = (0ab) -> Minv.(0ab) = (010); and this row DOT nrm=0
            if (r3.z() < 0.99f && r3.z() > -0.99f) { // not face parallel to ground - choose r1 perpendicular to nrm & 001
                r2.set(0,0,1); // therefore r1 is in plane of face.
                r1=r2^r3;
                r1.normalize();
            } else { // parallel to ground - make perpendicular to edge 1 of face
                r1=verts[idx[1]]-verts[idx[0]];
                r1.normalize();
            }
            r2=r3^r1;
        }
        for (int j=0; j<3; j++) { // and create the transpose matrix (inverse of rotation matrix)
            mx(0,j)=r1[j];
            mx(1,j)=r2[j];
            mx(2,j)=r3[j];
        }        
        //        mx.postTrans(mx,0.5f,0.5f,0.0f);
        mx(0,0)*=1.0f/wid;
        mx(1,0)*=1.0f/wid;
        mx(0,1)*=1.0f/ht;
        mx(1,1)*=1.0f/ht;
        if (mat->isFullFace()) { // set offset such that mx*verts[idx[0]] -> uv=(0,0)
            Vec3 pos;
            pos=mx*verts[idx[0]];
            mx(0,3)=pos.x();
            mx(1,3)=pos.y();
            mx(2,3)=pos.z();
        } else { // scale inversely to the texture preferred repeat size
            mx(0,3)=0.5f/wid;
            mx(1,3)=0.5f/ht;
        }
        //        mx.postScale(mx,1.0f/themat->TextureWidth, 1.0f/themat->TextureHeight,1);
    }
    inline int setnvop(const osg::ushort n) { // add a new hole in this face with n vertices
        _face *oldop=opening; 
        opening=new _face[nop+1]; 
        for (int i=0; i<nop; i++) opening[i].move(&oldop[i]);
        delete [] oldop;
        opening[nop].setnv(n);
        nop++;
        return (nop-1);
    }
    void move(class _face *oldop) { *this=*oldop; oldop->idx=NULL;}
    inline int getnv() { return nv;}
    inline int getvert(const int j) { return idx[j];}
    inline int complete() { return (idx && nv>0 && nset==nv);} // face has all defined
    inline int holecomplete() { if (!opening) return 1; // no hole, so it is complete
        return opening[nop-1].complete();} // latest opening in face has all vertices defined
    int getallverts(void) const { int ntot=nv;
        for (int i=0; i<nop; i++) ntot+=opening[i].getnv();
        return ntot;
    }
    void setnorm(const Vec3 *verts) { // set the face normal
        getnorm(verts);
        for (int i=0; i<nop; i++) {
            opening[i].setnorm(verts);
            if (nrm*opening[i].nrm > 0.0f) { // normals are parallel - reverse order of vertices
                opening[i].reverse();
                opening[i].setnorm(verts);
            }
        }
    }
    void setposes(avertex &poses, const int j, const Vec3 verts[]) const {
        poses.pos[0]=verts[idx[j]].x();
        poses.pos[1]=verts[idx[j]].y();
        poses.pos[2]=verts[idx[j]].z();
        poses.nrmv=nrm;
        poses.idx=idx[j];
    }
    void tesselate(const Vec3 verts[], const dwmaterial *themat, 
          GLUtesselator *ts, _dwobj *dwob, const Matrix *tmat) const;
    void link(const int idop, const _face *f2, const int idop2,const Vec3 verts[], const dwmaterial *themat) const; // to join up opposed faces of a hole
    inline const int getidx(int i) const { return idx[i];}
private:
    void linkholes(const Vec3 verts[], const dwmaterial *themat, const _face *f2) const;
    void reverse() { // reverse order of the vertices
        for (int j=0; j<nv/2; j++) {
            int it=idx[j];
            idx[j]=idx[nv-j-1];
            idx[nv-j-1]=it;
        }
    }
    int nop; // number of openings so far
    class _face *opening; // openings in this face. number of verts, vertex list for opening
    int nv; // number of vertices in the face
    int nset; // number read so far
    Vec3 nrm; // surface normal
    int *idx; // indices into the vertex list for the object
};

// structure for generating triangles (and tstrips, tfans etc)
// from a design workshop object.

class prims {
public:
    prims() { nbegin=0; primlengs=NULL;gsidx=NULL;nrmidx=NULL;
        txidx=NULL;nrms=NULL;txcoords=NULL; tmat=NULL;
        nload=0; nff=0; curmode=0;
    }
    ~prims() {    /*delete [] primlengs; delete [] nrms;
        delete [] gsidx; delete [] nrmidx; delete [] txcoords;*/
    }
    bool isGLtype() { return nbegtype[nbegin]==curmode;}
    void addv(avertex *pos) { // tesselation callback
        nrmidx[nload]=nload; // all normals for this face are same, flat shade
        gsidx[nload]=pos->idx; // vertex position index
        nrms[nload]=pos->nrmv;
        txcoords[nload].set(pos->uv[0],pos->uv[1]);
        txidx[nload]=nload; // matrix transformed txc
 
        ntesverts[nbegin]++;
        nload++;
    }
    void End() {
        if (nbegin<20 && ntesverts[nbegin]>0) {
            primlengs[nff+nbegin]=ntesverts[nbegin];
            nbegin++;
        }
    }
    void begin(GLenum op) { // part of a tesselator callback - starts a new primitive of type op
        if (nbegin<20) { // reSet counters
            nbegtype[nbegin]=op;
            ntesverts[nbegin]=0;
        }
        switch (op) {
        case GL_TRIANGLES:
        case GL_TRIANGLE_STRIP:
        case GL_TRIANGLE_FAN:
        case GL_QUADS:
        case GL_QUAD_STRIP:
        case GL_POLYGON:
            break;
        default:
                        // commenting out since this value is a local and
                        // is not used, RO July 2001.
            // int i=0;
            break;
        }
    }
    void combine( GLdouble coords[3], avertex *d[4], 
        GLfloat w[4], avertex **dataOut , _dwobj *dwob);
    void linkholes(const Vec3 verts[], const dwmaterial *themat, 
        const _face *f1, const _face *f2, 
        const int ipr[2], const int idx[], const int nv) {
        gsidx[nload]=f1->getidx(ipr[1]); // vertex position index
        gsidx[nload+1]=f1->getidx(ipr[0]); // vertex position index
        gsidx[nload+2]=f2->getidx(nv-ipr[0]-1); // vertex position index
        gsidx[nload+3]=f2->getidx(nv-ipr[1]-1); // vertex position index
        
        Matrix mx; // texture matrix transform to plane
        Vec3 s1,s2;
        Vec3 nrm;
        s1=verts[gsidx[nload+1]]-verts[gsidx[nload]];
        s2=verts[gsidx[nload+2]]-verts[gsidx[nload+1]];
        f1->norm(nrm, s2, s1);
        f1->settrans(mx, nrm, verts,themat);
        for (int j=0; j<4; j++) {
            Vec3 uv;
            uv=mx*verts[gsidx[nload]];
            txcoords[nload].set(uv[0],uv[1]);
            nrmidx[nload]=nload;
            txidx[nload]=nload; // matrix transformed txc
            nrms[nload]=nrm; // for per vertex normals
            nload++;
        }
        nff++;
    }
    void setmode(int md, const int nfnvf) { // define the GL primitive type & number of expected vertices
        GLenum mode[]= {GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, 
            GL_QUADS, GL_QUAD_STRIP, GL_POLYGON};
        curmode=mode[md];
        gsidx=new osg::ushort[6*nfnvf];
        nrmidx=new osg::ushort[6*nfnvf];
        txidx=new osg::ushort[6*nfnvf];
        txcoords=new Vec2[6*nfnvf]; // one texture coord per vertex
        nrms=new Vec3[6*nfnvf]; // one normal per face or per vertex
        primlengs=new int[nfnvf]; // primitive lengths
        nff=0;
        nload=0;
    }
    void tesselate(const _face &fc, const Vec3 verts[], const dwmaterial *themat,GLUtesselator* ts, _dwobj *dwob) 
    {    // generates a set of primitives all of one type (eg tris, qstrip trifan...)
        nbegin=0; // number of triangle strips etc generated
        fc.tesselate(verts, themat, ts, dwob, tmat);
        nff+=nbegin; // the number of primitives generated
    }
    void buildDrawable(Group *grp, Vec3 verts[], dwmaterial *themat, const int nverts) {
        if (nload>0 && nff>0) { // there are some strips of this type
            Geode *geode = new Geode;
            GeoSet *gset = new GeoSet;
            osg::ushort *nunrmidx=new osg::ushort[nload];
            osg::ushort *nutxidx = new osg::ushort[nload];
            osg::ushort *nusidx = new osg::ushort[nload];
            Vec3 *nunrms=new Vec3[nload]; // one normal per face (nff) or per vertex (nload)
            int *nuprimlengs=new int[nff]; // primitive lengths
            Vec2 *nutxc=new Vec2[nload];
            int i; // general counter
            for (i=0; i<nload; i++) {
                nutxc[i]=txcoords[i];
                nusidx[i]=gsidx[i];
                nutxidx[i]=txidx[i];
                nunrmidx[i]=nrmidx[i];
                nunrms[i]=nrms[i];
            }
            delete [] nrmidx;
            delete [] txidx;
            delete [] gsidx;
            delete [] txcoords;
            delete [] nrms;  // recycle memory /**/
            nrms=NULL;
            txcoords=NULL;
            nrmidx=txidx=gsidx=NULL;
            for (i=0; i<nff; i++) {
                nuprimlengs[i]=primlengs[i];
            }
            delete [] primlengs;
            primlengs=NULL;
            txcoords=NULL;
            
            geode->addDrawable(gset);
            grp->addChild( geode ); // add to the world outside
            gset->setNumPrims( nff );
            switch (curmode) {
            case GL_TRIANGLES: gset->setPrimType( osg::GeoSet::TRIANGLES );
                gset->setNumPrims( nload/3 );
                break;
            case GL_TRIANGLE_STRIP: gset->setPrimType( osg::GeoSet::TRIANGLE_STRIP );
                gset->setPrimLengths( nuprimlengs );
                break;
            case GL_TRIANGLE_FAN: gset->setPrimType( osg::GeoSet::TRIANGLE_FAN );
                gset->setPrimLengths( nuprimlengs );
                break;
            case GL_QUADS: gset->setPrimType( osg::GeoSet::QUADS );
                gset->setNumPrims( nload/4 );
                break;
            case GL_QUAD_STRIP: gset->setPrimType( osg::GeoSet::QUAD_STRIP );
                break;
            case GL_POLYGON: gset->setPrimType( osg::GeoSet::POLYGON );
                break;
            }
            gset->setNormalBinding(osg::GeoSet::BIND_PERVERTEX); //BIND_PERPRIM); //
            gset->setNormals(nunrms, nunrmidx);

            gset->setTextureCoords(nutxc,nutxidx);
            gset->setCoords( verts, nusidx );
            if (themat->isType(dwmaterial::PointLight) || themat->isType(dwmaterial::SpotLight)) {
                Vec4 pos;
                pos.set(0,0,0,1);
                for (int i=0; i<nverts; i++) {
                    pos[0]+=verts[i].x();
                    pos[1]+=verts[i].y();
                    pos[2]+=verts[i].z();
                }
                pos/=nverts;
                LightSource *ls=themat->makeLight(pos);
                grp->addChild(ls);
            } else {
                StateSet *dstate=themat->make();            
                gset->setStateSet( dstate );
            }
        } else {
            delete [] primlengs;
            delete [] nrms;
            delete [] gsidx;
            delete [] nrmidx;
            delete [] txcoords;
            delete [] txidx;
            primlengs=NULL; gsidx=NULL; nrmidx=NULL;
            txidx=NULL; nrms=NULL; txcoords=NULL;;
        }
    }
    void settmat(const Matrix *mx) {
        tmat= mx;
    }
private:
    int nbegin;
    int ntesverts[20]; // number of vertices in each primitive list - up to 20 restarts
    GLenum nbegtype[20]; // up to 20 restarts
    GLenum curmode;
    int nload; // numebr of vertices loaded into these arrays
    int nff; // primitive loaded
    int *primlengs;
    osg::ushort *gsidx;
    osg::ushort *nrmidx;
    osg::ushort *txidx;
    Vec3 *nrms; // one normal per face
    Vec2 *txcoords; // one texture coord per vertex
    const Matrix *tmat; // local texture matrix, or may be NULL for default mapping
};
static prims prd;

// tesselation subroutines
void CALLBACK myFaceBegin(GLenum op)
{// tess vertex call back
    prd.begin(op);
    //glBegin(op);
}
void CALLBACK myFaceEnd()
{// tess vertex call back
    if ( prd.isGLtype()) { // a prim of type == nvf ending; next counter needed
        prd.End();
    }
    //glEnd();
}
void CALLBACK myVertex(void *pv)
{// tess vertex call back with texture coord == void *pv1, 
    // dwob needed if there is a combine callback to add the new vertex to group
    if (prd.isGLtype()) { // a prim of type == nvf being created; use this vertex
        prd.addv((avertex *)pv);
    }
}
void CALLBACK combineCallback( GLdouble coords[3], avertex *d[4], 
                         GLfloat w[4], avertex **dataOut , _dwobj *dwob) 
{ 
    prd.combine(coords, d, w, dataOut,dwob);
}
void CALLBACK error (GLenum errno)
{ // tess error code
    const unsigned char *errm=gluErrorString(errno);
    printf("tesselator error %d %s\n", errno,errm);//, errm
}
    //==========
void _face::linkholes(const Vec3 verts[], const dwmaterial *themat, const _face *f2) const
{
    int ipr[2];
    ipr[0]=nv-1;
    for (int i=0; i<nv; i++) { // pairs of vertices
        ipr[1]=i;
        prd.linkholes(verts, themat, this, f2, ipr, idx, nv);
        ipr[0]=ipr[1];
    }
}
void _face::link(const int idop, const _face *f2, const int idop2,const Vec3 verts[], const dwmaterial *themat) const
{ // to join up opposed faces of a hole; starts using hole[idop] in THIS, ands at f2.Hole[idop2]
    opening[idop].linkholes(verts, themat, &f2->opening[idop2]);
}
//======== Edges link 2 vertices; indicate where a sharp crease can be found ==========
class _dwedge {
public:
    _dwedge(){;}
    ~_dwedge(){;}
    void set(int i, int j) { e1=i; e2=j; }
private:
    int e1,e2; // ends of the edge - it joins verts[e1] to verts[e2]
};
//===================
class _dwobj {  // class for design workshop read of a single object
public:
    _dwobj() {oldv=verts=NULL; nverts=nfaces=0; openings=NULL;faces=NULL; tmat=NULL; edges=NULL;
        nopens=nfaceverts=0; fc1=fc2=NULL; colour[0]=colour[1]=colour[2]=colour[3]=1;
    } 
    ~_dwobj() {/*delete verts; delete faces;delete openings;*/
        delete fc1;delete fc2;
    } 
    int readOpenings(FILE *fp, const int nexpected)
    { // read up to nexpected openings, each opening may have a number of vertices
        char buff[256];
        openings=new int[nexpected*2];
        fc1=new osg::ushort[nexpected];
        fc2=new osg::ushort[nexpected];
        nopens=0;
        int nvop=0; // current number of vertices in hole in object
        while (nopens<nexpected) { // for each opening
            if (dwfgets(buff, sizeof( buff ), fp )) {
                if (strncmp(buff, "Opening:",8)==0) {
                } else if (strncmp(buff, "faces:",6)==0) {
                    sscanf(buff, "faces: %hu %hu", fc1+nopens, fc2+nopens);
                } else if (strncmp(buff, "numVerts:",9)==0) {
                    int nvtot=nverts; // total number of hole vertices read so far
                    nvop=atoi(buff+9);
                    openings[nopens*2]=faces[fc1[nopens]].setnvop(nvop/2); // prepare opening in face
                    openings[nopens*2+1]=faces[fc2[nopens]].setnvop(nvop/2);
                    readVerts(fp, nvop);
                    for (; nvtot<nverts; nvtot++) {
                        if (faces[fc1[nopens]].holecomplete()) {
                            if (!faces[fc2[nopens]].holecomplete()) {
                                faces[fc2[nopens]].addholevtx(nvtot);
                            } else { // error
                            }
                        } else {
                            faces[fc1[nopens]].addholevtx(nvtot);
                        }
                    }
                    if (faces[fc2[nopens]].holecomplete()) {
                        nopens++;
                    }
                } else { // general line
                }
            }
        }
        return nopens;
    }
    int readEdges(FILE *fp, const int nexpected)
    { // read up to nexpected vertex pairs.  These are currently ignored.
        // will define crease edges in future.
        edges=new _dwedge[nexpected];
        nedges=0;
        if (edges) {
            char buff[256];
            while (nedges<nexpected) {
                if (dwfgets(buff, sizeof( buff ), fp )) {
                    int i1, i2;
                    sscanf(buff,"%d %d", &i1, &i2);
                    edges[nedges].set(i1,i2);
                    nedges++;
                }
            }
        }
        return nedges;
    }
    int readFaces(FILE *fp, const int nexpected)
    { // read up to nexpected faces
        faces=new _face[nexpected];
        char buff[256];
        if (faces) {
            while (nfaces<nexpected) {
                if (dwfgets(buff, sizeof( buff ), fp )) {
                    if (strncmp(buff,"numVerts:",9)==0) {
                        int nv=atoi(buff+9);
                        faces[nfaces].setnv(nv);
                    } else {
                        int idx=atoi(buff);
                        faces[nfaces].addvtx(idx);
                        if (faces[nfaces].complete()) {
                            nfaceverts+=faces[nfaces].getnv();
                            nfaces++;
                        }
                    }
                }
            }
        }
        return nfaces;
    }
    void buildDrawable(Group *grp);  // convert dwobj into osg geosets
    void setcolour(const float rgb[3]) {
        colour[0]=rgb[0]; colour[1]=rgb[1]; colour[2]=rgb[2];
    }
    void reset() {    faces=NULL; verts=NULL;
        nverts=nfaces=nfaceverts=nopens=nedges=0;
    }
    void setmat(dwmaterial *mt) {
        themat=mt;
    }
    int readVerts(FILE *fp, const int nexpected)
    { // read up to nexpected vertices
        int ntot=nverts+nexpected;
        char buff[256];
        Vec3 *oldv=verts; // extend the vertex list
        verts= new Vec3[ntot];
        for (int i=0; i<nverts; i++) verts[i]=oldv[i];
        delete [] oldv;
        while (nverts<ntot) {
            if (dwfgets(buff, sizeof( buff ), fp )) {
                float x,y,z;
                if (verts) {
                    sscanf(buff,"%f %f %f", &x, &y, &z);
                    verts[nverts].set(x,-y,z); // note DW uses a LH coord system (why??)
                }
            }
            nverts++;
        }
        return nverts;
    }
    int addvtx(float x, float y, float z) { // add a single vertex to the object
        oldv=verts; // extend the vertex list
        verts= new Vec3[nverts+1];
        for (int i=0; i<nverts; i++) verts[i]=oldv[i];
        verts[nverts].set(x,y,z); //
        nverts++;
        return nverts-1;
    }
    void settmat(const Matrix *mx) {
        tmat= new Matrix;
        *tmat=*mx;
    }
    void makeuv(Vec2 &uv, const double pos[]) {
        Vec3 p;
        Vec3 txc;
        p.set(pos[0],pos[1],pos[2]);
        txc = (*mx)*p;
        uv[0]=txc[0];
        uv[1]=txc[1];
    }
    inline void setmx(Matrix *m) { mx=m;}
private:
    Vec4 colour;
    Vec3 *verts;
    Vec3 *oldv; // may be deleted after a tesselation
    dwmaterial *themat;
    osg::ushort nverts,nfaces,nedges;
    osg::ushort nfaceverts;
    osg::ushort nopens;
    _face *faces;
    _dwedge *edges;
    int *openings;
    osg::ushort *fc1, *fc2; // openings[i] is in faces[fc1[i]] to faces[fc2[i]]
    Matrix *tmat;
    Matrix *mx; // current uvw transform for currently tessealting face
};

void _face::tesselate(const Vec3 verts[], const dwmaterial *themat, 
               GLUtesselator *ts, _dwobj *dwob, const Matrix *tmat) const {
    int nvall=getallverts();
    int nused=0;
    avertex *poses=new avertex[2*nvall]; // passed to tesselator to redraw
    Matrix mx; // texture matrix transform to plane
    settrans(mx, nrm, verts,themat);
    dwob->setmx(&mx); // may be used by combine callback to define txcoord
    gluTessBeginPolygon(ts, dwob); 
    gluTessBeginContour(ts); /**/
    for (int j=0; j<nv; j++) {
        Vec3 uv;
        /*if (tmat) 
        uv=(*tmat)*verts[idx[j]];
        else */
        uv=mx*verts[idx[j]];
        setposes(poses[nused], j, verts);
        poses[nused].uv[0]=uv[0];
        poses[nused].uv[1]=uv[1];
        poses[nused].themat=themat;
        gluTessVertex(ts, (double *)&(poses[nused]), (double *)(poses+nused));
        nused++;
    }
    gluTessEndContour(ts); 
    for (int k=0; k<nop; k++) { // now holes in the face
        gluTessBeginContour(ts);
        for (int j=0; j<opening[k].nv; j++) {
            Vec3 uv;
            /*if (tmat) uv=(*tmat)*verts[opening[k].idx[j]];
            else */
            uv=mx*verts[opening[k].idx[j]];
            opening[k].setposes(poses[nused], j, verts);
            poses[nused].nrmv*=-1; // get to agree with base polygon
            poses[nused].nrmv=nrm;
            poses[nused].uv[0]=uv[0];
            poses[nused].uv[1]=uv[1];
            poses[nused].themat=themat;
            gluTessVertex(ts, (double *)&(poses[nused]), (double *)(poses+nused));
            nused++;
        }
        gluTessEndContour(ts);/* */
    }
    gluTessEndPolygon(ts);
    delete [] poses;
}
void prims::combine( GLdouble coords[3], avertex *d[4], 
                    GLfloat w[4], avertex **dataOut , _dwobj *dwob) {
    if (nbegin<20 && isGLtype()) { // a prim of type == nvf being created; use this vertex
        avertex *newv = new avertex(); // (avertex *)calloc(1, sizeof(avertex)); 
        newv->pos[0] = coords[0]; 
        newv->pos[1] = coords[1]; 
        newv->pos[2] = coords[2];
        newv->uv[0] = newv->uv[1] =0;
        newv->nrmv[0] = newv->nrmv[1] = newv->nrmv[2] =0;
        for (int i=0; i<4; i++) {
            if (d[i]) {
                newv->uv[0] = w[i]*d[i]->uv[0];
                newv->uv[1] = w[i]*d[i]->uv[1];
                newv->nrmv[0] = w[i]*d[i]->nrmv[0];
                newv->nrmv[1] = w[i]*d[i]->nrmv[1];
                newv->nrmv[2] = w[i]*d[i]->nrmv[2];
                newv->themat=d[i]->themat;
            }
        }
        dwob->makeuv(newv->uv, newv->pos);
        newv->idx=dwob->addvtx(coords[0], coords[1], coords[2]);
        *dataOut = newv;
    } else {
        *dataOut = NULL;
    }
}
void _dwobj::buildDrawable(Group *grp)
{  // current DWobject complete; make a drawable, and add it to a osg::Group
    if (nfaces>0) {
        int nvf;
        int nfnvf=0; // number of vertices for faces plus holes
        int i; // a general counter
        for (i=0; i<nfaces; i++) { // for each face
            faces[i].setnorm(verts); // set its normal and any hole normals
            nfnvf+=faces[i].getallverts(); // get total vertices in object, defines dimensions of NEW arrays
        }
        
        GLUtesselator* ts=gluNewTess();
        gluTessCallback(ts, GLU_TESS_BEGIN, (void (CALLBACK *) ())myFaceBegin);  
        gluTessCallback(ts, GLU_TESS_VERTEX, (GLvoid (CALLBACK *) ())myVertex);  
        gluTessCallback(ts, GLU_TESS_END, (GLvoid (CALLBACK *) ())myFaceEnd);  
        gluTessCallback(ts, GLU_TESS_ERROR, (GLvoid (CALLBACK *) ())error);  
        gluTessCallback(ts, GLU_TESS_COMBINE_DATA, (GLvoid (CALLBACK *) ()) &combineCallback);
        prd.settmat(tmat);
        for (nvf=0; nvf<6; nvf++) { // for each length of face
            prd.setmode(nvf , nfnvf); // filter out only this type of tesselated face
            
            for (i=0; i<nfaces; i++) { // for each face
                prd.tesselate(faces[i],verts, themat, ts, this);
            }
            for (i=0; i<nopens && nvf==3; i++) { // for each hole, join up front & back with Quads
                faces[fc1[i]].link(openings[i*2], &faces[fc2[i]],openings[i*2+1],verts, themat);
            }
            prd.buildDrawable(grp, verts, themat, nverts);
            if (oldv) delete [] oldv;
            oldv=NULL;                
        }
        gluDeleteTess(ts);
    } // primitive types
}
////////// tesselation complete

// not currently used so commenting out, RO July 2001.
// static const char *keywords[] = { 
//     "numMaterials:","Material:","MaterialType:","TextureWidthAndHeight:","PictureFile:",
//     "Extrusion:","Cube:","Mat:", "Color:","numVerts:",
//     "numFaces:","numEdges:","numOpenings:", "Opacity:", "SpecularReflectivity:", 
//     "SmoothnessExponent:", "UVW:", "Brightness:", "Attentuation:","FallOffInnerHalfAngle:",
//     "OuterHalfAngle:","Brightness:","Attentuation:", "Phase:", "CurrPhase:", "numPhases:"
// };

class ReaderWriterDW : public osgDB::ReaderWriter
{
    public:
        virtual const char* className() { return "Design Workshop Database Reader"; }

        virtual bool acceptsExtension(const std::string& extension)
        {
            return osgDB::equalCaseInsensitive(extension,"dw");
        }

        virtual ReadResult readNode(const std::string& fileName,const osgDB::ReaderWriter::Options*)
        {   
        _dwobj obj;
        enum reading {NONE, MATERIAL, OBJECT};
        //osg::ushort nrecs=0; // number of records read after a divider (numVerts, numFaces, numOpenings...)
        int nexpected=0; // number of records to be read in a block
        dwmaterial *matpalet=NULL;
        int nmat=-1; // current element of matpalet being modified
        int nmn=0; // number of materials found
        reading rdg=NONE;

            std::string ext = osgDB::getFileExtension(fileName);
            if (!acceptsExtension(ext)) return ReadResult::FILE_NOT_HANDLED;

            char buff[256];

            notify(INFO)<<   "ReaderWriterDW::readNode( "<<fileName.c_str()<<" )\n";

            FILE *fp;

            if( (fp = fopen( fileName.c_str(), "r" )) == (FILE *)0L )
            {
                return std::string("Unable to open file \""+fileName+"\"");
            }
            Group *grp = new Group;

            while( !feof( fp ) )
            { // reads the Design Workshop format in ASCII
                if (dwfgets( buff, sizeof( buff ), fp )) {
                    if( buff[0] == '#' )
                        continue;
                    else if( strncmp(buff,"numMaterials:",13)==0) { // No of materials
                        nmn=atoi(buff+14);
                        matpalet=new dwmaterial[nmn];
                    } else if( strncmp(buff,"Material:",9)==0) { // No of materials
                        dwfgets(buff, sizeof( buff ), fp );
                        nmat++;
                        rdg=MATERIAL;
                        int id=atoi(buff);
                        matpalet[nmat].setid(id); // current material to be modified
                        
                    } else if (strncmp(buff, "Phase:",6)==0) {
                    } else if (strncmp(buff, "CurrPhase:",10)==0) {
                    } else if (strncmp(buff, "numPhases:",10)==0) {
                    } else if (strncmp(buff, "Opacity:",8)==0) {
                        float opacity=atof(buff+8);
                        matpalet[nmat].setopacity(opacity);
                    } else if (strncmp(buff, "FallOff:",8)==0) {
                        float fo=atof(buff+8);
                        matpalet[nmat].setFallOff(fo);
                    } else if (strncmp(buff, "InnerHalfAngle:",15)==0) {
                        float ha=atof(buff+15);
                        matpalet[nmat].setHalfAngleIn(ha);
                    } else if (strncmp(buff, "OuterHalfAngle:",15)==0) {
                        float ha=atof(buff+15);
                        matpalet[nmat].setHalfAngleOut(ha);
                    } else if (strncmp(buff, "Brightness:",11)==0) {
                        float br=atof(buff+11);
                        matpalet[nmat].setBright(br);
                    } else if (strncmp(buff, "Attentuation:",13)==0) { // oops - they can't spell
                        matpalet[nmat].setAtten(buff+13);
                    } else if (strncmp(buff, "MaterialType:",13)==0) {
                        matpalet[nmat].setType(buff+14);
                    } else if (strncmp(buff, "SpecularReflectivity:",21)==0) {
                        float spec=atof(buff+21);
                        matpalet[nmat].setspecular(spec);
                    } else if (strncmp(buff, "SmoothnessExponent:",19)==0) {
                        float spec=atof(buff+19);
                        matpalet[nmat].setspecexp(spec/100.0f); // convert to 0-1 from percent
                    } else if (strncmp(buff, "TextureWidthAndHeight:",22)==0) {
                        char *ct=strchr(buff+22,',');
                        float repx=atof(buff+23), repy=atof(ct+1);
                        matpalet[nmat].settxrep(repx, repy);
                    } else if (strncmp(buff, "PictureFile:",12)==0) {
                        char *end=strchr(buff,'\n'); // end of line
                        if (end) *end='\0'; // removed
                        matpalet[nmat].setfname(buff);
                    } else if( strncmp(buff,"Extrusion:",10)==0 ||
                        strncmp(buff,"Cube:",5)==0 || 
                        strncmp(buff,"Polyline:",9)==0 || 
                        strncmp(buff,"Polyhedron:",11)==0) {
                        rdg=OBJECT;
                        obj.buildDrawable(grp);
                        obj.reset();
                    } else if( strncmp(buff,"Mat:",4)==0) {
                        int mt=atoi(buff+4);
                        for (int j=0; j<nmn; j++) {
                            if (matpalet[j].getid() == mt) 
                                obj.setmat(&(matpalet[j]));
                        }
                    } else if( strncmp(buff,"Color:",6)==0) {
                        float rgb[3];
                        if (rdg==MATERIAL) { // get material colour
                            sscanf(buff+6,"%f %f %f", &rgb[0], &rgb[1], &rgb[2]);
                            matpalet[nmat].setcolour(rgb);
                        } else if (rdg==OBJECT) {
                            float rgb[3];
                            sscanf(buff+6,"%f %f %f", &rgb[0], &rgb[1], &rgb[2]);
                            rgb[0]/=65536.0f; // convert to range 0-1
                            rgb[1]/=65536.0f; // convert to range 0-1
                            rgb[2]/=65536.0f; // convert to range 0-1
                            obj.setcolour(rgb);
                        }
                    } else if( strncmp(buff,"numVerts:",9)==0) {
                        nexpected=atoi(buff+9);
                        obj.readVerts(fp, nexpected);
                        nexpected=0;
                    } else if( strncmp(buff,"numFaces:",9)==0) {
                        nexpected=atoi(buff+9);
                        obj.readFaces(fp, nexpected);
                        nexpected=0;
                    } else if( strncmp(buff,"numEdges:",9)==0) {
                        nexpected=atoi(buff+9);
                        obj.readEdges(fp, nexpected);
                        //nrecs=0; // a numVerts is followed by nv vetex postiions
                    } else if( strncmp(buff,"numOpenings:",12)==0) {
                        nexpected=atoi(buff+12);
                        //nrecs=0; // a numVerts is followed by nv vetex postiions
                        if (nexpected>0) obj.readOpenings(fp, nexpected);
                    } else if( strncmp(buff,"UVW:",4)==0) { // texture application matrix
                        Matrix mx;
                        sscanf(buff+4,"%f %f %f%f %f %f%f %f %f", 
                            &mx(0,0), &mx(0,1), &mx(0,2),
                            &mx(1,0), &mx(1,1), &mx(1,2),
                            &mx(2,0), &mx(2,1), &mx(2,2));
                        obj.settmat(&mx);
                    }
                }

            }
            fclose( fp );
            obj.buildDrawable(grp); // tidy up any remaining objects

            return grp;

        }
private:
};

int dwfgets(char *clin, int max, FILE *fin)
{ // replace fgets to detect EOL = char 13 as well as Creturn=10 GWM 111100
    // Macintosh produced files (such as those obtainable
    //from the great buildings site at www.Artifice.com) use 13 format, PC models use 10.
    int nread=0;
    char c1=1;
    do {
        if (!feof( fin )) {
            clin[nread]=c1=fgetc(fin);
            nread++;
        }
    } while (nread<max && c1!= 13 && c1!= 10 &&  feof( fin )== 0 );
    if (nread>0) clin[nread-1]='\0'; // null terminate and remove training blank
    return nread;
}

// now register with osg::Registry to instantiate the above
// reader/writer.
osgDB::RegisterReaderWriterProxy<ReaderWriterDW> g_readerWriter_DW_Proxy;

