#include <zephyr/device.h>
#include <zephyr/drivers/can.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

static const struct device *const can_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_canbus));

int main(void)
{
	const struct can_frame frame = {
		.id = 0x123,
		.dlc = 4,
		.flags = 0,
		.data = {0xDE, 0xAD, 0xBE, 0xEF},
	};

	int ret;

	printk("deadbeef_can start\n");

	if (!device_is_ready(can_dev)) {
		printk("CAN device is not ready: %s\n", can_dev->name);
		return 0;
	}

	printk("CAN device: %s\n", can_dev->name);

	ret = can_set_mode(can_dev, CAN_MODE_NORMAL);

	if (ret != 0) {
		printk("can_set_mode failed: %d\n", ret);
		return 0;
	}

	ret = can_start(can_dev);
    
	if (ret != 0) {
		printk("can_start failed: %d\n", ret);
		return 0;
	}

	while (1) {
		ret = can_send(can_dev, &frame, K_MSEC(100), NULL, NULL);

        if (ret == 0) {
			printk("TX: ID=0x%03x DATA=DE AD BE EF\n", frame.id);
		} else {
			printk("can_send failed: %d\n", ret);
		}

		k_sleep(K_SECONDS(1));
	}

	return 0;
}
