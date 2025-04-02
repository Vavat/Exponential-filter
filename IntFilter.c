#include "IntFilter.h"
#include <stm32f0xx.h>

/**
 * @fn void initFilter(int_filter_t*, uint8_t)
 * @brief Initialise filter coefficient
 *
 * @param p_filter Pointer to the filter structure variable.
 * @param filter_coefficient Filter coefficient. 0 - no filtration, 1 - 50%, 2 -
 * 75%, 3 - 87.5%, 4 - 93.75
 * @note filter_coefficient that approaches the resolution of filtered value
 * becomes highly inefficient. We recommend starting with 2 or 3 and adjusting
 * as needed.
 */
void initFilter(int_filter_t *p_filter, uint8_t filter_coefficient)
{
    if (filter_coefficient == 0) filter_coefficient = 1;
    p_filter->filter_coeff = filter_coefficient;
}

/**
 * @fn uint32_t runFilter(int_filter_t*, uint32_t)
 * @brief Run filtration event on a new sample.
 *
 * @param p_filter Filter structure variable pointer.
 * @param new_value New sample value.
 * @return Latest calculated filtered value. This value also gets stored in
 * filter structure as ->mean.
 */
uint32_t runFilter(int_filter_t *p_filter, uint32_t new_value)
{
    APSR_Type apsr;
    uint32_t filtered_value = p_filter->mean << p_filter->filter_coeff;
    filtered_value -= p_filter->mean;
    filtered_value += new_value;
    p_filter->mean = filtered_value >> p_filter->filter_coeff;
    apsr.w = __get_APSR();
    if (apsr.b.C)
    {
        p_filter->mean++;
    }
    p_filter->last = new_value;
    return (p_filter->mean);
}
