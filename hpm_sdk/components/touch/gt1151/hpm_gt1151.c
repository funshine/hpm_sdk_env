/*
 * Copyright (c) 2021 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "hpm_gt1151.h"
static uint8_t g_i2c_addr;

hpm_stat_t gt1151_read_data(gt1151_context_t *context, uint16_t addr, uint8_t *buf, uint32_t size)
{
    uint8_t r[2];
    r[0] = addr >> 8;
    r[1] = addr & 0xFF;
    return i2c_master_address_read(context->ptr, g_i2c_addr, r, sizeof(r), buf, size);
}

hpm_stat_t gt1151_write_data(gt1151_context_t *context, uint16_t addr, uint8_t *buf, uint32_t size)
{
    uint8_t r[2];
    r[0] = addr >> 8;
    r[1] = addr & 0xFF;

    return i2c_master_address_write(context->ptr, g_i2c_addr, r, sizeof(r), buf, size);
}

hpm_stat_t gt1151_read_register(gt1151_context_t *context, uint16_t reg, uint8_t *buf)
{
    return gt1151_read_data(context, reg, buf, 1);
}

hpm_stat_t gt1151_write_register(gt1151_context_t *context, uint16_t reg, uint8_t val)
{
    return gt1151_write_data(context, reg, &val, 1);
}

hpm_stat_t gt1151_soft_reset(gt1151_context_t *context)
{
    return gt1151_write_register(context, GT1151_CMD, GT1151_CMD_SOFT_RESET);
}

static uint8_t gt1151_calcualte_config_data_checksum(uint8_t *config)
{
    uint8_t checksum = 0;
    for (uint32_t i = 0; i < (GT1151_CONFIG_DATA_SIZE - 3); i++) {
        checksum += config[i];
    }
    return (~checksum + 1);
}

static uint16_t gt1151_calcualte_config_data_checksum_16(uint8_t *config)
{
    uint16_t checksum = 0;
    uint16_t raw_cfg_len = GT1151_CONFIG_DATA_SIZE - 3;
    for (uint32_t i = 0; i < raw_cfg_len; i+=2) {
        checksum += ((uint16_t)config[i] << 8) + (uint16_t)config[i+1]; // unaligned be16
    }
    return (~checksum + 1);
}

hpm_stat_t gt1151_read_config(gt1151_context_t *context, uint8_t *buf, uint8_t size)
{
    return gt1151_read_data(context, GT1151_CONFIG, buf, size);
}

hpm_stat_t gt1151_init(gt1151_context_t *context, uint16_t width, uint16_t height)
{
    hpm_stat_t stat = status_success;
    uint8_t config[GT1151_CONFIG_DATA_SIZE] = {0};

#ifdef GT1151_I2C_ADDR
    g_i2c_addr = GT1151_I2C_ADDR;
    stat = gt1151_read_data(context, GT1151_CONFIG, config, sizeof(config));
    if (stat != status_success) {
        return stat;
    }
#elif !GT1151_NO_AUTO_PROBE
    g_i2c_addr = GT1151_I2C_ADDR0;
    stat = gt1151_read_data(context, GT1151_CONFIG, config, sizeof(config));
    if (stat != status_success) {
        printf("0x%x failed to init GT1151", g_i2c_addr);
        g_i2c_addr = GT1151_I2C_ADDR1;
        printf(", try 0x%x\n", g_i2c_addr);
        stat = gt1151_read_data(context, GT1151_CONFIG, config, sizeof(config));
    }
    if (stat != status_success) {
        return stat;
    }
#else
    g_i2c_addr = GT1151_I2C_ADDR0;
    stat = gt1151_read_data(context, GT1151_CONFIG, config, sizeof(config));
    if (stat != status_success) {
        return stat;
    }
#endif

    uint16_t cal_checksum = gt1151_calcualte_config_data_checksum_16(config);
    uint16_t recv_checksum = ((uint16_t)config[GT1151_CONFIG_DATA_SIZE - 3] << 8) + (uint16_t)config[GT1151_CONFIG_DATA_SIZE - 2];
    if (recv_checksum != cal_checksum) {
        return status_fail;
    }

    // uint32_t pid = 0;
    // stat = gt1151_read_data(context, GT1151_ID_B0, (uint8_t*)&pid, sizeof(pid));
    // if (stat != status_success) {
    //     return stat;
    // }

    // if (pid != GT1151_PRODUCT_ID) {
    //     return status_fail;
    // }

    config[GT1151_CONFIG_DATA_RESOLUTION_XL] = width & 0xFF;
    config[GT1151_CONFIG_DATA_RESOLUTION_XH] = width >> 8;
    config[GT1151_CONFIG_DATA_RESOLUTION_YL] = height & 0xFF;
    config[GT1151_CONFIG_DATA_RESOLUTION_YH] = height >> 8;
    config[GT1151_CONFIG_DATA_TOUCH_NUMBER] = GT1151_MAX_TOUCH_POINTS;
    config[GT1151_CONFIG_DATA_MODULE_SWITCH1] = (config[GT1151_CONFIG_DATA_MODULE_SWITCH1] & ~0x3);

    cal_checksum = gt1151_calcualte_config_data_checksum_16(config);
    config[GT1151_CONFIG_DATA_SIZE - 3] = (cal_checksum & 0xFF00) >> 8;
    config[GT1151_CONFIG_DATA_SIZE - 2] = (cal_checksum & 0x00FF) >> 0;
    config[GT1151_CONFIG_DATA_SIZE - 1] = 1;
    /*
     * for (uint8_t i = 0; i < 5; i++) {
     *     gt1151_write_data(context, GT1151_CONFIG, config, GT1151_CONFIG_DATA_SIZE);
     * }
     */
    gt1151_write_register(context, GT1151_CMD, GT1151_CMD_SOFT_RESET);
    return status_success;
}

hpm_stat_t gt1151_read_touch_data(gt1151_context_t *context,
                                  gt1151_touch_data_t *touch_data)
{
    hpm_stat_t stat = status_success;

    stat = gt1151_read_data(context, GT1151_STATUS,
            (uint8_t *)touch_data, sizeof(gt1151_touch_data_t));
    if (stat != status_success) {
        return stat;
    }

    gt1151_write_register(context, GT1151_STATUS, 0);

    return stat;
}
