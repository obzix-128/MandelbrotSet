#include "ErrorHandler.h"


/*------------------------------------------------------------------------------------------------------------------------------------
Обработчик ошибок, выводит в stderr информацию об ошибке и имя функции, где она произошла.
ARGUMENTS: ErrorNumbers error    - номер ошибки
           const char* func_name - имя функции
------------------------------------------------------------------------------------------------------------------------------------*/
ErrorNumbers errorHandler(ErrorNumbers error, const char* func_name)
{
    fprintf(stderr, "%s\n", func_name);

    switch(error)
    {
        case NO_ERROR:
        {
            fprintf(stderr, "No errors were found\n");
            break;
        }
        case NULL_ADDRESS_ERROR:
        {
            fprintf(stderr, "Error: A null pointer was passed\n");
            break;
        }
        case LOADING_FONT_ERROR:
        {
            fprintf(stderr, "Error: Failed to load font\n");
            break;
        }
        case MODE_ERROR:
        {
            fprintf(stderr, "Error: Non-existent program mode selected\n");
            break;
        }
        default:
        {
            fprintf(stderr, "Unknown error\n");
            break;
        }
    }

    return NO_ERROR;
}