/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: DirectionCalculator.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef DirectionCalculator_HEADER
#define DirectionCalculator_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <deque>
#include "filt.h"

class DirectionCalculator : public AppCastingMOOSApp
{
 public:
   DirectionCalculator();
   ~DirectionCalculator();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
    void registerVariables();

    bool    GetVoltageData_ch1(std::string);
    bool    GetVoltageData_ch2(std::string);
    bool    Band_Filter(std::vector<float> &input);

    float   CalTDOA_by_cc(std::vector<float> ch1, std::vector<float> ch2);
    float   CalTDOA_by_peak(std::vector<float> ch1, std::vector<float> ch2);

    bool    Save_data(std::string filename, std::vector<float> in, std::string filepath);

 private: // Configuration variables
    
    float   m_fs;
    float   m_c;
    float   m_mic_dis;
    float   m_interval;
    float   m_output_angle;

    int     m_access_data_num;
    int     m_mic_num;
    int     m_low_fq;
    int     m_up_fq;
    int     m_filt_order;
    int     m_index;
    
    bool    m_use_cc; 
    

    std::deque<float> m_ch1;
    std::deque<float> m_ch2;
    std::deque<float> m_ch3;

    Filter *m_band_filter;
    
 private: // State variables
    bool    m_first_time;
};

#endif 
