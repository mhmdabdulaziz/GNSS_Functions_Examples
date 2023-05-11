/***
 *      Conure Tracker
 *    GNSS Module driver
 *      9th Apr 2023
 *  Author : Mohamed Abdulaziz
*/

#ifndef GNSS_H_
#define GNSS_H_

//Includes===========================//
#include "main.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//Macros=============================//
#define Cold 0
#define Warm 1
#define Hot 2

#define Sleep_Enable 1
#define Sleep_Disable 0

#define Low_Power_Enable 1
#define Low_Power_Disable 0

// Non Return Command
#define Command_NRET_Cold_Start_GNSS "$PQCOLD*05\r\n"
#define Command_NRET_Warm_Start_GNSS "$PQWARM*08\r\n"
#define Command_NRET_Hot_Start_GNSS "$PQHOT*52\r\n"
#define Command_NRET_GNSS_Reset "$PQSRR*52\r\n" 		/* Execute a system reset. The GNSS firmware will be rebooted */

// Return Commands with No Arguments
/*  When the GNSS engine is stopped, for instance waking up from sleep mode or after sending assistant
 time/position to the module, the command can be used to start the engine */
// Return Messages $PQSTARTGNSSOK*4C -- $PQSTARTGNSSERROR*10
#define Command_RET_NA_Start_GNSS "$PQSTARTGNSS*48\r\n"

/* Stop GNSS engine */
// Return Message $PQSTOPGNSSOK*14 -- $PQSTOPGNSSERROR*48
#define Command_RET_NA_STOP_GNSS "$PQSTOPGNSS*10\r\n"

// Get Sleep Mode Status
// Return Message $PQSLEEP,1*53 -- $PQSLEEP,0*52
#define Command_RET_NA_Get_Sleep_Status "$PQGETSLEEP*18\r\n"

// Get Low Power Mode Status
// Return Message $PQGLP,1*47 -- $PQGLP,0*46
#define Command_RET_NA_Get_Low_Power_Mode_Status "$PQGETGLP*0C\r\n"

// Set Baudrate Value for GNSS
#define Command_RET_NA_Set_Baudrate_115200 "$PQSETBAUD,115200*7A\r\n" // Default BaudRate
#define Command_RET_NA_Set_Baudrate_230400 "$PQSETBAUD,230400*78\r\n"
#define Command_RET_NA_Set_Baudrate_460800 "$PQSETBAUD,460800*77\r\n"
#define Command_RET_NA_Set_Baudrate_921600 "$PQSETBAUD,921600*71\r\n"

/* Get the information of GNSS constellation mask */
// Return Message $PQGETCNSTERROR*05 --  $PQCNST,"Number of Connected Satellite"*22
#define Command_RET_NA_Get_Satellite_Information "$PQGETCNST*5D\r\n"

// Return Commands with Arguments
/* Enable/disable sleep mode */
/*
 1- In sleep mode, the UART is inaccessible.
 2- When sleep mode is enabled with $PQSETSLEEP,1 and GNSS engine is stopped, the module can
 enter sleep mode through pulling down AP_REQ pin
 */
#define Command_RET_A_Enable_Sleep_Mode "$PQSETSLEEP,1*11\r\n"
#define Command_RET_A_Disable_Sleep_Mode "$PQSETSLEEP,0*11\r\n"  // Default Mode

/* Enable/disable GNSS Low Power (GLP) mode */
//The GNSS engine will be restarted if this command is executed successfully
#define Command_RET_A_Enable_GNSS_Low_Power "$PQSETGLP,1*05\r\n"
#define Command_RET_A_Disable_GNSS_Low_Power "$PQSETGLP,0*04\r\n"

/* Configure the GNSS constellation mask */
// Return Message $PQSETCNSTOK*4D -- $PQSETCNSTERROR*11
#define Command_RET_A_Set_Constellation_ALL "$PQSETCNST,63*60\r\n"
#define Command_RET_A_Set_Constellation_GPS	"$PQSETCNST,1*54\r\n"
#define Command_RET_A_Set_Constellation_Glonass "$PQSETCNST,2*57\r\n"
#define Command_RET_A_Set_Constellation_Galileo "$PQSETCNST,4*51\r\n"
#define Command_RET_A_Set_Constellation_Beidou "$PQSETCNST,8*5D\r\n"
#define Command_RET_A_Set_Constellation_NAVIC "$PQSETCNST,16*62\r\n"
#define Command_RET_A_Set_Constellation_QZSS "$PQSETCNST,32*64\r\n"
//Structures============================//
typedef struct{
    int hour;
    int minute;
    int second;
} Time_t;

typedef struct{
    int day;
    int month;
    int year;
} Date_t;

typedef struct{
    int degree;
    double minute;
    char direction;
} Latitude_t;

typedef struct{
    int degree;
    double minute;
    char direction;
} Longitude_t;

typedef struct{
    Latitude_t latitude;
    Longitude_t longitude;
} Coordinates_t;

typedef struct{
    Coordinates_t coordinates;
    double altitude;
    double speed;
    Time_t time;
    Date_t date;
    double pdop;
} GNSS_Parameters_t;
//Variables==========================//
extern GNSS_Parameters_t frame;
extern char *field;
extern char *fields[15];
extern int field_index;
//Functions Decleratin===============//

void GNSS_Test(void);

