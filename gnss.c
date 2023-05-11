/***
 *      Conure Tracker
 *    GNSS Module driver
 *      9th Apr 2023
 *  Author : Mohamed Abdulaziz
*/

//Includes================================//
#include "gnss.h"
//Variables===============================//
GNSS_Parameters_t frame;
char *field;
char *fields[15];
int field_index = 0;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
//Functions Implementation================//
void GNSS_Test(void) {
    char sentence[500];
    char OutputBuffer[252];

    //Get the NMEA sentence and store it in an array
    HAL_UART_Receive(&huart1, sentence, sizeof(sentence), HAL_MAX_DELAY);
    HAL_Delay(1000);

    //Call the parsing function to make each one of them do its task
    getLatitude(&frame, &sentence);
    getLongitude(&frame, &sentence);
    getAltitude(&frame, &sentence);
    getTime(&frame, &sentence);
    getDate(&frame, &sentence);
    getSpeed(&frame, &sentence);
    getPDOP(&frame, &sentence);

    sprintf(OutputBuffer, "=================GNSS=============\r\n"
                          "======================================\r\n"
                          "Latitude: %d %0.2f %c\r\nLongitude: %d %0.2f %c\r\nAltitude: %0.2f m\r\n"
                          "Speed: %0.2f Km/h\r\nTime: %2d:%2d:%2d\r\nDate: %2d/%2d/%4d\r\nPDOP: %0.2f\r\n"
                          "======================================\r\n",
                          frame.coordinates.latitude.degree, 
                          frame.coordinates.latitude.minute,
                          frame.coordinates.latitude.direction,
                          frame.coordinates.longitude.degree, 
                          frame.coordinates.longitude.minute,
                          frame.coordinates.longitude.direction,
                          frame.altitude, 
                          frame.speed, 
                          frame.time.hour, 
                          frame.time.minute, 
                          frame.time.second,
                          frame.date.day,
                          frame.date.month,
                          frame.date.year,
                          frame.pdop
            );

    //Print the values on the serial
	HAL_UART_Transmit(&huart2, OutputBuffer, sizeof(OutputBuffer), HAL_MAX_DELAY);
}

void getLatitude(GNSS_Parameters_t *frame, char *sentence) {
    // Define the start and end words of the sentence to search for
    char start_word[] = "GGA";
    char end_word[] = "\r\n";

    // Find the position of the start word in the sentence
    char *start_pos = strstr(sentence, start_word);
    if (start_pos == NULL) {
        printf("Start word not found.\n");
        return 0;
    }

    // Find the position of the end word in the sentence
    char *end_pos = strstr(start_pos, end_word);
    if (end_pos == NULL) {
        printf("End word not found.\n");
        return 0;
    }

    // Calculate the length of the phrase between the start and end words
    int phrase_length = end_pos - start_pos + strlen(end_word);

    // Create a buffer to store the phrase
    char *phrase = (char *) malloc((phrase_length + 1) * sizeof(char));

    // Copy the phrase into the buffer
    strncpy(phrase, start_pos, phrase_length);
    phrase[phrase_length] = '\0';

    // Split the phrase into fields delimited by commas
    char *field = strtok(phrase, ",");
    int field_index = 0;
    while (field != NULL && field_index < 15)
    {
        fields[field_index] = field;
        field_index++;
        field = strtok(NULL, ",");
    }

    // Extract the degrees, minutes and directions from the latitude field
    char degree_str[3], minute_str[8];
    strncpy(degree_str, fields[2], 2);
    degree_str[2] = '\0';

    frame->coordinates.latitude.degree = atoi(degree_str);

    strncpy(minute_str, fields[2] + 2, 7);
    minute_str[7] = '\0';

    frame->coordinates.latitude.minute = atof(minute_str);

    frame->coordinates.latitude.direction = fields[3][0];

    // Free the memory allocated for the phrase buffer
    free(phrase);
}

