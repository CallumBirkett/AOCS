#include "aocs_log_csv.h"

AOCS_Status aocs_csv_open(AocsCsvLog* log, const char* path) {

    if (!log || !path) return AOCS_ERR_INVALID_ARG;

    log->f = fopen(path, "w");
    if (!log->f) return AOCS_ERR_IO;

    return AOCS_OK;

}

AOCS_Status aocs_csv_write_header(AocsCsvLog* log) {

    if (!log || !log->f) return AOCS_ERR_INVALID_ARG;
    if (fprintf(log->f, "t\n") < 0) return AOCS_ERR_IO;

    return AOCS_OK;
}

AOCS_Status aocs_csv_write_row(AocsCsvLog* log, double t) {

    if (!log || !log->f) return AOCS_ERR_INVALID_ARG;
    if (fprintf(log->f, "%.9f\n", t) < 0) return AOCS_ERR_IO;

    return AOCS_OK;
}

void aocs_csv_close(AocsCsvLog* log) {

    if (!log) return;
    if (log->f) fclose(log->f);
    log->f = NULL;

}