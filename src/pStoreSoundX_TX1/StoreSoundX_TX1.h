/************************************************************/
/*    NAME: yhhuang                                              */
/*    ORGN: MIT                                             */
/*    FILE: StoreSoundX_TX1.h                                          */
/*    DATE: Jan 23th, 2019                             */
/************************************************************/

#ifndef StoreSoundX_TX1_HEADER
#define StoreSoundX_TX1_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <alsa/asoundlib.h>
#include <stdio.h>
#include <deque>

class StoreSoundX_TX1 : public AppCastingMOOSApp
{
 public:
   StoreSoundX_TX1();
   ~StoreSoundX_TX1();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

 protected:
   std::string fileTime(char);
   void makeDir();
   void setRecordParams();
   void SendData();
   void Capture();
   void Repeat();

 private: // Configuration variables
   bool m_start_record;
   bool m_start_check;
   bool m_pass_record;
   bool m_dir_flag;
   bool m_save_file;
   std::string m_path;
   std::string m_filename;
   std::string m_repeat;
   std::deque<int> m_tem_buffer;
   FILE *m_fp;
   FILE *m_fp_checking;

 private: // Record params
   std::string m_recordDevice;
   snd_pcm_t *m_handle;
   snd_pcm_hw_params_t *m_params;
   snd_pcm_uframes_t m_frames;
   unsigned int m_sampleRate;
   unsigned int m_channels;
   unsigned int m_recordTime;
   unsigned int m_passTime;
   unsigned int m_loops;
   int m_dir;
   int m_period_size;
   int m_bits;
   int m_send_size;
   char *m_period_buffer;
   
};

#endif 
