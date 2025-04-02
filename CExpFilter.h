#pragma once

/**
 * @brief Cutoff frequency of the exponential filter is governed by the following equation.
 * F3dB/Fsample = 1/(2*Pi) * acos(1-alpha²/(2*(1-alpha)))
 *
 * Common ratios of 3dB cut-off frequency vs alpha values.
 * alpha        3dB
 * 0.2          0.351977
 * 0.1          0.165653
 * 0.05         0.080589
 * 0.02         0.031735
 * 0.01         0.015787
 * 0.005        0.007874
 * 0.002        0.003145
 * 0.001        0.001572
 * 0.0005       0.000786
 *
 * Below alpha value of 0.1, the ratio of alpha to cut-off frequency is 1.6, i.e. if alpha is set to 0.01, then 3dB
 * cut-off frequency will be roughly 0.01 * 1.6 * Fsample. Above alpha of 0.1 relationship slightly deviates from
 * linearity, but then exponential filter with alpha above 0.1 stops making sense as it does very little useful
 * processing beyond smoothing some very sharp outliers.
 */

class ExpFilter
{
public:
    /**
     * @brief Construct a new Exp Filter object
     *
     * @param alpha Filtration coefficient. Defaults to 1 for no filtration.
     */
    ExpFilter(float alpha) : m_alpha(alpha), mb_initialised(false) {}

    /**
     * @brief Add new value to the filter.
     *
     * @param new_value New value.
     * @return float Updated filtered value.
     */
    float set(float new_value);

    /**
     * @brief Reset filter.
     */
    void reset();

    /**
     * @brief Get filtered value.
     *
     * @return float current filtered value.
     */
    float get() const;

    /**
     * @brief Set new filtration coefficient. Old data will not be affected.
     *
     * @param new_alpha New filtration coefficient.
     */
    void setAlpha(float new_alpha);

    /**
     * @brief Get the rolling maximum.
     *
     * @return float current rolling maximum.
     */
    float getMax() const
    {
        return m_max;
    }

    /**
     * @brief Get the rolling minimum.
     *
     * @return float current rolling minimum.
     */
    float getMin() const
    {
        return m_min;
    }

    /**
     * @brief Get the standard deviation.
     *
     * @return float current rolling standard deviation.
     */
    float getSTD() const
    {
        return m_stdev;
    }

    /**
     * @brief Get the Last unfiltered value input into the filter.
     *
     * @return float Last unfiltered raw value.
     */
    float getLastValue() const
    {
        return m_last_value;
    }

private:
    inline float filter(float old_value, float new_value);

    float m_alpha;       // Smoothing factor (0 < m_alpha ≤ 1).
    float m_average;     // Stores the running average.
    float m_last_value;  // Last raw value that was input into the filter.
    float m_stdev;       // Stores the running standard deviation.
    float m_min;         // Stores running minimum.
    float m_max;         // Stores running maximum.
    float m_min_threshold;
    float m_max_threshold;
    float mb_initialised;  // Tracks whether the filter has been initialized.
};
