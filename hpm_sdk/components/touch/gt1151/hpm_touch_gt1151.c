/*
 * Copyright (c) 2021 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "board.h"
#include "hpm_touch.h"
#include "hpm_gpio_drv.h"
#include "hpm_gt1151.h"
#include "hpm_touch.h"

gt1151_context_t gt1151 = {0};

hpm_stat_t touch_get_data(touch_point_t *points, uint8_t *num_of_points)
{
    hpm_stat_t stat = status_success;
    gt1151_touch_data_t touch_data = {0};
    uint8_t num, i;

    stat = gt1151_read_touch_data(&gt1151, &touch_data);
    if (stat != status_success) {
        printf("gt1151 read data failed\n");
        return stat;
    }
    /* the buffer status is ready*/
    if (GT1151_GET_STATUS_BUFFER_STAT(touch_data.status) == 1) {
        num = GT1151_GET_STATUS_NUM_OF_POINTS(touch_data.status);
        *num_of_points = num;
        if (num > 0 && num <= GT1151_MAX_TOUCH_POINTS) {
            for (i = 0; i < num; i++) {
                points[i].x = (touch_data.points[i].x_h & 0xF) << 8 | touch_data.points[i].x_l;
                points[i].y = (touch_data.points[i].y_h & 0xF) << 8 | touch_data.points[i].y_l;
            }
        } else {
            stat = status_touch_points_over_number;
        }
    } else {
        stat = status_touch_buffer_no_ready;
    }
    gt1151_write_register(&gt1151, GT1151_STATUS, 0);
    return stat;
}

void pull_int_pin(bool high)
{
    gpio_set_pin_output(BOARD_CAP_INTR_GPIO, BOARD_CAP_INTR_GPIO_INDEX, BOARD_CAP_INTR_GPIO_PIN);
    gpio_write_pin(BOARD_CAP_INTR_GPIO, BOARD_CAP_INTR_GPIO_INDEX, BOARD_CAP_INTR_GPIO_PIN, high);
}

void float_int_pin(void)
{
    gpio_set_pin_input(BOARD_CAP_INTR_GPIO, BOARD_CAP_INTR_GPIO_INDEX, BOARD_CAP_INTR_GPIO_PIN);
}

hpm_stat_t touch_init(I2C_Type *i2c_ptr)
{
    hpm_stat_t stat = status_success;

    gt1151.ptr = i2c_ptr;

    stat = gt1151_init(&gt1151, BOARD_LCD_WIDTH, BOARD_LCD_HEIGHT);
    if (stat != status_success) {
        return stat;
    }
    gt1151_write_register(&gt1151, GT1151_CMD, GT1151_CMD_READ_COORD_STAT);

    return stat;
}
