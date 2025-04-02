#pragma once
#include "stdint.h"

typedef struct
{
    uint8_t filter_coeff;
    uint32_t mean;
    uint32_t last;
} int_filter_t;

void initFilter(int_filter_t *filter, uint8_t filter_coefficient);
uint32_t runFilter(int_filter_t *filter, uint32_t new_value);
