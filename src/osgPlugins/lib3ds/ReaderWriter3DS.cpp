#include <osg/Notify>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture>
#include <osg/Material>
#include <osg/TexEnv>
#include <osg/ref_ptr>
#include <osg/MatrixTransform>

#include <osgDB/Registry>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osgDB/ReadFile>

//MIKEC debug only for PrintVisitor
#include <osg/NodeVisitor>


#include <file.h>
#include <mesh.h>
#include <material.h>
#include <vector.h>
#include <matrix.h>
#include <node.h>
#include <quat.h>

#include <stdlib.h>
#include <string.h>

#include <set>
#include <map>

using namespace std;
using namespace osg;

class PrintVisitor : public NodeVisitor
{

   public:
   
        PrintVisitor():NodeVisitor(NodeVisitor::TRAVERSE_ALL_CHILDREN)
        {
            _indent = 0;
            _step = 4;
        }
        
        inline void moveIn() { _indent += _step; }
        inline void moveOut() { _indent -= _step; }
        inline void writeIndent() 
        {
            for(int i=0;i<_indent;++i) std::cout << " ";
        }
                
        virtual void apply(Node& node)
        {
            moveIn();
            writeIndent(); std::cout << node.className() << " name=" << node.getName() << std::endl;
            traverse(node);
            moveOut();
        }

        virtual void apply(Geode& node)         { apply((Node&)node); }
        virtual void apply(Billboard& node)     { apply((Geode&)node); }
        virtual void apply(LightSource& node)   { apply((Node&)node); }
        
        virtual void apply(Group& node)         { apply((Node&)node); }
        virtual void apply(Transform& node)     { apply((Group&)node); }
        virtual void apply(Switch& node)        { apply((Group&)node); }
        virtual void apply(LOD& node)           { apply((Group&)node); }
        virtual void apply(Impostor& node)      { apply((LOD&)node); }

   protected:
    
        int _indent;
        int _step;
};

typedef std::map<std::string,osg::StateSet*> StateSetMap;
class ReaderWriter3DS : public osgDB::ReaderWriter
{
    public:

        ReaderWriter3DS();

        virtual const char* className() { return "3DS Auto Studio Reader"; }
        virtual bool acceptsExtension(const std::string& extension) { return extension=="3ds"; }

        virtual ReadResult readNode(const std::string& fileName, const osgDB::ReaderWriter::Options*);

        typedef std::vector<int> FaceList;
        typedef std::map<std::string,osg::StateSet*> GeoStateMap;

    protected:

        osg::Texture*  createTexture(Lib3dsTextureMap *texture,const char* label,bool& transparancy);
        osg::StateSet* createStateSet(Lib3dsMaterial *materials);
        osg::Drawable* createDrawable(Lib3dsMesh *meshes,FaceList& faceList, Lib3dsMatrix* matrix);

        std::string _directory;
        bool _useSmoothingGroups;
        bool _usePerVertexNormals;

        // MIKEC
        osg::Node* processMesh(StateSetMap& drawStateMap,osg::Group* parent,Lib3dsMesh* mesh, Lib3dsMatrix* matrix);
        osg::Node* processNode(StateSetMap drawStateMap,Lib3dsFile *f,Lib3dsNode *node);

};

// now register with Registry to instantiate the above
// reader/writer.
osgDB::RegisterReaderWriterProxy<ReaderWriter3DS> g_readerWriter_3DS_Proxy;

ReaderWriter3DS::ReaderWriter3DS()
{
    _useSmoothingGroups = true;
    _usePerVertexNormals = true;
}


/**
    These print methods for 3ds hacking
*/
void pad(int level) {
    for(int i=0;i<level;i++) cout<<"  ";
}
void print(Lib3dsMesh *mesh,int level);
void print(Lib3dsUserData *user,int level);
void print(Lib3dsNodeData *user,int level);
void print(Lib3dsObjectData *object,int level);
void print(Lib3dsNode *node, int level);

