#if !defined(SETTING_H)
    #define SETTING_H

    #define BUFF_SIZE   32

    const char REQUEST_CODE ='d';
    const char OPTIMAL_VALUE_CHANE_ACK[BUFF_SIZE] = "done";
    //const char SETTING_CODE ='s';

    #define WAITING_DELAY   2000

    const byte NODE_ADDRESS[6]          =   {"00001"};
    const byte CONTROLLER_ADDRESS[5]    =   {0xCE, 0xCC, 0xCE, 0xCC, 0xCE};

    // Buzzer
    #define ERROR_BEEP              3
    #define INFO_BEEP               2
    #define SUCCESS_BEEP            1
    #define BEEP_TIME_INTERVAL      100

#endif // SETTING_H
