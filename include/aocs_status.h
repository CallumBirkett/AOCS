// error codes and early return helper 

#ifndef AOCS_STATUS_H
#define AOCS_STATUS_H

// error codes
typedef enum {
    AOCS_OK = 0,
    AOCS_ERR_INVALID_ARG = 1,
    AOCS_ERR_IO = 2,
} AOCS_Status;

// if an error code is returned, stop early
#define AOCS_TRY(expr)                      \
        do {                                \
            AOCS_Status _s = (expr);        \
            if (_s != AOCS_OK) return _s;   \
        } while (0)

// define human-readable output
const char* aocs_status_str(AOCS_Status s);

#endif

