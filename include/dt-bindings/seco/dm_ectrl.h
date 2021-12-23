



#define WDT_CTRL_REG                   0x00
#define GINO_TASK_REG                  0x01
#define DVI2LVDS_FLAGS_REG             0x02
#define DATA_REG                       0x03
#define INDEX_REG                      0x04
#define WDT_DELAY_REG                  0x05
#define WDT_TIMEOUT_REG                0x06
#define WDT_TIMER1_REG                 0x07
#define WDT_TIMER2_REG                 0x08
#define WDT_CONFIG_REG                 0x09
#define ADC_READING_0_REG              0x0A
#define ADC_READING_1_REG              0x0B
#define BUILDREV_REG                   0x0C
#define FW_ID_REV_REG                  0x0E
#define FLAG_REG                       0x0F 
#define ENABLE_REG                     0x10
#define STATUS_REG                     0x11

/* Flash registers */
#define ALWAYS_STATE_REG               0x01
#define WDT_F_DELAY_REG_LSB            0x0A
#define WDT_F_DELAY_REG_MSB            0x0B
#define WDT_F_TIMEOUT_REG_LSB          0x0C
#define WDT_F_TIMEOUT_REG_MSB          0x0D
#define WDT_F_CONFIG_REG_LSB           0x0E
#define WDT_F_CONFIG_REG_MSB           0x0F
#define BOOT0_REG                      0x10
#define BOOT1_REG                      0x11
#define BOOT2_REG                      0x12
#define BOOT3_REG                      0x13
#define EN_FLASH_REG_LSB               0x14
#define EN_FLASH_REG_MSB               0x15

#define SBLOCK_CMD                     0x55AA
#define HALT_CMD                       0x6101
#define REBOOT_CMD                     0x6505


#define PWR_BUTTON                     0
#define RST_BUTTON                     1
#define SLEEP_SIGNAL                   2
#define BATLOW_HL_SIGNAL               3	// when BATLOW becomes HIGH - battery charge low
#define BATLOW_LH_SIGNAL               4 	// when BATLOW becomes LOW - battery full
#define LID_HL_SIGNAL                  5	// when LID becomes HIGH
#define LID_LH_SIGNAL                  6	// when LID becomes LOW
#define WAKE_SIGNAL                    7
#define PWR_BTN_4SEC                   8


#define PWR_BUTTON_MASK_REG            0x0001
#define RST_BUTTON_MASK_REG            0x0002
#define SLEEP_SIGNAL_MASK_REG          0x0004
#define BATLOW_HL_SIGNAL_MASK_REG      0x0008
#define BATLOW_LH_SIGNAL_MASK_REG      0x0010
#define LID_HL_SIGNAL_MASK_REG         0x0020
#define LID_LH_SIGNAL_MASK_REG         0x0040
#define WAKE_SIGNAL_MASK_REG           0x0080
#define PWR_BTN_4SEC_MASK_REG          0x0100


#define PWR_BUTTON_SHIFT_REG           0
#define RST_BUTTON_SHIFT_REG           1
#define SLEEP_SIGNAL_SHIFT_REG         2
#define BATLOW_HL_SIGNAL_SHIFT_REG     3
#define BATLOW_LH_SIGNAL_SHIFT_REG     4
#define LID_HL_SIGNAL_SHIFT_REG        5
#define LID_LH_SIGNAL_SHIFT_REG        6
#define WAKE_SIGNAL_SHIFT_REG          7
#define PWR_BTN_4SEC_SHIFT_REG         8




#define PWR_BUTTON_NAME                "PowerButton"
#define RST_BUTTON_NAME                "ResetButton"
#define SLEEP_SIGNAL_NAME              "Sleep"
#define BATLOW_HL_SIGNAL_NAME          "BatteryLow_H2L"
#define BATLOW_LH_SIGNAL_NAME          "BatteryLow_L2H"
#define LID_HL_SIGNAL_NAME             "Lid_H2L"
#define LID_LH_SIGNAL_NAME             "Lid_L2H"
#define WAKE_SIGNAL_NAME               "Wake"
#define PWR_BTN_4SEC_NAME              "PoworButton4Sec"


#define PWR_BUTTON_LABEL               "power_button"
#define RST_BUTTON_LABEL               "reset_button"
#define SLEEP_SIGNAL_LABEL             "sleep"
#define BATLOW_HL_SIGNAL_LABEL         "battery_low_h2l"
#define BATLOW_LH_SIGNAL_LABEL         "battery_low_l2h"
#define LID_HL_SIGNAL_LABEL            "lid_h2l"
#define LID_LH_SIGNAL_LABEL            "lid_l2h"
#define WAKE_SIGNAL_LABEL              "wake"
#define PWR_BTN_4SEC_LABEL             "pwr_btn_4sec"


#define EVNT_PWR_BTN_STATE_LABEL       "power_button"
#define EVNT_RST_BTN_STATE_LABEL       "reset_button"
#define EVNT_SLEEP_STATE_LABEL         "sleep"
#define EVNT_BATTERY_STATE_LABEL       "battery_low"
#define EVNT_LID_STATE_LABEL           "lid"
#define EVNT_WAKE_STATE_LABEL          "wake"


#define PM_STATE_ALWAYS_ON             "always_on"
#define PM_STATE_ALWAYS_OFF            "always_off"

#define ALWAYS_ON                      0x0055
#define ALWAYS_OFF                     0x0000 


#define WDT_SHIFT_ENABLE               0
#define WDT_SHIFT_EVENT                1

#define WDT_MASK_ENABLE                (0x0001 << WDT_SHIFT_ENABLE)
#define WDT_MASK_EVENT                 (0x0007 << WDT_SHIFT_EVENT)

#define WDT_CTRL_WDTOUT_RESET          (0x0001 << 8) // Auto Resettable Byte
#define WDT_CTRL_WDTOUT_TRIGGER        (0x0002 << 8) // Auto Resettable Byte

#define INPUT_DEV_NAME                 "Embedded Controller"

#define ECTRL_USE_OWN_STATE            0



enum wdt_event {
	WDT_EVNT_WDOUT          = 0,
	WDT_EVNT_RESET          = 1,
	WDT_EVNT_PWRBTN1SEC     = 2,
	WDT_EVNT_PWRBTN4SEC     = 3,
	WDT_EVNT_BOOT_RECOVERY  = 4,
};


enum ECTRL_EVENTS {
	EVNT_PWR_BTN, EVNT_RST_BTN, EVNT_SLEEP,
	EVNT_BATTERY, EVNT_LID, EVNT_WAKE
};


struct data_list {
	u16 data;
	struct list_head list;
};

enum {
	STOP_OP = (unsigned short int)0, 	// stop all operations
	R_OP    = (unsigned short int)1,	// read register operation
	W_OP    = (unsigned short int)2,	// write register operation
	RVE_OP  = (unsigned short int)3,	// read vector's element operation
	WVE_OP  = (unsigned short int)4,	// write vector's element operation
};

struct ectrl_reg {
	u16 addr;
	u16 data;
};

struct ectrl_reg_rw {
	unsigned short int op;
	struct ectrl_reg reg;
};



struct dm_ectrl_ops {
    int (*get_board_id)( struct udevice *dev, u8 *id, char *major_v, u8 *minor_v );
};
