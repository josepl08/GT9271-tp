/*
 * gt9271.c
 *
 *  Created on: Nov 13, 2020
 *      Author: Desarrollo
 */

#include "gt9271.h"

uint8_t gtp_cfg_data_0[] = {
		// datos extraídos de código ejemplo BuyDisplay.com
		0x00,0x20,0x03,0xE0,0x01,0x0A,0x0D,0x00,
		0x01,0x0A,0x28,0x0F,0x50,0x32,0x03,0x08,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x8E,0x2E,0x88,0x23,0x21,
		0x31,0x0D,0x00,0x00,0x00,0x01,0x03,0x1D,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x1E,0x50,0x94,0xC5,0x02,
		0x07,0x00,0x00,0x04,0x80,0x21,0x00,0x6B,
		0x28,0x00,0x59,0x31,0x00,0x4B,0x3B,0x00,
		0x3F,0x48,0x00,0x3F,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x15,0x14,0x11,0x10,0x0F,0x0E,0x0D,0x0C,
		0x09,0x08,0x07,0x06,0x05,0x04,0x01,0x00,
		0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,
		0x04,0x06,0x07,0x08,0x0A,0x0C,0x0D,0x0F,
		0x10,0x11,0x12,0x13,0x19,0x1B,0x1C,0x1E,
		0x1F,0x20,0x21,0x22,0x23,0x24,0x25,0x26,
		0x27,0x28,0xFF,0xFF,0xFF,0xFF,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x4D,0x01,
};
uint8_t gtp_cfg_data_1[] = {
		// datos extraídos de código ejemplo BuyDisplay.com + 5ms coor refresh rate
		0x00,0x20,0x03,0xE0,0x01,0x0A,0x0D,0x00,
		0x01,0x0A,0x28,0x0F,0x50,0x32,0x03,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x8E,0x2E,0x88,0x23,0x21,
		0x31,0x0D,0x00,0x00,0x00,0x01,0x03,0x1D,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x1E,0x50,0x94,0xC5,0x02,
		0x07,0x00,0x00,0x04,0x80,0x21,0x00,0x6B,
		0x28,0x00,0x59,0x31,0x00,0x4B,0x3B,0x00,
		0x3F,0x48,0x00,0x3F,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x15,0x14,0x11,0x10,0x0F,0x0E,0x0D,0x0C,
		0x09,0x08,0x07,0x06,0x05,0x04,0x01,0x00,
		0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,
		0x04,0x06,0x07,0x08,0x0A,0x0C,0x0D,0x0F,
		0x10,0x11,0x12,0x13,0x19,0x1B,0x1C,0x1E,
		0x1F,0x20,0x21,0x22,0x23,0x24,0x25,0x26,
		0x27,0x28,0xFF,0xFF,0xFF,0xFF,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x4D,0x01,
};

uint32_t gtp_cgf_data_len = 186;

static gt9271_i2c_status_t gt9271_write_cfg_data(gt9271_h *handler, uint8_t cfg);
static uint8_t gt9271_calc_chksum(uint8_t *buff, uint32_t len);

