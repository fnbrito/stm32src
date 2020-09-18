/*
 *
C to assembler menu hook
 * 
 */
#include <stdio.h> //TESTE
#include <stdint.h>
#include <ctype.h>

#include "common.h"

int add_test(int x, int y);

ParserReturnVal_t AddTest(int action)
{
    if(action==CMD_SHORT_HELP) return CmdReturnOk;
    if(action==CMD_LONG_HELP) {
    printf("Addition Test\n\n"
    "This command tests new addition function\n"
    );

    return CmdReturnOk;
    }
    printf("add_test returned: %d\n", add_test(88, 99) );
    
    return CmdReturnOk;
}
ADD_CMD("add", AddTest,"Test the new add function")