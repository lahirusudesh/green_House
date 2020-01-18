#if !defined(SETTING_H)
    #define SETTING_H

    #define CENTRAL_NAME "Test Location"

    const char REQUEST_CODE =   'd';


    #define WAITING_DELAY   2000
    #define BUFF_SIZE       32 // Buffer size canot get more than 32byte

    const byte NODE_ADDRESS[][5]        =   {   0xCC, 0xCE, 0xCC, 0xCE, 0xCC,
                                                0xCC, 0xCE, 0xCC, 0xCE, 0xEE,
                                            };
    const byte CONTROLLER_ADDRESS[5]    =   {0xCE, 0xCC, 0xCE, 0xCC, 0xCE};

    #define ERROR_MSG   "ERROR"
    #define NUMITEMS(arg) ((unsigned int) (sizeof(arg) / sizeof(arg[0]))) // Count number of arry Items

    #define RETRY_TIMES 5

    #define DEBUG false

#endif // SETTING_H
