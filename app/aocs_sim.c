#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aocs_status.h"
#include "aocs_log_csv.h"

typedef struct {

    double t_end;
    double dt;
    unsigned int seed;
    const char* log_path;

} SimArgs;

static void sim_args_set_defaults(SimArgs* a) {

    a->t_end = 1.0;
    a->dt = 0.1;
    a->seed = 1u;
    a->log_path = "out.csv";

}

// read in doubles
static AOCS_Status parse_double(const char* s, double* out) {
    if (!s || !out) return AOCS_ERR_INVALID_ARG;
    char* end = NULL;
    double v = strtod(s, &end);
    if (end == s || *end != '\0') return AOCS_ERR_INVALID_ARG;
    *out = v;
    return AOCS_OK;
}

// read in unsigned int
static AOCS_Status parse_uint(const char* s, unsigned int* out) {
    if (!s || !out) return AOCS_ERR_INVALID_ARG;
    char* end = NULL;
    unsigned long v = strtoul(s, &end, 10);
    if (end == s || *end != '\0') return AOCS_ERR_INVALID_ARG;
    *out = (unsigned int)v;
    return AOCS_OK;
}

// CLI
static AOCS_Status sim_args_parse(SimArgs* a, int argc, char** argv) { 
    if (!a) return AOCS_ERR_INVALID_ARG;
    sim_args_set_defaults(a); 

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--t_end") == 0) {
            if (i + 1 >= argc) return AOCS_ERR_INVALID_ARG;
            AOCS_TRY(parse_double(argv[++i], &a->t_end));
        } else if (strcmp(argv[i], "--dt") == 0) {
            if (i + 1 >= argc) return AOCS_ERR_INVALID_ARG;
            AOCS_TRY(parse_double(argv[++i], &a->dt));
        } else if (strcmp(argv[i], "--seed") == 0) {
            if (i + 1 >= argc) return AOCS_ERR_INVALID_ARG;
            AOCS_TRY(parse_uint(argv[++i], &a->seed));
        } else if (strcmp(argv[i], "--log") == 0) {
            if (i + 1 >= argc) return AOCS_ERR_INVALID_ARG;
            a->log_path = argv[++i];
        } else {
            return AOCS_ERR_INVALID_ARG; // indicates some unknown flag
        }
    }

    if (a->dt <= 0.0 || a->t_end < 0.0) return AOCS_ERR_INVALID_ARG;
    return AOCS_OK;
}

static AOCS_Status run_sim(const SimArgs* a) {
    if (!a) return AOCS_ERR_INVALID_ARG;

    AocsCsvLog log = {0};
    AOCS_TRY(aocs_csv_open(&log, a->log_path));
    AOCS_TRY(aocs_csv_write_header(&log));

    for (double t = 0.0; t <= a->t_end + 0.5 * a-> dt; t += a->dt) {
        AOCS_TRY(aocs_csv_write_row(&log, t));
    }

    aocs_csv_close(&log);
    return AOCS_OK;
}

int main(int argc, char** argv) {
    SimArgs args;
    AOCS_Status s = sim_args_parse(&args, argc, argv);
    if (s != AOCS_OK) {
        fprintf(stderr, "Argument error (%s)\n)", aocs_status_str(s));
        fprintf(stderr, "Usage: ./aocs_sim [--t_end T] [--dt DT] [--seed N] [--log PATH]\n");
        return 2;
    }

    printf("running sim: t_end=%.3f dt = %.3f seed %u log=%s\n",
         args.t_end, args.dt, args.seed, args.log_path);
    
    s= run_sim(&args);
    if (s != AOCS_OK) {
        fprintf(stderr, "Sim failed (%s)\n", aocs_status_str(s));
        return 1;
    }

    return 0;
}
