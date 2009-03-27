#pragma ident "$Id$"

/**
 * @file ObsID.cpp
 * gpstk::ObsID - Identifies types of observations
 */

//============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 2.1 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
//  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//  Copyright 2004, The University of Texas at Austin
//
//============================================================================

//============================================================================
//
//This software developed by Applied Research Laboratories at the University of
//Texas at Austin, under contract to an agency or agencies within the U.S. 
//Department of Defense. The U.S. Government retains all rights to use,
//duplicate, distribute, disclose, or release this software. 
//
//Pursuant to DoD Directive 523024 
//
// DISTRIBUTION STATEMENT A: This software has been approved for public 
//                           release, distribution is unlimited.
//
//=============================================================================



#include "ObsID.hpp"


namespace gpstk
{
   std::map< ObsID::TrackingCode,    std::string > ObsID::tcStrings;
   std::map< ObsID::CarrierBand,     std::string > ObsID::cbStrings;
   std::map< ObsID::ObservationType, std::string > ObsID::otStrings;

   ObsID::Initializer singleton;

   ObsID::Initializer::Initializer()
   {
      otStrings[otUnknown]  = "Unknown Type";
      otStrings[otRange]    = "pseudorange";
      otStrings[otPhase]    = "phase";
      otStrings[otDoppler]  = "Doppler";
      otStrings[otSNR]      = "SNR";
      otStrings[otSSI]      = "SSI";
      otStrings[otLLI]      = "LLI";
      otStrings[otTrackLen] = "Track Length";
      otStrings[otLast]     = "otLast";
      otStrings[otPlaceholder] = "otPlaceholder";

      cbStrings[cbUnknown] = "Unknown Band";
      cbStrings[cbL1]      = "L1/E1";
      cbStrings[cbL2]      = "L2";
      cbStrings[cbL5]      = "L5/E5a";
      cbStrings[cbL1L2]    = "L1+L2";
      cbStrings[cbG1]      = "G1";
      cbStrings[cbG2]      = "G2";
      cbStrings[cbE5b]     = "E5b";
      cbStrings[cbE5ab]    = "L5a+b";
      cbStrings[cbLast]    = "cbLast";
      cbStrings[cbPlaceholder] = "cbPlaceholder";

      tcStrings[tcUnknown] = "Unknown Code";
      tcStrings[tcCA]      = "C/A";
      tcStrings[tcP]       = "P";
      tcStrings[tcY]       = "Y";
      tcStrings[tcW]       = "W";
      tcStrings[tcN]       = "N";
      tcStrings[tcM]       = "M";
      tcStrings[tcC2M]     = "C2M";
      tcStrings[tcC2L]     = "C2L";
      tcStrings[tcC2LM]    = "C2L+M";
      tcStrings[tcI5]      = "I5";
      tcStrings[tcQ5]      = "Q5";
      tcStrings[tcIQ5]     = "I+Q";
      tcStrings[tcA]       = "A";
      tcStrings[tcB]       = "B";
      tcStrings[tcC]       = "C";
      tcStrings[tcBC]      = "B+C";
      tcStrings[tcABC]     = "A+B+C";
      tcStrings[tcLast]    = "tcLast";
      tcStrings[tcPlaceholder] = "tcPlaceholder";
   }


   // Convenience output method
   std::ostream& ObsID::dump(std::ostream& s) const
   {
      s << ObsID::cbStrings[band] << " "
        << ObsID::tcStrings[code] << " "
        << ObsID::otStrings[type];

      return s;
   }


   bool ObsID::isValid() const
   {
      // At some time this needs to be implimented.
      return false;
   }


   /* Static method to add new ObservationType's
    * @param s      Identifying string for the new ObservationType
    */
   ObsID::ObservationType ObsID::newObservationType(const std::string& s)
   {
      ObservationType newId =
         static_cast<ObservationType>(ObsID::otStrings.rbegin()->first + 1);

      ObsID::otStrings[newId] = s;

      return newId;
   }


   /* Static method to add new CarrierBand's
    * @param s      Identifying string for the new CarrierBand
    */
   ObsID::CarrierBand ObsID::newCarrierBand(const std::string& s)
   {
      CarrierBand newId =
         static_cast<CarrierBand>(ObsID::cbStrings.rbegin()->first + 1);

      ObsID::cbStrings[newId] = s;

      return newId;
   }


   /* Static method to add new TrackingCode's
    * @param s      Identifying string for the new TrackingCode
    */
   ObsID::TrackingCode ObsID::newTrackingCode(const std::string& s)
   {
      TrackingCode newId =
         static_cast<TrackingCode>(ObsID::tcStrings.rbegin()->first + 1);

      ObsID::tcStrings[newId] = s;

      return newId;
   }


   // Equality requires all fields to be the same
   bool ObsID::operator==(const ObsID& right) const
   { return type==right.type &&  band==right.band && code==right.code; }


   // This ordering is somewhat arbitrary but is required to be able to
   // use an ObsID as an index to a std::map.  If an application needs
   // some other ordering, inherit and override this function.
   bool ObsID::operator<(const ObsID& right) const
   {
      if (band == right.band)
         if (code == right.code)
            return type < right.type;
         else
            return code < right.code;
      else
         return band < right.band;

      // This should never be reached...
      return false;
   }


   namespace StringUtils
   {
      // convert this object to a string representation
      std::string asString(const ObsID& p)
      {
         std::ostringstream oss;
         p.dump(oss);
         return oss.str();
      }
   }


   // stream output for ObsID
   std::ostream& operator<<(std::ostream& s, const ObsID& p)
   {
      p.dump(s);
      return s;
   }

}
