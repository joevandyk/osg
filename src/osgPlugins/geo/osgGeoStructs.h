/*===========================================================================*\

NAME:			osgGeoStructs.h

DESCRIPTION:	OSG data format for reading a Geo file into OSG

AUTHOR:			Geoff Michel

//	-------------------------------------------------------------------------


\ *===========================================================================*/



#ifndef _GEO_STRUCTS_H_
#define _GEO_STRUCTS_H_


class geoField { // holds one field of data as read from the disk of a GEO file
public:
	geoField() {
		tokenId=TypeId=0; numItems=0;storeSize=0;
	}

	unsigned char *readStorage(std::ifstream &fin, const unsigned sz) {
		unsigned char *st=new unsigned char[numItems*sz];
		storeSize=sz;
		fin.read((char *)st, sz*numItems);
		return st;
	}
	void storageRead(std::ifstream &fin) {
		switch (TypeId) {
		case DB_CHAR:
			storage=readStorage(fin,SIZEOF_CHAR);
			break;
		case DB_SHORT:
			storage=readStorage(fin,SIZEOF_SHORT);
			break;
		case DB_INT:
			storage=readStorage(fin,SIZEOF_INT);
			break;
		case DB_FLOAT:
			storage=readStorage(fin,SIZEOF_FLOAT);
			break;
		case DB_LONG:
			storage=readStorage(fin,SIZEOF_LONG);
			break;
		case DB_ULONG:
			storage=readStorage(fin,SIZEOF_ULONG);
			break;
		case DB_DOUBLE:
			storage=readStorage(fin,SIZEOF_DOUBLE);
			break;
		case DB_VEC2F:
			storage=readStorage(fin,SIZEOF_VEC2F);
			break;
		case DB_VEC3F:
			storage=readStorage(fin,SIZEOF_VEC3F);
			break;
		case DB_VEC4F:
			storage=readStorage(fin,SIZEOF_VEC4F);
			break;
		case DB_VEC16F:
			storage=readStorage(fin,SIZEOF_VEC16F);
			break;
		case DB_VEC2I:
			storage=readStorage(fin,SIZEOF_VEC2I);
			break;
		case DB_VEC3I:
			storage=readStorage(fin,SIZEOF_VEC3I);
			break;
		case DB_VEC4I:
			storage=readStorage(fin,SIZEOF_VEC4I);
			break;
		case DB_VEC2D:
			storage=readStorage(fin,SIZEOF_VEC2D);
			break;
		case DB_VEC3D:
			storage=readStorage(fin,SIZEOF_VEC3D);
			break;
		case DB_VEC4D:
			storage=readStorage(fin,SIZEOF_VEC4D);
			break;
		case DB_VEC16D:
			storage=readStorage(fin,SIZEOF_VEC16D);
			break;
		case DB_VRTX_STRUCT:
			storage=readStorage(fin,SIZEOF_VRTX_STRUCT);
			break;
		case DB_UINT:
			storage=readStorage(fin,SIZEOF_UINT);
			break;
		case DB_USHORT:
			storage=readStorage(fin,SIZEOF_USHORT);
			break;
		case DB_UCHAR:
			storage=readStorage(fin,SIZEOF_UCHAR);
			break;
		case DB_EXT_STRUCT:
			storage=readStorage(fin,SIZEOF_EXT_STRUCT);
			break;
		case DB_SHORT_WITH_PADDING:
			storage=readStorage(fin,SIZEOF_ULONG);
			break;
		case DB_CHAR_WITH_PADDING:
			storage=readStorage(fin,SIZEOF_CHAR_WITH_PADDING);
			break;
		case DB_USHORT_WITH_PADDING:
			storage=readStorage(fin,SIZEOF_USHORT_WITH_PADDING);
			break;
		case DB_UCHAR_WITH_PADDING:
			storage=readStorage(fin,SIZEOF_UCHAR_WITH_PADDING);
			break;
		case DB_BOOL_WITH_PADDING:
			storage=readStorage(fin,SIZEOF_BOOL_WITH_PADDING);
			break;
		case DB_EXTENDED_FIELD_STRUCT:
			storage=readStorage(fin,SIZEOF_EXTENDED_FIELD_STRUCT);
			break;
		case DB_VEC4UC:
			storage=readStorage(fin,SIZEOF_VEC4UC);
			break;
		case DB_DISCRETE_MAPPING_STRUCT:
			storage=readStorage(fin,SIZEOF_DISCRETE_MAPPING_STRUCT);
			break;
		case DB_BITFLAGS:
			storage=readStorage(fin,SIZEOF_BITFLAGS);
			break;
		}
	}
	void readfile(std::ifstream &fin) { 
		unsigned char tokid, type;
		unsigned short nits;
		if (!fin.eof()) {
			fin.read((char *)&tokid,1);fin.read((char *)&type,1);
			fin.read((char *)&nits,sizeof(unsigned short));
			if (TypeId == GEO_DB_EXTENDED_FIELD) {
				fin.read((char *)&tokenId,sizeof(tokenId));fin.read((char *)&TypeId,sizeof(TypeId));
				fin.read((char *)&numItems,sizeof(unsigned int));
			} else {
				tokenId=tokid; TypeId=type;
				numItems=nits;
			}
			storageRead(fin); // allocate & fill the storage
		}
	}
	void writefile(std::ofstream &fout) { // write binary file
		if (numItems<32767 && tokenId<256) {
			unsigned char tokid=tokenId, type=TypeId;
			fout.write((char *)&tokid, 1);fout.write((char *)&type,1);
			fout.write((char *)&numItems,sizeof(unsigned short));
		} else {
		}
		fout.write((char *)storage, storeSize*numItems);
	}
	inline unsigned char getToken() const { return tokenId;}
	inline unsigned char getType() const { return TypeId;}
	inline unsigned short getNum() const { return numItems;}
	inline unsigned char *getstore (unsigned int i) const {
		return storage+i*storeSize;
	}
	void uncompress() { // follow the recipe to uncompress
		if (TypeId==DB_VEC3F) { // already uncompressed
		} else {
			float *norms=new float[numItems*SIZEOF_VEC3F]; // uncompressed size
			for (uint i=0; i<numItems; i++) {
				switch (TypeId) {
				case DB_UINT:
					norms[3*i]=storage[4*i+1]/255.0f;
					norms[3*i+1]=storage[4*i+2]/255.0f;
					norms[3*i+2]=storage[4*i+3]/255.0f;
					if (storage[4*i] & 0x01) norms[3*i] *= -1;
					if (storage[4*i] & 0x02) norms[3*i+1] *= -1;
					if (storage[4*i] & 0x04) norms[3*i+2] *= -1;
					break;
				case DB_SHORT:
					norms[3*i]=(storage[6*i]*255+storage[6*i+1])/32767.0f;
					norms[3*i+1]=(storage[6*i+2]*255+storage[6*i+3])/32767.0f;
					norms[3*i+2]=(storage[6*i+4]*255+storage[6*i+5])/32767.0f;
					break;
				case DB_CHAR:
					norms[3*i]=storage[3*i]/127.0f;
					norms[3*i+1]=storage[3*i+1]/127.0f;
					norms[3*i+2]=storage[3*i+2]/127.0f;
					break;
				}
			}
			delete [] storage;
			TypeId=DB_VEC3F;
			storage=(unsigned char *)norms;
		}
	}
	inline void warn(const char *type, unsigned tval) const { if (getType() != tval) 
			osg::notify(osg::WARN) << "Wrong type " << type << (int)tval <<" expecting "<< (int)getType() << std::endl;}
	inline unsigned int getUInt() const {warn("getUInt",DB_UINT); return *((unsigned int*)storage);} // return int value
	inline char *getChar() const {warn("getChar",DB_CHAR); return (char *)storage;} // return chars, eg for name or file name
	inline unsigned char getUChar() const {warn("getUChar",DB_CHAR); return *storage;} // return chars, eg for name or file name
	inline int getInt() const {
		warn("getInt", DB_INT); 
		int val;
		memcpy(&val,storage,sizeof(int));
		return val;} // return int value
	inline float getFloat() const {warn("getFloat", DB_FLOAT); return (*(float *)storage);	}
	inline float *getFloatArr() const {warn("getFloatArr", DB_FLOAT); return ( (float *)storage);	}
	inline int *getIntArr() const {warn("getIntArr", DB_INT); return ( (int *)storage);	}
	inline float *getVec3Arr() const {warn("getVec3Arr", DB_VEC3F); return ( (float *)storage);	}
	inline float *getMat44Arr() const {warn("getMat44Arr", DB_VEC16F); return ( (float *)storage);	}
	inline double getDouble() const {warn("getDouble", DB_DOUBLE); return (*(double *)storage);	}
	inline unsigned char *getUCh4Arr() const {warn("getUChArr", DB_VEC4UC); return ((unsigned char *)storage);	}
	inline bool getBool() const {warn("getBool", DB_BOOL_WITH_PADDING); return (storage[0] != 0);	}
	friend inline std::ostream& operator << (osgDB::Output& output, const geoField& gf)
    {
		if (gf.tokenId!=GEO_DB_LAST_FIELD) {
			output.indent() << " Field:token " << (int)gf.tokenId << " datatype " << (int)gf.TypeId
				<< " num its " << gf.numItems << " size " << gf.storeSize << std::endl;
			if (gf.TypeId==DB_CHAR) output.indent();
			for (uint i=0; i<gf.numItems; i++) {
				int j,k;
				union {
					unsigned char *uch;
					char *ch;
					float *ft;
					int *in;
					uint *uin;
					short *sh;
					unsigned short *ush;
					long *ln;
					unsigned long *uln;
					double *dbl;
				} st;
				st.uch=gf.storage+i*gf.storeSize;
				switch (gf.TypeId) {
				case DB_CHAR:
					if (st.ch[0]) output << st.ch[0];
					break;
				case DB_SHORT:
					output.indent() << st.sh[0] << std::endl;
					break;
				case DB_INT:
					output.indent() << gf.getInt() << std::endl;
					break;
				case DB_FLOAT:
					output.indent() << gf.getFloat() << std::endl;
					break;
				case DB_LONG:
					output.indent() << st.ln[0] << std::endl;
					break;
				case DB_ULONG:
					output.indent() << st.uln[0] << std::endl;
					break;
				case DB_DOUBLE:
					output.indent() << st.dbl[0] << std::endl;
					break;
				case DB_VEC2F:
					output.indent() << st.ft[0] << " " << st.ft[1];
					output << std::endl;
					break;
				case DB_VEC3F:
					output.indent();
					for (j=0; j<3; j++) output << st.ft[j] << " ";
					output << std::endl;
					break;
				case DB_VEC4F:
					output.indent();
					for (j=0; j<4; j++) output << st.ft[j] << " ";
					output << std::endl;
					break;
				case DB_VEC16F:
					for (j=0; j<4; j++) {
						output.indent();
						for (k=0; k<4; k++) output << st.ft[j*4+k] << " ";
						output << std::endl;
					}
					break;
				case DB_VEC2I:
					output.indent() << st.in[0] << " " << st.in[1] << std::endl;
					break;
				case DB_VEC3I:
					output.indent();
					for ( j=0; j<3; j++) output << " " << st.in[j];
					output << std::endl;
					break;
				case DB_VEC4I:
					output.indent();
					for ( j=0; j<4; j++) output << st.in[j] << " ";
					output << std::endl;
					break;
				case DB_VEC2D:
					output.indent();
					for ( j=0; j<2; j++) output << st.dbl[j] << " ";
					output << std::endl;
					break;
				case DB_VEC3D:
					output.indent();
					for ( j=0; j<3; j++) output << st.dbl[j] << " ";
					output << std::endl;
					break;
				case DB_VEC4D:
					output.indent();
					for ( j=0; j<4; j++) output << st.dbl[j] << " ";
					output << std::endl;
					break;
				case DB_VEC16D:
					for (j=0; j<4; j++) {
						output.indent();
						for (k=0; k<4; k++) output << st.dbl[j*4+k] << " ";
						output << std::endl;
					}
					break;
				case DB_VRTX_STRUCT:
					output.indent() << st.ch[0] << std::endl;
					break;
				case DB_UINT:
					output.indent() << st.uin[0] << std::endl;
					break;
				case DB_USHORT:
					output.indent() << st.ush[0] << std::endl;
					break;
				case DB_UCHAR:
					output.indent() << (int)st.ch[0] << std::endl;
					break;
				case DB_EXT_STRUCT:
					output.indent() << st.ch[0] << std::endl;
					break;
				case DB_SHORT_WITH_PADDING:
					output.indent() << st.ch[0] << std::endl;
					break;
				case DB_CHAR_WITH_PADDING:
					output.indent() << st.ch[0] << std::endl;
					break;
				case DB_USHORT_WITH_PADDING:
					output.indent() << st.ch[0] << std::endl;
					break;
				case DB_UCHAR_WITH_PADDING:
					output.indent() << (int)st.ch[0] << std::endl;
					break;
				case DB_BOOL_WITH_PADDING:
					output.indent() << (gf.getBool()?"True":"False") << std::endl;
					break;
				case DB_EXTENDED_FIELD_STRUCT:
					output.indent() << st.ch[0] << std::endl;
					break;
				case DB_VEC4UC:
					output.indent();
					for ( j=0; j<4; j++) output << (int)st.uch[j] << " ";
					output << std::endl;
					break;
				case DB_DISCRETE_MAPPING_STRUCT:
					output.indent() << st.ch[i] << std::endl;
					break;
				case DB_BITFLAGS:
					output.indent() << st.ch[i] << std::endl;
					break;
				}
			}
		}
		if (gf.TypeId==DB_CHAR) output << std::endl;
	    return output; 	// to enable cascading, monkey copy from osg\plane or \quat, Ubyte4, vec2,3,4,... 
	}
private:
	unsigned short tokenId, TypeId; // these are longer than standard field; are extended field length
	uint numItems;
	unsigned char *storage; // data relating
	uint storeSize; // size*numItems in storage
};

