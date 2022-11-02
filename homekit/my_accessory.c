/*
 * my_accessory.c
 * Define the accessory in C language using the Macro in characteristics.h
 *
 *  Created on: 2020-05-15
 *      Author: Mixiaoxiao (Wang Bin)
 */

#include <homekit/homekit.h>
#include <homekit/characteristics.h>

void my_accessory_identify(homekit_value_t _value) {
	printf("accessory identify\n");
}

// 插座 (HAP section 8.30 开发文档 8.30节)
// 要求特性: ON（需要ON或OFF的状态）
// 可选特性: NAME（可以设置设备名称）

// 格式: bool; 开发文档 9.70节; write the .setter function to get the switch-event sent from iOS Home APP.
homekit_characteristic_t cha_outlet_on = HOMEKIT_CHARACTERISTIC_(ON, false);

// format: string; HAP section 9.62; max length 64
homekit_characteristic_t cha_name = HOMEKIT_CHARACTERISTIC_(NAME, "Outlet");

homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_outlet, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Outlet"),                  //设备名称
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "XCuiTech-HomeKit"),//制造商
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "2022.1022"),      //序列号
            HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266"),                //模块
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),        //固件版本
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
            NULL
        }),
		HOMEKIT_SERVICE(OUTLET, .primary=true, .characteristics=(homekit_characteristic_t*[]){
			&cha_outlet_on,
			&cha_name,
			NULL
		}),
        NULL
    }),
    NULL
};

// password:设备密码
// setupId: 设备ID
homekit_server_config_t config = {
		.accessories = accessories,
		.password = "321-11-654",
        .setupId="12QW"
};