void print(Lib3dsMatrix matrix,int level) {
    pad(level); cout << matrix[0][0] <<" "<< matrix[0][1] <<" "<< matrix[0][2] <<" "<< matrix[0][3] << endl;
    pad(level); cout << matrix[1][0] <<" "<< matrix[1][1] <<" "<< matrix[1][2] <<" "<< matrix[1][3] << endl;
    pad(level); cout << matrix[2][0] <<" "<< matrix[2][1] <<" "<< matrix[2][2] <<" "<< matrix[2][3] << endl;
    pad(level); cout << matrix[3][0] <<" "<< matrix[3][1] <<" "<< matrix[3][2] <<" "<< matrix[3][3] << endl;
}
void print(Lib3dsMesh *mesh,int level) {
    if (mesh) {
        pad(level); cout << "mesh name " << mesh->name  << endl;
        print(mesh->matrix,level);
    } else {
        pad(level); cout << "no mesh " << endl;
    }
}
void print(Lib3dsUserData *user,int level) {
    if (user) {
        pad(level); cout << "user data" << endl;
        //print(user->mesh,level+1);
    } else {
        pad(level); cout << "no user data" << endl;
    }
}
void print(Lib3dsNodeData *node,int level) {
    if (node) {
        pad(level); cout << "node data:" << endl;
        // nodedata->object is a union of many types
        print((Lib3dsObjectData *)&node->object,level+1);
    } else {
        pad(level); cout << "no user data" << endl;
    }
}
void print(Lib3dsObjectData *object,int level) {
    if (object) {
        pad(level); cout << "objectdata instance [" << object->instance << "]" << endl;
        pad(level); cout << "pivot     " << object->pivot[0] <<" "<< object->pivot[1] <<" "<< object->pivot[2] << endl;
        pad(level); cout << "pos       " << object->pos[0] <<" "<< object->pos[1] <<" "<< object->pos[2] << endl;
        pad(level); cout << "scl       " << object->scl[0] <<" "<< object->scl[1] <<" "<< object->scl[2] << endl;
        pad(level); cout << "rot       " << object->rot[0] <<" "<< object->rot[1] <<" "<< object->rot[2] <<" "<< object->rot[3] << endl;
    } else {
        pad(level); cout << "no object data" << endl;
    }
}

void print(Lib3dsNode *node, int level) {
    
    pad(level); cout << "node name [" << node->name << "]" << endl;
    pad(level); cout << "node id    " << node->node_id << endl;
    pad(level); cout << "node parent id " << node->parent_id << endl;
    pad(level); cout << "node matrix:" << endl;
    print(node->matrix,level+1);
    print(&node->data,level);
    print(&node->user,level);
    

    for(Lib3dsNode *child=node->childs; child; child=child->next) {
        print(child,level+1);
    }

}

// Transforms points by matrix if 'matrix' is not NULL
// Creates a Geode and Geometry (as parent,child) and adds the Geode to 'parent' parameter iff 'parent' is non-NULL
// Returns ptr to the Geode
osg::Node* ReaderWriter3DS::processMesh(StateSetMap& drawStateMap,osg::Group* parent,Lib3dsMesh* mesh, Lib3dsMatrix* matrix) {
    typedef std::vector<int> FaceList;
    typedef std::map<std::string,FaceList> MaterialFaceMap;
    MaterialFaceMap materialFaceMap;
    for (unsigned int i=0; i<mesh->faces; ++i)
    {
        materialFaceMap[mesh->faceL[i].material].push_back(i);
    }

    if (materialFaceMap.empty())
    {
        osg::notify(osg::NOTICE)<<"Warning : no triangles assigned to mesh '"<<mesh->name<<"'"<< std::endl;
        return NULL;
    }
    else
    {

        osg::Geode* geode = new osg::Geode;
        geode->setName(mesh->name);

        for(MaterialFaceMap::iterator itr=materialFaceMap.begin();
            itr!=materialFaceMap.end();
            ++itr)
        {
            FaceList& faceList = itr->second;
            
            if (_useSmoothingGroups)
            {

                typedef std::map<int,FaceList> SmoothingFaceMap;
                SmoothingFaceMap smoothingFaceMap;
                for (FaceList::iterator flitr=faceList.begin();
                     flitr!=faceList.end();
                     ++flitr)
                {
                    smoothingFaceMap[mesh->faceL[*flitr].smoothing].push_back(*flitr);
                }

                for(SmoothingFaceMap::iterator sitr=smoothingFaceMap.begin();
                    sitr!=smoothingFaceMap.end();
                    ++sitr)
                {
                    // each smoothing group to have its own geom 
                    // to ensure the vertices on adjacent groups
                    // don't get shared.
                    FaceList& smoothFaceMap = sitr->second;

                    osg::Drawable* drawable = createDrawable(mesh,smoothFaceMap,matrix);
                    if (drawable)
                    {
                        drawable->setStateSet(drawStateMap[itr->first]);
                        geode->addDrawable(drawable);
                    }
                }
            }
            else // ignore smoothing groups.
            {
                osg::Drawable* drawable = createDrawable(mesh,faceList,matrix);
                if (drawable)
                {
                    drawable->setStateSet(drawStateMap[itr->first]);
                    geode->addDrawable(drawable);
                }
            }
        }

        if (parent) parent->addChild(geode);
        return geode;
    }
}


