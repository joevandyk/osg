#include <osg/ApplicationUsage>
#include <osg/Math>

using namespace osg;

ApplicationUsage::ApplicationUsage(const std::string& commandLineUsage):
    _commandLineUsage(commandLineUsage)
{
}

ApplicationUsage* ApplicationUsage::instance()
{
    static ApplicationUsage s_applicationUsage;
    return &s_applicationUsage;
}

void ApplicationUsage::addUsageExplanation(Type type,const std::string& option,const std::string& explanation)
{
    switch(type)
    {
        case(COMMAND_LINE_OPTION):
            addCommandLineOption(option,explanation);
            break;
        case(ENVIRONMENTAL_VARIABLE):
            addEnvironmentalVariable(option,explanation);
            break;
        case(KEYBOARD_MOUSE_BINDING):
            addKeyboardMouseBinding(option,explanation);
            break;
    }
}

void ApplicationUsage::addCommandLineOption(const std::string& option,const std::string& explanation,const std::string& defaultValue)
{
    _commandLineOptions[option]=explanation;
    _commandLineOptionsDefaults[option]=defaultValue;
}

void ApplicationUsage::addEnvironmentalVariable(const std::string& option,const std::string& explanation, const std::string& defaultValue)
{
    _environmentalVariables[option]=explanation;
    _environmentalVariablesDefaults[option]=defaultValue;
}

void ApplicationUsage::addKeyboardMouseBinding(const std::string& option,const std::string& explanation)
{
    _keyboardMouse[option]=explanation;
}

void ApplicationUsage::getFormattedString(std::string& str, const UsageMap& um,unsigned int widthOfOutput,bool showDefaults,const UsageMap& ud)
{

    unsigned int maxNumCharsInOptions = 0;
    ApplicationUsage::UsageMap::const_iterator citr;
    for(citr=um.begin();
        citr!=um.end();
        ++citr)
    {
        maxNumCharsInOptions = maximum(maxNumCharsInOptions,(unsigned int)citr->first.length());
    }
    
    unsigned int fullWidth = widthOfOutput;
    unsigned int optionPos = 2;
    unsigned int explanationPos = optionPos+maxNumCharsInOptions+2;
    unsigned int defaultPos = 0;
    if (showDefaults)
    {
        defaultPos = explanationPos;
        explanationPos = optionPos+8;
    }
    unsigned int explanationWidth = fullWidth-explanationPos;

    std::string line;
    
    for(citr=um.begin();
        citr!=um.end();
        ++citr)
    {
        line.assign(fullWidth,' ');
        line.replace(optionPos,citr->first.length(),citr->first);

        if (showDefaults)
        {
            UsageMap::const_iterator ditr = ud.find(citr->first);
            if (ditr != ud.end())
            {
                line.replace(defaultPos, std::string::npos, "");
                if (ditr->second != "")
                {
                    line += "[";
                    line += ditr->second;
                    line += "]";
                }
                str += line;
                str += "\n";
                line.assign(fullWidth,' ');
            }
        }
        
        const std::string& explanation = citr->second;
        std::string::size_type pos = 0;
        std::string::size_type offset = 0;
        bool firstInLine = true;
        while (pos<explanation.length())
        {
            if (firstInLine) offset = 0;
                    
            // skip any leading white space.
            while (pos<explanation.length() && explanation[pos]==' ')
            {
                if (firstInLine) ++offset;
                ++pos;
            }
            
            firstInLine = false;
        
            std::string::size_type width = minimum((std::string::size_type)(explanation.length()-pos),(std::string::size_type)(explanationWidth-offset));
            std::string::size_type slashn_pos = explanation.find('\n',pos);
            
            unsigned int extraSkip = 0;
            bool concatinated = false;
            if (slashn_pos!=std::string::npos)
            {
                if (slashn_pos<pos+width)
                {
                    width = slashn_pos-pos;
                    ++extraSkip;
                    firstInLine = true;
                }
                else if (slashn_pos==pos+width) 
                {
                    ++extraSkip;
                    firstInLine = true;
                }
            }
            
            if (pos+width<explanation.length())
            {
                // now reduce width until we get a space or a return
                // so that we ensure that whole words are printed.
                while (width>0 && 
                       explanation[pos+width]!=' ' && 
                       explanation[pos+width]!='\n') --width;
                       
                if (width==0)
                {
                    // word must be longer than a whole line so will need
                    // to concatinate it.
                    width = explanationWidth-1;
                    concatinated = true;
                }
            }

            line.replace(explanationPos+offset,explanationWidth, explanation, pos, width);

            if (concatinated) { str += line; str += "-\n"; }
            else { str += line; str += "\n"; }
            
            // move to the next line of output.
            line.assign(fullWidth,' ');
            
            pos += width+extraSkip;

            
        }
                
    }
}

void ApplicationUsage::write(std::ostream& output, const ApplicationUsage::UsageMap& um,unsigned int widthOfOutput,bool showDefaults,const ApplicationUsage::UsageMap& ud)
{
    std::string str;
    getFormattedString(str, um, widthOfOutput, showDefaults, ud);
    output << str << std::endl;
}

void ApplicationUsage::write(std::ostream& output, unsigned int type, unsigned int widthOfOutput, bool showDefaults)
{

    output << "Usage: "<<getCommandLineUsage()<<std::endl;
    bool needspace = false;
    if ((type&COMMAND_LINE_OPTION) && !getCommandLineOptions().empty())
    {
        if (needspace) output << std::endl;
        output << "Options";
        if (showDefaults) output << " [and default value]";
        output << ":"<<std::endl;
        write(output,getCommandLineOptions(),widthOfOutput,showDefaults,getCommandLineOptionsDefaults());
        needspace = true;
    }
    
    if ((type&ENVIRONMENTAL_VARIABLE) && !getEnvironmentalVariables().empty())
    {
        if (needspace) output << std::endl;
        output << "Environmental Variables";
        if (showDefaults) output << " [and default value]";
        output << ":"<<std::endl;
        write(output,getEnvironmentalVariables(),widthOfOutput,showDefaults,getEnvironmentalVariablesDefaults());
        needspace = true;
    }

    if ((type&KEYBOARD_MOUSE_BINDING) && !getKeyboardMouseBindings().empty())
    {
        if (needspace) output << std::endl;
        output << "Keyboard and Mouse Bindings:"<<std::endl;
        write(output,getKeyboardMouseBindings(),widthOfOutput);
        needspace = true;
    }

}

