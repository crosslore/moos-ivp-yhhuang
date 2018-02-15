#!/bin/bash 
#-----------------------------------------------------------
#  Part 1: Check for and handle command-line arguments
#-----------------------------------------------------------
TIME_WARP=1
JUST_MAKE="no"
for ARGI; do
    if [ "${ARGI}" = "--help" -o "${ARGI}" = "-h" ] ; then
	printf "%s [SWITCHES] [time_warp]   \n" $0
	printf "  --just_make, -j    \n" 
	printf "  --help, -h         \n" 
	exit 0;
    elif [ "${ARGI//[^0-9]/}" = "$ARGI" -a "$TIME_WARP" = 1 ]; then 
        TIME_WARP=$ARGI
    elif [ "${ARGI}" = "--just_build" -o "${ARGI}" = "-j" ] ; then
	JUST_MAKE="yes"
    else 
	printf "Bad Argument: %s \n" $ARGI
	exit 0
    fi
done

#-----------------------------------------------------------
#  Part 2: Create the .moos and .bhv files. 
#-----------------------------------------------------------
SNAME1="gilda"  
SNAME2="henry"  
START_POS1="0,0"    
START_POS2="80,0"   
LOITER_POS1="x=0,y=-75"
LOITER_POS2="x=125,y=-50"

SHORE_LISTEN="9300"

nsplug meta_vehicle.moos targ_$SNAME1.moos -f WARP=$TIME_WARP \
    SNAME=$SNAME1      START_POS=$START_POS1                  \
    SPORT="9001"       SHARE_LISTEN="9301"                    \
    STYPE="kayak"      SHORE_LISTEN=$SHORE_LISTEN           

nsplug meta_vehicle.moos targ_$SNAME2.moos -f WARP=$TIME_WARP \
    SNAME=$SNAME2      START_POS=$START_POS2                  \
    SPORT="9002"       SHARE_LISTEN="9302"                    \
    STYPE="kayak"      SHORE_LISTEN=$SHORE_LISTEN            

nsplug meta_vehicle.bhv targ_$SNAME1.bhv -f SNAME=$SNAME1     \
    START_POS=$START_POS1 LOITER_POS=$LOITER_POS1       

nsplug meta_vehicle.bhv targ_$SNAME2.bhv -f SNAME=$SNAME2     \
    START_POS=$START_POS2 LOITER_POS=$LOITER_POS2       

nsplug meta_shoreside.moos targ_shoreside.moos -f WARP=$TIME_WARP \
   SNAME="shoreside"  SHARE_LISTEN=$SHORE_LISTEN  SPORT="9000"     
if [ ${JUST_MAKE} = "yes" ] ; then
    exit 0
fi

#-----------------------------------------------------------
#  Part 3: Launch the processes
#-----------------------------------------------------------
printf "Launching $SNAME MOOS Community (WARP=%s) \n"  $TIME_WARP
pAntler targ_shoreside.moos >& /dev/null &
printf "Launching $SNAME1 MOOS Community (WARP=%s) \n" $TIME_WARP
pAntler targ_$SNAME1.moos >& /dev/null &
printf "Launching $SNAME2 MOOS Community (WARP=%s) \n" $TIME_WARP
pAntler targ_$SNAME2.moos >& /dev/null &
printf "Done \n"

#-----------------------------------------------------------
#  Part 4: Launch uMAC and kill everything upon exiting uMAC
#-----------------------------------------------------------
uMAC targ_shoreside.moos
printf "Killing all processes ... \n"
kill %1 %2 %3 
printf "Done killing processes.   \n"
