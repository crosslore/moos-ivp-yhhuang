/************************************************************/
/*    NAME: YHHuang                                              */
/*    ORGN: MIT/NTU                                             */
/*    FILE: CommunicationAngle_yhhuang.cpp                   */
/*    DATE: MAR.1/2018                                                */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "CommunicationAngle_yhhuang.h"
#include <unistd.h>
using namespace std;

//---------------------------------------------------------
// Constructor

CommunicationAngle_yhhuang::CommunicationAngle_yhhuang()
{
  m_surface_sound_speed  = 0;
  m_sound_speed_gradient = 0;
  m_water_depth          = 0;
  m_time_interval        = 0;
  m_collaborator         = "";
}

//---------------------------------------------------------
// Destructor

CommunicationAngle_yhhuang::~CommunicationAngle_yhhuang()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool CommunicationAngle_yhhuang::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();

#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

     if(key == "FOO") 
       cout << "great!";

     else if(key == "NAV_X")
         m_NAV_X = msg.GetDouble();
     else if(key == "NAV_Y")
         m_NAV_Y = msg.GetDouble();
     else if(key == "NAV_HEADING")
         m_NAV_HEADING = msg.GetDouble();
     else if(key == "NAV_SPEED")
         m_NAV_SPEED = msg.GetDouble();
     else if(key == "NAV_DEPTH")
         m_NAV_DEPTH = msg.GetDouble();
     else if(key == m_collnavx)
         m_CO_NAV_X  = msg.GetDouble();
     else if(key == m_collnavy)
         m_CO_NAV_Y  = msg.GetDouble();
     else if(key == m_collnavdepth)
         m_CO_NAV_DEPTH = msg.GetDouble();
     else if(key == m_collnavspeed)
         m_CO_NAV_SPEED = msg.GetDouble();
     else if(key == m_collnavhead)
         m_CO_NAV_HEADING = msg.GetDouble();
     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool CommunicationAngle_yhhuang::OnConnectToServer()
{
   registerVariables();
   return(true);
}

// double CommunicationAngle_yhhuang::thetaZeroCalculate(double theta,double Radius)
// {
//    double thetaZero;   //output
//    double total_distance;
//    double x_dif_sq;
//    double y_dif_sq;
    
//     x_dif_sq = pow((m_NAV_X-m_CO_NAV_X),2);
//     y_dif_sq = pow((m_NAV_Y-m_CO_NAV_Y),2);
//     total_distance = sqrt(x_dif_sq+y_dif_sq);
     
//     thetaZero = asin((total_distance+Radius*sin(theta))/Radius);
    
//    return(thetaZero); 
// }

// double CommunicationAngle_yhhuang::RCalculate()
// {
//     double Radius;  //Output Radius
    
//     Radius  = (m_surface_sound_speed+(m_sound_speed_gradient* m_greater_depth))/m_sound_speed_gradient;


//     return(Radius);
// }

// double CommunicationAngle_yhhuang::thetaCalculate(double radius)
// {
//     cout<<m_collnavdepth<<m_CO_NAV_DEPTH<<endl;
//     cout<<"m_NAV_DEPTH:"<<m_NAV_DEPTH<<endl;
//     double depth_dif = sqrt(pow(m_NAV_DEPTH-m_CO_NAV_DEPTH,2));
   
//     cout<<"depth_dif:"<<depth_dif<<endl; 
//     double theta    = acos((depth_dif+(m_surface_sound_speed/m_greater_depth))/radius);

//     cout<<"theta:"<<theta<<endl;

