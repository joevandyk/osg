#include <osgGA/KeySwitchMatrixManipulator>
#include <osg/Notify>

using namespace osgGA;

void KeySwitchMatrixManipulator::addMatrixManipulator(int key, std::string name, MatrixManipulator *cm)
{
    if(!cm) return;

    _manips[key]=std::make_pair(name,osg::ref_ptr<MatrixManipulator>(cm));
    if(!_current.valid()){
        _current=cm;
        _current->setNode(_current->getNode());
    }
}

void KeySwitchMatrixManipulator::addNumberedMatrixManipulator(MatrixManipulator *cm)
{
    if(!cm) return;
    addMatrixManipulator('1'+_manips.size(),cm->className(),cm);
}

void KeySwitchMatrixManipulator::selectMatrixManipulator(unsigned int num)
{
    unsigned int manipNo = 0;
    KeyManipMap::iterator itr;
    for(itr=_manips.begin();
        manipNo!=num && itr!=_manips.end();
        ++itr,++manipNo)
    {
    }
    
    if (itr!=_manips.end())
    {
        if (_current.valid())
        {
            if ( !itr->second.second->getNode() ) {
                itr->second.second->setNode(_current->getNode());
            }
            itr->second.second->setByMatrix(_current->getMatrix());
        }
        _current = itr->second.second;
    }
}

void KeySwitchMatrixManipulator::setNode(osg::Node* node)
{
    for(KeyManipMap::iterator itr=_manips.begin();
        itr!=_manips.end();
        ++itr)
    {
    
        itr->second.second->setNode(node);
    }
}

MatrixManipulator* KeySwitchMatrixManipulator::getMatrixManipulator(unsigned int num)
{
    KeyManipMap::iterator itr = _manips.find(num); 
    if (itr!=_manips.end()) return itr->second.second.get(); 
    else return 0;
}

const MatrixManipulator* KeySwitchMatrixManipulator::getMatrixManipulator(unsigned int num) const
{
    KeyManipMap::const_iterator itr = _manips.find(num); 
    if (itr!=_manips.end()) return itr->second.second.get(); 
    else return 0;
}

bool KeySwitchMatrixManipulator::handle(const GUIEventAdapter& ea,GUIActionAdapter& aa)
{
    if(ea.getEventType()==GUIEventAdapter::KEYDOWN){

        KeyManipMap::iterator it=_manips.find(ea.getKey());
        if(it != _manips.end()){
            osg::notify(osg::INFO)<<"Switching to manipulator: "<<(*it).second.first<<std::endl;
            if ( !it->second.second->getNode() ) {
                it->second.second->setNode(_current->getNode());
            }
            it->second.second->setByMatrix(_current->getMatrix());
            it->second.second->init(ea,aa);
            _current = it->second.second;

            //_cameraManipChangeCallbacks.notify(this);

        }
    }

    return _current->handle(ea,aa);
}

void KeySwitchMatrixManipulator::getUsage(osg::ApplicationUsage& usage) const
{
    for(KeyManipMap::const_iterator itr=_manips.begin();
        itr!=_manips.end();
        ++itr)
    {
        std::string key; key += (char)(itr->first);
        std::string explanation(std::string("Select '")+itr->second.first+std::string("' camera manipulator"));
        if (_current==itr->second.second) explanation += " (default)";

        usage.addKeyboardMouseBinding(key,explanation);
        itr->second.second->getUsage(usage);
    }
}