/**
How to cope with pivot points in 3ds (short version)

  All object coordinates in 3ds are stored in world space, this is why you can just rip out the meshes and use/draw them without meddeling further
  Unfortunately, this gets a bit wonky with objects with pivot points (conjecture: PP support is retro fitted into the .3ds format and so doesn't fit perfectly?)

  Objects with pivot points have a position relative to their PP, so they have to undergo this transform:

    invert the mesh matrix, apply this matrix to the object. This puts the object back at the origin
    Transform the object by the nodes (nnegative) pivot point coords, this puts the PP at the origin
    Tranform the node by the node matrix, which does the orientation about the pivot point, (and currently) transforms the object back by a translation to the PP.

  */
osg::Node* ReaderWriter3DS::processNode(StateSetMap drawStateMap,Lib3dsFile *f,Lib3dsNode *node) {
    
    osg::Group* group=NULL;// created on demand if we find we have children to group together
    

    // Handle all children of this node for hierarchical assemblies
    Lib3dsNode *p;
    for (p=node->childs; p!=0; p=p->next) {
        if (!group) {
            group =new osg::Group;
            group->setName(node->name);
        }
        group->addChild(processNode(drawStateMap,f,p));
    }
    
    // MIKEC: possible BUG - 3ds files APPEAR to enforce unqiue names, so this is OK, but I am not 100% sure
    // failed to find any alternative way to do it in lib3ds though, and lib3ds player.c application uses this method
    Lib3dsMesh *mesh=lib3ds_file_mesh_by_name(f,node->name);
    if (mesh) {
        Lib3dsObjectData* object=&node->data.object;
        Lib3dsMatrix mesh_inverse;
        osg::Matrix osgmatrix;
        
        lib3ds_matrix_copy(mesh_inverse,mesh->matrix); 
        lib3ds_matrix_inv(mesh_inverse);
        
        Lib3dsMatrix M,N;
        lib3ds_matrix_identity(M);
        lib3ds_matrix_identity(N);
        lib3ds_matrix_copy(M,node->matrix);
        N[3][0]=-object->pivot[0];
        N[3][1]=-object->pivot[1];
        N[3][2]=-object->pivot[2];

        bool pivoted=false;
        if ( (object->pivot[0]!=0.0) || (object->pivot[1]!=0.0) || (object->pivot[2]!=0.0) ) {
            pivoted=true; // there is a pivot point, so we must use it
        }

        /*cout<<"M"<<node->name<<endl;
        print(M,0);
        cout<<"N"<<endl;
        print(N,0);*/

        if (pivoted) {
            // Transform object's pivot point to the world origin
            osg::MatrixTransform* T=new osg::MatrixTransform;
            osgmatrix.set(
                N[0][0],N[0][1],N[0][2],N[0][3],
                N[1][0],N[1][1],N[1][2],N[1][3],
                N[2][0],N[2][1],N[2][2],N[2][3],
                N[3][0],N[3][1],N[3][2],N[3][3]); 
            T->setMatrix(osgmatrix);
            T->setName("3DSPIVOTPOINT: Translate pivotpoint to (world) origin");
            //cout<<"Translation for "<<node->name<<" is "<<osgmatrix<<endl;

            // rotate about "origin" (after the transform this is the world origin)
            // BUG this matrix also contains the translation to the pivot point - we should plit that out (maybe)
            osg::MatrixTransform* R=new osg::MatrixTransform;
            osgmatrix.set(
                M[0][0],M[0][1],M[0][2],M[0][3],
                M[1][0],M[1][1],M[1][2],M[1][3],
                M[2][0],M[2][1],M[2][2],M[2][3],
                M[3][0],M[3][1],M[3][2],M[3][3]); 
            R->setMatrix(osgmatrix);
            R->setName("3DSPIVOTPOINT: Rotate");
                
            /*
            cout<<"Rotation for "<<node->name<<" is "<<osgmatrix<<endl;
            osg::Quat quat;
            quat.set(osgmatrix);
            osg::Vec3 axis;
            float angle;
            quat.getRotate(angle,axis);
            cout<<"which is "<<osg::RadiansToDegrees(angle)<<" degrees around "<<axis<<endl;
            */
            /*
            printf("%s---------------\n",node->name);
            printf("mesh matrix :\n");         print(mesh->matrix,1);
            printf("mesh inverse:\n");         print(mesh_inverse,1);
            printf("node matrix :\n");         print(matrix,1);
            printf("pivot=%f,%f,%f pos=%f,%f,%f\n",object->pivot[0],object->pivot[1],object->pivot[2],object->pos[0],object->pos[1],object->pos[2]);
            */

            if (group) {
                // Always in reverse order...
                group->addChild(R); 
                R->addChild(T);
                processMesh(drawStateMap,T,mesh,&mesh_inverse); // creates geometry under modifier node
                return group;
            } else {
                // We are a pivoted node with no children
                R->addChild(T);
                processMesh(drawStateMap,T,mesh,&mesh_inverse); // creates geometry under modifier node
                return R;
            }
        } else {
            if(group) {
                // add our geometry to group (where our children already are)
                processMesh(drawStateMap,group,mesh,NULL); // creates geometry under modifier node
                return group;
            } else {
                // didnt use group for children
                // return a ptr directly to the Geode for this mesh
                return processMesh(drawStateMap,NULL,mesh,NULL); 
            }    
        }

    } else {
        // no mesh for this node - probably a camera or something of that persuasion
        //cout << "no mesh for object " << node->name << endl;
        return group; // we have no mesh, but we might have children
    }
}