/***
 * @brief : Parses the latitude from an NMEA sentence and updates the latitude value 
 *          of the GNSS_Parameters_t structure.
 * 
 * @param frame     A pointer to the GNSS_Parameters_t structure to update.
 * @param sentence  A string containing the NMEA sentence to parse.
*/
void getLatitude(GNSS_Parameters_t *frame, char *sentence);
/***
 * @brief : Parses the longitude from an NMEA sentence and updates the longitude value 
 *          of the GNSS_Parameters_t structure.
 * 
 * @param frame     A pointer to the GNSS_Parameters_t structure to update.
 * @param sentence  A string containing the NMEA sentence to parse.
*/
void getLongitude(GNSS_Parameters_t *frame, char *sentence);
/***
 * @brief : Parses the altitude from an NMEA sentence and updates the altitude value 
 *          of the GNSS_Parameters_t structure.
 * 
 * @param frame     A pointer to the GNSS_Parameters_t structure to update.
 * @param sentence  A string containing the NMEA sentence to parse.
*/
void getAltitude(GNSS_Parameters_t *frame, char *sentence);
/***
 * @brief : Parses the time from an NMEA sentence and updates the time value 
 *          of the GNSS_Parameters_t structure.
 * 
 * @param frame     A pointer to the GNSS_Parameters_t structure to update.
 * @param sentence  A string containing the NMEA sentence to parse.
*/
void getTime(GNSS_Parameters_t *frame, char *sentence);
/***
 * @brief : Parses the date from an NMEA sentence and updates the date value 
 *          of the GNSS_Parameters_t structure.
 * 
 * @param frame     A pointer to the GNSS_Parameters_t structure to update.
 * @param sentence  A string containing the NMEA sentence to parse.
*/
void getDate(GNSS_Parameters_t *frame, char *sentence);
/***
 * @brief : Parses the speed from an NMEA sentence and updates the speed value 
 *          of the GNSS_Parameters_t structure.
 * 
 * @param frame     A pointer to the GNSS_Parameters_t structure to update.
 * @param sentence  A string containing the NMEA sentence to parse.
*/
void getSpeed(GNSS_Parameters_t *frame, char *sentence);
/***
 * @brief : Parses the speed from an NMEA sentence and updates the speed value 
 *          of the GNSS_Parameters_t structure.
 * 
 * @param frame     A pointer to the GNSS_Parameters_t structure to update.
 * @param sentence  A string containing the NMEA sentence to parse.
*/
void getPDOP(GNSS_Parameters_t *frame, char *sentence);
/***
 * @brief : This function starts the GNSS module in a specified mode (cold, warm, or hot).
 * 
 * @param Mode Variable that represents the starting mode of the GNSS module. The possible 
 *              values are Cold, Warm, and Hot start modes.
*/
void GNSS_Start_Mode_NResp(uint8_t Mode);
/***
 * @brief : This function sends a command to reset the GNSS module.
 * 
 * @param no
*/
void GNSS_Reset_NResp(void);
/***
 * @brief : This function sends a command to start the GNSS module.
 * 
 * @param no
*/
void GNSS_Start_Resp(void);
/***
 * @brief : This function sends a command to stop the GNSS module.
 * 
 * @param no
*/
void GNSS_Stop_Resp(void);
/***
 * @brief : This function retrieves the sleep status of the GNSS module and returns 0 
 *          if sleep mode is disabled or 1 if sleep mode is enabled.
 * 
 * @param no
*/
uint8_t GNSS_Get_Sleep_Status_Resp(void);
/***
 * @brief : This function retrieves the low power mode status of the GNSS module 
 *          and prints a message indicating whether low power mode is enabled or disabled.
 * 
 * @param no
*/
uint8_t GNSS_Get_Low_Power_Mode_Status_Resp(void);
/***
 * @brief : This function sets the baud rate of the UART communication with the GNSS 
 *          module to a specified value (115200, 230400, 460800, or 921600).
 * 
 * @param Baudrate Variable that represents the baud rate at which the GNSS module should 
 *                  communicate with the host system. The possible values are 115200, 230400, 460800, and 921600.
*/
void GNSS_Set_Baudrate_Resp(uint32_t Baudrate);
/***
 * @brief : This function is not implemented and appears to be a placeholder for 
 *          retrieving information about the satellites being used for GNSS.
 * 
 * @param no
*/
uint8_t GNSS_Get_Satellite_Info_Resp(void);
/***
 * @brief : This function enables or disables sleep mode of the GNSS module based on 
 *          the Sleep_Mode parameter.
 * 
 * @param Sleep_Mode  Variable that represents whether sleep mode should be enabled or disabled. If the value is Sleep_Enable (0), 
 *                     then sleep mode will be enabled. If the value is Sleep_Disable (1), then sleep mode will be disabled.
*/
void GNSS_Sleep_Mode_Enable_Disable_Resp(uint8_t Sleep_Mode);
/***
 * @brief : This function enables or disables low power mode of the GNSS module based on the 
 *          Low_Power_Mode parameter.
 * 
 * @param Low_Power_Mode variable that represents whether low power mode should be enabled or disabled. 
 *                         If the value is Low_Power_Enable (0), then low power mode will be enabled. If the value is Low_Power_Disable (1), 
 *                         then low power mode will be disabled.
*/
void GNSS_Low_Power_Mode_Enable_Disable_Resp(uint8_t Low_Power_Mode);

#endif /* GNSS_H_ */
