#include "weather_data.h"
#include <string.h>
#include <time.h>

void weather_data_init(weather_data_t *data)
{
    if (!data) return;

    memset(data, 0, sizeof(weather_data_t));
    data->is_valid = false;
    data->last_update = 0;
}

bool weather_data_is_current(const weather_data_t *data, uint32_t max_age_seconds)
{
    if (!data || !data->is_valid) {
        return false;
    }

    time_t now = time(NULL);
    uint32_t age = (uint32_t)now - data->last_update;

    return age <= max_age_seconds;
}