void getLongitude(GNSS_Parameters_t *frame, char *sentence) {
    // Define the start and end words of the sentence to search for
    char start_word[] = "GGA";
    char end_word[] = "\r\n";

    // Find the position of the start word in the sentence
    char *start_pos = strstr(sentence, start_word);
    if (start_pos == NULL) {
        printf("Start word not found.\n");
        return 0;
    }

    // Find the position of the end word in the sentence
    char *end_pos = strstr(start_pos, end_word);
    if (end_pos == NULL) {
        printf("End word not found.\n");
        return 0;
    }

    // Calculate the length of the phrase between the start and end words
    int phrase_length = end_pos - start_pos + strlen(end_word);

    // Create a buffer to store the phrase
    char *phrase = (char *) malloc((phrase_length + 1) * sizeof(char));

    // Copy the phrase into the buffer
    strncpy(phrase, start_pos, phrase_length);
    phrase[phrase_length] = '\0';

    // Split the phrase into fields delimited by commas
    char *field = strtok(phrase, ",");
    field_index = 0;
    while (field != NULL && field_index < 15)
    {
        fields[field_index] = field;
        field_index++;
        field = strtok(NULL, ",");
    }

    // Extract the degrees, minutes and directions from the longitude field
    char degree_str[3], minute_str[8];
    strncpy(degree_str, fields[4], 3);
    degree_str[3] = '\0';

    frame->coordinates.longitude.degree = atoi(degree_str);

    strncpy(minute_str, fields[4] + 3, 7);
    minute_str[7] = '\0';

    frame->coordinates.longitude.minute = atof(minute_str);

    frame->coordinates.longitude.direction = fields[5][0];

    // Free the memory allocated for the phrase buffer
    free(phrase);
}

void getAltitude(GNSS_Parameters_t *frame, char *sentence) {
    // Define the start and end words of the sentence to search for
    char start_word[] = "GGA";
    char end_word[] = "\r\n";

    // Find the position of the start word in the sentence
    char *start_pos = strstr(sentence, start_word);
    if (start_pos == NULL) {
        printf("Start word not found.\n");
        return 0;
    }

    // Find the position of the end word in the sentence
    char *end_pos = strstr(start_pos, end_word);
    if (end_pos == NULL) {
        printf("End word not found.\n");
        return 0;
    }

    // Calculate the length of the phrase between the start and end words
    int phrase_length = end_pos - start_pos + strlen(end_word);

    // Create a buffer to store the phrase
    char *phrase = (char *) malloc((phrase_length + 1) * sizeof(char));

    // Copy the phrase into the buffer
    strncpy(phrase, start_pos, phrase_length);
    phrase[phrase_length] = '\0';

    // Split the phrase into fields delimited by commas
    char *field = strtok(phrase, ",");
    field_index = 0;
    while (field != NULL && field_index < 15)
    {
        fields[field_index] = field;
        field_index++;
        field = strtok(NULL, ",");
    }

    frame->altitude = atof(fields[9]);

    // Free the memory allocated for the phrase buffer
    free(phrase);
}

void getTime(GNSS_Parameters_t *frame, char *sentence) {
    // Define the start and end words of the sentence to search for
    char start_word[] = "GGA";
    char end_word[] = "\r\n";

    // Find the position of the start word in the sentence
    char *start_pos = strstr(sentence, start_word);
    if (start_pos == NULL) {
        printf("Start word not found.\n");
        return 0;
    }

    // Find the position of the end word in the sentence
    char *end_pos = strstr(start_pos, end_word);
    if (end_pos == NULL) {
        printf("End word not found.\n");
        return 0;
    }

    // Calculate the length of the phrase between the start and end words
    int phrase_length = end_pos - start_pos + strlen(end_word);

    // Create a buffer to store the phrase
    char *phrase = (char *) malloc((phrase_length + 1) * sizeof(char));

    // Copy the phrase into the buffer
    strncpy(phrase, start_pos, phrase_length);
    phrase[phrase_length] = '\0';

    // Split the phrase into fields delimited by commas
    char *field = strtok(phrase, ",");
    field_index = 0;
    while (field != NULL && field_index < 15)
    {
        fields[field_index] = field;
        field_index++;
        field = strtok(NULL, ",");
    }

    // Extract the hour, minute, and second from the time field
    char hour_str[3], minute_str[3], second_str[3];
    strncpy(hour_str, fields[1], 2);
    hour_str[2] = '\0';
    strncpy(minute_str, fields[1] + 2, 2);
    minute_str[2] = '\0';
    strncpy(second_str, fields[1] + 4, 2);
    second_str[2] = '\0';
    char *endptr;

    frame->time.hour = atoi(hour_str);
    frame->time.minute = atoi(minute_str);
    frame->time.second = atof(second_str);

    // Free the memory allocated for the phrase buffer
    free(phrase);
}

