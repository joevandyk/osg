#ifndef _CONVERTFROMINVENTOR_H_
#define _CONVERTFROMINVENTOR_H_

#include <osg/Group>
#include <osg/Geometry>
#include <osg/PrimitiveSet>
#include <osg/Texture2D>
#include <osg/Light>
#include <Inventor/actions/SoCallbackAction.h>
#include <vector>
#include <stack>

class ConvertFromInventor
{
    public:
        ConvertFromInventor();
        ~ConvertFromInventor();

        osg::Node* convert(SoNode* rootIVNode);

    private:

        // Callback functions for converting inventor scene graph to osg 
        // scene graph

        static SoCallbackAction::Response preShape(void* data,
                                 SoCallbackAction* action, const SoNode* node);
        static SoCallbackAction::Response postShape(void* data,
                                 SoCallbackAction* action, const SoNode* node);
        static SoCallbackAction::Response preGroup(void* data,
                                 SoCallbackAction* action, const SoNode* node);
        static SoCallbackAction::Response postGroup(void* data,
                                 SoCallbackAction* action, const SoNode* node);
        static SoCallbackAction::Response preTexture(void* data, 
                                 SoCallbackAction* action, const SoNode* node);
        static SoCallbackAction::Response preLight(void* data, 
                                 SoCallbackAction* action, const SoNode* node);
        static SoCallbackAction::Response preRotor(void* data, 
                                 SoCallbackAction* action, const SoNode* node);
        static SoCallbackAction::Response prePendulum(void* data, 
                                 SoCallbackAction* action, const SoNode* node);
        static SoCallbackAction::Response preShuttle(void* data, 
                                 SoCallbackAction* action, const SoNode* node);
        static SoCallbackAction::Response postLOD(void* data,
                                 SoCallbackAction* action, const SoNode* node);
        ///Callback for VRMLImageTexture
        ///\param data The pointer to ConvertFromInventor object
        ///\param action The action handling class
        ///\param node The current VRMLImageTexture node
        static SoCallbackAction::Response preVRMLImageTexture(void* data,
                                 SoCallbackAction* action, const SoNode* node);

        static void addTriangleCB(void* data, SoCallbackAction* action,
                              const SoPrimitiveVertex *v0,
                                  const SoPrimitiveVertex *v1,
                                  const SoPrimitiveVertex *v2);
        static void addLineSegmentCB(void* data, SoCallbackAction* action,
                                     const SoPrimitiveVertex *v0,
                                     const SoPrimitiveVertex *v1);
        static void addPointCB(void* data, SoCallbackAction* action,
                               const SoPrimitiveVertex *v0);

    private:

        void addVertex(SoCallbackAction* action, const SoPrimitiveVertex* v, 
                       int index);

        osg::ref_ptr<osg::StateSet> getStateSet(SoCallbackAction* action);

        osg::Texture2D* convertIVTexToOSGTex(const SoNode* soNode, 
                                             SoCallbackAction* action);

        void transformLight(SoCallbackAction* action, const SbVec3f& vec,
                            osg::Vec3& transVec);

        // OSG doesn't seem to have a transpose function for matrices
        void transposeMatrix(osg::Matrix& mat);

    private:

        // Normal and color binding
        osg::Geometry::AttributeBinding normalBinding;
        osg::Geometry::AttributeBinding colorBinding;

        // List of vertices, normals, colors and texture coordinates
        std::vector<osg::Vec3> vertices;
        std::vector<osg::Vec3> normals;
        std::vector<osg::Vec4> colors;
        std::vector<osg::Vec2> textureCoords;

        // Num of primitive and primitive type 
        int numPrimitives;
        osg::PrimitiveSet::Mode primitiveType;

        // Vertex ordering
        enum VertexOrder { CLOCKWISE, COUNTER_CLOCKWISE };
        VertexOrder vertexOrder;

        // Stack of group nodes (used to build the scene graph)
        std::stack<osg::Group* > groupStack;
        // Stack of texture nodes (used for attaching the right texture to the
        // geosets). Supported types are SoTexture2 and SoVRMLImageTexture for now.
        std::stack<const SoNode*> soTexStack;

        // Mapping from SoTexture2 and SoVRMLImageTexture to already converted
        // osg::Texture2D - avoids duplication of same texture objects
        std::map<const SoNode*, osg::Texture2D*> ivToOsgTexMap;

        // Stack to maintain the list of lights at each level of the 
        // scenegraph
        typedef std::vector<osg::Light *> LightList;
        std::stack<LightList> lightStack;

        osg::ref_ptr<osg::MatrixTransform> _root;///<The root node;
};

#endif
