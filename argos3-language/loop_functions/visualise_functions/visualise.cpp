#include "visualise.h"

#include <algorithm>
#include <cstring>
#include <cerrno>

/****************************************/
/****************************************/

CVisualise::CVisualise() {
}

/****************************************/
/****************************************/

CVisualise::~CVisualise() {
   /* Nothing to do */
}

/****************************************/
/****************************************/

void CVisualise::Init(TConfigurationNode& t_tree) {
    /* Get output file name from XML tree */
    GetNodeAttribute(t_tree, "input", m_strInFile);
    int step;
    GetNodeAttribute(t_tree, "step", step);
    /* Open the file for text read */
    m_cInFile.open(m_strInFile.c_str(), std::ofstream::in);
    if(m_cInFile.fail()) {
        THROW_ARGOSEXCEPTION("Error opening file \"" << m_strInFile << "\": " << ::strerror(errno));
    }
   
    std::string l, line;
    int i = 1;
    while (std::getline(m_cInFile, l))
    {
        line = l;
        if(i==step)
            break;
        ++i;
    }
    
    //if the step is 0 or bigger than the number of step, we'll get the last step
    int pos = line.find_first_of("(),");
    while(pos != std::string::npos) {
        line.erase(pos,1);
        pos = line.find_first_of("(),",pos);
    }
    
    pos = line.find("N");
    while(pos != std::string::npos) {
        line.replace(pos,3,"0");
        pos = line.find("N");
    }
    
    float a, b, c;
    std::istringstream iss(line);
    iss >> i;
    CSpace::TMapPerType& cFBMap = GetSpace().GetEntitiesByType("foot-bot");
    CSpace::TMapPerType::iterator it = cFBMap.begin();
    while(iss >> a >> b >> c) {
        CFootBotEntity& footbotEntity = *any_cast<CFootBotEntity*>(it->second);
        footbotEntity.GetEmbodiedEntity().GetOriginAnchor().Position.SetX(a);
        footbotEntity.GetEmbodiedEntity().GetOriginAnchor().Position.SetY(b);
        (static_cast<CFootBotAggregation_NG&>(footbotEntity.GetControllableEntity().GetController())).hear(c);
        ++it;
    }
}

/****************************************/
/****************************************/

void CVisualise::Reset() {
   /* Close the output file */
   m_cInFile.close();
   if(m_cInFile.fail()) {
      THROW_ARGOSEXCEPTION("Error closing file \"" << m_strInFile << "\": " << ::strerror(errno));
   }
   /* Open the file for text writing */
   m_cInFile.open(m_strInFile.c_str(), std::ofstream::in);
   if(m_cInFile.fail()) {
      THROW_ARGOSEXCEPTION("Error opening file \"" << m_strInFile << "\": " << ::strerror(errno));
   }
}

/****************************************/
/****************************************/

void CVisualise::Destroy() {
   /* Close the output file */
   m_cInFile.close();
   if(m_cInFile.fail()) {
      THROW_ARGOSEXCEPTION("Error closing file \"" << m_strInFile << "\": " << ::strerror(errno));
   }
}

/****************************************/
/****************************************/

/* Register this loop functions into the ARGoS plugin system */
REGISTER_LOOP_FUNCTIONS(CVisualise, "visualise_functions");