osgDB::ReaderWriter::ReadResult ReaderWriter3DS::readNode(const std::string& fileName, const osgDB::ReaderWriter::Options*)
{

    Lib3dsFile *f = lib3ds_file_load(fileName.c_str());
    if (f==NULL) return ReadResult::FILE_NOT_HANDLED;

    // MIKEC
    // This appears to build the matrix structures for the 3ds structure
    // It wasn't previously necessary because all the meshes are stored in world coordinates
    // but is VERY necessary if you want to use pivot points...
    lib3ds_file_eval(f,0.0f); // second param is time 't' for animated files

    _directory = osgDB::getFilePath(fileName);

    osg::Group* group = new osg::Group;
    group->setName(fileName);

    StateSetMap drawStateMap;

    for (Lib3dsMaterial *mat=f->materials; mat; mat=mat->next)
    {
        drawStateMap[mat->name] = createStateSet(mat);
    }
    
    /*{
        int level=0;
        std::cout << "NODE TRAVERSAL of file "<<f->name<<std::endl;
        for(Lib3dsNode *node=f->nodes; node; node=node->next) {
            print(node,level+1);
        }
        std::cout << "MESH TRAVERSAL of file "<<f->name<<std::endl;
        for(Lib3dsMesh *mesh=f->meshes; mesh; mesh=mesh->next) {
            print(mesh,level+1);
        }
    }*/


    // We can traverse by meshes (old method, broken for pivot points, but otherwise works), or by nodes (new method, not so well tested yet)
    // if your model is broken, especially wrt object positions try setting this flag. If that fixes it,
    // send me the model
    bool traverse_nodes=false;
    
    // MIKEC: have found 3ds files with NO node structure - only meshes, for this case we fall back to the old traverse-by-meshes code
    // Loading and re-exporting these files from 3DS produces a file with correct node structure, so perhaps these are not 100% conformant?
    if (f->nodes == NULL) {
        osg::notify(osg::WARN)<<"Warning: in 3ds loader: file has no nodes, traversing by meshes instead"<< std::endl;
        traverse_nodes=true;
    }

    if (traverse_nodes) { // old method
        for (Lib3dsMesh *mesh=f->meshes; mesh; mesh=mesh->next) {
            processMesh(drawStateMap,group,mesh,NULL);
        }
    } else { // new method
        for(Lib3dsNode *node=f->nodes; node; node=node->next) {
            group->addChild(processNode(drawStateMap,f,node));
        }
    } 

    
    /*cout << "Final OSG node structure looks like this:"<< endl;
    PrintVisitor pv;
    group->accept(pv);*/
    
    
    lib3ds_file_free(f);

    return group;
}