//     return(theta);
// }

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool CommunicationAngle_yhhuang::Iterate()
{
  AppCastingMOOSApp::Iterate();
  
  //double thetaZero=45*3.1415926/180;
  double thetaZero=-0.504573;
  double theta;
  double upper_bound=90*3.1415926/180;
  double lower_bound=0;
  double Radius;
  double output_theta;
  double czZero;
  double cz;
  double costheta;
 if(m_NAV_DEPTH>m_CO_NAV_DEPTH){
     m_greater_depth = m_NAV_DEPTH;
 	 m_smaller_depth = m_CO_NAV_DEPTH;
 }
 else{
     m_greater_depth = m_CO_NAV_DEPTH;
     m_smaller_depth = m_NAV_DEPTH;
 }

 	czZero   = m_surface_sound_speed+m_sound_speed_gradient*m_smaller_depth;
 	cz 		 = m_surface_sound_speed+m_sound_speed_gradient*m_greater_depth;


	unsigned int a=0;
	while(1){
     sleep(1);	
     	Radius = czZero/(m_sound_speed_gradient*cos(thetaZero));
            cout<<"thetaZero="<<thetaZero<<endl;
            cout<<"upper_bound="<<upper_bound<<endl;
            cout<<"lower_bound="<<lower_bound<<endl;    
        costheta = cos(thetaZero)*(cz)/(czZero);
//        cout<<"costheta"<<costheta<<endl;
        if(costheta>1){
            thetaZero +=3.1415926/180;
            cout<<"thetaZero="<<thetaZero<<endl;
        }
            
        else{
            
     	theta  = acos(cos(thetaZero)*(cz)/(czZero));
        if(theta==thetaZero)
            theta = -theta;
        cout<<"theta:"<<theta<<endl;
     	double distance_sq = (2*pow(Radius,2)*(1-cos(thetaZero-theta)));
     	double distance    = sqrt(distance_sq);
     	//distance of two point of the estimated theta

     	double x_dif_sq = pow((m_NAV_X-m_CO_NAV_X),2);
     	double y_dif_sq = pow((m_NAV_Y-m_CO_NAV_Y),2);
     	double z_dif_sq = pow((m_NAV_DEPTH-m_CO_NAV_DEPTH),2);
         double total_distance = sqrt(x_dif_sq+y_dif_sq+z_dif_sq);
        

			double diff = total_distance-distance;
            cout<<"total_distance="<<total_distance<<endl;
            cout<<"distance="<<distance<<endl; 
            cout<<"diff="<<diff<<endl;
         if(sqrt(pow(total_distance-distance,2))<5){
         	Notify("ELEV_ANGLE",thetaZero);
         	cout<<"Get!"<<endl;
         	break;
         }
         else if(thetaZero!=-thetaZero && thetaZero>0){
             thetaZero = -thetaZero;
//             cout<<"thetaZero="<<thetaZero<<endl;
         }

         else if((total_distance-distance)>5){
            
             thetaZero = -thetaZero;
            upper_bound = thetaZero;
            
         	thetaZero = (lower_bound+thetaZero)/2;
         	cout<<"thetaZero_down"<<endl;
 //           cout<<"thetaZero now="<<thetaZero<<endl;
 //           cout<<"upper_bound="<<upper_bound<<endl;
//            cout<<"lower_bound="<<lower_bound<<endl;
         }
         else if((total_distance-distance)<-5){
        
             thetaZero = -thetaZero;
            lower_bound = thetaZero;
         	thetaZero = (upper_bound+thetaZero)/2;
         	cout<<"thetaZero_up"<<endl;
 			        
         }
        }
  			a++;   
         // cout<<"thetaZero now is:"<<thetaZero<<endl;
	}  
cout<<"a="<<a<<endl;
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool CommunicationAngle_yhhuang::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = toupper(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if(param == "FOO") {
      handled = true;
    }
    else if(param == "BAR") {
      handled = true;
    }
    else if(param == "SURFACE_SOUND_SPEED"){
   
        
      string str_surface_sound_speed = value;
      stringstream ss;
      ss<<str_surface_sound_speed;
      ss>>m_surface_sound_speed;
      
      handled = true;
    }
    else if(param == "SOUND_SPEED_GRADIENT"){
      
        string str_sound_speed_gradient = value;
        stringstream ss;
        ss<<str_sound_speed_gradient;
        ss>>m_sound_speed_gradient;

        handled = true;
    }
    else if(param == "WATER_DEPTH"){
     
        string str_water_depth = value;
        stringstream ss;
        ss<<str_water_depth;
        ss>>m_water_depth;

        handled = true;
    }
    else if(param == "TIME_INTERVAL"){   
     
        
        string str_time_interval = value;
        stringstream ss;
        ss<<str_time_interval;
        ss>>m_time_interval;
        
        handled = true;
    }

    else if(param == "COLLABORATOR"){

       m_collaborator = value;
       handled = true;
    }

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void CommunicationAngle_yhhuang::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
     Register("NAV_X",0);
     Register("NAV_Y",0);
     Register("NAV_DEPTH",0);
     Register("NAV_HEADING",0);
     Register("NAV_SPEED",0);
     Register("OWNER_VEHICLE",0);
     Register(m_collaborator,0);
     
     m_collnavx     = m_collaborator+"_NAV_X";
     m_collnavy     = m_collaborator+"_NAV_Y";
     m_collnavdepth = m_collaborator+"_NAV_DEPTH";
     m_collnavhead  = m_collaborator+"_NAV_HEADING";
     m_collnavspeed = m_collaborator+"NAV_SPEED";
     
     Register(m_collnavx,0);
     Register(m_collnavy,0);
     Register(m_collnavdepth,0);
     Register(m_collnavspeed,0);
     Register(m_collnavhead,0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool CommunicationAngle_yhhuang::buildReport() 
{
  m_msgs << "============================================ \n";
  m_msgs << "File:                                        \n";
  m_msgs << "============================================ \n";

  ACTable actab(4);
  actab.addHeaderLines();
  //actab << "one" << "two" << "three" << "four";
  m_msgs << actab.getFormattedString();
  
  m_msgs<<"\nsurface_sound_speed="<<m_surface_sound_speed;
  m_msgs<<"\nsound_speed_gradient="<<m_sound_speed_gradient;
  m_msgs<<"\nwater_depth="<<m_water_depth;
  m_msgs<<"\ntime_interval"<<m_time_interval;
  m_msgs<<"\nm_greater_depth:"<<m_greater_depth<<endl;
  return(true);
}