void getDate(GNSS_Parameters_t *frame, char *sentence) {
    // Define the start and end words of the sentence to search for
    char start_word[] = "RMC";
    char end_word[] = "\r\n";

    // Find the position of the start word in the sentence
    char *start_pos = strstr(sentence, start_word);
    if (start_pos == NULL) {
        printf("Start word not found.\n");
        return;
    }

    // Find the position of the end word in the sentence
    char *end_pos = strstr(start_pos, end_word);
    if (end_pos == NULL) {
        printf("End word not found.\n");
        return;
    }

    // Calculate the length of the phrase between the start and end words
    int phrase_length = end_pos - start_pos + strlen(end_word);

    // Create a buffer to store the phrase
    char *phrase = (char *) malloc((phrase_length + 1) * sizeof(char));

    // Copy the phrase into the buffer
    strncpy(phrase, start_pos, phrase_length);
    phrase[phrase_length] = '\0';

    // Split the phrase into fields delimited by commas
    char *field = strtok(phrase, ",");
    int field_index = 0;
    while (field != NULL && field_index < 15)
    {
        fields[field_index] = field;
        field_index++;
        field = strtok(NULL, ",");
    }

    // Extract the day, month, and year from the date field
    char date_str[7];
    strncpy(date_str, fields[9], 6);
    date_str[6] = '\0';

    char day_str[3], month_str[3], year_str[3];
    strncpy(day_str, date_str, 2);
    day_str[2] = '\0';
    strncpy(month_str, date_str + 2, 2);
    month_str[2] = '\0';
    strncpy(year_str, date_str + 4, 2);
    year_str[2] = '\0';

    frame->date.day = atoi(day_str);
    frame->date.month = atoi(month_str);
    frame->date.year = atoi(year_str) + 2000;

    // Free the memory allocated for the phrase buffer
    free(phrase);
}

void getSpeed(GNSS_Parameters_t *frame, char *sentence) {
    // Define the start and end words of the sentence to search for
    char start_word[] = "VTG";
    char end_word[] = "\r\n";

    // Find the position of the start word in the sentence
    char *start_pos = strstr(sentence, start_word);
    if (start_pos == NULL) {
        printf("Start word not found.\n");
        return 0;
    }

    // Find the position of the end word in the sentence
    char *end_pos = strstr(start_pos, end_word);
    if (end_pos == NULL) {
        printf("End word not found.\n");
        return 0;
    }

    // Calculate the length of the phrase between the start and end words
    int phrase_length = end_pos - start_pos + strlen(end_word);

    // Create a buffer to store the phrase
    char *phrase = (char *) malloc((phrase_length + 1) * sizeof(char));

    // Copy the phrase into the buffer
    strncpy(phrase, start_pos, phrase_length);
    phrase[phrase_length] = '\0';

    // Split the phrase into fields delimited by commas
    char *field = strtok(phrase, ",");
    int field_index = 0;
    while (field != NULL && field_index < 15)
    {
        fields[field_index] = field;
        field_index++;
        field = strtok(NULL, ",");
    }

    // Extracts the speed from its field
    frame->speed = (atof(fields[7]));

    // Free the memory allocated for the phrase buffer
    free(phrase);
}