/**
use matrix to pretransform geometry, or NULL to do nothing
*/
osg::Drawable*   ReaderWriter3DS::createDrawable(Lib3dsMesh *m,FaceList& faceList,Lib3dsMatrix *matrix)
{

    osg::Geometry* geom = new osg::Geometry;

    unsigned int i;
    
    std::vector<int> orig2NewMapping;
    for(i=0;i<m->points;++i) orig2NewMapping.push_back(-1);

    unsigned int noVertex=0;
    FaceList::iterator fitr;
    for (fitr=faceList.begin();
        fitr!=faceList.end();
        ++fitr)
    {

        Lib3dsFace& face = m->faceL[*fitr];

        if (orig2NewMapping[face.points[0]]<0)
            orig2NewMapping[face.points[0]] = noVertex++;

        if (orig2NewMapping[face.points[1]]<0)
            orig2NewMapping[face.points[1]] = noVertex++;

        if (orig2NewMapping[face.points[2]]<0)
            orig2NewMapping[face.points[2]] = noVertex++;

    }

    // create vertices.
    osg::Vec3Array* osg_coords = new osg::Vec3Array(noVertex);
    geom->setVertexArray(osg_coords);

    Lib3dsVector c;
       
    for (i=0; i<m->points; ++i)
    {
        if (orig2NewMapping[i]>=0)
        {
            if (matrix)
            {
                lib3ds_vector_transform(c,*matrix, m->pointL[i].pos);
                (*osg_coords)[orig2NewMapping[i]].set(c[0],c[1],c[2]);
            }
            else
            {
                // original no transform code.
                (*osg_coords)[orig2NewMapping[i]].set(m->pointL[i].pos[0],m->pointL[i].pos[1],m->pointL[i].pos[2]);
            }
        }
    }

    // create texture coords if needed.
    if (m->texels>0)
    {
        if (m->texels==m->points)
        {
            osg::Vec2Array* osg_tcoords = new osg::Vec2Array(noVertex);
            geom->setTexCoordArray(0,osg_tcoords);
            for (i=0; i<m->texels; ++i)
            {
                if (orig2NewMapping[i]>=0) (*osg_tcoords)[orig2NewMapping[i]].set(m->texelL[i][0],m->texelL[i][1]);
            }
        }
        else
        {
            osg::notify(osg::WARN)<<"Warning: in 3ds loader m->texels ("<<m->texels<<") != m->points ("<<m->points<<")"<< std::endl;
        }
    }

    // create normals.
    if (_usePerVertexNormals)
    {
        osg::Vec3Array* osg_normals=osg_normals = new osg::Vec3Array(noVertex);
        
        // initialize normal list to zero's.
        for (i=0; i<noVertex; ++i)
        {
            (*osg_normals)[i].set(0.0f,0.0f,0.0f);
        }

        for (fitr=faceList.begin();
            fitr!=faceList.end();
            ++fitr)
        {
            Lib3dsFace& face = m->faceL[*fitr];

            (*osg_normals)[orig2NewMapping[face.points[0]]] += osg::Vec3(face.normal[0],face.normal[1],face.normal[2]);;
            (*osg_normals)[orig2NewMapping[face.points[1]]] += osg::Vec3(face.normal[0],face.normal[1],face.normal[2]);;
            (*osg_normals)[orig2NewMapping[face.points[2]]] += osg::Vec3(face.normal[0],face.normal[1],face.normal[2]);;

        }

        // normalize the normal list to unit length normals.
        for (i=0; i<noVertex; ++i)
        {
            (*osg_normals)[i].normalize();
        }

        geom->setNormalArray(osg_normals);
        geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

    }
    else 
    {
        osg::Vec3Array* osg_normals = new osg::Vec3Array(faceList.size());
        osg::Vec3Array::iterator normal_itr = osg_normals->begin();
        for (fitr=faceList.begin();
            fitr!=faceList.end();
            ++fitr)
        {
            Lib3dsFace& face = m->faceL[*fitr];
            *(normal_itr++) =  osg::Vec3(face.normal[0],face.normal[1],face.normal[2]);
        }
        geom->setNormalArray(osg_normals);
        geom->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);
    }
    
    
    // create primitives
    int numIndices = faceList.size()*3;
    UShortDrawElements* elements = new osg::UShortDrawElements(osg::Primitive::TRIANGLES,numIndices);
    UShortDrawElements::iterator index_itr = elements->begin();

    for (fitr=faceList.begin();
        fitr!=faceList.end();
        ++fitr)
    {
        Lib3dsFace& face = m->faceL[*fitr];

        *(index_itr++) = orig2NewMapping[face.points[0]];
        *(index_itr++) = orig2NewMapping[face.points[1]];
        *(index_itr++) = orig2NewMapping[face.points[2]];
    }
   
    geom->addPrimitive(elements);

    return geom;
}


