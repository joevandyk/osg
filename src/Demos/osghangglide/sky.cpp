#include <math.h>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture>
#include <osg/TexEnv>
#include <osg/Depth>
#include <osg/StateSet>

#include <osgDB/ReadFile>

#ifdef _MSC_VER
#pragma warning( disable : 4244 )
#pragma warning( disable : 4305 )
#endif

using namespace osg;

Node *makeSky( void )
{
    int i, j;
    float lev[] = { -5, -1.0, 1.0, 15.0, 30.0, 60.0, 90.0  };
    float cc[][4] =
    {
        { 0.0, 0.0, 0.15 },
        { 0.0, 0.0, 0.15 },
        { 0.4, 0.4, 0.7 },
        { 0.2, 0.2, 0.6 },
        { 0.1, 0.1, 0.6 },
        { 0.1, 0.1, 0.6 },
        { 0.1, 0.1, 0.6 },
    };
    float x, y, z;
    float alpha, theta;
    float radius = 20.0f;
    int nlev = sizeof( lev )/sizeof(float);

    Geometry *geom = new Geometry;

    Vec3Array& coords = *(new Vec3Array(19*nlev));
    Vec4Array& colors = *(new Vec4Array(19*nlev));
    Vec2Array& tcoords = *(new Vec2Array(19*nlev));
    
    
    int ci, ii;
    ii = ci = 0;

    for( i = 0; i < nlev; i++ )
    {
        for( j = 0; j <= 18; j++ )
        {
            alpha = osg::DegreesToRadians(lev[i]);
            theta = osg::DegreesToRadians((float)(j*20));

            x = radius * cosf( alpha ) * cosf( theta );
            y = radius * cosf( alpha ) * -sinf( theta );
            z = radius * sinf( alpha );

            coords[ci][0] = x;
            coords[ci][1] = y;
            coords[ci][2] = z;

            colors[ci][0] = cc[i][0];
            colors[ci][1] = cc[i][1];
            colors[ci][2] = cc[i][2];
            colors[ci][3] = 1.0;

            tcoords[ci][0] = (float)j/18.0;
            tcoords[ci][0] = (float)i/(float)(nlev-1);

            ci++;
        }


    }

    for( i = 0; i < nlev-1; i++ )
    {
        UShortDrawElements* drawElements = new UShortDrawElements(Primitive::TRIANGLE_STRIP);
        drawElements->reserve(38);

        for( j = 0; j <= 18; j++ )
        {
            drawElements->push_back((i+1)*19+j);
            drawElements->push_back((i+0)*19+j);
        }

        geom->addPrimitive(drawElements);
    }
    
    geom->setVertexArray( &coords );
    geom->setTexCoordArray( 0, &tcoords );

    geom->setColorArray( &colors );
    geom->setColorBinding( Geometry::BIND_PER_VERTEX );


    Texture *tex = new Texture;
    tex->setImage(osgDB::readImageFile("Images/white.rgb"));

    StateSet *dstate = new StateSet;

    dstate->setTextureAttributeAndModes(0, tex, StateAttribute::OFF );
    dstate->setTextureAttribute(0, new TexEnv );
    dstate->setMode( GL_LIGHTING, StateAttribute::OFF );
    dstate->setMode( GL_CULL_FACE, StateAttribute::ON );
    

    // clear the depth to the far plane.
    osg::Depth* depth = new osg::Depth;
    depth->setFunction(osg::Depth::ALWAYS);
    depth->setRange(1.0,1.0);   
    dstate->setAttributeAndModes(depth,StateAttribute::ON );

    dstate->setRenderBinDetails(-2,"RenderBin");

    geom->setStateSet( dstate );

    Geode *geode = new Geode;
    geode->addDrawable( geom );

    geode->setName( "Sky" );

    return geode;
}
