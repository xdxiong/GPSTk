#pragma ident "$Id: Rinex3ObsHeader.cpp 1709 2009-02-18 20:27:47Z btolman $"

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

/**
 * @file Rinex3ObsHeader.cpp
 * Encapsulate header of Rinex observation file, including I/O
 */

#include "StringUtils.hpp"
#include "SystemTime.hpp"
#include "Rinex3ObsStream.hpp"
#include "Rinex3ObsHeader.hpp"

using namespace std;
using namespace gpstk::StringUtils;

namespace gpstk
{
   const string Rinex3ObsHeader::stringVersion           = "RINEX VERSION / TYPE";
   const string Rinex3ObsHeader::stringRunBy             = "PGM / RUN BY / DATE";
   const string Rinex3ObsHeader::stringComment           = "COMMENT";
   const string Rinex3ObsHeader::stringMarkerName        = "MARKER NAME";
   const string Rinex3ObsHeader::stringMarkerNumber      = "MARKER NUMBER";
   const string Rinex3ObsHeader::stringMarkerType        = "MARKER TYPE";
   const string Rinex3ObsHeader::stringObserver          = "OBSERVER / AGENCY";
   const string Rinex3ObsHeader::stringReceiver          = "REC # / TYPE / VERS";
   const string Rinex3ObsHeader::stringAntennaType       = "ANT # / TYPE";
   const string Rinex3ObsHeader::stringAntennaPosition   = "APPROX POSITION XYZ";
   const string Rinex3ObsHeader::stringAntennaDeltaHEN   = "ANTENNA: DELTA H/E/N";
   const string Rinex3ObsHeader::stringAntennaDeltaXYZ   = "ANTENNA: DELTA X/Y/Z";
   const string Rinex3ObsHeader::stringAntennaPhaseCtr   = "ANTENNA: PHASECENTER";
   const string Rinex3ObsHeader::stringAntennaBsightXYZ  = "ANTENNA: B.SIGHT XYZ";
   const string Rinex3ObsHeader::stringAntennaZeroDirAzi = "ANTENNA: ZERODIR AZI";
   const string Rinex3ObsHeader::stringAntennaZeroDirXYZ = "ANTENNA: ZERODIR XYZ";
   const string Rinex3ObsHeader::stringCenterOfMass      = "CENTER OF MASS: XYZ";
   const string Rinex3ObsHeader::stringSystemNumObs      = "SYS / # / OBS TYPES";
   const string Rinex3ObsHeader::stringSigStrengthUnit   = "SIGNAL STRENGTH UNIT";
   const string Rinex3ObsHeader::stringInterval          = "INTERVAL";
   const string Rinex3ObsHeader::stringFirstTime         = "TIME OF FIRST OBS";
   const string Rinex3ObsHeader::stringLastTime          = "TIME OF LAST OBS";
   const string Rinex3ObsHeader::stringReceiverOffset    = "RCV CLOCK OFFS APPL";
   const string Rinex3ObsHeader::stringSystemDCBSapplied = "SYS / DCBS APPLIED";
   const string Rinex3ObsHeader::stringSystemPCVSapplied = "SYS / PCVS APPLIED";
   const string Rinex3ObsHeader::stringSystemScaleFac    = "SYS / SCALE FACTOR";
   const string Rinex3ObsHeader::stringLeapSeconds       = "LEAP SECONDS";
   const string Rinex3ObsHeader::stringNumSats           = "# OF SATELLITES";
   const string Rinex3ObsHeader::stringPrnObs            = "PRN / # OF OBS";
   const string Rinex3ObsHeader::stringEoH               = "END OF HEADER";


//   std::vector<Rinex3ObsHeader::Rinex3ObsType> Rinex3ObsHeader::RegisteredRinex3ObsTypes
//      = Rinex3ObsHeader::StandardRinex3ObsTypes;


   void Rinex3ObsHeader::reallyPutRecord(FFStream& ffs) const
      throw(std::exception, FFStreamError, StringException)
   {
      cout << "***** Rinex3ObsHeader::reallyPutRecord():" << endl;

      Rinex3ObsStream& strm = dynamic_cast<Rinex3ObsStream&>(ffs);

      cout << "      dynamic cast for Rinex3ObsStream" << endl;

//      strm.header = *this;

      cout << "      strm.header assigned to *this" << endl;

      unsigned long allValid;
      if      (version == 3.0)   allValid = allValid30;
      else if (version == 3.00)  allValid = allValid30;
      else
      {
         FFStreamError err("Unknown RINEX version: " + asString(version,2));
         err.addText("Make sure to set the version correctly.");
         GPSTK_THROW(err);
      }

      cout << "      allValid assigned" << endl;

      if ((valid & allValid) != allValid)
      {
         cout << "Version = " << version << endl;
         cout << "allValid30 = " << hex << setfill('0') << setw(2) << nouppercase << allValid30 << endl;
         cout << "allValid   = " << hex << setfill('0') << setw(2) << nouppercase << allValid << endl;
         cout << "   valid   = " << hex << setfill('0') << setw(2) << nouppercase << valid << endl;
         cout << "      OOPS." << endl;
         cout << "Version     " << (valid & validVersion        ) << endl;
         cout << "Run By      " << (valid & validRunBy          ) << endl;
         cout << "Marker Name " << (valid & validMarkerName     ) << endl;
         cout << "Marker Type " << (valid & validMarkerType     ) << endl;
         cout << "Observer    " << (valid & validObserver       ) << endl;
         cout << "Receiver    " << (valid & validReceiver       ) << endl;
         cout << "Antenna Type" << (valid & validAntennaType    ) << endl;
         cout << "Antenna DHEN" << (valid & validAntennaDeltaHEN) << endl;
         cout << "Sys Obs Type" << (valid & validSystemObsType  ) << endl;
         cout << "Interval    " << (valid & validInterval       ) << endl;
         cout << "First Time  " << (valid & validFirstTime      ) << endl;
         cout << "End Header  " << (valid & validEoH            ) << endl;
         FFStreamError err("Incomplete or invalid header.");
         err.addText("Make sure you set all header valid bits for all of the available data.");
         GPSTK_THROW(err);
      }

      try
      {
         cout << "      trying to call WriteheaderRecords(strm)" << endl;
         WriteHeaderRecords(strm);
      }
      catch(FFStreamError& e)
      {
         GPSTK_RETHROW(e);
      }
      catch(StringException& e)
      {
         GPSTK_RETHROW(e);
      }

   }  // end Rinex3ObsHeader::reallyPutRecord