osg::Texture*  ReaderWriter3DS::createTexture(Lib3dsTextureMap *texture,const char* label,bool& transparancy)
{
    if (texture && *(texture->name))
    {
        std::string fileName = osgDB::findFileInDirectory(texture->name,_directory,true);
        if (fileName.empty())
        {
            osg::notify(osg::WARN) << "texture '"<<texture->name<<"' not found"<< std::endl;
            return NULL;
        }

        if (label) osg::notify(osg::DEBUG_INFO) << label;
        else osg::notify(osg::DEBUG_INFO) << "texture name";
        osg::notify(osg::DEBUG_INFO) << " '"<<texture->name<<"'"<< std::endl;
        osg::notify(osg::DEBUG_INFO) << "    texture flag        "<<texture->flags<< std::endl;
        osg::notify(osg::DEBUG_INFO) << "    LIB3DS_DECALE       "<<((texture->flags)&LIB3DS_DECALE)<< std::endl;
        osg::notify(osg::DEBUG_INFO) << "    LIB3DS_MIRROR       "<<((texture->flags)&LIB3DS_MIRROR)<< std::endl;
        osg::notify(osg::DEBUG_INFO) << "    LIB3DS_NEGATE       "<<((texture->flags)&LIB3DS_NEGATE)<< std::endl;
        osg::notify(osg::DEBUG_INFO) << "    LIB3DS_NO_TILE      "<<((texture->flags)&LIB3DS_NO_TILE)<< std::endl;
        osg::notify(osg::DEBUG_INFO) << "    LIB3DS_SUMMED_AREA  "<<((texture->flags)&LIB3DS_SUMMED_AREA)<< std::endl;
        osg::notify(osg::DEBUG_INFO) << "    LIB3DS_ALPHA_SOURCE "<<((texture->flags)&LIB3DS_ALPHA_SOURCE)<< std::endl;
        osg::notify(osg::DEBUG_INFO) << "    LIB3DS_TINT         "<<((texture->flags)&LIB3DS_TINT)<< std::endl;
        osg::notify(osg::DEBUG_INFO) << "    LIB3DS_IGNORE_ALPHA "<<((texture->flags)&LIB3DS_IGNORE_ALPHA)<< std::endl;
        osg::notify(osg::DEBUG_INFO) << "    LIB3DS_RGB_TINT     "<<((texture->flags)&LIB3DS_RGB_TINT)<< std::endl;

        osg::Image* osg_image = osgDB::readImageFile(fileName.c_str());
        if (osg_image==NULL)
        {
            osg::notify(osg::NOTICE) << "Warning: Cannot create texture "<<texture->name<< std::endl;
            return NULL;
        }

        osg::Texture* osg_texture = new osg::Texture;
        osg_texture->setImage(osg_image);

        // does the texture support transparancy?
        transparancy = ((texture->flags)&LIB3DS_ALPHA_SOURCE)!=0;

        // what is the wrap mode of the texture.
        osg::Texture::WrapMode wm = ((texture->flags)&LIB3DS_NO_TILE) ?
        osg::Texture::CLAMP : wm=osg::Texture::REPEAT;
        osg_texture->setWrap(osg::Texture::WRAP_S,wm);
        osg_texture->setWrap(osg::Texture::WRAP_T,wm);
        osg_texture->setWrap(osg::Texture::WRAP_R,wm);
                                 // bilinear.
        osg_texture->setFilter(osg::Texture::MIN_FILTER,osg::Texture::LINEAR_MIPMAP_NEAREST);

        return osg_texture;
    }
    else
        return NULL;
}


