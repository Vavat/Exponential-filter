#include "CExpFilter.h"
#include <cmath>

float CExpFilter::set(float new_value)
{
    if (mb_initialised)
    {
        m_last_value = new_value;
        // Average
        m_average = filter(m_average, new_value);
        /* Min/max threshold is slowly tracking towards average. Once new value is outside the threshold that becomes a
         * new min/max value and threshold is pushed outside as well. */
        m_min_threshold = filter(m_min_threshold, m_average);
        m_max_threshold = filter(m_max_threshold, m_average);
        if (new_value < m_min_threshold)
        {
            m_min           = new_value;
            m_min_threshold = new_value;
        }

        if (m_max_threshold < new_value)
        {
            m_max           = new_value;
            m_max_threshold = new_value;
        }
        /* Standard deviation is calculated by filtering variance and then taking square root of variance. */
        float variance = pow(m_average - new_value, 2);
        variance       = filter(pow(m_stdev, 2), variance);
        m_stdev        = sqrt(variance);
    }
    else
    {
        m_average       = new_value;
        m_last_value    = new_value;
        m_min           = new_value;
        m_max           = new_value;
        m_min_threshold = new_value;
        m_max_threshold = new_value;
        m_stdev         = 0;
        mb_initialised  = true;
    }
    return m_average;
}

float CExpFilter::get() const
{
    return m_average;
}

void CExpFilter::reset()
{
    mb_initialised = false;
}

void CExpFilter::setAlpha(float new_alpha)
{
    if (new_alpha > 0.0f && new_alpha <= 1.0f)
    {
        m_alpha = new_alpha;
    }
}

inline float CExpFilter::filter(float old_value, float new_value)
{
    float output = old_value * (1 - m_alpha);
    output += new_value * m_alpha;
    return output;
}
