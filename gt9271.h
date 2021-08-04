/*
 * gt9271.h
 *
 *  Created on: Nov 13, 2020
 *      Author: Desarrollo
 */

#ifndef INC_GT9271_H_
#define INC_GT9271_H_



#include "stm32f7xx_hal.h"


#define 	GT9271_MAX_FRAMES			200

#define		GT9271_STAT_INITIALIZED		0x01

#define 	GT9271_REG_CFG_VERSION		0X8047
#define		GT9271_REG_PROD_ID_BASE		0x8140
#define 	GT9271_REG_READ_COORD		0x814E

typedef enum{
	gt9271_i2c_stat_OK,
	gt9271_i2c_stat_ERR
}gt9271_i2c_status_t;

typedef enum{
	gt9271_init_NA,
	gt9271_init_OK,
	gt9271_init_ERR
}gt9271_init_stat_t;

typedef enum{
	gt9271_tp_NOTOUCH,
	gt9271_tp_PRESSED,
	gt9271_tp_ERR
}gt9271_tp_stat_t;

typedef struct{
	uint16_t 	x;
	uint16_t	y;
	uint16_t 	size;
	uint8_t 	id;
}gt9271_touch_point_t;

typedef struct{
	uint8_t 	w_addres;
	uint8_t 	r_addres;
	uint8_t 	status;
	uint8_t		is_data_ready;

	uint8_t 	cpt_id[4];
	uint8_t 	fw_version[2];

	uint8_t		n_points;
	gt9271_touch_point_t touch_point[10];
	uint16_t 	x_resolution;
	uint16_t 	y_resolution;
	uint8_t    threshold[2];

	uint16_t	x_max_val;
	uint16_t	y_max_val;

	uint8_t		checksum;

	// callbacks
	gt9271_i2c_status_t (*gt9271_i2c_write)(uint16_t, uint8_t*, uint32_t);
	gt9271_i2c_status_t (*gt9271_i2c_read)(uint16_t, uint8_t*, uint32_t);
	void (*gt_9271_rst_pin_set)(uint8_t);
	void (*gt_9271_delay)(uint32_t);
}gt9271_h;

gt9271_init_stat_t gt9271_init(gt9271_h *handler);
gt9271_i2c_status_t gt9271_read_reg(gt9271_h *handler, uint16_t addr, uint8_t *data, const uint32_t len);
gt9271_i2c_status_t gt9271_write_reg(gt9271_h *handler, uint16_t addr, uint8_t *data, const uint32_t len);
gt9271_tp_stat_t gt9271_read_coordinates(gt9271_h *handler);
gt9271_i2c_status_t gt9271_set_touch_points(gt9271_h *handler, uint8_t num);
#endif /* INC_GT9271_H_ */