void getPDOP(GNSS_Parameters_t *frame, char *sentence) {
    // Define the start and end words of the sentence to search for
    char start_word[] = "GSA";
    char end_word[] = "\r\n";

    // Find the position of the start word in the sentence
    char *start_pos = strstr(sentence, start_word);
    if (start_pos == NULL) {
        printf("Start word not found.\n");
        return 0;
    }

    // Find the position of the end word in the sentence
    char *end_pos = strstr(start_pos, end_word);
    if (end_pos == NULL) {
        printf("End word not found.\n");
        return 0;
    }

    // Calculate the length of the phrase between the start and end words
    int phrase_length = end_pos - start_pos + strlen(end_word);

    // Create a buffer to store the phrase
    char *phrase = (char *) malloc((phrase_length + 1) * sizeof(char));

    // Copy the phrase into the buffer
    strncpy(phrase, start_pos, phrase_length);
    phrase[phrase_length] = '\0';

    // Split the phrase into fields delimited by commas
    char *field = strtok(phrase, ",");
    int field_index = 0;
    while (field != NULL && field_index < 15)
    {
        fields[field_index] = field;
        field_index++;
        field = strtok(NULL, ",");
    }

    // Extracts the PDOP from its field
    frame->pdop = atof(fields[4]);

    // Free the memory allocated for the phrase buffer
    free(phrase);
}

void GNSS_Start_Mode_NResp(uint8_t Mode) {
	switch (Mode) {
	case Cold: {
        // Transmit the command to start the GNSS module in cold mode
		HAL_UART_Transmit(&huart2, Command_NRET_Cold_Start_GNSS, 12,
		HAL_MAX_DELAY);
		break;
	}

	case Warm: {
        // Transmit the command to start the GNSS module in warm mode
		HAL_UART_Transmit(&huart2, Command_NRET_Warm_Start_GNSS, 12,
		HAL_MAX_DELAY);
		break;
	}
	case Hot: {
        // Transmit the command to start the GNSS module in hot mode
		HAL_UART_Transmit(&huart2, Command_NRET_Hot_Start_GNSS, 11,
		HAL_MAX_DELAY);
		break;
	}
	}
}

void GNSS_Reset_NResp(void) {
    //Sends a reset command to the GNSS module via UART.
	HAL_UART_Transmit(&huart2, Command_NRET_GNSS_Reset, 11, HAL_MAX_DELAY);
}

void GNSS_Start_Resp(void) {
    //Sends a start command to the GNSS module via UART and expects a response from the module.
	HAL_UART_Transmit(&huart2, Command_RET_NA_Start_GNSS, 17, HAL_MAX_DELAY);
}

void GNSS_Stop_Resp(void) {
    //Sends a stop command to the GNSS module via UART and expects a response from the module.
	HAL_UART_Transmit(&huart2, Command_RET_NA_STOP_GNSS, 16, HAL_MAX_DELAY);
}

uint8_t GNSS_Get_Sleep_Status_Resp(void) {
	uint8_t Sleep_Buffer[20];
	uint8_t Command[] = Command_RET_NA_Get_Sleep_Status;
	uint8_t Sleep_Test[] = "$PQSLEEP,1*53";
	HAL_UART_Transmit(&huart2, Command, 16, HAL_MAX_DELAY);
		// FIXME remove UART1
	HAL_UART_Transmit(&huart1, Command, 16, HAL_MAX_DELAY);
	HAL_UART_Receive(&huart1, Sleep_Buffer, 12, 5000);
	if (strcmp(Sleep_Buffer, Sleep_Test)) {
		HAL_UART_Transmit(&huart2, "Sleep Mode: Disabled\r\n", 22,
		HAL_MAX_DELAY);
		return 0;
	} else {
		HAL_UART_Transmit(&huart2, "Sleep Mode: Enabled\r\n", 21,
		HAL_MAX_DELAY);
		return 1;
	}
	//HAL_UART_Transmit(&huart2,"Sleep Mode: ---\r\n",17,HAL_MAX_DELAY);
}

uint8_t GNSS_Get_Low_Power_Mode_Status_Resp(void) {
		// Create buffers for receiving data from UART
	uint8_t Low_Mode_Buffer[20];
	uint8_t Low_Mode_Test[] = "$PQGLP,1*47\r\n";

	// Transmit the command to get low power mode status
	HAL_UART_Transmit(&huart2, Command_RET_NA_Get_Low_Power_Mode_Status, 14,
	HAL_MAX_DELAY);
	
	// Receive the response from the GNSS module
	HAL_UART_Receive(&huart2, Low_Mode_Buffer, sizeof(Low_Mode_Buffer),
	HAL_MAX_DELAY);

	// Compare the received response with the expected response to determine if low power mode is enabled or disabled
	if (strcmp(Low_Mode_Buffer, Low_Mode_Test)) {
		// Low power mode is disabled
		HAL_UART_Transmit(&huart2, "Low Power Mode: Disabled\r\n", 26,
		HAL_MAX_DELAY);
	} else {
		// Low power mode is enabled
		HAL_UART_Transmit(&huart2, "Low Power Mode: Enabled\r\n", 25,
		HAL_MAX_DELAY);
	}
}

