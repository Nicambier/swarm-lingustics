#include "aggregation.h"

#include <algorithm>
#include <cstring>
#include <cerrno>

/****************************************/
/****************************************/

static const Real OBJECT_RADIUS            = 0.1f;
static const Real OBJECT_DIAMETER          = OBJECT_RADIUS * 2.0f;
static const Real CONSTRUCTION_AREA_MIN_X  = 4.5f;
static const Real CONSTRUCTION_AREA_MAX_X  = 5.2f;
static const Real CONSTRUCTION_AREA_SIZE_X = CONSTRUCTION_AREA_MAX_X - CONSTRUCTION_AREA_MIN_X;
static const Real CONSTRUCTION_AREA_SIZE_Y = 4.0f - 0.2f - 0.2f; // arena size_y - west_wall size_y - east_wall size_y

/****************************************/
/****************************************/

CAggregation::CAggregation() {
}

/****************************************/
/****************************************/

CAggregation::~CAggregation() {
   /* Nothing to do */
}

/****************************************/
/****************************************/

void CAggregation::Init(TConfigurationNode& t_tree) {
   /* Get output file name from XML tree */
   GetNodeAttribute(t_tree, "output", m_strOutFile);
   /* Open the file for text writing */
   m_cOutFile.open(m_strOutFile.c_str(), std::ofstream::out | std::ofstream::trunc);
   if(m_cOutFile.fail()) {
      THROW_ARGOSEXCEPTION("Error opening file \"" << m_strOutFile << "\": " << ::strerror(errno));
   }
}

/****************************************/
/****************************************/

void CAggregation::Reset() {
   /* Close the output file */
   m_cOutFile.close();
   if(m_cOutFile.fail()) {
      THROW_ARGOSEXCEPTION("Error closing file \"" << m_strOutFile << "\": " << ::strerror(errno));
   }
   /* Open the file for text writing */
   m_cOutFile.open(m_strOutFile.c_str(), std::ofstream::out | std::ofstream::trunc);
   if(m_cOutFile.fail()) {
      THROW_ARGOSEXCEPTION("Error opening file \"" << m_strOutFile << "\": " << ::strerror(errno));
   }
}

/****************************************/
/****************************************/

void CAggregation::Destroy() {
   /* Close the output file */
   m_cOutFile.close();
   if(m_cOutFile.fail()) {
      THROW_ARGOSEXCEPTION("Error closing file \"" << m_strOutFile << "\": " << ::strerror(errno));
   }
}

/****************************************/
/****************************************/

void CAggregation::PreStep() {
   /* Nothing to do */
}

/****************************************/
/****************************************/

void CAggregation::PostStep() {
    m_cOutFile << GetSpace().GetSimulationClock() << "\t";
    CSpace::TMapPerType& cFBMap = GetSpace().GetEntitiesByType("foot-bot");
		for(CSpace::TMapPerType::iterator it = cFBMap.begin(); it != cFBMap.end(); ++it) {
			CFootBotEntity& footbotEntity = *any_cast<CFootBotEntity*>(it->second);
			Real Robot_X = footbotEntity.GetEmbodiedEntity().GetOriginAnchor().Position.GetX();
			Real Robot_Y = footbotEntity.GetEmbodiedEntity().GetOriginAnchor().Position.GetY();
            std::string state = (static_cast<CFootBotAggregation&>(footbotEntity.GetControllableEntity().GetController())).GetState();
			m_cOutFile << "(" << Robot_X << ", " << Robot_Y << ", " << state << ") ";
		}
     m_cOutFile << std::endl;
}

bool CAggregation::IsExperimentFinished() {
    bool stabilised = true;
    CSpace::TMapPerType& cFBMap = GetSpace().GetEntitiesByType("foot-bot");
		for(CSpace::TMapPerType::iterator it = cFBMap.begin(); it != cFBMap.end() and stabilised; ++it) {
            CFootBotEntity& footbotEntity = *any_cast<CFootBotEntity*>(it->second);
			CFootBotAggregation& footbot = static_cast<CFootBotAggregation&>(footbotEntity.GetControllableEntity().GetController());
			if(footbot.LastMove() < 600)
                stabilised = false;
		}
     return stabilised;
}

/****************************************/
/****************************************/

/* Register this loop functions into the ARGoS plugin system */
REGISTER_LOOP_FUNCTIONS(CAggregation, "aggregation_loop_functions");
