#include "status_handler.hpp"

StatusHandler::StatusHandler(int socket, WemosAansturen& aansturen)
    : socket_(socket), wemosAansturen_(aansturen)
{
    memset(buffer, 0, sizeof(buffer));  // Ensure buffer is initialized
}