void GNSS_Set_Baudrate_Resp(uint32_t Baudrate) {
    // Use a switch statement to select the appropriate command based on the desired baudrate.
	switch (Baudrate) {
	case 115200: {
		HAL_UART_Transmit(&huart2, Command_RET_NA_Set_Baudrate_115200, 22,
		HAL_MAX_DELAY);
		break;
	}

	case 230400: {
		HAL_UART_Transmit(&huart2, Command_RET_NA_Set_Baudrate_230400, 22,
		HAL_MAX_DELAY);
		break;
	}

	case 460800: {
		HAL_UART_Transmit(&huart2, Command_RET_NA_Set_Baudrate_460800, 22,
		HAL_MAX_DELAY);
		break;
	}

	case 921600: {
		HAL_UART_Transmit(&huart2, Command_RET_NA_Set_Baudrate_921600, 22,
		HAL_MAX_DELAY);
		break;
	}
	}
}

uint8_t GNSS_Get_Satellite_Info_Resp(void) {
//	uint8_t Satellite_Buffer[300];
//
//	// Send command to request satellite information
//	HAL_UART_Transmit(&huart2, Command_RET_NA_Get_Satellite_Info, 16, HAL_MAX_DELAY);
//
//	// Receive satellite information from GNSS module
//	HAL_UART_Receive(&huart2, Satellite_Buffer, sizeof(Satellite_Buffer), HAL_MAX_DELAY);
//
//	// Check if there are any satellites in view
//	if (strstr(Satellite_Buffer, "$GPGSV") != NULL) {
//		HAL_UART_Transmit(&huart2, "Satellites in view: ", 20, HAL_MAX_DELAY);
//		// Parse satellite information and get the number of satellites in view
//		uint8_t num_of_sats = Satellite_Info_Parse(Satellite_Buffer);
//		// Convert the number of satellites to a string and send it to the UART
//		char num_of_sats_str[3];
//		sprintf(num_of_sats_str, "%d", num_of_sats);
//		HAL_UART_Transmit(&huart2, num_of_sats_str, strlen(num_of_sats_str), HAL_MAX_DELAY);
//		HAL_UART_Transmit(&huart2, "\r\n", 2, HAL_MAX_DELAY);
//		return num_of_sats;
//	} else {
//		HAL_UART_Transmit(&huart2, "No satellites in view\r\n", 23, HAL_MAX_DELAY);
//		return 0;
//	}
}

void GNSS_Sleep_Mode_Enable_Disable_Resp(uint8_t Sleep_Mode) {
	switch (Sleep_Mode) 
	{
		case Sleep_Enable: 
		{
			HAL_UART_Transmit(&huart2, Command_RET_A_Enable_Sleep_Mode, 18,
			HAL_MAX_DELAY);
			break;
		}

		case Sleep_Disable: 
		{
			HAL_UART_Transmit(&huart2, Command_RET_A_Disable_Sleep_Mode, 18,
			HAL_MAX_DELAY);
			break;
		}
	}
}

void GNSS_Low_Power_Mode_Enable_Disable_Resp(uint8_t Low_Power_Mode) {
	switch (Low_Power_Mode) {
	case Low_Power_Enable: {
		HAL_UART_Transmit(&huart2, Command_RET_A_Enable_GNSS_Low_Power, 16,
		HAL_MAX_DELAY);
		break;
	}

	case Low_Power_Disable: {
		HAL_UART_Transmit(&huart2, Command_RET_A_Disable_GNSS_Low_Power, 16,
		HAL_MAX_DELAY);
		break;
	}
	}
}
