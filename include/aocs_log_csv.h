#ifndef AOCS_LOG_CSV_H
#define AOCS_LOG_CSV_H

#include <stdio.h>
#include "aocs_status.h"

typedef struct {
    FILE* f;
} AocsCsvLog;

AOCS_Status aocs_csv_open(AocsCsvLog* log, const char* path);
AOCS_Status aocs_csv_write_header(AocsCsvLog* log);
AOCS_Status aocs_csv_write_row(AocsCsvLog* log, double t);
void aocs_csv_close(AocsCsvLog* log);

#endif