   // This function computes the number of valid header records which WriteHeaderRecords will write.
   int Rinex3ObsHeader::NumberHeaderRecordsToBeWritten(void) const throw()
   {
      int n = 0;

      if (valid & Rinex3ObsHeader::validVersion          ) n++;
      if (valid & Rinex3ObsHeader::validRunBy            ) n++;
      if (valid & Rinex3ObsHeader::validComment          ) n += commentList.size();
      if (valid & Rinex3ObsHeader::validMarkerName       ) n++;
      if (valid & Rinex3ObsHeader::validMarkerNumber     ) n++;
      if (valid & Rinex3ObsHeader::validMarkerType       ) n++;
      if (valid & Rinex3ObsHeader::validObserver         ) n++;
      if (valid & Rinex3ObsHeader::validReceiver         ) n++;
      if (valid & Rinex3ObsHeader::validAntennaType      ) n++;
      if (valid & Rinex3ObsHeader::validAntennaPosition  ) n++;
      if (valid & Rinex3ObsHeader::validAntennaDeltaHEN  ) n++;
      if (valid & Rinex3ObsHeader::validAntennaDeltaXYZ  ) n++;
      if (valid & Rinex3ObsHeader::validAntennaPhaseCtr  ) n++;
      if (valid & Rinex3ObsHeader::validAntennaBsightXYZ ) n++;
      if (valid & Rinex3ObsHeader::validAntennaZeroDirAzi) n++;
      if (valid & Rinex3ObsHeader::validAntennaZeroDirXYZ) n++;
      if (valid & Rinex3ObsHeader::validCenterOfMass     ) n++;
      if (valid & Rinex3ObsHeader::validSystemObsType    ) n += 1 + (obsTypeList.size()-1)/9;
      if (valid & Rinex3ObsHeader::validSigStrengthUnit  ) n++;
      if (valid & Rinex3ObsHeader::validInterval         ) n++;
      if (valid & Rinex3ObsHeader::validFirstTime        ) n++;
      if (valid & Rinex3ObsHeader::validLastTime         ) n++;
      if (valid & Rinex3ObsHeader::validReceiverOffset   ) n++;
      if (valid & Rinex3ObsHeader::validSystemDCBSapplied) n++;
      if (valid & Rinex3ObsHeader::validSystemPCVSapplied) n++;
      if (valid & Rinex3ObsHeader::validSystemScaleFac   ) n++;
      if (valid & Rinex3ObsHeader::validLeapSeconds      ) n++;
      if (valid & Rinex3ObsHeader::validNumSats          ) n++;
      if (valid & Rinex3ObsHeader::validPrnObs           )
         n += numObsForSat.size() * (1+numObsForSat.begin()->second.size()/9);
      if (valid & Rinex3ObsHeader::validEoH              ) n++;

      return n;
   }


