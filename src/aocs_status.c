#include "aocs_status.h"

// return human-readable status updates
const char* aocs_status_str(AOCS_Status s){
    switch (s) {
        case AOCS_OK: return "AOCS_OK";
        case AOCS_ERR_INVALID_ARG: return "AOCS_ERR_INVALID_ARG";
        case AOCS_ERR_IO: return "AOCS_ERR_IO";
        default: return "AOCS_ERR_UNKNOWN";
    }
}