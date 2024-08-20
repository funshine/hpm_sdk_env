/*
 * Copyright (c) 2021 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef HPM_GT1151_H
#define HPM_GT1151_H
#include "board.h"
#include "hpm_common.h"
#include "hpm_i2c_drv.h"

#ifdef BOARD_GT1151_ADDR
/* if i2c addres is specified by board, use it */
#define GT1151_I2C_ADDR BOARD_GT1151_ADDR
/* no auto probe in this case */
#define GT1151_NO_AUTO_PROBE 1
#else
#undef GT1151_I2C_ADDR

/* enable auto probe */
#ifndef GT1151_NO_AUTO_PROBE
#define GT1151_NO_AUTO_PROBE 0
#endif

/* i2c device address candidates */
#define GT1151_I2C_ADDR0 (0x14U)
#define GT1151_I2C_ADDR1 (0x5DU)
#endif

#define GT1151_PRODUCT_ID (0x38353131U)
/*
 * GT1151 registers at operation mode
 */

#define GT1151_CMD                    (0x8040U)
#define   GT1151_CMD_READ_COORD_STAT  (0U)
#define   GT1151_CMD_READ_RAW_DATA    (1U)
#define   GT1151_CMD_SOFT_RESET       (2U)
#define   GT1151_CMD_READ_SCREEN_OFF  (5U)

#define GT1151_CONFIG                    (0x8050U)

#define GT1151_ID_B0                     (0x8140U)
#define GT1151_ID_B1                     (0x8141U)
#define GT1151_ID_B2                     (0x8142U)
#define GT1151_ID_B3                     (0x8143U)
#define GT1151_FW_PATCH_CID              (0x8144U)
#define GT1151_FW_PATCH_MAJOR            (0x8145U)
#define GT1151_FW_PATCH_MINOR            (0x8146U)
#define GT1151_FW_MASK_MAJOR             (0x8147U)
#define GT1151_FW_MASK_MINOR             (0x8148U)
#define GT1151_FW_MASK_INSIDE            (0x8149U)
#define GT1151_VENDOR_ID                 (0x814AU)
#define GT1151_STATUS                    (0x814EU)
#define GT1151_GET_STATUS_NUM_OF_POINTS(x)  ((x) & 0xFU)
#define GT1151_GET_STATUS_LARGE_DETECT(x)   (((x) & 0x40U) >> 6)
#define GT1151_GET_STATUS_BUFFER_STAT(x)    (((x) & 0x80U) >> 7)
#define GT1151_FIRST_POINT               (0x814FU)

#define GT1151_MAX_TOUCH_POINTS             (10U)
#define GT1151_CONFIG_DATA_SIZE             (239U)
#define GT1151_CONFIG_DATA_RESOLUTION_XL    (1U)
#define GT1151_CONFIG_DATA_RESOLUTION_XH    (2U)
#define GT1151_CONFIG_DATA_RESOLUTION_YL    (3U)
#define GT1151_CONFIG_DATA_RESOLUTION_YH    (4U)
#define GT1151_CONFIG_DATA_TOUCH_NUMBER     (5U)
#define GT1151_CONFIG_DATA_MODULE_SWITCH1   (6U)

typedef struct {
    uint8_t track_id;
    uint8_t x_l;
    uint8_t x_h;
    uint8_t y_l;
    uint8_t y_h;
    uint8_t size_l;
    uint8_t size_h;
    uint8_t reserved;
} gt1151_touch_point_t;

typedef struct {
    uint8_t status;
    gt1151_touch_point_t points[GT1151_MAX_TOUCH_POINTS];
} gt1151_touch_data_t;

typedef struct {
    I2C_Type *ptr;
} gt1151_context_t;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * gt1151 initialization routine
 */
hpm_stat_t gt1151_init(gt1151_context_t *context, uint16_t width, uint16_t height);

/*
 * gt1151 read touch data
 */
hpm_stat_t gt1151_read_touch_data(gt1151_context_t *context,
                                  gt1151_touch_data_t *touch_data);

/*
 * gt1151 read data
 */
hpm_stat_t gt1151_read_data(gt1151_context_t *context, uint16_t addr,
                            uint8_t *buf, uint32_t size);

/*
 * gt1151 write value to given register
 */
hpm_stat_t gt1151_write_register(gt1151_context_t *context,
                                 uint16_t reg, uint8_t val);

/*
 * gt1151 read value of given register
 */
hpm_stat_t gt1151_read_register(gt1151_context_t *context, uint16_t reg, uint8_t *buf);

/*
 * gt1151 read config data
 */
hpm_stat_t gt1151_read_config(gt1151_context_t *context, uint8_t *buf, uint8_t size);
#ifdef __cplusplus
}
#endif
#endif /* HPM_GT1151_H */