   // This function writes all valid header records.
   void Rinex3ObsHeader::WriteHeaderRecords(FFStream& ffs) const
      throw(FFStreamError, StringException)
   {
      Rinex3ObsStream& strm = dynamic_cast<Rinex3ObsStream&>(ffs);
      string line;
      if (valid & validVersion)
      {
         cout << "*** Version" << endl;
         line  = rightJustify(asString(version,2), 9);
         line += string(11, ' ');

         if ((fileType[0] != 'O') && (fileType[0] != 'o'))
         {
            FFStreamError err("This isn't a Rinex Observation file: " +
                              fileType.substr(0,1));
            GPSTK_THROW(err);
         }

         if (system.system == RinexSatID::systemUnknown)
         {
            FFStreamError err("Invalid satellite system");
            GPSTK_THROW(err);
         }

         line += leftJustify(string("Observation data"), 20);
         std::string str;
         str = system.systemChar();
         str = str + " (" + system.systemString() + ")";
         line += leftJustify(str, 20);
         line += stringVersion;
         strm << line << endl;
         strm.lineNumber++;
      }
      if (valid & validRunBy)
      {
         cout << "*** Run By" << endl;
         line  = leftJustify(fileProgram, 20);
         line += leftJustify(fileAgency , 20);
         SystemTime sysTime;
         string curDate = (static_cast<CivilTime>(sysTime)).printf("%04Y%02m%02d %02H%02M%02S %P");
         line += leftJustify(curDate, 20);
         line += stringRunBy;
         strm << line << endl;
         strm.lineNumber++;
      }
      if (valid & validComment)
      {
         cout << "*** Comments" << endl;
         vector<string>::const_iterator itr = commentList.begin();
         while (itr != commentList.end())
         {
            line  = leftJustify((*itr), 60);
            line += stringComment;
            strm << line << endl;
            strm.lineNumber++;
            itr++;
         }
      }
      if (valid & validMarkerName)
      {
         cout << "*** Marker Name" << endl;
         line  = leftJustify(markerName, 60);
         line += stringMarkerName;
         strm << line << endl;
         strm.lineNumber++;
      }
      if (valid & validMarkerNumber)
      {
         cout << "*** Marker Number" << endl;
         line  = leftJustify(markerNumber, 20);
         line += string(40, ' ');
         line += stringMarkerNumber;
         strm << line << endl;
         strm.lineNumber++;
      }
      if (valid & validMarkerType)
      {
         cout << "*** Marker Type" << endl;
         line  = leftJustify(markerType, 20);
         line += string(40, ' ');
         line += stringMarkerType;
         strm << line << endl;
         strm.lineNumber++;
      }
      if (valid & validObserver)
      {
         cout << "*** Observer" << endl;
         line  = leftJustify(observer, 20);
         line += leftJustify(agency  , 40);
         line += stringObserver;
         strm << line << endl;
         strm.lineNumber++;
      }
      if (valid & validReceiver)
      {
         cout << "*** Receiver" << endl;
         line  = leftJustify(recNo  , 20);
         line += leftJustify(recType, 20);
         line += leftJustify(recVers, 20);
         line += stringReceiver;
         strm << line << endl;
         strm.lineNumber++;
      }
      if (valid & validAntennaType)
      {
         cout << "*** Antenna Type" << endl;
         line  = leftJustify(antNo  , 20);
         line += leftJustify(antType, 20);
         line += string(20, ' ');
         line += stringAntennaType;
         strm << line << endl;
         strm.lineNumber++;
      }
      if (valid & validAntennaPosition)
      {
         cout << "*** Antenna Position" << endl;
         line  = rightJustify(asString(antennaPosition[0], 4), 14);
         line += rightJustify(asString(antennaPosition[1], 4), 14);
         line += rightJustify(asString(antennaPosition[2], 4), 14);
         line += string(18, ' ');
         line += stringAntennaPosition;
         strm << line << endl;
         strm.lineNumber++;
      }
      if (valid & validAntennaDeltaHEN)
      {
         cout << "*** Antenna Delta HEN" << endl;
         line  = rightJustify(asString(antennaDeltaHEN[0], 4), 14);
         line += rightJustify(asString(antennaDeltaHEN[1], 4), 14);
         line += rightJustify(asString(antennaDeltaHEN[2], 4), 14);
         line += string(18, ' ');
         line += stringAntennaDeltaHEN;
         strm << line << endl;
         strm.lineNumber++;
      }
      if (valid & validAntennaDeltaXYZ)
      {
         cout << "*** Antenna Delta XYZ" << endl;
         line  = rightJustify(asString(antennaDeltaXYZ[0], 4), 14);
         line += rightJustify(asString(antennaDeltaXYZ[1], 4), 14);
         line += rightJustify(asString(antennaDeltaXYZ[2], 4), 14);
         line += string(18, ' ');
         line += stringAntennaDeltaXYZ;
         strm << line << endl;
         strm.lineNumber++;
      }
      if (valid & validAntennaPhaseCtr)
      {
         cout << "*** Antenna Phase Center" << endl;
         line  =  leftJustify(antennaSatSys , 1);
         line += rightJustify(antennaObsCode, 3);
         line += rightJustify(asString(antennaPhaseCtr[0], 4),  9);
         line += rightJustify(asString(antennaPhaseCtr[1], 4), 14);
         line += rightJustify(asString(antennaPhaseCtr[2], 4), 14);
         line += string(18, ' ');
         line += stringAntennaPhaseCtr;
         strm << line << endl;
         strm.lineNumber++;
      }
      if (valid & validAntennaBsightXYZ)
      {
         cout << "*** Antenna Boresight XYZ" << endl;
         line  = rightJustify(asString(antennaBsightXYZ[0], 4), 14);
         line += rightJustify(asString(antennaBsightXYZ[1], 4), 14);
         line += rightJustify(asString(antennaBsightXYZ[2], 4), 14);
         line += string(18, ' ');
         line += stringAntennaBsightXYZ;
         strm << line << endl;
         strm.lineNumber++;
      }
      if (valid & validAntennaZeroDirAzi)
      {
         cout << "*** Antenna Zero Dir Azi" << endl;
         line  = rightJustify(asString(antennaZeroDirAzi, 4), 14);
         line += string(46, ' ');
         line += stringAntennaZeroDirAzi;
         strm << line << endl;
         strm.lineNumber++;
      }
      if (valid & validAntennaZeroDirXYZ)
      {
         cout << "*** Antenna Zero Dir XYZ" << endl;
         line  = rightJustify(asString(antennaZeroDirXYZ[0], 4), 14);
         line += rightJustify(asString(antennaZeroDirXYZ[1], 4), 14);
         line += rightJustify(asString(antennaZeroDirXYZ[2], 4), 14);
         line += string(18, ' ');
         line += stringAntennaZeroDirXYZ;
         strm << line << endl;
         strm.lineNumber++;
      }
      if (valid & validCenterOfMass)
      {
         cout << "*** Center of Mass" << endl;
         line  = rightJustify(asString(centerOfMass[0], 4), 14);
         line += rightJustify(asString(centerOfMass[1], 4), 14);
         line += rightJustify(asString(centerOfMass[2], 4), 14);
         line += string(18, ' ');
         line += stringCenterOfMass;
         strm << line << endl;
         strm.lineNumber++;
      }
      if (valid & validSystemObsType)
      {
         cout << "*** System Obs Type" << endl;
         const int maxObsPerLine = 13;

         std::map<std::string,vector<ObsID> >::iterator mapIter;
         for (mapIter == mapObsTypes.begin(); mapIter != mapObsTypes.end(); mapIter++)
         {
            int obsWritten = 0;
            line = ""; // make sure the line contents are reset.

            vector<ObsID>::const_iterator listItr = (mapIter->second).begin();

            while (listItr != (mapIter->second).end())
            {
               // the first line needs to have the GNSS type and # of obs
               if (obsWritten == 0)
               {
                  line +=  leftJustify(mapIter->first, 1);
                  line += string(2, ' ');
                  line  = rightJustify(asString(obsTypeList.size()), 3);
               }
               // if you hit 13, write out the line and start a new one
               else if ((obsWritten % maxObsPerLine) == 0)
               {
                  line += stringSystemNumObs;
                  strm << line << endl;
                  strm.lineNumber++;
                  line  = string(6, ' ');
               }
               line += string(1, ' ');
               line += rightJustify(asString(*listItr), 3);
               obsWritten++;
               listItr++;
            }
            line += string(60 - line.size(), ' ');
            line += stringSystemNumObs;
            strm << line << endl;
            strm.lineNumber++;

         }
      }
      if (valid & validSigStrengthUnit)
      {
         line  = leftJustify(sigStrengthUnit, 20);
         line += string(40, ' ');
         line += stringSigStrengthUnit;
         strm << line << endl;
         strm.lineNumber++;
      }
      if (valid & validInterval)
      {
         line  = rightJustify(asString(interval, 3), 10);
         line += string(50, ' ');
         line += stringInterval;
         strm << line << endl;
         strm.lineNumber++;
      }
      if (valid & validFirstTime)
      {
         line  = writeTime(firstObs);
         line += string(60 - line.size(), ' ');
         line += stringFirstTime;
         strm << line << endl;
         strm.lineNumber++;
      }
      if (valid & validLastTime)
      {
         line  = writeTime(lastObs);
         line += string(60 - line.size(), ' ');
         line += stringLastTime;
         strm << line << endl;
         strm.lineNumber++;
      }
      if (valid & validReceiverOffset)
      {
         line  = rightJustify(asString(receiverOffset), 6);
         line += string(54, ' ');
         line += stringReceiverOffset;
         strm << line << endl;
         strm.lineNumber++;
      }
      if (valid & validSystemDCBSapplied)
      {
         for (int i = 0; i < infoDCBS.size(); i++)
         {
           line  = rightJustify(infoDCBS[i].satSys,  1);
           line += string(1, ' ');
           line += rightJustify(infoDCBS[i].name  , 17);
           line += string(1, ' ');
           line += rightJustify(infoDCBS[i].source, 40);
           line += stringSystemDCBSapplied;
           strm << line << endl;
           strm.lineNumber++;
         }
      }
      if (valid & validSystemPCVSapplied)
      {
         for (int i = 0; i < infoPCVS.size(); i++)
         {
           line  = rightJustify(infoPCVS[i].satSys,  1);
           line += string(1, ' ');
           line += rightJustify(infoPCVS[i].name  , 17);
           line += string(1, ' ');
           line += rightJustify(infoPCVS[i].source, 40);
           line += stringSystemPCVSapplied;
           strm << line << endl;
           strm.lineNumber++;
         }
      }
      if (valid & validSystemScaleFac)
      {
         static const int maxObsPerLine = 12;

         static const int size = 4;
         int factors[size] = {1,10,100,1000};
         vector<std::string> obsTypes;

         std::map<std::string, sfacMap>::const_iterator mapIter;
         for (mapIter == sysSfacMap.begin(); mapIter != sysSfacMap.end(); mapIter++) // loop over GNSSes
         {
            std::map<ObsID, int>::const_iterator iter;

            for (int i = 0; i < size; i++) // loop over possible factors (above)
            {
               int count = 0;
               obsTypes.clear(); // clear the list of Obs Types we're going to make

               for (iter == mapIter->second.begin(); iter != mapIter->second.end(); iter++) // loop over scale factor map
               {
                  if ( iter->second == factors[i] )
                  {
                     count++;
                     obsTypes.push_back(asString(iter->first.type));
                  }
               }
               if ( count == 0 ) continue; // no entries with this factor

               line  =  leftJustify(mapIter->first, 1);
               line += string(1, ' ');
               line += rightJustify(asString(factors[i]), 4);
               line += string(2, ' ');
               line += rightJustify(asString(count     ), 2);

               for (i = 0; i < count; i++)
               {
                  if (i > 0 && (i % maxObsPerLine) == 0 )
                  {
                     line += stringSystemScaleFac;
                     strm << line << endl;
                     strm.lineNumber++;
                     line  = string(6, ' ');
                  }
                  line += string(1, ' ');
                  line += rightJustify(asString(iter->first.type), 3);
               }
               line += stringSystemScaleFac;
               strm << line << endl;
               strm.lineNumber++;
            }
         }
      }

      if (valid & validLeapSeconds)
      {
         line  = rightJustify(asString(leapSeconds), 6);
         line += string(54, ' ');
         line += stringLeapSeconds;
         strm << line << endl;
         strm.lineNumber++;
      }
      if (valid & validNumSats)
      {
         line  = rightJustify(asString(numSVs), 6);
         line += string(54, ' ');
         line += stringNumSats;
         strm << line << endl;
         strm.lineNumber++;
      }
      if (valid & validPrnObs)
      {
         const int maxObsPerLine = 9;
         map<SatID, vector<int> >::const_iterator itr = numObsForSat.begin();
         while (itr != numObsForSat.end())
         {
            int numObsWritten = 0;
            
            vector<int>::const_iterator vecItr = (*itr).second.begin();
            while (vecItr != (*itr).second.end())
            {
               if (numObsWritten == 0)
               {
                  try
                  {
                     RinexSatID prn((*itr).first);
                     line = string(3, ' ') + prn.toString();
                  }
                  catch (Exception& e)
                  {
                     FFStreamError ffse(e);
                     GPSTK_RETHROW(ffse); 
                  }
               }
               else if ((numObsWritten % maxObsPerLine) == 0)
               {
                  line += stringPrnObs;
                  strm << line << endl;
                  strm.lineNumber++;
                  line  = string(6, ' ');
               }
               line += rightJustify(asString(*vecItr), 6);
               ++vecItr;
               ++numObsWritten;
            }
            line += string(60 - line.size(), ' ');
            line += stringPrnObs;
            strm << line << endl;
            strm.lineNumber++;
            itr++;
         }
      }
      if (valid & validEoH)
      {
         line  = string(60, ' ');
         line += stringEoH;
         strm << line << endl;
         strm.lineNumber++;
      }   
   }   // end Rinex3ObsHeader::WriteHeaderRecords()


