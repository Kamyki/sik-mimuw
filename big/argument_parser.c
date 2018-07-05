//
// Created by Kamil Mykitiuk on 02/06/2018.
//

#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "argument_parser.h"
#include "err.h"

bool used_mcast = false;
char error[100];

void convertPort(int64_t* port) {
    *port = strtol(optarg, NULL, 10);
    if (*port < 0) {
        syserr("error reading port");
    }
    if (*port < 0 || *port > UINT16_MAX) {
        syserr("Port number should be between 0 and UINT16_MAX");
    }
}

void convertNumber(int64_t* number_pinter) {
    *number_pinter = strtol(optarg, NULL, 10);
    if (*number_pinter < 0) {
        syserr("error converting number");
    }
    if (*number_pinter < 0) {
        syserr("number should be greater than 0");
    }
}

void defaullParams(parameters* params) {
    strcpy(params->MCAST_ADDR, "255.255.255.255");
    strcpy(params->DISCOVER_ADDR, "255.255.255.255");
    params->DATA_PORT = 20000 + (376861 % 10000);
    params->CTRL_PORT = 30000 + (376861 % 10000);
    params->UI_PORT = 10000 + (376861 % 10000);
    params->PSIZE = 512;
    params->BSIZE = 65536;
    params->FSIZE = 128 * 1024;
    params->RTIME = 250;
    strcpy(params->NAZWA, "Nienazwany Nadajnik");
}

void parseArgumentsTransmiter(int argc, char* argv[], parameters* params) {
    int c;
    defaullParams(params);
    while ((c = getopt(argc, argv, ":a:d:C:U:b:f:R:p:n:")) != -1) {
        switch (c) {
            case 'a':
                strcpy(params->MCAST_ADDR, optarg);
                used_mcast = true;
                break;
            case 'd':
                strcpy(params->DISCOVER_ADDR, optarg);
                break;
            case 'C':
                convertPort(&(params->CTRL_PORT));
                break;
            case 'U':
                convertPort(&(params->UI_PORT));
                break;
            case 'p':
                convertNumber(&(params->PSIZE));
                break;
            case 'b':
                convertNumber(&(params->BSIZE));
                break;
            case 'f':
                convertNumber(&(params->FSIZE));
                break;
            case 'R':
                convertNumber(&(params->RTIME));
                break;
            case 'n':
                if (strlen(optarg) > 64) {
                    syserr("Name is too long, max 64 characters");
                }
                strcpy(params->NAZWA, optarg);
                break;
            case '?':
                if (isprint(optopt))
                    snprintf(error, 100, "Unknown option `-%c'.", optopt);
                else
                    snprintf(error, 100, "Unknown option character `\\x%x'.", optopt);
                syserr(error);
                break;
            case ':':
                snprintf (error, 100, "Option -%c requires an argument.", optopt);
                syserr(error);
                break;
            default:
                syserr("Error parsing arguments");
        }
    }
    if (used_mcast != true)
        syserr("Parameter -a is required");
}

void parseArgumentsReciver(int argc, char* argv[], parameters* params) {
    int c;
    defaullParams(params);
    while ((c = getopt(argc, argv, ":d:C:U:b:R:")) != -1) {
        switch (c) {
            case 'd':
                strcpy(params->DISCOVER_ADDR, optarg);
                break;
            case 'C':
                convertPort(&(params->CTRL_PORT));
                break;
            case 'U':
                convertPort(&(params->UI_PORT));
                break;
            case 'b':
                convertNumber(&(params->BSIZE));
                break;
            case 'R':
                convertNumber(&(params->RTIME));
                break;
            case '?':
                if (isprint(optopt))
                    snprintf(error, 100, "Unknown option `-%c'.", optopt);
                else
                    snprintf(error, 100, "Unknown option character `\\x%x'.", optopt);
                syserr(error);
                break;
            case ':':
                snprintf (error, 100, "Option -%c requires an argument.", optopt);
                syserr(error);
                break;
            default:
                abort();
        }
    }
}
