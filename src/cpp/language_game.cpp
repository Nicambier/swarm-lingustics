#include "language_game.h"

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

CLanguageGame::CLanguageGame() {
}

/****************************************/
/****************************************/

CLanguageGame::~CLanguageGame() {
   /* Nothing to do */
}

/****************************************/
/****************************************/

void CLanguageGame::Init(TConfigurationNode& t_tree) {
   /* Get output file name from XML tree */
   //GetNodeAttribute(t_tree, "output", m_strOutFile);
   /* Open the file for text writing */
   /*m_cOutFile.open(m_strOutFile.c_str(), std::ofstream::out | std::ofstream::trunc);
   if(m_cOutFile.fail()) {
      THROW_ARGOSEXCEPTION("Error opening file \"" << m_strOutFile << "\": " << ::strerror(errno));
   }*/
   /* Write a header line */
   //m_cOutFile << "Word\t# Bots\t" << std::endl;
}

/****************************************/
/****************************************/

void CLanguageGame::Reset() {
   /* Close the output file */
   /*m_cOutFile.close();
   if(m_cOutFile.fail()) {
      THROW_ARGOSEXCEPTION("Error closing file \"" << m_strOutFile << "\": " << ::strerror(errno));
   }*/
   /* Open the file for text writing */
   /*m_cOutFile.open(m_strOutFile.c_str(), std::ofstream::out | std::ofstream::trunc);
   if(m_cOutFile.fail()) {
      THROW_ARGOSEXCEPTION("Error opening file \"" << m_strOutFile << "\": " << ::strerror(errno));
   }*/
   /* Write a header line */
   //m_cOutFile << "Word\t# Bots\t" << std::endl;
}

/****************************************/
/****************************************/

void CLanguageGame::Destroy() {
   /* Close the output file */
   /*m_cOutFile.close();
   if(m_cOutFile.fail()) {
      THROW_ARGOSEXCEPTION("Error closing file \"" << m_strOutFile << "\": " << ::strerror(errno));
   }*/
}

/****************************************/
/****************************************/

void CLanguageGame::PreStep() {
   /* Nothing to do */
}

/****************************************/
/****************************************/

void CLanguageGame::PostStep() {
    //std::string txt = "caca";
   /* Output a line for this step */
   /*m_cOutFile << txt << "\t"
              << 1 << "\t"
              << std::endl;*/
}

/****************************************/
/****************************************/

CColor CLanguageGame::GetFloorColor(const CVector2& c_position_on_plane) {
   /* Check if the given point is within the construction area */
   if(c_position_on_plane.GetX() >= CONSTRUCTION_AREA_MIN_X &&
      c_position_on_plane.GetX() <= CONSTRUCTION_AREA_MAX_X) {
      /* Yes, it is - return black */
      return CColor::BLACK;
   }
   /* No, it isn't - return white */
   return CColor::WHITE;
}

/****************************************/
/****************************************/

bool ObjectYCoordCompare(const CVector3& c_vec1,
                         const CVector3& c_vec2) {
   return c_vec1.GetY() < c_vec2.GetY();
}

/****************************************/
/****************************************/

/* Register this loop functions into the ARGoS plugin system */
REGISTER_LOOP_FUNCTIONS(CLanguageGame, "language_game");