   // This function parses a single header record.
   void Rinex3ObsHeader::ParseHeaderRecord(string& line)
      throw(FFStreamError)
   {
      string label(line, 60, 20);
         
      if (label == stringVersion)
      {
         version  = asDouble(line.substr( 0,20));
         fileType = strip(   line.substr(20,20));
         satSys   = strip(   line.substr(40,20));
         if ( fileType[0] != 'O' && fileType[0] != 'o')
         {
            FFStreamError e("This isn't a RINEX 3 Obs file.");
            GPSTK_THROW(e);
         }
         if ( satSys[0] != 'G' && satSys[0] != 'g' &&
              satSys[0] != 'R' && satSys[0] != 'r' &&
              satSys[0] != 'E' && satSys[0] != 'e' &&
              satSys[0] != 'S' && satSys[0] != 's' &&
              satSys[0] != 'M' && satSys[0] != 'm'    )
         {
           FFStreamError e("The satellite system isn't valid.");
           GPSTK_THROW(e);
         }
         valid |= validVersion;
      }
      else if (label == stringRunBy)
      {
         fileProgram = strip(line.substr( 0,20));
         fileAgency  = strip(line.substr(20,20));
         date        = strip(line.substr(40,20));
         valid |= validRunBy;
      }
      else if (label == stringComment)
      {
         commentList.push_back(strip(line.substr(0,60)));
         valid |= validComment;
      }
      else if (label == stringMarkerName)
      {
         markerName = strip(line.substr(0,60));
         valid |= validMarkerName;
      }
      else if (label == stringMarkerNumber)
      {
         markerNumber = strip(line.substr(0,20));
         valid |= validMarkerNumber;
      }
      else if (label == stringMarkerType)
      {
         cout << "*** Trying to read MarkerType" << endl;
         markerType = strip(line.substr(0,20));
         cout << "*** Read MarkerType" << endl;
         cout << "*** MarkerType = " << markerType << endl;
         valid |= validMarkerType;
         cout << "*** Set validMarkerType" << endl;
      }
      else if (label == stringObserver)
      {
         observer = strip(line.substr( 0,20));
         agency   = strip(line.substr(20,40));
         valid |= validObserver;
      }
      else if (label == stringReceiver)
      {
         recNo   = strip(line.substr( 0,20));
         recType = strip(line.substr(20,20));
         recVers = strip(line.substr(40,20));
         valid |= validReceiver;
      }
      else if (label ==stringAntennaType)
      {
         antNo   = strip(line.substr( 0,20));
         antType = strip(line.substr(20,20));
         valid |= validAntennaType;
      }
      else if (label == stringAntennaPosition)
      {
         antennaPosition[0] = asDouble(line.substr( 0,14));
         antennaPosition[1] = asDouble(line.substr(14,14));
         antennaPosition[2] = asDouble(line.substr(28,14));
         valid |= validAntennaPosition;
      }
      else if (label == stringAntennaDeltaHEN)
      {
         antennaDeltaHEN[0] = asDouble(line.substr( 0,14));
         antennaDeltaHEN[1] = asDouble(line.substr(14,14));
         antennaDeltaHEN[2] = asDouble(line.substr(28,14));
         valid |= validAntennaDeltaHEN;
      }
      else if (label == stringAntennaDeltaXYZ)
      {
         antennaDeltaXYZ[0] = asDouble(line.substr( 0,14));
         antennaDeltaXYZ[1] = asDouble(line.substr(14,14));
         antennaDeltaXYZ[2] = asDouble(line.substr(28,14));
         valid |= validAntennaDeltaXYZ;
      }
      else if (label == stringAntennaPhaseCtr)
      {
         antennaSatSys  = strip(line.substr(0,2));
         antennaObsCode = strip(line.substr(2,3));
         antennaPhaseCtr[0] = asDouble(line.substr( 5, 9));
         antennaPhaseCtr[1] = asDouble(line.substr(14,14));
         antennaPhaseCtr[2] = asDouble(line.substr(28,14));
         valid |= validAntennaPhaseCtr;
      }
      else if (label == stringAntennaBsightXYZ)
      {
         antennaBsightXYZ[0] = asDouble(line.substr( 0,14));
         antennaBsightXYZ[1] = asDouble(line.substr(14,14));
         antennaBsightXYZ[2] = asDouble(line.substr(28,14));
         valid |= validAntennaBsightXYZ;
      }
      else if (label == stringAntennaZeroDirAzi)
      {
         antennaZeroDirAzi = asDouble(line.substr(0,14));
         valid |= validAntennaBsightXYZ;
      }
      else if (label == stringAntennaZeroDirXYZ)
      {
         antennaZeroDirXYZ[0] = asDouble(line.substr( 0,14));
         antennaZeroDirXYZ[1] = asDouble(line.substr(14,14));
         antennaZeroDirXYZ[2] = asDouble(line.substr(28,14));
         valid |= validAntennaBsightXYZ;
      }
      else if (label == stringCenterOfMass)
      {
         centerOfMass[0] = asDouble(line.substr( 0,14));
         centerOfMass[1] = asDouble(line.substr(14,14));
         centerOfMass[2] = asDouble(line.substr(28,14));
         valid |= validCenterOfMass;
      }
      else if (label == stringSystemNumObs)
      {
         static const int maxObsPerLine = 13;

         satSysPrev = satSysTemp;
         satSysTemp = strip(line.substr(0,1));
         numObsPrev = numObs;
         numObs     = asInt(line.substr(3,3));

         if ( satSysTemp == " " ) // it's a continuation line; use previous info.
         {
            satSysTemp = satSysPrev;
            numObs = numObsPrev;
            std::vector<ObsID> newTypeList = mapObsTypes.find(satSysTemp)->second;
            for (int i = obsTypeList.size();
                 (i < numObs) && ( (i % maxObsPerLine) < maxObsPerLine); i++)
            {
               int position = 4*(i % maxObsPerLine) + 6 + 1;
//               ObsID rt = convertObsType(line.substr(position,3));
               ObsID rt(ObsID::otPhase, ObsID::cbL1, ObsID::tcCA);
               newTypeList.push_back(rt);
            }
         }
         else
         {
            std::vector<ObsID> newTypeList;
            for (int i = 0; (i < numObs) && (i < maxObsPerLine); i++)
            {
               int position = 4*i + 6 + 1;
//               ObsID rt = convertObsType(line.substr(position,3));
               ObsID rt(ObsID::otPhase, ObsID::cbL1, ObsID::tcCA);
               newTypeList.push_back(rt);
            }
            mapObsTypes[satSysTemp] = newTypeList;
         }

         valid |= validSystemObsType;
      }
      else if (label == stringSigStrengthUnit)
      {
         sigStrengthUnit = strip(line.substr(0,20));
         valid |= validSigStrengthUnit;
      }
      else if (label == stringInterval)
      {
         interval = asDouble(line.substr(0,10));
         valid |= validInterval;
      }
      else if (label == stringFirstTime)
      {
         firstObs = parseTime(line);
         valid |= validFirstTime;
      }
      else if (label == stringLastTime)
      {
         lastObs = parseTime(line);
         valid |= validLastTime;
      }
      else if (label == stringReceiverOffset)
      {
         receiverOffset = asInt(line.substr(0,6));
         valid |= validReceiverOffset;
      }

      else if (label == stringSystemDCBSapplied)
      {
         Rinex3CorrInfo tempInfo;
         tempInfo.satSys = strip(line.substr( 0, 1));
         tempInfo.name   = strip(line.substr( 2,17));
         tempInfo.source = strip(line.substr(20,40));
         infoDCBS.push_back(tempInfo);
         valid |= validSystemDCBSapplied;
      }
      else if (label == stringSystemPCVSapplied)
      {
         Rinex3CorrInfo tempInfo;
         tempInfo.satSys = strip(line.substr( 0, 1));
         tempInfo.name   = strip(line.substr( 2,17));
         tempInfo.source = strip(line.substr(20,40));
         infoPCVS.push_back(tempInfo);
         valid |= validSystemPCVSapplied;
      }
      else if (label == stringSystemScaleFac)
      {
         static const int maxObsPerLine = 12;

         satSysTemp = strip(line.substr(0,1));
         factor     = asInt(line.substr(2,4));
         numObs     = asInt(line.substr(8,2));

         int startPosition = 0;

         if ( satSysTemp == " " ) // it's a continuation line; use prev. info., end pt. to start
         {
            satSysTemp = satSysPrev;
            factor     = factorPrev;
            numObs     = numObsPrev;

            startPosition = sysSfacMap[satSysTemp].size();
         }

         // 0/blank numObs means factor applies to all obs types in appropriate obsTypeList
         if (numObs == 0) numObs = mapObsTypes[satSysTemp].size();

         for (int i = startPosition; (i < numObs) && (i % maxObsPerLine < maxObsPerLine); i++)
         {
            int position = 4*i + 10 + 1;
//            ObsID tempType = convertObsType(strip(line.substr(position,3)));
            ObsID tempType(ObsID::otPhase, ObsID::cbL1, ObsID::tcCA);
            sysSfacMap[satSysTemp].insert(make_pair(tempType, factor));
         }

         // save values in case next line is a continuation line
         satSysPrev = satSysTemp;
         factorPrev = factor;
         numObsPrev = numObs;

         valid |= validSystemScaleFac;
      }
      else if (label == stringLeapSeconds)
      {
         leapSeconds = asInt(line.substr(0,6));
         valid |= validLeapSeconds;
      }
      else if (label == stringNumSats)
      {
         numSVs = asInt(line.substr(0,6)) ;
         valid |= validNumSats;
      }
      else if (label == stringPrnObs)
      {
         static const int maxObsPerLine = 9;
         // continuation lines:
         // you have to know what PRN this is continuing for, hence lastPRN
         std::string GNSS(1, lastPRN.systemChar());
         if ( lastPRN.id != -1 && numObsForSat[lastPRN].size() != mapObsTypes[GNSS].size() )
         {
            for (int i = numObsForSat[lastPRN].size(); 
                 (i < mapObsTypes[GNSS].size()) && ( (i % maxObsPerLine) < maxObsPerLine);
                 i++                                                                                      )
            {
               numObsForSat[lastPRN].push_back(asInt(line.substr(6*(i%maxObsPerLine)+6,6)));
            }
         }
         else
         {
            try
            {
               lastPRN.fromString(line.substr(3,3));
            }
            catch (Exception& e)
            {
               FFStreamError ffse(e);
               GPSTK_RETHROW(ffse);
            }
            vector<int> numObsList;
            for (int i = 0; 
                 (i < obsTypeList.size()) && (i < maxObsPerLine); i++)
            {
               numObsList.push_back(asInt(line.substr(6*i+6,6)));
            }

            numObsForSat[lastPRN] = numObsList;
         }
         valid |= validPrnObs;
      }
      else if (label == stringEoH)
      {
         cout << "Found End of Header" << endl;
         valid |= validEoH;
      }
      else
      {
         FFStreamError e("Unidentified label: " + label);
         GPSTK_THROW(e);
      }
   }   // end of Rinex3ObsHeader::ParseHeaderRecord(string& line)