gt9271_init_stat_t gt9271_init(gt9271_h *handler){
	uint8_t w_buf[10];
	uint8_t r_buf[10];
	uint8_t cfg_chksum;
	uint32_t	i;
	gt9271_i2c_status_t i2c_stat;
	gt9271_init_stat_t init_stat;

	i2c_stat = gt9271_i2c_stat_OK;

	handler->gt_9271_rst_pin_set(0);
	handler->gt_9271_delay(200);
	handler->gt_9271_rst_pin_set(1);

	// 1. 	OBTENER DIRECCIONES DE LECTURA/ESCRITURA (0X28-0X29 / 0XBA-0XBB)
	handler->gt_9271_delay(100);
	// 1.1 	prueba enviando comando escritura
	w_buf[0] = GT9271_REG_PROD_ID_BASE >> 8;
	w_buf[1] = GT9271_REG_PROD_ID_BASE &  0xFF;
	// 1.2	escritura i2c
	if(handler->gt9271_i2c_write(0xBB, w_buf, 2) != gt9271_i2c_stat_OK){
		handler->gt_9271_delay(100);
		if(handler->gt9271_i2c_write(0x28, w_buf, 2) !=  gt9271_i2c_stat_OK){
			return gt9271_init_ERR;
		}else{
			handler->w_addres = 0x28;
			handler->r_addres = 0x29;
		}
	}else{
		handler->w_addres = 0xBB;
		handler->r_addres = 0xBA;
	}
	// 2. OBTENER ID DEL CPT
	handler->gt9271_i2c_write(handler->w_addres, w_buf, 2);
	handler->gt9271_i2c_read(handler->r_addres, r_buf, 4);
	for(i=0; i<4; i++){
		handler->cpt_id[i] = r_buf[i];
	}
	// 3. OBTENER VERSION DE FIRMWARE?
	w_buf[0] = GT9271_REG_CFG_VERSION >> 8;
	w_buf[1] = GT9271_REG_CFG_VERSION &  0xFF;
	handler->gt9271_i2c_write(handler->w_addres, w_buf, 2);
	handler->gt9271_i2c_read(handler->r_addres, r_buf, 1);
	for(i=0; i<4; i++){
		handler->cpt_id[i] = r_buf[i];
	}

	// Obtener checksum de  configuración
	cfg_chksum = gt9271_calc_chksum(gtp_cfg_data_1, 184);
	handler->checksum = cfg_chksum;
	gtp_cfg_data_1[184] = cfg_chksum;
	// Mandar datos de configuración
	//i2c_stat = gt9271_write_cfg_data(handler,1);
	i2c_stat = gt9271_i2c_stat_OK;
	if(i2c_stat != gt9271_i2c_stat_OK){
		init_stat= gt9271_init_ERR;
	}else{
		init_stat = gt9271_init_OK;
	}
	handler->gt_9271_delay(500);
	// establecer umbral
/*	w_buf[0]=40;
	w_buf[1]=22;
	stat = gt9271_write_reg(handler, 0x8053, w_buf, 2);
	w_buf[0]=1;
	stat = gt9271_write_reg(handler, 0x8100, w_buf, 1);*/
	return init_stat;
}

gt9271_i2c_status_t gt9271_write_reg(gt9271_h *handler, uint16_t addr, uint8_t *data, const uint32_t len){
	uint8_t 	frame[GT9271_MAX_FRAMES];
	uint32_t	i;
	gt9271_i2c_status_t i2c_stat;

	if (len > GT9271_MAX_FRAMES){
		return gt9271_i2c_stat_ERR;
	}
	frame[0] = (uint8_t)(addr >>   8);
	frame[1] = (uint8_t)(addr & 0xFF);
	for(i=0; i<len; i++){
		frame[i+2] = data[i];
	}
	i2c_stat = handler->gt9271_i2c_write(handler->w_addres, frame, len+2);
	return i2c_stat;
}