osg::StateSet* ReaderWriter3DS::createStateSet(Lib3dsMaterial *mat)
{
    if (mat==NULL) return NULL;

    osg::StateSet* stateset = new osg::StateSet;

    osg::Material* material = new osg::Material;

    float transparency = mat->transparency;
    float alpha = 1.0f-transparency;

    osg::Vec4 ambient(mat->ambient[0],mat->ambient[1],mat->ambient[2],alpha);
    osg::Vec4 diffuse(mat->diffuse[0],mat->diffuse[1],mat->diffuse[2],alpha);
    osg::Vec4 specular(mat->specular[0],mat->specular[1],mat->specular[2],alpha);

    float shininess = mat->shininess;

    material->setAmbient(osg::Material::FRONT_AND_BACK,ambient);
    material->setDiffuse(osg::Material::FRONT_AND_BACK,diffuse);
    material->setSpecular(osg::Material::FRONT_AND_BACK,specular);
    material->setShininess(osg::Material::FRONT_AND_BACK,shininess);

    stateset->setAttribute(material);

    bool decal = true;
    bool textureTransparancy=false;
    osg::Texture* texture1_map = createTexture(&(mat->texture1_map),"texture1_map",textureTransparancy);
    if (texture1_map)
    {
        stateset->setTextureAttributeAndModes(0,texture1_map,osg::StateAttribute::ON);
        
        // not sure exactly how to interpret what is best for .3ds
        // but the default text env MODULATE doesn't work well, and
        // DECAL seems to work better.
        osg::TexEnv* texenv = new osg::TexEnv;
        if (decal)
        {
            texenv->setMode(osg::TexEnv::DECAL);
        }
        else
        {
            texenv->setMode(osg::TexEnv::MODULATE);
        }

        stateset->setTextureAttribute(0,texenv);
    }

    if (transparency>0.0f || textureTransparancy)
    {
        stateset->setMode(GL_BLEND,osg::StateAttribute::ON);
        stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    }

/*
    osg::ref_ptr<osg::Texture> texture1_mask = createTexture(&(mat->texture1_mask),"texture1_mask",textureTransparancy);
    osg::ref_ptr<osg::Texture> texture2_map = createTexture(&(mat->texture2_map),"texture2_map",textureTransparancy);
    osg::ref_ptr<osg::Texture> texture2_mask = createTexture(&(mat->texture2_mask),"texture2_mask",textureTransparancy);
    osg::ref_ptr<osg::Texture> opacity_map = createTexture(&(mat->opacity_map),"opacity_map",textureTransparancy);
    osg::ref_ptr<osg::Texture> opacity_mask = createTexture(&(mat->opacity_mask),"opacity_mask",textureTransparancy);
    osg::ref_ptr<osg::Texture> bump_map = createTexture(&(mat->bump_map),"bump_map",textureTransparancy);
    osg::ref_ptr<osg::Texture> bump_mask = createTexture(&(mat->bump_mask),"bump_mask",textureTransparancy);
    osg::ref_ptr<osg::Texture> specular_map = createTexture(&(mat->specular_map),"specular_map",textureTransparancy);
    osg::ref_ptr<osg::Texture> specular_mask = createTexture(&(mat->specular_mask),"specular_mask",textureTransparancy);
    osg::ref_ptr<osg::Texture> shininess_map = createTexture(&(mat->shininess_map),"shininess_map",textureTransparancy);
    osg::ref_ptr<osg::Texture> shininess_mask = createTexture(&(mat->shininess_mask),"shininess_mask",textureTransparancy);
    osg::ref_ptr<osg::Texture> self_illum_map = createTexture(&(mat->self_illum_map),"self_illum_map",textureTransparancy);
    osg::ref_ptr<osg::Texture> self_illum_mask = createTexture(&(mat->self_illum_mask),"self_illum_mask",textureTransparancy);
    osg::ref_ptr<osg::Texture> reflection_map = createTexture(&(mat->reflection_map),"reflection_map",textureTransparancy);
    osg::ref_ptr<osg::Texture> reflection_mask = createTexture(&(mat->reflection_mask),"reflection_mask",textureTransparancy);
*/
    return stateset;
}