   // This function parses the entire header from the given stream
   void Rinex3ObsHeader::reallyGetRecord(FFStream& ffs)
      throw(std::exception, FFStreamError, 
            gpstk::StringUtils::StringException)
   {
      Rinex3ObsStream& strm = dynamic_cast<Rinex3ObsStream&>(ffs);

      // If already read, just return.
      if (strm.headerRead == true) return;

      // Since we're reading a new header, we need to reinitialize all our list structures.
      // All the other objects should be ok.  This also applies if we threw an exception
      // the first time we read the header and are now re-reading it.  Some of these could
      // be full and we need to empty them.
      clear();

      string line;

      while (!(valid & validEoH))
      {
         strm.formattedGetLine(line);
         StringUtils::stripTrailing(line);

         if (line.length() == 0)
         {
            FFStreamError e("No data read");
            GPSTK_THROW(e);
         }
         else if (line.length() < 60 || line.length() > 80)
         {
            FFStreamError e("Invalid line length");
            GPSTK_THROW(e);
         }

         try
         {
            ParseHeaderRecord(line);
         }
         catch(FFStreamError& e)
         {
            GPSTK_RETHROW(e);
         }

      }   // end while(not end of header)

      cout << "*** Finished reading Header lines." << endl;

      unsigned long allValid;
      if      (version == 3.0)  allValid = allValid30;
      else if (version == 3.00) allValid = allValid30;
      else
      {
         FFStreamError e("Unknown or unsupported RINEX version " + 
                         asString(version));
         GPSTK_THROW(e);
      }

      if ( (valid & allValid) != allValid)
      {
         FFStreamError e("Incomplete or invalid header");
         GPSTK_THROW(e);               
      }

      // If we get here, we should have reached the end of header line
      strm.header = *this;
      strm.headerRead = true;

   }  // end of reallyGetRecord()