class georecord { // holds a single record with a vector of geoFields as read from disk
public:
    typedef std::vector< geoField > geoFieldList;
	georecord() {id=0; parent=NULL; instance=NULL; nod=NULL; }
	~georecord() {;}
	inline const uint getType(void) const {return id;}
    typedef std::vector<osg::MatrixTransform *> instancelist; // list if unused instance matrices
	void addInstance(osg::MatrixTransform *mtr) { mtrlist.push_back(mtr);}
	inline  void setNode(osg::Node *n) {
		nod=n;
		{
			for (instancelist::iterator itr=mtrlist.begin();
			itr!=mtrlist.end();
			++itr) {
				(*itr)->addChild(nod);
			}
			mtrlist.clear();
		}
	}
	inline osg::Node *getNode() const { return nod;}
	inline void setparent(georecord *p) { parent=p;}
	inline class georecord *getparent() const { return parent;}
	inline std::vector<georecord *> getchildren(void) const { return children;}
	void addchild(class georecord *gr) { children.push_back(gr);}
	georecord *getLastChild(void) const { return children.back();}
	void addBehaviourRecord(class georecord *gr) { behaviour.push_back(gr);}
	void addMappingRecord(class georecord *gr) { tmap.push_back(gr);}
	std::vector< georecord *>getBehaviour() const { return behaviour;}
	const geoFieldList getFields() const { return fields;}
	inline bool isVar(void) const {
		switch (id) {
		case DB_DSK_FLOAT_VAR:
		case DB_DSK_INT_VAR:
		case DB_DSK_LONG_VAR:
		case DB_DSK_DOUBLE_VAR:
		case DB_DSK_BOOL_VAR:
		case DB_DSK_FLOAT2_VAR:
		case DB_DSK_FLOAT3_VAR:
		case DB_DSK_FLOAT4_VAR:
			
		case DB_DSK_INTERNAL_VARS:		   		
		case DB_DSK_LOCAL_VARS:		   		
		case DB_DSK_EXTERNAL_VARS:
			return true;
		default:
			return false;
		}
	}
	inline bool isAction(void) const {
		switch (id) {
		case DB_DSK_CLAMP_ACTION:
		case DB_DSK_RANGE_ACTION			:
		case DB_DSK_ROTATE_ACTION			:
		case DB_DSK_TRANSLATE_ACTION		:
		case DB_DSK_SCALE_ACTION			:
		case DB_DSK_ARITHMETIC_ACTION		:
		case DB_DSK_LOGIC_ACTION			:
		case DB_DSK_CONDITIONAL_ACTION	:
		case DB_DSK_LOOPING_ACTION		:
		case DB_DSK_COMPARE_ACTION		:
		case DB_DSK_VISIBILITY_ACTION		:
		case DB_DSK_STRING_CONTENT_ACTION	:
		case DB_DSK_COLOR_RAMP_ACTION:
		case DB_DSK_LINEAR_ACTION			:
		case DB_DSK_TASK_ACTION			:
		case DB_DSK_PERIODIC_ACTION		:
		case DB_DSK_PERIODIC2_ACTION		:
		case DB_DSK_TRIG_ACTION			:
		case DB_DSK_INVERSE_ACTION		:
		case DB_DSK_TRUNCATE_ACTION		:
		case DB_DSK_ABS_ACTION			:
		case DB_DSK_IF_THEN_ELSE_ACTION	:
		case DB_DSK_DCS_ACTION			:
		case DB_DSK_DISCRETE_ACTION:
		case DB_DSK_SQRT_ACTION			: // an undefined, square root action
			return true;
		default:
			return false;
		}
		return false;
	}
	void readfile(std::ifstream &fin) {
		if (!fin.eof()) {
			fin.read((char *)&id,sizeof(int));
			if (id==DB_DSK_PUSH) {
				// there are no fields for a push
			} else if (id==DB_DSK_POP) {
				// there are no fields for a pop
			} else { // get the fields
				geoField gf;
				do {
					gf.readfile(fin);
//					if (id == DB_DSK_NORMAL_POOL && gf.getToken()==GEO_DB_NORMAL_POOL_VALUES) {
						// uncompress the normals
//						gf.uncompress();
//					}
					fields.push_back(gf);
				} while (gf.getToken()!=GEO_DB_LAST_FIELD);
			}
		}
	}
	void writefile(std::ofstream &fout) { // write binary file
			fout.write((char *)&id,sizeof(int));
			 { // output the fields
				for (geoFieldList::iterator itr=fields.begin();
				itr!=fields.end();
				++itr)
				{
					itr->writefile(fout);
				}
			}
	}
	friend inline std::ostream& operator << (osgDB::Output& output, const georecord& gr)
    {
		switch (gr.id) {
		case DB_DSK_PUSH: output << "Push" << std::endl; break;
		case DB_DSK_POP: output << "Pop" << std::endl; break;
		case DB_DSK_HEADER: output << "Header" << std::endl; break;
		case DB_DSK_GROUP: output << "Group" << std::endl; break;
	//	case DB_DSK_BILLBOARD: output << "Billboard" << std::endl; break;
		case DB_DSK_SEQUENCE: output << "Sequence" << std::endl; break;
		case DB_DSK_LOD: output << "LOD" << std::endl; break;
		case DB_DSK_GEODE: output << "Geode" << std::endl; break;
		case DB_DSK_POLYGON: output << "Polygon" << std::endl; break;
		case DB_DSK_MESH: output << "Mesh" << std::endl; break;
		case DB_DSK_CUBE: output << "Cube" << std::endl; break;
		case DB_DSK_SPHERE: output << "Sphere" << std::endl; break;
		case DB_DSK_CONE: output << "Cone" << std::endl; break;
		case DB_DSK_CYLINDER: output << "Cylinder" << std::endl; break;
		case DB_DSK_VERTEX: output << "Vertex" << std::endl; break;
		case DB_DSK_TEXTURE: output << "Texture" << std::endl; break;
		case DB_DSK_MATERIAL: output << "Material" << std::endl; break;
		case DB_DSK_VIEW: output << "View" << std::endl; break;
		case DB_DSK_EXTENSION_LIST: output << "Extensions" << std::endl; break;
		case DB_DSK_COORD_POOL: output << "Coords" << std::endl; break;
		case DB_DSK_NORMAL_POOL: output << "Normals" << std::endl; break;
		case DB_DSK_SWITCH: output << "Switch" << std::endl; break;
		case DB_DSK_TEXT: output << "Text" << std::endl; break;
		case DB_DSK_BASE_GROUP: output << "Base group" << std::endl; break;
		case DB_DSK_BASE_SURFACE: output << "Base Surface" << std::endl; break;
		case DB_DSK_INSTANCE: output << "Instance" << std::endl; break;
		case DB_DSK_LIGHTPT: output << "Light Point" << std::endl; break;
		case DB_DSK_EXTERNAL: output << "External" << std::endl; break;
		case DB_DSK_PAGE: output << "Page" << std::endl; break;
		case DB_DSK_COLOR_PALETTE: output << "Colour palette" << std::endl; break;
		case DB_DSK_PERSPECTIVE_GRID_INFO: output << "Perspective Grid Info" << std::endl; break;
		case DB_DSK_INTERNAL_VARS: output << "Internal vars" << std::endl; break;
		case DB_DSK_LOCAL_VARS: output << "Local vars" << std::endl; break;
		case DB_DSK_EXTERNAL_VARS: output << "External vars" << std::endl; break;
		// behaviours
		case DB_DSK_BEHAVIOR: output << "Behaviour" << std::endl; break;
		case DB_DSK_CLAMP_ACTION: output << "clamp action" << std::endl; break;
		case DB_DSK_RANGE_ACTION: output << "range action" << std::endl; break;
		case DB_DSK_ROTATE_ACTION: output << "rotate action" << std::endl; break;
		case DB_DSK_TRANSLATE_ACTION: output << "translate action" << std::endl; break;
		case DB_DSK_SCALE_ACTION: output << "scale action" << std::endl; break;
		case DB_DSK_ARITHMETIC_ACTION: output << "arithmetic action" << std::endl; break;
		case DB_DSK_LOGIC_ACTION: output << "logic action" << std::endl; break;
		case DB_DSK_CONDITIONAL_ACTION: output << "conditional action" << std::endl; break;
		case DB_DSK_LOOPING_ACTION: output << "looping action" << std::endl; break;
		case DB_DSK_COMPARE_ACTION: output << "compare action" << std::endl; break;
		case DB_DSK_VISIBILITY_ACTION: output << "visibility action" << std::endl; break;
		case DB_DSK_STRING_CONTENT_ACTION: output << "string content action" << std::endl; break;
		// var types
		case DB_DSK_FLOAT_VAR: output << "Float var" << std::endl; break;
		case DB_DSK_INT_VAR: output << "Int var" << std::endl; break;
		case DB_DSK_LONG_VAR: output << "Long var" << std::endl; break;
		case DB_DSK_DOUBLE_VAR: output << "Double var" << std::endl; break;
		case DB_DSK_BOOL_VAR: output << "Bool var" << std::endl; break;
		default: output << " inp record " << gr.id << std::endl; break;
		}
 
		for (geoFieldList::const_iterator itr=gr.fields.begin();
		itr!=gr.fields.end();
		++itr)
		{
			output << *itr;
		}
		output << std::endl;
		std::vector< georecord *>bhv=gr.getBehaviour();
		for (std::vector< georecord *>::const_iterator rcitr=bhv.begin();
		rcitr!=bhv.end();
		++rcitr)
		{
			output.indent() << "Behave ";
			output << (**rcitr);
		}
	    return output; 	// to enable cascading, monkey copy from osg\plane or \quat, Ubyte4, vec2,3,4,... 
	}
	geoField *getModField(const int fieldid) { // return modifiable field if it exists.
		for (geoFieldList::iterator itr=fields.begin();
		itr!=fields.end();
		++itr)
		{
			if (itr->getToken()==fieldid) return &(*itr);
		}
		return NULL;
	}
	const geoField *getField(const int fieldid) const { // return field if it exists.
		for (geoFieldList::const_iterator itr=fields.begin();
		itr!=fields.end();
		++itr)
		{
			if (itr->getToken()==fieldid) return &(*itr);
		}
		return NULL;
	}
	void setMaterial(osg::Material *mt) const {
		if (id == DB_DSK_MATERIAL) {
			for (geoFieldList::const_iterator itr=fields.begin();
			itr!=fields.end();
			++itr)
			{
				float *fval;
				if (itr->getToken()==GEO_DB_MAT_AMBIENT) {
					fval= (float *)(*itr).getstore(0);
					mt->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(fval[0],fval[1],fval[2],fval[3]));
				}
				if (itr->getToken()==GEO_DB_MAT_DIFFUSE) {
					fval= (float *)(*itr).getstore(0);
					mt->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(fval[0],fval[1],fval[2],fval[3]));
				}
				if (itr->getToken()==GEO_DB_MAT_SPECULAR) {
					fval= (float *)(*itr).getstore(0);
					mt->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(fval[0],fval[1],fval[2],fval[3]));
				}
				if (itr->getToken()==GEO_DB_MAT_EMISSIVE) {
					fval= (float *)(*itr).getstore(0);
					mt->setEmission(osg::Material::FRONT_AND_BACK, osg::Vec4(fval[0],fval[1],fval[2],fval[3]));
				}
				if (itr->getToken()==GEO_DB_MAT_SHININESS) {
					fval= (float *)(*itr).getstore(0);
					mt->setShininess(osg::Material::FRONT_AND_BACK, fval[0]);
				}
			}
		}
	}
	unsigned int getNumFields(void) const { return fields.size();}
private:
	uint id;
	std::vector<geoField> fields; // each geo record has a variable number of fields
	class georecord *parent; // parent of pushed/popped records
	class georecord *instance; // this record is an instance of the pointed to record
	std::vector< georecord *> tmap; // texture mapping records of this record
	std::vector< georecord *> behaviour; // behaviour & action records of this record
	std::vector< georecord *> children; // children of this record
	osg::Node *nod; // the node that this record has been converted to (useful for instances)
	instancelist mtrlist; // list of matrices of instances not yet satisfied
};


typedef std::vector< georecord > geoRecordList;

#endif //_GEO_STRUCTS_H_
