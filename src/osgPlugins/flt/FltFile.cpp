#include "FltFile.h"
#include "Registry.h"
#include "Record.h"
#include "RecordVisitor.h"
#include "ExternalRecord.h"
#include "flt2osg.h"             // ConvertFromFLT
#include "Input.h"

#include <osg/Node>
#include <osg/NodeVisitor>
#include <osg/Notify>

#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

#include <string>


using namespace flt;


FltFile::FltFile(
    ColorPool* pColorPool,
    TexturePool* pTexturePool,
    MaterialPool* pMaterialPool)
{
    if (pColorPool)
    {
        // use external color palette, ignore internal
        _useInternalColorPalette = false;
        setColorPool( pColorPool );
    }
    else
    {
        // use internal color palette
        _useInternalColorPalette = true;
        setColorPool( new ColorPool );
    }

    if (pTexturePool)
    {
        // use external texture palette, ignore internal
        _useInternalTexturePalette = false;
        setTexturePool( pTexturePool );
    }
    else
    {
        // use internal texture palette
        _useInternalTexturePalette = true;
        setTexturePool( new TexturePool );
    }

    if (pMaterialPool)
    {
        // use external material palette, ignore internal
        _useInternalMaterialPalette = false;
        setMaterialPool( pMaterialPool );
    }
    else
    {
        // use internal material palette
        _useInternalMaterialPalette = true;
        setMaterialPool( new MaterialPool );
    }

    // instances are always internally defined 
    setInstancePool( new InstancePool );
}


osg::Object* FltFile::readObject(const std::string& fileName)
{
    return readNode(fileName);
}


osg::Node* FltFile::readNode(const std::string& fileName)
{
    _directory = osgDB::getFilePath(fileName);

    if (readModel(fileName))
    {
        // Convert record tree to osg scene graph
        return convert();
    }

    return NULL;
}


osg::Group* FltFile::convert()
{
    ConvertFromFLT visit;
    return visit.convert(getHeaderRecord());
}


// Read flight model (include externals)
bool FltFile::readModel(const std::string& fileName)
{
    if (readFile(fileName))
    {
        readExternals();
        return getHeaderRecord() ? true : false;
    }
    return false;
}


bool FltFile::readFile(const std::string& fileName)
{
    FileInput fin;

    if (!fin.open(fileName)) 
    {
        // havn't found file, look in OSGFILEPATH
        std::string foundFileName = osgDB::findDataFile(fileName);

        if (foundFileName.empty()) return false;
        if (!fin.open(foundFileName)) return false;
    }

    osg::notify(osg::INFO) << "Loading " << fileName << " ... " << std::endl;

    Record* pRec = fin.readCreateRecord(this);
    if (pRec == NULL)
    {
        osg::notify(osg::WARN) << "File not found " << fileName << std::endl;
        return false;
    }

    _headerRecord = (HeaderRecord*)pRec;
    if (pRec->isPrimaryNode())   // Header
        pRec->readLocalData(fin);// Read rest of file

    fin.close();

    return true;
}


#define REGISTER_FLT 1

// This class was originally scoped within FltFile::readExternals() function.
// Irix 7.3 compilers hork on this.

    class ReadExternal : public RecordVisitor
    {
        public:
            ReadExternal(FltFile* fltFile)
            {
                _pFltFile = fltFile;
                setTraverseMode(RecordVisitor::TRAVERSE_ALL_CHILDREN);
            }

            virtual void apply(ExternalRecord& rec)
            {
                SExternalReference* pSExternal = (SExternalReference*)rec.getData();

                if (pSExternal)
                {
                    FltFile*        pExternalFltFile = NULL;
                    ColorPool*      pColorPool    = NULL;
                    TexturePool*    pTexturePool  = NULL;
                    MaterialPool*   pMaterialPool = NULL;
                    std::string filename(pSExternal->szPath);

                    osg::notify(osg::INFO) << "External=" << filename << std::endl;

                    if (rec.getFlightVersion() > 13)
                    {
                        if (pSExternal->dwFlags & ExternalRecord::COLOR_PALETTE_OVERRIDE)
                            pColorPool = NULL;
                        else
                            pColorPool = _pFltFile->getColorPool();

                        if (pSExternal->dwFlags & ExternalRecord::TEXTURE_PALETTE_OVERRIDE)
                            pTexturePool = NULL;
                        else
                            pTexturePool = _pFltFile->getTexturePool();

                        if (pSExternal->dwFlags & ExternalRecord::MATERIAL_PALETTE_OVERRIDE)
                            pMaterialPool = NULL;
                        else
                            pMaterialPool = _pFltFile->getMaterialPool();
                    }

    #if REGISTER_FLT
                    pExternalFltFile = Registry::instance()->getFltFile(filename);
                    if (pExternalFltFile == NULL)
                    {
                        pExternalFltFile = new FltFile(pColorPool, pTexturePool, pMaterialPool);
                        pExternalFltFile->readModel(filename);
                    }
                    Registry::instance()->addFltFile(filename, pExternalFltFile);
    #else
                    pExternalFltFile = new FltFile(pColorPool, pTexturePool, pMaterialPool);
                    pExternalFltFile->readModel(filename);
    #endif
                    rec.setExternal(pExternalFltFile);
                }
            }

        public:

            FltFile* _pFltFile;
    };

void FltFile::readExternals()
{
    ReadExternal visitor(this);
    _headerRecord->accept(visitor);
}


int FltFile::getFlightVersion() const
{
    if (_headerRecord.get())
    {
        SHeader* pSHeader = (SHeader*)_headerRecord.get()->getData();
        if (pSHeader)
            return pSHeader->diFormatRevLev;
    }
    return 0;
}