  /*

   Rinex3ObsHeader::Rinex3ObsType Rinex3ObsHeader::convertObsType(const string& oneObs)
      throw(FFStreamError)
   {
      Rinex3ObsType ot(RegisteredRinex3ObsTypes[0]);   // Unknown type
      for (int i = 0; i < RegisteredRinex3ObsTypes.size(); i++) {
         if (RegisteredRinex3ObsTypes[i].type == oneObs)
         {
            ot = RegisteredRinex3ObsTypes[i];
            break;
         }
         //FFStreamError e("Bad obs type: " + oneObs);
         //GPSTK_THROW(e);
      }
      return ot;
   }


   string Rinex3ObsHeader::convertObsType(const Rinex3ObsHeader::Rinex3ObsType& oneObs)
      throw(FFStreamError)
   {
      return oneObs.type;
   }


   // return 1 if type already defined,
   //        0 if successful
   //       -1 if not successful - invalid input
   int RegisterExtendedRinex3ObsType(string t, string d, string u, unsigned int dep)
   {
      if (t.empty()) return -1;
      // throw if t="UN" ?
      // check that it is not duplicated
      for (int i = 0; i < Rinex3ObsHeader::RegisteredRinex3ObsTypes.size(); i++)
      {
         if (Rinex3ObsHeader::RegisteredRinex3ObsTypes[i].type == t) { return 1; }
      }
      Rinex3ObsHeader::Rinex3ObsType ot;
      if (t.size()>2) t.resize(2,' '); ot.type = stripTrailing(t);
      if (d.size()>20) d.resize(20,' '); ot.description = stripTrailing(d);
      if (u.size()>10) u.resize(10,' '); ot.units = stripTrailing(u);
//      ot.depend = dep;
      Rinex3ObsHeader::RegisteredRinex3ObsTypes.push_back(ot);
      return 0;
   }

  */

