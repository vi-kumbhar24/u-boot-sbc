

/*
 * Detects board revision.
 * Rev_B = 0x0
 * Rev_C = 0x1
 * Rev_D = 0x2
 * Rev_E = 0x3
 * Rev_F = 0x4
 * Rev_G = 0x5
 * Rev_H = 0x6
 * Rev_I = 0x7
 */
#define BOARD_CODE_0                IMX_GPIO_NR(2, 6)
#define BOARD_CODE_1                IMX_GPIO_NR(2, 7)
#define BOARD_CONF                  IMX_GPIO_NR(5, 3)


#define is_SBC_REVB(x)  (((x) & 0x3) == 0x0)
#define is_SBC_REVC(x)  (((x) & 0x3) == 0x1)


/*
 * Detects the board model.
 * FULL   - 1GB RAM, 2xEth, WiFi, motion sensors  -> GPIO2_0 = 1
 * BASIC  - 512 RAM, 1xEth, WiFi, motion sensors  -> GPIO2_0 = 0
 */
#define SECO_B08_TYPE_BASIC                 1
#define SECO_B08_TYPE_FULL                  2

#define is_SBC_BASIC(x)   ((x)== SECO_B08_TYPE_BASIC)
#define is_SBC_FULL(x)    ((x)== SECO_B08_TYPE_FULL)