gt9271_i2c_status_t gt9271_read_reg(gt9271_h *handler, uint16_t addr, uint8_t *data, const uint32_t len){
	uint8_t 	frame[2];
	gt9271_i2c_status_t i2c_stat;

	if (len > GT9271_MAX_FRAMES){
		return gt9271_i2c_stat_ERR;
	}
	frame[0] = (uint8_t)(addr >>   8);
	frame[1] = (uint8_t)(addr & 0xFF);

	i2c_stat = handler->gt9271_i2c_write(handler->w_addres, frame, 2);
	if(i2c_stat){
		return gt9271_i2c_stat_ERR;
	}
	i2c_stat = handler->gt9271_i2c_read(handler->r_addres, data, len);
	if(i2c_stat){
		return gt9271_i2c_stat_ERR;
	}
	return i2c_stat;
}
gt9271_i2c_status_t gt9271_set_touch_points(gt9271_h *handler, uint8_t num){
	uint8_t temp;
	gt9271_i2c_status_t stat;
	temp = num & 0x0F;
	stat = gt9271_write_reg(handler, 0x804C, &temp, 1);
	return stat;
}
gt9271_tp_stat_t gt9271_read_coordinates(gt9271_h *handler){
	uint8_t i2c_recv_data[81];
	uint8_t status_reg=0, points, i, idx_base, wbyte_buf, rbyte_buf;
	uint8_t threshold[2];
	uint16_t temp;
	gt9271_i2c_status_t status;
	gt9271_tp_stat_t	tp_stat;
	// leer status de buffer
	status = gt9271_read_reg(handler, GT9271_REG_READ_COORD, i2c_recv_data, 81);
	if(status != gt9271_i2c_stat_OK){
		return gt9271_tp_ERR;
	}
	status_reg = i2c_recv_data[0];
	handler->status = status_reg;
	points = status_reg & 0x0F;
	handler->n_points = points;
	// limpiar buffer status
	wbyte_buf = 0;
	status = gt9271_write_reg(handler, GT9271_REG_READ_COORD, &wbyte_buf, 1);
	if(status != gt9271_i2c_stat_OK){
		return gt9271_tp_ERR;
	}
	//status = gt9271_read_reg(handler, GT9271_REG_READ_COORD, &rbyte_buf, 1);
	if(handler->is_data_ready==1){ // bit7:


		/* LEER CADA UNO DE LOS PUNTOs. De momento se leerán los registros de los
		 * 10 puntos. Posteriormente se buscará optimizar para sólo leer los puntos
		 * solicitados.
		 */
		for(i=0;i<10;i++){
			idx_base = 8*i;
			handler->touch_point[i].id = i2c_recv_data[idx_base+1];
			temp = (uint16_t)i2c_recv_data[idx_base+3];
			temp<<=8;
			temp+=i2c_recv_data[idx_base+2];
			handler->touch_point[i].x = temp;
			temp = (uint16_t)i2c_recv_data[idx_base+5];
			temp<<=8;
			temp+=i2c_recv_data[idx_base+4];
			handler->touch_point[i].y = temp;
			temp = (uint16_t)i2c_recv_data[idx_base+7];
			temp<<=8;
			temp+=i2c_recv_data[idx_base+6];
			handler->touch_point[i].size = temp;
		}
		tp_stat = gt9271_tp_PRESSED;

	}else{
/*		for(i=0;i<10;i++){
			handler->touch_point[i].id=0;
			handler->touch_point[i].x=0;
			handler->touch_point[i].y=0;
			handler->touch_point[i].size=0;
		}*/
		tp_stat = gt9271_tp_NOTOUCH;
	}
	handler->is_data_ready=0;
	status = gt9271_read_reg(handler, 0x8053, handler->threshold, 2);
	return tp_stat;
}
// funciones estáticas
static gt9271_i2c_status_t gt9271_write_cfg_data(gt9271_h *handler, uint8_t cfg){
	gt9271_i2c_status_t stat;
	switch(cfg){
	case 0:
		stat = gt9271_write_reg(handler, GT9271_REG_CFG_VERSION, gtp_cfg_data_0, gtp_cgf_data_len);
		break;
	case 1:
		stat = gt9271_write_reg(handler, GT9271_REG_CFG_VERSION, gtp_cfg_data_1, gtp_cgf_data_len);
		break;
	default:
		stat = gt9271_i2c_stat_ERR;
		break;
	}

	return stat;
}
static uint8_t gt9271_calc_chksum(uint8_t *buff, uint32_t len){
	uint8_t i;
	uint8_t chksum = 0;
	for (i=0; i<len;i++){
		chksum+=buff[i];
	}
	return ~chksum+1;
}