   CivilTime Rinex3ObsHeader::parseTime(const string& line) const
   {
      int year, month, day, hour, min;
      double sec;
   
      year  = asInt(   line.substr(0,  6 ));
      month = asInt(   line.substr(6,  6 ));
      day   = asInt(   line.substr(12, 6 ));
      hour  = asInt(   line.substr(18, 6 ));
      min   = asInt(   line.substr(24, 6 ));
      sec   = asDouble(line.substr(30, 13));

      return CivilTime(year, month, day, hour, min, sec);
   }


   string Rinex3ObsHeader::writeTime(const CivilTime& civtime) const
   {
      string line;

      line  = rightJustify(asString<short>(civtime.year    )   ,  6);
      line += rightJustify(asString<short>(civtime.month   )   ,  6);
      line += rightJustify(asString<short>(civtime.day     )   ,  6);
      line += rightJustify(asString<short>(civtime.hour    )   ,  6);
      line += rightJustify(asString<short>(civtime.minute  )   ,  6);
      line += rightJustify(asString(       civtime.second,7)   , 13);
      line += rightJustify((civtime.getTimeSystem()).asString(),  8);

      return line;
   }


   void Rinex3ObsHeader::dump(ostream& s) const
   {
      int i,j;

      string str = system.systemChar() + " (" + system.systemString() + ")";

      s << "---------------------------------- REQUIRED ----------------------------------\n";
      s << "Rinex Version " << fixed << setw(5) << setprecision(2) << version
        << ",  File type " << fileType << ",  System " << str << ".\n";
      s << "Prgm: " << fileProgram << ",  Run: " << date << ",  By: " << fileAgency << endl;
      s << "Marker name: " << markerName << ", ";
      s << "Marker type: " << markerType << ".\n";
      s << "Observer : " << observer << ",  Agency: " << agency << endl;
      s << "Rec#: " << recNo << ",  Type: " << recType << ",  Vers: " << recVers << endl;
      s << "Antenna # : " << antNo << ",  Type : " << antType << endl;
      s << "Position      (XYZ,m) : " << setprecision(4) << antennaPosition << ".\n";
      s << "Antenna Delta (HEN,m) : " << setprecision(4) << antennaDeltaHEN << ".\n";
      map<string,vector<ObsID> >::const_iterator iter;
      for (iter = mapObsTypes.begin(); iter != mapObsTypes.end(); iter++)
      {
         RinexSatID rsid;
         rsid.fromString(iter->first);
         s << rsid.systemString() << " Observation types (" << iter->second.size() << "):\n";
         for (i = 0; i < iter->second.size(); i++) 
            s << " Type #" << i+1 << " = "
              << asString(iter->second[i]) << endl;
//              << " "  << iter->second[i].description
//              << " (" << iter->second[i].units << ")." << endl;
      }
      s << "Time of first obs " << firstObs.printf("%04Y/%02m/%02d %02H:%02M:%010.7f %P");
      s << "(This header is ";
      if ((valid & allValid30) == allValid30) s << "VALID 3.00";
      else s << "NOT VALID";
      s << " RINEX 3.)\n";

      if (!(valid & validVersion        )) s << " Version / Type      is NOT valid\n";
      if (!(valid & validRunBy          )) s << " Pgm / Run By / Date is NOT valid\n";
      if (!(valid & validMarkerName     )) s << " Marker Name         is NOT valid\n";
      if (!(valid & validMarkerType     )) s << " Marker Type         is NOT valid\n";
      if (!(valid & validObserver       )) s << " Observer / Agency   is NOT valid\n";
      if (!(valid & validReceiver       )) s << " Receiver # / Type   is NOT valid\n";
      if (!(valid & validAntennaType    )) s << " Antenna Type        is NOT valid\n";
//      if (!(valid & validAntennaPosition)) s << " Antenna Position    is NOT valid\n";
      if (!(valid & validAntennaDeltaHEN)) s << " Antenna Delta HEN   is NOT valid\n";
      if (!(valid & validSystemObsType  )) s << " Sys / # / Obs Type  is NOT valid\n";
      if (!(valid & validFirstTime      )) s << " Time of First Obs   is NOT valid\n";
      if (!(valid & validEoH            )) s << " End of Header       is NOT valid\n";

      s << "---------------------------------- OPTIONAL ----------------------------------\n";
      if (valid & validMarkerNumber     ) s << "Marker number : " << markerNumber << endl;
      if (valid & validAntennaDeltaXYZ  ) s << "Antenna Delta    (XYZ,m) : "
                                            << setprecision(4) << antennaDeltaXYZ   << endl;
      if (valid & validAntennaPhaseCtr  ) s << "Antenna PhaseCtr (XYZ,m) : "
                                            << setprecision(4) << antennaPhaseCtr   << endl;
      if (valid & validAntennaBsightXYZ ) s << "Antenna B.sight  (XYZ,m) : "
                                            << setprecision(4) << antennaBsightXYZ  << endl;
      if (valid & validAntennaZeroDirAzi) s << "Antenna ZeroDir  (deg)   : "
                                            << setprecision(4) << antennaZeroDirAzi << endl;
      if (valid & validAntennaZeroDirXYZ) s << "Antenna ZeroDir  (XYZ,m) : "
                                            << setprecision(4) << antennaZeroDirXYZ << endl;
      if (valid & validCenterOfMass     ) s << "Center of Mass   (XYZ,m) : "
                                            << setprecision(4) << antennaPhaseCtr   << endl;
      if (valid & validSigStrengthUnit  ) s << "Signal Strenth Unit = " << sigStrengthUnit << endl;
      if (valid & validInterval         ) s << "Interval = "
                                            << fixed << setw(7) << setprecision(3) << interval << endl;
      if (valid & validLastTime         ) s << "Time of Last Obs "
                                            << lastObs.printf("%04Y/%02m/%02d %02H:%02M:%010.7f %P");
      if (valid & validReceiverOffset   ) s << "Clock offset record is present and offsets "
                                            << (receiverOffset?"ARE":"are NOT") << " applied." << endl;
      if (valid & validSystemDCBSapplied)
      {
         for (i = 0; i < infoDCBS.size(); i++)
         {
            RinexSatID rsid;
            rsid.fromString(infoDCBS[i].satSys);
            s << "System DCBS Correction Applied to " << rsid.systemString()
              << " data using program " << infoDCBS[i].name << endl;
            s << " from source " << infoDCBS[i].source << "." << endl;
         }
      }
      if (valid & validSystemPCVSapplied)
      {
         for (i = 0; i < infoPCVS.size(); i++)
         {
            RinexSatID rsid;
            rsid.fromString(infoPCVS[i].satSys);
            s << "System PCVS Correction Applied to " << rsid.systemString()
              << " data using program " << infoPCVS[i].name << endl;
            s << " from source " << infoPCVS[i].source << "." << endl;
         }
      }
      if (valid & validSystemScaleFac)
      {
         std::map<std::string, sfacMap>::const_iterator mapIter;
         for (mapIter == sysSfacMap.begin(); mapIter != sysSfacMap.end(); mapIter++) // loop over GNSSes
         {
            RinexSatID rsid;
            rsid.fromString(mapIter->first);
            s << rsid.systemString() << " scale factors applied:" << endl;
            std::map<ObsID, int>::const_iterator iter;
            for (iter == mapIter->second.begin(); iter != mapIter->second.end(); iter++) // loop over scale factor map
               s << "   " << iter->first.type << " " << iter->second << endl;
         }
      }
      if (valid & validLeapSeconds    ) s << "Leap seconds: " << leapSeconds << endl;
      if (valid & validNumSats        ) s << "Number of Satellites with data : " << numSVs << endl;
      if (valid & validPrnObs         )
      {
         s << " PRN and number of observations for each obs type:" << endl;
         s << "   ";
         for (i = 0; i < obsTypeList.size(); i++)
            s << setw(7) << asString(obsTypeList[i]);
         s << endl;
         map<SatID, vector<int> >::const_iterator sat_itr = numObsForSat.begin();
         while (sat_itr != numObsForSat.end())
         {
            vector<int> obsvec=sat_itr->second;
            s << " " << RinexSatID(sat_itr->first) << " ";
            for (i=0; i<obsvec.size(); i++) s << " " << setw(6) << obsvec[i];
            s << endl;
            sat_itr++;
         }
      }
      if (commentList.size() && !(valid & validComment)) s << " Comment is NOT valid\n";
      s << "Comments (" << commentList.size() << ") :\n";
      for (i = 0; i < commentList.size(); i++)
         s << commentList[i] << endl;

      s << "-------------------------------- END OF HEADER -------------------------------\n";
   }


  /*

   // Pretty print a list of standard Rinex observation types
   void DisplayStandardRinex3ObsTypes(ostream& s)
   {
      s << "The list of standard Rinex obs types:\n";
      s << "  OT Description          Units\n";
      s << "  -- -------------------- ---------\n";
      for (int i = 0; i < Rinex3ObsHeader::StandardRinex3ObsTypes.size(); i++)
      {
         string line;
         line  =             string("  ")+Rinex3ObsHeader::StandardRinex3ObsTypes[i].type;
         line += leftJustify(string(" ") +Rinex3ObsHeader::StandardRinex3ObsTypes[i].description,21);
         line += leftJustify(string(" ") +Rinex3ObsHeader::StandardRinex3ObsTypes[i].units,11);
         s << line << endl;
      }
   }


   // Pretty print a list of registered extended Rinex observation types
   void DisplayExtendedRinex3ObsTypes(ostream& s)
   {
      s << "The list of available extended Rinex obs types:\n";
      s << "  OT Description          Units     Required input (EP=ephemeris,PS=Rx Position)\n";
      s << "  -- -------------------- --------- ------------------\n";
      for (int i = Rinex3ObsHeader::StandardRinex3ObsTypes.size();
               i < Rinex3ObsHeader::RegisteredRinex3ObsTypes.size(); i++)
      {
         string line;
         line  =             string("  ")+Rinex3ObsHeader::RegisteredRinex3ObsTypes[i].type;
         line += leftJustify(string(" ") +Rinex3ObsHeader::RegisteredRinex3ObsTypes[i].description,21);
         line += leftJustify(string(" ") +Rinex3ObsHeader::RegisteredRinex3ObsTypes[i].units,11);
         for (int j = 1; j <= 6; j++)
         {
            if (j==3 || j==4) continue;
//            if (Rinex3ObsHeader::RegisteredRinex3ObsTypes[i].depend &
//                Rinex3ObsHeader::StandardRinex3ObsTypes[j].depend    )
//                  line += string(" ")+Rinex3ObsHeader::StandardRinex3ObsTypes[j].type;
//            else line += string("   ");
         }
//         if (Rinex3ObsHeader::RegisteredRinex3ObsTypes[i].depend & Rinex3ObsHeader::Rinex3ObsType::EPdepend)
//            line += string(" EP"); else line += string("   ");
//         if (Rinex3ObsHeader::RegisteredRinex3ObsTypes[i].depend & Rinex3ObsHeader::Rinex3ObsType::PSdepend)
//            line += string(" PS"); else line += string("   ");
         s << line << endl;
      }
   }

  */


} // namespace gpstk
