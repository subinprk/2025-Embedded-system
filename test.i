# 0 "src/main.c"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "src/main.c"
# 1 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/io.h" 1 3
# 99 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/io.h" 3
# 1 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/sfr_defs.h" 1 3
# 126 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/sfr_defs.h" 3
# 1 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/inttypes.h" 1 3
# 37 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/inttypes.h" 3
# 1 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/lib/gcc/avr/15.2.0/include/stdint.h" 1 3 4
# 9 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/lib/gcc/avr/15.2.0/include/stdint.h" 3 4
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
# 1 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/stdint.h" 1 3 4
# 125 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/stdint.h" 3 4
typedef signed int int8_t __attribute__((__mode__(__QI__)));
typedef unsigned int uint8_t __attribute__((__mode__(__QI__)));
typedef signed int int16_t __attribute__ ((__mode__ (__HI__)));
typedef unsigned int uint16_t __attribute__ ((__mode__ (__HI__)));
typedef signed int int32_t __attribute__ ((__mode__ (__SI__)));
typedef unsigned int uint32_t __attribute__ ((__mode__ (__SI__)));

typedef signed int int64_t __attribute__((__mode__(__DI__)));
typedef unsigned int uint64_t __attribute__((__mode__(__DI__)));
# 146 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/stdint.h" 3 4
typedef int16_t intptr_t;




typedef uint16_t uintptr_t;
# 163 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/stdint.h" 3 4
typedef int8_t int_least8_t;




typedef uint8_t uint_least8_t;




typedef int16_t int_least16_t;




typedef uint16_t uint_least16_t;




typedef int32_t int_least32_t;




typedef uint32_t uint_least32_t;







typedef int64_t int_least64_t;






typedef uint64_t uint_least64_t;
# 217 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/stdint.h" 3 4
typedef int8_t int_fast8_t;




typedef uint8_t uint_fast8_t;




typedef int16_t int_fast16_t;




typedef uint16_t uint_fast16_t;




typedef int32_t int_fast32_t;




typedef uint32_t uint_fast32_t;







typedef int64_t int_fast64_t;






typedef uint64_t uint_fast64_t;
# 277 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/stdint.h" 3 4
typedef int64_t intmax_t;




typedef uint64_t uintmax_t;
# 12 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/lib/gcc/avr/15.2.0/include/stdint.h" 2 3 4
#pragma GCC diagnostic pop
# 38 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/inttypes.h" 2 3
# 77 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/inttypes.h" 3
typedef int32_t int_farptr_t;





typedef uint32_t uint_farptr_t;
# 127 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/sfr_defs.h" 2 3
# 100 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/io.h" 2 3
# 538 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/io.h" 3
# 1 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 1 3
# 65 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef volatile uint8_t register8_t;
typedef volatile uint16_t register16_t;
typedef volatile uint32_t register32_t;
# 115 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef struct AC_struct
{
    register8_t CTRLA;
    register8_t reserved_1[1];
    register8_t MUXCTRLA;
    register8_t reserved_2[1];
    register8_t DACREF;
    register8_t reserved_3[1];
    register8_t INTCTRL;
    register8_t STATUS;
} AC_t;


typedef enum AC_HYSMODE_enum
{
    AC_HYSMODE_OFF_gc = (0x00<<1),
    AC_HYSMODE_10mV_gc = (0x01<<1),
    AC_HYSMODE_25mV_gc = (0x02<<1),
    AC_HYSMODE_50mV_gc = (0x03<<1)
} AC_HYSMODE_t;


typedef enum AC_INTMODE_enum
{
    AC_INTMODE_BOTHEDGE_gc = (0x00<<4),
    AC_INTMODE_NEGEDGE_gc = (0x02<<4),
    AC_INTMODE_POSEDGE_gc = (0x03<<4)
} AC_INTMODE_t;


typedef enum AC_LPMODE_enum
{
    AC_LPMODE_DIS_gc = (0x00<<3),
    AC_LPMODE_EN_gc = (0x01<<3)
} AC_LPMODE_t;


typedef enum AC_MUXNEG_enum
{
    AC_MUXNEG_PIN0_gc = (0x00<<0),
    AC_MUXNEG_PIN1_gc = (0x01<<0),
    AC_MUXNEG_PIN2_gc = (0x02<<0),
    AC_MUXNEG_DACREF_gc = (0x03<<0)
} AC_MUXNEG_t;


typedef enum AC_MUXPOS_enum
{
    AC_MUXPOS_PIN0_gc = (0x00<<3),
    AC_MUXPOS_PIN1_gc = (0x01<<3),
    AC_MUXPOS_PIN2_gc = (0x02<<3),
    AC_MUXPOS_PIN3_gc = (0x03<<3)
} AC_MUXPOS_t;
# 176 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef struct ADC_struct
{
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t CTRLC;
    register8_t CTRLD;
    register8_t CTRLE;
    register8_t SAMPCTRL;
    register8_t MUXPOS;
    register8_t reserved_1[1];
    register8_t COMMAND;
    register8_t EVCTRL;
    register8_t INTCTRL;
    register8_t INTFLAGS;
    register8_t DBGCTRL;
    register8_t TEMP;
    register8_t reserved_2[2];
    __extension__ union { register16_t RES; struct { register8_t RESL; register8_t RESH; }; };
    __extension__ union { register16_t WINLT; struct { register8_t WINLTL; register8_t WINLTH; }; };
    __extension__ union { register16_t WINHT; struct { register8_t WINHTL; register8_t WINHTH; }; };
    register8_t CALIB;
    register8_t reserved_3[1];
} ADC_t;


typedef enum ADC_ASDV_enum
{
    ADC_ASDV_ASVOFF_gc = (0x00<<4),
    ADC_ASDV_ASVON_gc = (0x01<<4)
} ADC_ASDV_t;


typedef enum ADC_DUTYCYC_enum
{
    ADC_DUTYCYC_DUTY50_gc = (0x00<<0),
    ADC_DUTYCYC_DUTY25_gc = (0x01<<0)
} ADC_DUTYCYC_t;


typedef enum ADC_INITDLY_enum
{
    ADC_INITDLY_DLY0_gc = (0x00<<5),
    ADC_INITDLY_DLY16_gc = (0x01<<5),
    ADC_INITDLY_DLY32_gc = (0x02<<5),
    ADC_INITDLY_DLY64_gc = (0x03<<5),
    ADC_INITDLY_DLY128_gc = (0x04<<5),
    ADC_INITDLY_DLY256_gc = (0x05<<5)
} ADC_INITDLY_t;


typedef enum ADC_MUXPOS_enum
{
    ADC_MUXPOS_AIN0_gc = (0x00<<0),
    ADC_MUXPOS_AIN1_gc = (0x01<<0),
    ADC_MUXPOS_AIN2_gc = (0x02<<0),
    ADC_MUXPOS_AIN3_gc = (0x03<<0),
    ADC_MUXPOS_AIN4_gc = (0x04<<0),
    ADC_MUXPOS_AIN5_gc = (0x05<<0),
    ADC_MUXPOS_AIN6_gc = (0x06<<0),
    ADC_MUXPOS_AIN7_gc = (0x07<<0),
    ADC_MUXPOS_AIN8_gc = (0x08<<0),
    ADC_MUXPOS_AIN9_gc = (0x09<<0),
    ADC_MUXPOS_AIN10_gc = (0x0A<<0),
    ADC_MUXPOS_AIN11_gc = (0x0B<<0),
    ADC_MUXPOS_AIN12_gc = (0x0C<<0),
    ADC_MUXPOS_AIN13_gc = (0x0D<<0),
    ADC_MUXPOS_AIN14_gc = (0x0E<<0),
    ADC_MUXPOS_AIN15_gc = (0x0F<<0),
    ADC_MUXPOS_DACREF_gc = (0x1C<<0),
    ADC_MUXPOS_TEMPSENSE_gc = (0x1E<<0),
    ADC_MUXPOS_GND_gc = (0x1F<<0)
} ADC_MUXPOS_t;


typedef enum ADC_PRESC_enum
{
    ADC_PRESC_DIV2_gc = (0x00<<0),
    ADC_PRESC_DIV4_gc = (0x01<<0),
    ADC_PRESC_DIV8_gc = (0x02<<0),
    ADC_PRESC_DIV16_gc = (0x03<<0),
    ADC_PRESC_DIV32_gc = (0x04<<0),
    ADC_PRESC_DIV64_gc = (0x05<<0),
    ADC_PRESC_DIV128_gc = (0x06<<0),
    ADC_PRESC_DIV256_gc = (0x07<<0)
} ADC_PRESC_t;


typedef enum ADC_REFSEL_enum
{
    ADC_REFSEL_INTREF_gc = (0x00<<4),
    ADC_REFSEL_VDDREF_gc = (0x01<<4),
    ADC_REFSEL_VREFA_gc = (0x02<<4)
} ADC_REFSEL_t;


typedef enum ADC_RESSEL_enum
{
    ADC_RESSEL_10BIT_gc = (0x00<<2),
    ADC_RESSEL_8BIT_gc = (0x01<<2)
} ADC_RESSEL_t;


typedef enum ADC_SAMPNUM_enum
{
    ADC_SAMPNUM_ACC1_gc = (0x00<<0),
    ADC_SAMPNUM_ACC2_gc = (0x01<<0),
    ADC_SAMPNUM_ACC4_gc = (0x02<<0),
    ADC_SAMPNUM_ACC8_gc = (0x03<<0),
    ADC_SAMPNUM_ACC16_gc = (0x04<<0),
    ADC_SAMPNUM_ACC32_gc = (0x05<<0),
    ADC_SAMPNUM_ACC64_gc = (0x06<<0)
} ADC_SAMPNUM_t;


typedef enum ADC_WINCM_enum
{
    ADC_WINCM_NONE_gc = (0x00<<0),
    ADC_WINCM_BELOW_gc = (0x01<<0),
    ADC_WINCM_ABOVE_gc = (0x02<<0),
    ADC_WINCM_INSIDE_gc = (0x03<<0),
    ADC_WINCM_OUTSIDE_gc = (0x04<<0)
} ADC_WINCM_t;
# 306 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef struct BOD_struct
{
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t reserved_1[6];
    register8_t VLMCTRLA;
    register8_t INTCTRL;
    register8_t INTFLAGS;
    register8_t STATUS;
    register8_t reserved_2[4];
} BOD_t;


typedef enum BOD_ACTIVE_enum
{
    BOD_ACTIVE_DIS_gc = (0x00<<2),
    BOD_ACTIVE_ENABLED_gc = (0x01<<2),
    BOD_ACTIVE_SAMPLED_gc = (0x02<<2),
    BOD_ACTIVE_ENWAKE_gc = (0x03<<2)
} BOD_ACTIVE_t;


typedef enum BOD_LVL_enum
{
    BOD_LVL_BODLEVEL0_gc = (0x00<<0),
    BOD_LVL_BODLEVEL2_gc = (0x02<<0),
    BOD_LVL_BODLEVEL7_gc = (0x07<<0)
} BOD_LVL_t;


typedef enum BOD_SAMPFREQ_enum
{
    BOD_SAMPFREQ_1KHZ_gc = (0x00<<4),
    BOD_SAMPFREQ_125HZ_gc = (0x01<<4)
} BOD_SAMPFREQ_t;


typedef enum BOD_SLEEP_enum
{
    BOD_SLEEP_DIS_gc = (0x00<<0),
    BOD_SLEEP_ENABLED_gc = (0x01<<0),
    BOD_SLEEP_SAMPLED_gc = (0x02<<0)
} BOD_SLEEP_t;


typedef enum BOD_VLMCFG_enum
{
    BOD_VLMCFG_BELOW_gc = (0x00<<1),
    BOD_VLMCFG_ABOVE_gc = (0x01<<1),
    BOD_VLMCFG_CROSS_gc = (0x02<<1)
} BOD_VLMCFG_t;


typedef enum BOD_VLMLVL_enum
{
    BOD_VLMLVL_5ABOVE_gc = (0x00<<0),
    BOD_VLMLVL_15ABOVE_gc = (0x01<<0),
    BOD_VLMLVL_25ABOVE_gc = (0x02<<0)
} BOD_VLMLVL_t;
# 373 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef struct CCL_struct
{
    register8_t CTRLA;
    register8_t SEQCTRL0;
    register8_t SEQCTRL1;
    register8_t reserved_1[2];
    register8_t INTCTRL0;
    register8_t reserved_2[1];
    register8_t INTFLAGS;
    register8_t LUT0CTRLA;
    register8_t LUT0CTRLB;
    register8_t LUT0CTRLC;
    register8_t TRUTH0;
    register8_t LUT1CTRLA;
    register8_t LUT1CTRLB;
    register8_t LUT1CTRLC;
    register8_t TRUTH1;
    register8_t LUT2CTRLA;
    register8_t LUT2CTRLB;
    register8_t LUT2CTRLC;
    register8_t TRUTH2;
    register8_t LUT3CTRLA;
    register8_t LUT3CTRLB;
    register8_t LUT3CTRLC;
    register8_t TRUTH3;
    register8_t reserved_3[40];
} CCL_t;


typedef enum CCL_CLKSRC_enum
{
    CCL_CLKSRC_CLKPER_gc = (0x00<<1),
    CCL_CLKSRC_IN2_gc = (0x01<<1),
    CCL_CLKSRC_OSC20M_gc = (0x04<<1),
    CCL_CLKSRC_OSCULP32K_gc = (0x05<<1),
    CCL_CLKSRC_OSCULP1K_gc = (0x06<<1)
} CCL_CLKSRC_t;


typedef enum CCL_EDGEDET_enum
{
    CCL_EDGEDET_DIS_gc = (0x00<<7),
    CCL_EDGEDET_EN_gc = (0x01<<7)
} CCL_EDGEDET_t;


typedef enum CCL_FILTSEL_enum
{
    CCL_FILTSEL_DISABLE_gc = (0x00<<4),
    CCL_FILTSEL_SYNCH_gc = (0x01<<4),
    CCL_FILTSEL_FILTER_gc = (0x02<<4)
} CCL_FILTSEL_t;


typedef enum CCL_INSEL0_enum
{
    CCL_INSEL0_MASK_gc = (0x00<<0),
    CCL_INSEL0_FEEDBACK_gc = (0x01<<0),
    CCL_INSEL0_LINK_gc = (0x02<<0),
    CCL_INSEL0_EVENTA_gc = (0x03<<0),
    CCL_INSEL0_EVENTB_gc = (0x04<<0),
    CCL_INSEL0_IO_gc = (0x05<<0),
    CCL_INSEL0_AC0_gc = (0x06<<0),
    CCL_INSEL0_USART0_gc = (0x08<<0),
    CCL_INSEL0_SPI0_gc = (0x09<<0),
    CCL_INSEL0_TCA0_gc = (0x0A<<0),
    CCL_INSEL0_TCB0_gc = (0x0C<<0)
} CCL_INSEL0_t;


typedef enum CCL_INSEL1_enum
{
    CCL_INSEL1_MASK_gc = (0x00<<4),
    CCL_INSEL1_FEEDBACK_gc = (0x01<<4),
    CCL_INSEL1_LINK_gc = (0x02<<4),
    CCL_INSEL1_EVENTA_gc = (0x03<<4),
    CCL_INSEL1_EVENTB_gc = (0x04<<4),
    CCL_INSEL1_IO_gc = (0x05<<4),
    CCL_INSEL1_AC0_gc = (0x06<<4),
    CCL_INSEL1_USART1_gc = (0x08<<4),
    CCL_INSEL1_SPI0_gc = (0x09<<4),
    CCL_INSEL1_TCA0_gc = (0x0A<<4),
    CCL_INSEL1_TCB1_gc = (0x0C<<4)
} CCL_INSEL1_t;


typedef enum CCL_INSEL2_enum
{
    CCL_INSEL2_MASK_gc = (0x00<<0),
    CCL_INSEL2_FEEDBACK_gc = (0x01<<0),
    CCL_INSEL2_LINK_gc = (0x02<<0),
    CCL_INSEL2_EVENTA_gc = (0x03<<0),
    CCL_INSEL2_EVENTB_gc = (0x04<<0),
    CCL_INSEL2_IO_gc = (0x05<<0),
    CCL_INSEL2_AC0_gc = (0x06<<0),
    CCL_INSEL2_USART2_gc = (0x08<<0),
    CCL_INSEL2_SPI0_gc = (0x09<<0),
    CCL_INSEL2_TCA0_gc = (0x0A<<0),
    CCL_INSEL2_TCB2_gc = (0x0C<<0)
} CCL_INSEL2_t;


typedef enum CCL_INTMODE0_enum
{
    CCL_INTMODE0_INTDISABLE_gc = (0x00<<0),
    CCL_INTMODE0_RISING_gc = (0x01<<0),
    CCL_INTMODE0_FALLING_gc = (0x02<<0),
    CCL_INTMODE0_BOTH_gc = (0x03<<0)
} CCL_INTMODE0_t;


typedef enum CCL_INTMODE1_enum
{
    CCL_INTMODE1_INTDISABLE_gc = (0x00<<2),
    CCL_INTMODE1_RISING_gc = (0x01<<2),
    CCL_INTMODE1_FALLING_gc = (0x02<<2),
    CCL_INTMODE1_BOTH_gc = (0x03<<2)
} CCL_INTMODE1_t;


typedef enum CCL_INTMODE2_enum
{
    CCL_INTMODE2_INTDISABLE_gc = (0x00<<4),
    CCL_INTMODE2_RISING_gc = (0x01<<4),
    CCL_INTMODE2_FALLING_gc = (0x02<<4),
    CCL_INTMODE2_BOTH_gc = (0x03<<4)
} CCL_INTMODE2_t;


typedef enum CCL_INTMODE3_enum
{
    CCL_INTMODE3_INTDISABLE_gc = (0x00<<6),
    CCL_INTMODE3_RISING_gc = (0x01<<6),
    CCL_INTMODE3_FALLING_gc = (0x02<<6),
    CCL_INTMODE3_BOTH_gc = (0x03<<6)
} CCL_INTMODE3_t;


typedef enum CCL_SEQSEL0_enum
{
    CCL_SEQSEL0_DISABLE_gc = (0x00<<0),
    CCL_SEQSEL0_DFF_gc = (0x01<<0),
    CCL_SEQSEL0_JK_gc = (0x02<<0),
    CCL_SEQSEL0_LATCH_gc = (0x03<<0),
    CCL_SEQSEL0_RS_gc = (0x04<<0)
} CCL_SEQSEL0_t;


typedef enum CCL_SEQSEL1_enum
{
    CCL_SEQSEL1_DISABLE_gc = (0x00<<0),
    CCL_SEQSEL1_DFF_gc = (0x01<<0),
    CCL_SEQSEL1_JK_gc = (0x02<<0),
    CCL_SEQSEL1_LATCH_gc = (0x03<<0),
    CCL_SEQSEL1_RS_gc = (0x04<<0)
} CCL_SEQSEL1_t;
# 537 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef struct CLKCTRL_struct
{
    register8_t MCLKCTRLA;
    register8_t MCLKCTRLB;
    register8_t MCLKLOCK;
    register8_t MCLKSTATUS;
    register8_t reserved_1[12];
    register8_t OSC20MCTRLA;
    register8_t OSC20MCALIBA;
    register8_t OSC20MCALIBB;
    register8_t reserved_2[5];
    register8_t OSC32KCTRLA;
    register8_t reserved_3[3];
    register8_t XOSC32KCTRLA;
    register8_t reserved_4[3];
} CLKCTRL_t;


typedef enum CLKCTRL_CLKSEL_enum
{
    CLKCTRL_CLKSEL_OSC20M_gc = (0x00<<0),
    CLKCTRL_CLKSEL_OSCULP32K_gc = (0x01<<0),
    CLKCTRL_CLKSEL_XOSC32K_gc = (0x02<<0),
    CLKCTRL_CLKSEL_EXTCLK_gc = (0x03<<0)
} CLKCTRL_CLKSEL_t;


typedef enum CLKCTRL_CSUT_enum
{
    CLKCTRL_CSUT_1K_gc = (0x00<<4),
    CLKCTRL_CSUT_16K_gc = (0x01<<4),
    CLKCTRL_CSUT_32K_gc = (0x02<<4),
    CLKCTRL_CSUT_64K_gc = (0x03<<4)
} CLKCTRL_CSUT_t;


typedef enum CLKCTRL_PDIV_enum
{
    CLKCTRL_PDIV_2X_gc = (0x00<<1),
    CLKCTRL_PDIV_4X_gc = (0x01<<1),
    CLKCTRL_PDIV_8X_gc = (0x02<<1),
    CLKCTRL_PDIV_16X_gc = (0x03<<1),
    CLKCTRL_PDIV_32X_gc = (0x04<<1),
    CLKCTRL_PDIV_64X_gc = (0x05<<1),
    CLKCTRL_PDIV_6X_gc = (0x08<<1),
    CLKCTRL_PDIV_10X_gc = (0x09<<1),
    CLKCTRL_PDIV_12X_gc = (0x0A<<1),
    CLKCTRL_PDIV_24X_gc = (0x0B<<1),
    CLKCTRL_PDIV_48X_gc = (0x0C<<1)
} CLKCTRL_PDIV_t;
# 597 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef enum CCP_enum
{
    CCP_SPM_gc = (0x9D<<0),
    CCP_IOREG_gc = (0xD8<<0)
} CCP_t;
# 610 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef struct CPUINT_struct
{
    register8_t CTRLA;
    register8_t STATUS;
    register8_t LVL0PRI;
    register8_t LVL1VEC;
} CPUINT_t;
# 626 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef struct CRCSCAN_struct
{
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t STATUS;
    register8_t reserved_1[1];
} CRCSCAN_t;


typedef enum CRCSCAN_MODE_enum
{
    CRCSCAN_MODE_PRIORITY_gc = (0x00<<4)
} CRCSCAN_MODE_t;


typedef enum CRCSCAN_SRC_enum
{
    CRCSCAN_SRC_FLASH_gc = (0x00<<0),
    CRCSCAN_SRC_APPLICATION_gc = (0x01<<0),
    CRCSCAN_SRC_BOOT_gc = (0x02<<0)
} CRCSCAN_SRC_t;
# 655 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef struct EVSYS_struct
{
    register8_t STROBE;
    register8_t reserved_1[15];
    register8_t CHANNEL0;
    register8_t CHANNEL1;
    register8_t CHANNEL2;
    register8_t CHANNEL3;
    register8_t CHANNEL4;
    register8_t CHANNEL5;
    register8_t CHANNEL6;
    register8_t CHANNEL7;
    register8_t reserved_2[8];
    register8_t USERCCLLUT0A;
    register8_t USERCCLLUT0B;
    register8_t USERCCLLUT1A;
    register8_t USERCCLLUT1B;
    register8_t USERCCLLUT2A;
    register8_t USERCCLLUT2B;
    register8_t USERCCLLUT3A;
    register8_t USERCCLLUT3B;
    register8_t USERADC0;
    register8_t USEREVOUTA;
    register8_t USEREVOUTB;
    register8_t USEREVOUTC;
    register8_t USEREVOUTD;
    register8_t USEREVOUTE;
    register8_t USEREVOUTF;
    register8_t USERUSART0;
    register8_t USERUSART1;
    register8_t USERUSART2;
    register8_t USERUSART3;
    register8_t USERTCA0;
    register8_t USERTCB0;
    register8_t USERTCB1;
    register8_t USERTCB2;
    register8_t USERTCB3;
    register8_t reserved_3[8];
} EVSYS_t;


typedef enum EVSYS_CHANNEL_enum
{
    EVSYS_CHANNEL_OFF_gc = (0x00<<0),
    EVSYS_CHANNEL_CHANNEL0_gc = (0x01<<0),
    EVSYS_CHANNEL_CHANNEL1_gc = (0x02<<0),
    EVSYS_CHANNEL_CHANNEL2_gc = (0x03<<0),
    EVSYS_CHANNEL_CHANNEL3_gc = (0x04<<0),
    EVSYS_CHANNEL_CHANNEL4_gc = (0x05<<0),
    EVSYS_CHANNEL_CHANNEL5_gc = (0x06<<0),
    EVSYS_CHANNEL_CHANNEL6_gc = (0x07<<0),
    EVSYS_CHANNEL_CHANNEL7_gc = (0x08<<0)
} EVSYS_CHANNEL_t;


typedef enum EVSYS_GENERATOR_enum
{
    EVSYS_GENERATOR_OFF_gc = (0x00<<0),
    EVSYS_GENERATOR_UPDI_gc = (0x01<<0),
    EVSYS_GENERATOR_RTC_OVF_gc = (0x06<<0),
    EVSYS_GENERATOR_RTC_CMP_gc = (0x07<<0),
    EVSYS_GENERATOR_RTC_PIT0_gc = (0x08<<0),
    EVSYS_GENERATOR_RTC_PIT1_gc = (0x09<<0),
    EVSYS_GENERATOR_RTC_PIT2_gc = (0x0A<<0),
    EVSYS_GENERATOR_RTC_PIT3_gc = (0x0B<<0),
    EVSYS_GENERATOR_CCL_LUT0_gc = (0x10<<0),
    EVSYS_GENERATOR_CCL_LUT1_gc = (0x11<<0),
    EVSYS_GENERATOR_CCL_LUT2_gc = (0x12<<0),
    EVSYS_GENERATOR_CCL_LUT3_gc = (0x13<<0),
    EVSYS_GENERATOR_AC0_OUT_gc = (0x20<<0),
    EVSYS_GENERATOR_ADC0_RESRDY_gc = (0x24<<0),
    EVSYS_GENERATOR_PORT0_PIN0_gc = (0x40<<0),
    EVSYS_GENERATOR_PORT0_PIN1_gc = (0x41<<0),
    EVSYS_GENERATOR_PORT0_PIN2_gc = (0x42<<0),
    EVSYS_GENERATOR_PORT0_PIN3_gc = (0x43<<0),
    EVSYS_GENERATOR_PORT0_PIN4_gc = (0x44<<0),
    EVSYS_GENERATOR_PORT0_PIN5_gc = (0x45<<0),
    EVSYS_GENERATOR_PORT0_PIN6_gc = (0x46<<0),
    EVSYS_GENERATOR_PORT0_PIN7_gc = (0x47<<0),
    EVSYS_GENERATOR_PORT1_PIN0_gc = (0x48<<0),
    EVSYS_GENERATOR_PORT1_PIN1_gc = (0x49<<0),
    EVSYS_GENERATOR_PORT1_PIN2_gc = (0x4A<<0),
    EVSYS_GENERATOR_PORT1_PIN3_gc = (0x4B<<0),
    EVSYS_GENERATOR_PORT1_PIN4_gc = (0x4C<<0),
    EVSYS_GENERATOR_PORT1_PIN5_gc = (0x4D<<0),
    EVSYS_GENERATOR_PORT1_PIN6_gc = (0x4E<<0),
    EVSYS_GENERATOR_PORT1_PIN7_gc = (0x4F<<0),
    EVSYS_GENERATOR_USART0_XCK_gc = (0x60<<0),
    EVSYS_GENERATOR_USART1_XCK_gc = (0x61<<0),
    EVSYS_GENERATOR_USART2_XCK_gc = (0x62<<0),
    EVSYS_GENERATOR_USART3_XCK_gc = (0x63<<0),
    EVSYS_GENERATOR_SPI0_SCK_gc = (0x68<<0),
    EVSYS_GENERATOR_TCA0_OVF_LUNF_gc = (0x80<<0),
    EVSYS_GENERATOR_TCA0_HUNF_gc = (0x81<<0),
    EVSYS_GENERATOR_TCA0_CMP0_gc = (0x84<<0),
    EVSYS_GENERATOR_TCA0_CMP1_gc = (0x85<<0),
    EVSYS_GENERATOR_TCA0_CMP2_gc = (0x86<<0),
    EVSYS_GENERATOR_TCB0_CAPT_gc = (0xA0<<0),
    EVSYS_GENERATOR_TCB1_CAPT_gc = (0xA2<<0),
    EVSYS_GENERATOR_TCB2_CAPT_gc = (0xA4<<0),
    EVSYS_GENERATOR_TCB3_CAPT_gc = (0xA6<<0)
} EVSYS_GENERATOR_t;


typedef enum EVSYS_STROBE0_enum
{
    EVSYS_STROBE0_EV_STROBE_CH0_gc = (0x01<<0),
    EVSYS_STROBE0_EV_STROBE_CH1_gc = (0x02<<0),
    EVSYS_STROBE0_EV_STROBE_CH2_gc = (0x04<<0),
    EVSYS_STROBE0_EV_STROBE_CH3_gc = (0x08<<0),
    EVSYS_STROBE0_EV_STROBE_CH4_gc = (0x10<<0),
    EVSYS_STROBE0_EV_STROBE_CH5_gc = (0x20<<0),
    EVSYS_STROBE0_EV_STROBE_CH6_gc = (0x40<<0),
    EVSYS_STROBE0_EV_STROBE_CH7_gc = (0x80<<0)
} EVSYS_STROBE0_t;
# 778 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef struct FUSE_struct
{
    register8_t WDTCFG;
    register8_t BODCFG;
    register8_t OSCCFG;
    register8_t reserved_1[2];
    register8_t SYSCFG0;
    register8_t SYSCFG1;
    register8_t APPEND;
    register8_t BOOTEND;
} FUSE_t;


typedef FUSE_t NVM_FUSES_t;


typedef enum ACTIVE_enum
{
    ACTIVE_DIS_gc = (0x00<<2),
    ACTIVE_ENABLED_gc = (0x01<<2),
    ACTIVE_SAMPLED_gc = (0x02<<2),
    ACTIVE_ENWAKE_gc = (0x03<<2)
} ACTIVE_t;


typedef enum CRCSRC_enum
{
    CRCSRC_FLASH_gc = (0x00<<6),
    CRCSRC_BOOT_gc = (0x01<<6),
    CRCSRC_BOOTAPP_gc = (0x02<<6),
    CRCSRC_NOCRC_gc = (0x03<<6)
} CRCSRC_t;


typedef enum FREQSEL_enum
{
    FREQSEL_16MHZ_gc = (0x01<<0),
    FREQSEL_20MHZ_gc = (0x02<<0)
} FREQSEL_t;


typedef enum LVL_enum
{
    LVL_BODLEVEL0_gc = (0x00<<5),
    LVL_BODLEVEL2_gc = (0x02<<5),
    LVL_BODLEVEL7_gc = (0x07<<5)
} LVL_t;


typedef enum PERIOD_enum
{
    PERIOD_OFF_gc = (0x00<<0),
    PERIOD_8CLK_gc = (0x01<<0),
    PERIOD_16CLK_gc = (0x02<<0),
    PERIOD_32CLK_gc = (0x03<<0),
    PERIOD_64CLK_gc = (0x04<<0),
    PERIOD_128CLK_gc = (0x05<<0),
    PERIOD_256CLK_gc = (0x06<<0),
    PERIOD_512CLK_gc = (0x07<<0),
    PERIOD_1KCLK_gc = (0x08<<0),
    PERIOD_2KCLK_gc = (0x09<<0),
    PERIOD_4KCLK_gc = (0x0A<<0),
    PERIOD_8KCLK_gc = (0x0B<<0)
} PERIOD_t;


typedef enum RSTPINCFG_enum
{
    RSTPINCFG_GPIO_gc = (0x00<<3),
    RSTPINCFG_RST_gc = (0x01<<3)
} RSTPINCFG_t;


typedef enum SAMPFREQ_enum
{
    SAMPFREQ_1KHZ_gc = (0x00<<4),
    SAMPFREQ_125HZ_gc = (0x01<<4)
} SAMPFREQ_t;


typedef enum SLEEP_enum
{
    SLEEP_DIS_gc = (0x00<<0),
    SLEEP_ENABLED_gc = (0x01<<0),
    SLEEP_SAMPLED_gc = (0x02<<0)
} SLEEP_t;


typedef enum SUT_enum
{
    SUT_0MS_gc = (0x00<<0),
    SUT_1MS_gc = (0x01<<0),
    SUT_2MS_gc = (0x02<<0),
    SUT_4MS_gc = (0x03<<0),
    SUT_8MS_gc = (0x04<<0),
    SUT_16MS_gc = (0x05<<0),
    SUT_32MS_gc = (0x06<<0),
    SUT_64MS_gc = (0x07<<0)
} SUT_t;


typedef enum WINDOW_enum
{
    WINDOW_OFF_gc = (0x00<<4),
    WINDOW_8CLK_gc = (0x01<<4),
    WINDOW_16CLK_gc = (0x02<<4),
    WINDOW_32CLK_gc = (0x03<<4),
    WINDOW_64CLK_gc = (0x04<<4),
    WINDOW_128CLK_gc = (0x05<<4),
    WINDOW_256CLK_gc = (0x06<<4),
    WINDOW_512CLK_gc = (0x07<<4),
    WINDOW_1KCLK_gc = (0x08<<4),
    WINDOW_2KCLK_gc = (0x09<<4),
    WINDOW_4KCLK_gc = (0x0A<<4),
    WINDOW_8KCLK_gc = (0x0B<<4)
} WINDOW_t;
# 903 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef struct LOCKBIT_struct
{
    register8_t LOCKBIT;
    register8_t reserved_1[1];
} LOCKBIT_t;


typedef enum LB_enum
{
    LB_RWLOCK_gc = (0x3A<<0),
    LB_NOLOCK_gc = (0xC5<<0)
} LB_t;
# 923 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef struct NVMCTRL_struct
{
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t STATUS;
    register8_t INTCTRL;
    register8_t INTFLAGS;
    register8_t reserved_1[1];
    __extension__ union { register16_t DATA; struct { register8_t DATAL; register8_t DATAH; }; };
    __extension__ union { register16_t ADDR; struct { register8_t ADDRL; register8_t ADDRH; }; };
    register8_t reserved_2[6];
} NVMCTRL_t;


typedef enum NVMCTRL_CMD_enum
{
    NVMCTRL_CMD_NONE_gc = (0x00<<0),
    NVMCTRL_CMD_PAGEWRITE_gc = (0x01<<0),
    NVMCTRL_CMD_PAGEERASE_gc = (0x02<<0),
    NVMCTRL_CMD_PAGEERASEWRITE_gc = (0x03<<0),
    NVMCTRL_CMD_PAGEBUFCLR_gc = (0x04<<0),
    NVMCTRL_CMD_CHIPERASE_gc = (0x05<<0),
    NVMCTRL_CMD_EEERASE_gc = (0x06<<0),
    NVMCTRL_CMD_FUSEWRITE_gc = (0x07<<0)
} NVMCTRL_CMD_t;
# 956 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef struct PORT_struct
{
    register8_t DIR;
    register8_t DIRSET;
    register8_t DIRCLR;
    register8_t DIRTGL;
    register8_t OUT;
    register8_t OUTSET;
    register8_t OUTCLR;
    register8_t OUTTGL;
    register8_t IN;
    register8_t INTFLAGS;
    register8_t PORTCTRL;
    register8_t reserved_1[5];
    register8_t PIN0CTRL;
    register8_t PIN1CTRL;
    register8_t PIN2CTRL;
    register8_t PIN3CTRL;
    register8_t PIN4CTRL;
    register8_t PIN5CTRL;
    register8_t PIN6CTRL;
    register8_t PIN7CTRL;
    register8_t reserved_2[8];
} PORT_t;


typedef enum PORT_ISC_enum
{
    PORT_ISC_INTDISABLE_gc = (0x00<<0),
    PORT_ISC_BOTHEDGES_gc = (0x01<<0),
    PORT_ISC_RISING_gc = (0x02<<0),
    PORT_ISC_FALLING_gc = (0x03<<0),
    PORT_ISC_INPUT_DISABLE_gc = (0x04<<0),
    PORT_ISC_LEVEL_gc = (0x05<<0)
} PORT_ISC_t;
# 999 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef struct PORTMUX_struct
{
    register8_t EVSYSROUTEA;
    register8_t CCLROUTEA;
    register8_t USARTROUTEA;
    register8_t TWISPIROUTEA;
    register8_t TCAROUTEA;
    register8_t TCBROUTEA;
    register8_t reserved_1[10];
} PORTMUX_t;


typedef enum PORTMUX_EVOUTA_enum
{
    PORTMUX_EVOUTA_DEFAULT_gc = (0x00<<0),
    PORTMUX_EVOUTA_ALT1_gc = (0x01<<0)
} PORTMUX_EVOUTA_t;


typedef enum PORTMUX_EVOUTB_enum
{
    PORTMUX_EVOUTB_DEFAULT_gc = (0x00<<1)
} PORTMUX_EVOUTB_t;


typedef enum PORTMUX_EVOUTC_enum
{
    PORTMUX_EVOUTC_DEFAULT_gc = (0x00<<2),
    PORTMUX_EVOUTC_ALT1_gc = (0x01<<2)
} PORTMUX_EVOUTC_t;


typedef enum PORTMUX_EVOUTD_enum
{
    PORTMUX_EVOUTD_DEFAULT_gc = (0x00<<3),
    PORTMUX_EVOUTD_ALT1_gc = (0x01<<3)
} PORTMUX_EVOUTD_t;


typedef enum PORTMUX_EVOUTE_enum
{
    PORTMUX_EVOUTE_DEFAULT_gc = (0x00<<4)
} PORTMUX_EVOUTE_t;


typedef enum PORTMUX_EVOUTF_enum
{
    PORTMUX_EVOUTF_DEFAULT_gc = (0x00<<5)
} PORTMUX_EVOUTF_t;


typedef enum PORTMUX_SPI0_enum
{
    PORTMUX_SPI0_DEFAULT_gc = (0x00<<0),
    PORTMUX_SPI0_ALT1_gc = (0x01<<0),
    PORTMUX_SPI0_ALT2_gc = (0x02<<0),
    PORTMUX_SPI0_NONE_gc = (0x03<<0)
} PORTMUX_SPI0_t;


typedef enum PORTMUX_TCA0_enum
{
    PORTMUX_TCA0_PORTA_gc = (0x00<<0),
    PORTMUX_TCA0_PORTB_gc = (0x01<<0),
    PORTMUX_TCA0_PORTC_gc = (0x02<<0),
    PORTMUX_TCA0_PORTD_gc = (0x03<<0),
    PORTMUX_TCA0_PORTE_gc = (0x04<<0),
    PORTMUX_TCA0_PORTF_gc = (0x05<<0)
} PORTMUX_TCA0_t;


typedef enum PORTMUX_TCB0_enum
{
    PORTMUX_TCB0_DEFAULT_gc = (0x00<<0),
    PORTMUX_TCB0_ALT1_gc = (0x01<<0)
} PORTMUX_TCB0_t;


typedef enum PORTMUX_TCB1_enum
{
    PORTMUX_TCB1_DEFAULT_gc = (0x00<<1),
    PORTMUX_TCB1_ALT1_gc = (0x01<<1)
} PORTMUX_TCB1_t;


typedef enum PORTMUX_TCB2_enum
{
    PORTMUX_TCB2_DEFAULT_gc = (0x00<<2),
    PORTMUX_TCB2_ALT1_gc = (0x01<<2)
} PORTMUX_TCB2_t;


typedef enum PORTMUX_TCB3_enum
{
    PORTMUX_TCB3_DEFAULT_gc = (0x00<<3),
    PORTMUX_TCB3_ALT1_gc = (0x01<<3)
} PORTMUX_TCB3_t;


typedef enum PORTMUX_TWI0_enum
{
    PORTMUX_TWI0_DEFAULT_gc = (0x00<<4),
    PORTMUX_TWI0_ALT1_gc = (0x01<<4),
    PORTMUX_TWI0_ALT2_gc = (0x02<<4),
    PORTMUX_TWI0_NONE_gc = (0x03<<4)
} PORTMUX_TWI0_t;


typedef enum PORTMUX_USART0_enum
{
    PORTMUX_USART0_DEFAULT_gc = (0x00<<0),
    PORTMUX_USART0_ALT1_gc = (0x01<<0),
    PORTMUX_USART0_NONE_gc = (0x03<<0)
} PORTMUX_USART0_t;


typedef enum PORTMUX_USART1_enum
{
    PORTMUX_USART1_DEFAULT_gc = (0x00<<2),
    PORTMUX_USART1_ALT1_gc = (0x01<<2),
    PORTMUX_USART1_NONE_gc = (0x03<<2)
} PORTMUX_USART1_t;


typedef enum PORTMUX_USART2_enum
{
    PORTMUX_USART2_DEFAULT_gc = (0x00<<4),
    PORTMUX_USART2_ALT1_gc = (0x01<<4),
    PORTMUX_USART2_NONE_gc = (0x03<<4)
} PORTMUX_USART2_t;


typedef enum PORTMUX_USART3_enum
{
    PORTMUX_USART3_DEFAULT_gc = (0x00<<6),
    PORTMUX_USART3_ALT1_gc = (0x01<<6),
    PORTMUX_USART3_NONE_gc = (0x03<<6)
} PORTMUX_USART3_t;
# 1145 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef struct RSTCTRL_struct
{
    register8_t RSTFR;
    register8_t SWRR;
    register8_t reserved_1[2];
} RSTCTRL_t;
# 1160 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef struct RTC_struct
{
    register8_t CTRLA;
    register8_t STATUS;
    register8_t INTCTRL;
    register8_t INTFLAGS;
    register8_t TEMP;
    register8_t DBGCTRL;
    register8_t CALIB;
    register8_t CLKSEL;
    __extension__ union { register16_t CNT; struct { register8_t CNTL; register8_t CNTH; }; };
    __extension__ union { register16_t PER; struct { register8_t PERL; register8_t PERH; }; };
    __extension__ union { register16_t CMP; struct { register8_t CMPL; register8_t CMPH; }; };
    register8_t reserved_1[2];
    register8_t PITCTRLA;
    register8_t PITSTATUS;
    register8_t PITINTCTRL;
    register8_t PITINTFLAGS;
    register8_t reserved_2[1];
    register8_t PITDBGCTRL;
    register8_t reserved_3[10];
} RTC_t;


typedef enum RTC_CLKSEL_enum
{
    RTC_CLKSEL_INT32K_gc = (0x00<<0),
    RTC_CLKSEL_INT1K_gc = (0x01<<0),
    RTC_CLKSEL_TOSC32K_gc = (0x02<<0),
    RTC_CLKSEL_EXTCLK_gc = (0x03<<0)
} RTC_CLKSEL_t;


typedef enum RTC_PERIOD_enum
{
    RTC_PERIOD_OFF_gc = (0x00<<3),
    RTC_PERIOD_CYC4_gc = (0x01<<3),
    RTC_PERIOD_CYC8_gc = (0x02<<3),
    RTC_PERIOD_CYC16_gc = (0x03<<3),
    RTC_PERIOD_CYC32_gc = (0x04<<3),
    RTC_PERIOD_CYC64_gc = (0x05<<3),
    RTC_PERIOD_CYC128_gc = (0x06<<3),
    RTC_PERIOD_CYC256_gc = (0x07<<3),
    RTC_PERIOD_CYC512_gc = (0x08<<3),
    RTC_PERIOD_CYC1024_gc = (0x09<<3),
    RTC_PERIOD_CYC2048_gc = (0x0A<<3),
    RTC_PERIOD_CYC4096_gc = (0x0B<<3),
    RTC_PERIOD_CYC8192_gc = (0x0C<<3),
    RTC_PERIOD_CYC16384_gc = (0x0D<<3),
    RTC_PERIOD_CYC32768_gc = (0x0E<<3)
} RTC_PERIOD_t;


typedef enum RTC_PRESCALER_enum
{
    RTC_PRESCALER_DIV1_gc = (0x00<<3),
    RTC_PRESCALER_DIV2_gc = (0x01<<3),
    RTC_PRESCALER_DIV4_gc = (0x02<<3),
    RTC_PRESCALER_DIV8_gc = (0x03<<3),
    RTC_PRESCALER_DIV16_gc = (0x04<<3),
    RTC_PRESCALER_DIV32_gc = (0x05<<3),
    RTC_PRESCALER_DIV64_gc = (0x06<<3),
    RTC_PRESCALER_DIV128_gc = (0x07<<3),
    RTC_PRESCALER_DIV256_gc = (0x08<<3),
    RTC_PRESCALER_DIV512_gc = (0x09<<3),
    RTC_PRESCALER_DIV1024_gc = (0x0A<<3),
    RTC_PRESCALER_DIV2048_gc = (0x0B<<3),
    RTC_PRESCALER_DIV4096_gc = (0x0C<<3),
    RTC_PRESCALER_DIV8192_gc = (0x0D<<3),
    RTC_PRESCALER_DIV16384_gc = (0x0E<<3),
    RTC_PRESCALER_DIV32768_gc = (0x0F<<3)
} RTC_PRESCALER_t;
# 1240 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef struct SIGROW_struct
{
    register8_t DEVICEID0;
    register8_t DEVICEID1;
    register8_t DEVICEID2;
    register8_t SERNUM0;
    register8_t SERNUM1;
    register8_t SERNUM2;
    register8_t SERNUM3;
    register8_t SERNUM4;
    register8_t SERNUM5;
    register8_t SERNUM6;
    register8_t SERNUM7;
    register8_t SERNUM8;
    register8_t SERNUM9;
    register8_t reserved_1[7];
    register8_t OSCCAL32K;
    register8_t reserved_2[3];
    register8_t OSCCAL16M0;
    register8_t OSCCAL16M1;
    register8_t OSCCAL20M0;
    register8_t OSCCAL20M1;
    register8_t reserved_3[4];
    register8_t TEMPSENSE0;
    register8_t TEMPSENSE1;
    register8_t OSC16ERR3V;
    register8_t OSC16ERR5V;
    register8_t OSC20ERR3V;
    register8_t OSC20ERR5V;
    register8_t reserved_4[9];
    register8_t CHECKSUM1;
    register8_t reserved_5[16];
} SIGROW_t;
# 1282 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef struct SLPCTRL_struct
{
    register8_t CTRLA;
    register8_t reserved_1[1];
} SLPCTRL_t;


typedef enum SLPCTRL_SMODE_enum
{
    SLPCTRL_SMODE_IDLE_gc = (0x00<<1),
    SLPCTRL_SMODE_STDBY_gc = (0x01<<1),
    SLPCTRL_SMODE_PDOWN_gc = (0x02<<1)
} SLPCTRL_SMODE_t;
# 1306 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef struct SPI_struct
{
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t INTCTRL;
    register8_t INTFLAGS;
    register8_t DATA;
    register8_t reserved_1[3];
} SPI_t;


typedef enum SPI_MODE_enum
{
    SPI_MODE_0_gc = (0x00<<0),
    SPI_MODE_1_gc = (0x01<<0),
    SPI_MODE_2_gc = (0x02<<0),
    SPI_MODE_3_gc = (0x03<<0)
} SPI_MODE_t;


typedef enum SPI_PRESC_enum
{
    SPI_PRESC_DIV4_gc = (0x00<<1),
    SPI_PRESC_DIV16_gc = (0x01<<1),
    SPI_PRESC_DIV64_gc = (0x02<<1),
    SPI_PRESC_DIV128_gc = (0x03<<1)
} SPI_PRESC_t;
# 1341 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef struct SYSCFG_struct
{
    register8_t reserved_1[1];
    register8_t REVID;
    register8_t EXTBRK;
    register8_t reserved_2[21];
    register8_t OCDM;
    register8_t OCDMS;
    register8_t reserved_3[6];
} SYSCFG_t;
# 1360 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef struct TCA_SINGLE_struct
{
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t CTRLC;
    register8_t CTRLD;
    register8_t CTRLECLR;
    register8_t CTRLESET;
    register8_t CTRLFCLR;
    register8_t CTRLFSET;
    register8_t reserved_1[1];
    register8_t EVCTRL;
    register8_t INTCTRL;
    register8_t INTFLAGS;
    register8_t reserved_2[2];
    register8_t DBGCTRL;
    register8_t TEMP;
    register8_t reserved_3[16];
    __extension__ union { register16_t CNT; struct { register8_t CNTL; register8_t CNTH; }; };
    register8_t reserved_4[4];
    __extension__ union { register16_t PER; struct { register8_t PERL; register8_t PERH; }; };
    __extension__ union { register16_t CMP0; struct { register8_t CMP0L; register8_t CMP0H; }; };
    __extension__ union { register16_t CMP1; struct { register8_t CMP1L; register8_t CMP1H; }; };
    __extension__ union { register16_t CMP2; struct { register8_t CMP2L; register8_t CMP2H; }; };
    register8_t reserved_5[8];
    __extension__ union { register16_t PERBUF; struct { register8_t PERBUFL; register8_t PERBUFH; }; };
    __extension__ union { register16_t CMP0BUF; struct { register8_t CMP0BUFL; register8_t CMP0BUFH; }; };
    __extension__ union { register16_t CMP1BUF; struct { register8_t CMP1BUFL; register8_t CMP1BUFH; }; };
    __extension__ union { register16_t CMP2BUF; struct { register8_t CMP2BUFL; register8_t CMP2BUFH; }; };
    register8_t reserved_6[2];
} TCA_SINGLE_t;


typedef struct TCA_SPLIT_struct
{
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t CTRLC;
    register8_t CTRLD;
    register8_t CTRLECLR;
    register8_t CTRLESET;
    register8_t reserved_1[4];
    register8_t INTCTRL;
    register8_t INTFLAGS;
    register8_t reserved_2[2];
    register8_t DBGCTRL;
    register8_t reserved_3[17];
    register8_t LCNT;
    register8_t HCNT;
    register8_t reserved_4[4];
    register8_t LPER;
    register8_t HPER;
    register8_t LCMP0;
    register8_t HCMP0;
    register8_t LCMP1;
    register8_t HCMP1;
    register8_t LCMP2;
    register8_t HCMP2;
    register8_t reserved_5[18];
} TCA_SPLIT_t;


typedef union TCA_union
{
    TCA_SINGLE_t SINGLE;
    TCA_SPLIT_t SPLIT;
} TCA_t;


typedef enum TCA_SINGLE_CLKSEL_enum
{
    TCA_SINGLE_CLKSEL_DIV1_gc = (0x00<<1),
    TCA_SINGLE_CLKSEL_DIV2_gc = (0x01<<1),
    TCA_SINGLE_CLKSEL_DIV4_gc = (0x02<<1),
    TCA_SINGLE_CLKSEL_DIV8_gc = (0x03<<1),
    TCA_SINGLE_CLKSEL_DIV16_gc = (0x04<<1),
    TCA_SINGLE_CLKSEL_DIV64_gc = (0x05<<1),
    TCA_SINGLE_CLKSEL_DIV256_gc = (0x06<<1),
    TCA_SINGLE_CLKSEL_DIV1024_gc = (0x07<<1)
} TCA_SINGLE_CLKSEL_t;


typedef enum TCA_SINGLE_CMD_enum
{
    TCA_SINGLE_CMD_NONE_gc = (0x00<<2),
    TCA_SINGLE_CMD_UPDATE_gc = (0x01<<2),
    TCA_SINGLE_CMD_RESTART_gc = (0x02<<2),
    TCA_SINGLE_CMD_RESET_gc = (0x03<<2)
} TCA_SINGLE_CMD_t;


typedef enum TCA_SINGLE_DIR_enum
{
    TCA_SINGLE_DIR_UP_gc = (0x00<<0),
    TCA_SINGLE_DIR_DOWN_gc = (0x01<<0)
} TCA_SINGLE_DIR_t;


typedef enum TCA_SINGLE_EVACT_enum
{
    TCA_SINGLE_EVACT_POSEDGE_gc = (0x00<<1),
    TCA_SINGLE_EVACT_ANYEDGE_gc = (0x01<<1),
    TCA_SINGLE_EVACT_HIGHLVL_gc = (0x02<<1),
    TCA_SINGLE_EVACT_UPDOWN_gc = (0x03<<1)
} TCA_SINGLE_EVACT_t;


typedef enum TCA_SINGLE_WGMODE_enum
{
    TCA_SINGLE_WGMODE_NORMAL_gc = (0x00<<0),
    TCA_SINGLE_WGMODE_FRQ_gc = (0x01<<0),
    TCA_SINGLE_WGMODE_SINGLESLOPE_gc = (0x03<<0),
    TCA_SINGLE_WGMODE_DSTOP_gc = (0x05<<0),
    TCA_SINGLE_WGMODE_DSBOTH_gc = (0x06<<0),
    TCA_SINGLE_WGMODE_DSBOTTOM_gc = (0x07<<0)
} TCA_SINGLE_WGMODE_t;


typedef enum TCA_SPLIT_CLKSEL_enum
{
    TCA_SPLIT_CLKSEL_DIV1_gc = (0x00<<1),
    TCA_SPLIT_CLKSEL_DIV2_gc = (0x01<<1),
    TCA_SPLIT_CLKSEL_DIV4_gc = (0x02<<1),
    TCA_SPLIT_CLKSEL_DIV8_gc = (0x03<<1),
    TCA_SPLIT_CLKSEL_DIV16_gc = (0x04<<1),
    TCA_SPLIT_CLKSEL_DIV64_gc = (0x05<<1),
    TCA_SPLIT_CLKSEL_DIV256_gc = (0x06<<1),
    TCA_SPLIT_CLKSEL_DIV1024_gc = (0x07<<1)
} TCA_SPLIT_CLKSEL_t;


typedef enum TCA_SPLIT_CMD_enum
{
    TCA_SPLIT_CMD_NONE_gc = (0x00<<2),
    TCA_SPLIT_CMD_UPDATE_gc = (0x01<<2),
    TCA_SPLIT_CMD_RESTART_gc = (0x02<<2),
    TCA_SPLIT_CMD_RESET_gc = (0x03<<2)
} TCA_SPLIT_CMD_t;
# 1506 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef struct TCB_struct
{
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t reserved_1[2];
    register8_t EVCTRL;
    register8_t INTCTRL;
    register8_t INTFLAGS;
    register8_t STATUS;
    register8_t DBGCTRL;
    register8_t TEMP;
    __extension__ union { register16_t CNT; struct { register8_t CNTL; register8_t CNTH; }; };
    __extension__ union { register16_t CCMP; struct { register8_t CCMPL; register8_t CCMPH; }; };
    register8_t reserved_2[2];
} TCB_t;


typedef enum TCB_CLKSEL_enum
{
    TCB_CLKSEL_CLKDIV1_gc = (0x00<<1),
    TCB_CLKSEL_CLKDIV2_gc = (0x01<<1),
    TCB_CLKSEL_CLKTCA_gc = (0x02<<1)
} TCB_CLKSEL_t;


typedef enum TCB_CNTMODE_enum
{
    TCB_CNTMODE_INT_gc = (0x00<<0),
    TCB_CNTMODE_TIMEOUT_gc = (0x01<<0),
    TCB_CNTMODE_CAPT_gc = (0x02<<0),
    TCB_CNTMODE_FRQ_gc = (0x03<<0),
    TCB_CNTMODE_PW_gc = (0x04<<0),
    TCB_CNTMODE_FRQPW_gc = (0x05<<0),
    TCB_CNTMODE_SINGLE_gc = (0x06<<0),
    TCB_CNTMODE_PWM8_gc = (0x07<<0)
} TCB_CNTMODE_t;
# 1550 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef struct TWI_struct
{
    register8_t CTRLA;
    register8_t DUALCTRL;
    register8_t DBGCTRL;
    register8_t MCTRLA;
    register8_t MCTRLB;
    register8_t MSTATUS;
    register8_t MBAUD;
    register8_t MADDR;
    register8_t MDATA;
    register8_t SCTRLA;
    register8_t SCTRLB;
    register8_t SSTATUS;
    register8_t SADDR;
    register8_t SDATA;
    register8_t SADDRMASK;
    register8_t reserved_1[1];
} TWI_t;


typedef enum TWI_ACKACT_enum
{
    TWI_ACKACT_ACK_gc = (0x00<<2),
    TWI_ACKACT_NACK_gc = (0x01<<2)
} TWI_ACKACT_t;


typedef enum TWI_AP_enum
{
    TWI_AP_STOP_gc = (0x00<<0),
    TWI_AP_ADR_gc = (0x01<<0)
} TWI_AP_t;


typedef enum TWI_BUSSTATE_enum
{
    TWI_BUSSTATE_UNKNOWN_gc = (0x00<<0),
    TWI_BUSSTATE_IDLE_gc = (0x01<<0),
    TWI_BUSSTATE_OWNER_gc = (0x02<<0),
    TWI_BUSSTATE_BUSY_gc = (0x03<<0)
} TWI_BUSSTATE_t;


typedef enum TWI_MCMD_enum
{
    TWI_MCMD_NOACT_gc = (0x00<<0),
    TWI_MCMD_REPSTART_gc = (0x01<<0),
    TWI_MCMD_RECVTRANS_gc = (0x02<<0),
    TWI_MCMD_STOP_gc = (0x03<<0)
} TWI_MCMD_t;


typedef enum TWI_SCMD_enum
{
    TWI_SCMD_NOACT_gc = (0x00<<0),
    TWI_SCMD_COMPTRANS_gc = (0x02<<0),
    TWI_SCMD_RESPONSE_gc = (0x03<<0)
} TWI_SCMD_t;


typedef enum TWI_SDAHOLD_enum
{
    TWI_SDAHOLD_OFF_gc = (0x00<<2),
    TWI_SDAHOLD_50NS_gc = (0x01<<2),
    TWI_SDAHOLD_300NS_gc = (0x02<<2),
    TWI_SDAHOLD_500NS_gc = (0x03<<2)
} TWI_SDAHOLD_t;


typedef enum TWI_SDASETUP_enum
{
    TWI_SDASETUP_4CYC_gc = (0x00<<4),
    TWI_SDASETUP_8CYC_gc = (0x01<<4)
} TWI_SDASETUP_t;


typedef enum TWI_TIMEOUT_enum
{
    TWI_TIMEOUT_DISABLED_gc = (0x00<<2),
    TWI_TIMEOUT_50US_gc = (0x01<<2),
    TWI_TIMEOUT_100US_gc = (0x02<<2),
    TWI_TIMEOUT_200US_gc = (0x03<<2)
} TWI_TIMEOUT_t;
# 1642 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef struct USART_struct
{
    register8_t RXDATAL;
    register8_t RXDATAH;
    register8_t TXDATAL;
    register8_t TXDATAH;
    register8_t STATUS;
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t CTRLC;
    __extension__ union { register16_t BAUD; struct { register8_t BAUDL; register8_t BAUDH; }; };
    register8_t CTRLD;
    register8_t DBGCTRL;
    register8_t EVCTRL;
    register8_t TXPLCTRL;
    register8_t RXPLCTRL;
    register8_t reserved_1[1];
} USART_t;


typedef enum USART_ABW_enum
{
    USART_ABW_WDW0_gc = (0x00<<6),
    USART_ABW_WDW1_gc = (0x01<<6),
    USART_ABW_WDW2_gc = (0x02<<6),
    USART_ABW_WDW3_gc = (0x03<<6)
} USART_ABW_t;


typedef enum USART_CHSIZE_enum
{
    USART_CHSIZE_5BIT_gc = (0x00<<0),
    USART_CHSIZE_6BIT_gc = (0x01<<0),
    USART_CHSIZE_7BIT_gc = (0x02<<0),
    USART_CHSIZE_8BIT_gc = (0x03<<0),
    USART_CHSIZE_9BITL_gc = (0x06<<0),
    USART_CHSIZE_9BITH_gc = (0x07<<0)
} USART_CHSIZE_t;


typedef enum USART_CMODE_enum
{
    USART_CMODE_ASYNCHRONOUS_gc = (0x00<<6),
    USART_CMODE_SYNCHRONOUS_gc = (0x01<<6),
    USART_CMODE_IRCOM_gc = (0x02<<6),
    USART_CMODE_MSPI_gc = (0x03<<6)
} USART_CMODE_t;


typedef enum USART_PMODE_enum
{
    USART_PMODE_DISABLED_gc = (0x00<<4),
    USART_PMODE_EVEN_gc = (0x02<<4),
    USART_PMODE_ODD_gc = (0x03<<4)
} USART_PMODE_t;


typedef enum USART_RS485_enum
{
    USART_RS485_OFF_gc = (0x00<<0),
    USART_RS485_EXT_gc = (0x01<<0),
    USART_RS485_INT_gc = (0x02<<0)
} USART_RS485_t;


typedef enum USART_RXMODE_enum
{
    USART_RXMODE_NORMAL_gc = (0x00<<1),
    USART_RXMODE_CLK2X_gc = (0x01<<1),
    USART_RXMODE_GENAUTO_gc = (0x02<<1),
    USART_RXMODE_LINAUTO_gc = (0x03<<1)
} USART_RXMODE_t;


typedef enum USART_SBMODE_enum
{
    USART_SBMODE_1BIT_gc = (0x00<<3),
    USART_SBMODE_2BIT_gc = (0x01<<3)
} USART_SBMODE_t;
# 1729 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef struct USERROW_struct
{
    register8_t USERROW0;
    register8_t USERROW1;
    register8_t USERROW2;
    register8_t USERROW3;
    register8_t USERROW4;
    register8_t USERROW5;
    register8_t USERROW6;
    register8_t USERROW7;
    register8_t USERROW8;
    register8_t USERROW9;
    register8_t USERROW10;
    register8_t USERROW11;
    register8_t USERROW12;
    register8_t USERROW13;
    register8_t USERROW14;
    register8_t USERROW15;
    register8_t USERROW16;
    register8_t USERROW17;
    register8_t USERROW18;
    register8_t USERROW19;
    register8_t USERROW20;
    register8_t USERROW21;
    register8_t USERROW22;
    register8_t USERROW23;
    register8_t USERROW24;
    register8_t USERROW25;
    register8_t USERROW26;
    register8_t USERROW27;
    register8_t USERROW28;
    register8_t USERROW29;
    register8_t USERROW30;
    register8_t USERROW31;
    register8_t USERROW32;
    register8_t USERROW33;
    register8_t USERROW34;
    register8_t USERROW35;
    register8_t USERROW36;
    register8_t USERROW37;
    register8_t USERROW38;
    register8_t USERROW39;
    register8_t USERROW40;
    register8_t USERROW41;
    register8_t USERROW42;
    register8_t USERROW43;
    register8_t USERROW44;
    register8_t USERROW45;
    register8_t USERROW46;
    register8_t USERROW47;
    register8_t USERROW48;
    register8_t USERROW49;
    register8_t USERROW50;
    register8_t USERROW51;
    register8_t USERROW52;
    register8_t USERROW53;
    register8_t USERROW54;
    register8_t USERROW55;
    register8_t USERROW56;
    register8_t USERROW57;
    register8_t USERROW58;
    register8_t USERROW59;
    register8_t USERROW60;
    register8_t USERROW61;
    register8_t USERROW62;
    register8_t USERROW63;
} USERROW_t;
# 1805 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef struct VPORT_struct
{
    register8_t DIR;
    register8_t OUT;
    register8_t IN;
    register8_t INTFLAGS;
} VPORT_t;
# 1821 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef struct VREF_struct
{
    register8_t CTRLA;
    register8_t CTRLB;
} VREF_t;


typedef enum VREF_AC0REFSEL_enum
{
    VREF_AC0REFSEL_0V55_gc = (0x00<<0),
    VREF_AC0REFSEL_1V1_gc = (0x01<<0),
    VREF_AC0REFSEL_2V5_gc = (0x02<<0),
    VREF_AC0REFSEL_4V34_gc = (0x03<<0),
    VREF_AC0REFSEL_1V5_gc = (0x04<<0),
    VREF_AC0REFSEL_AVDD_gc = (0x07<<0)
} VREF_AC0REFSEL_t;


typedef enum VREF_ADC0REFSEL_enum
{
    VREF_ADC0REFSEL_0V55_gc = (0x00<<4),
    VREF_ADC0REFSEL_1V1_gc = (0x01<<4),
    VREF_ADC0REFSEL_2V5_gc = (0x02<<4),
    VREF_ADC0REFSEL_4V34_gc = (0x03<<4),
    VREF_ADC0REFSEL_1V5_gc = (0x04<<4)
} VREF_ADC0REFSEL_t;
# 1855 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/iom4809.h" 3
typedef struct WDT_struct
{
    register8_t CTRLA;
    register8_t STATUS;
} WDT_t;


typedef enum WDT_PERIOD_enum
{
    WDT_PERIOD_OFF_gc = (0x00<<0),
    WDT_PERIOD_8CLK_gc = (0x01<<0),
    WDT_PERIOD_16CLK_gc = (0x02<<0),
    WDT_PERIOD_32CLK_gc = (0x03<<0),
    WDT_PERIOD_64CLK_gc = (0x04<<0),
    WDT_PERIOD_128CLK_gc = (0x05<<0),
    WDT_PERIOD_256CLK_gc = (0x06<<0),
    WDT_PERIOD_512CLK_gc = (0x07<<0),
    WDT_PERIOD_1KCLK_gc = (0x08<<0),
    WDT_PERIOD_2KCLK_gc = (0x09<<0),
    WDT_PERIOD_4KCLK_gc = (0x0A<<0),
    WDT_PERIOD_8KCLK_gc = (0x0B<<0)
} WDT_PERIOD_t;


typedef enum WDT_WINDOW_enum
{
    WDT_WINDOW_OFF_gc = (0x00<<4),
    WDT_WINDOW_8CLK_gc = (0x01<<4),
    WDT_WINDOW_16CLK_gc = (0x02<<4),
    WDT_WINDOW_32CLK_gc = (0x03<<4),
    WDT_WINDOW_64CLK_gc = (0x04<<4),
    WDT_WINDOW_128CLK_gc = (0x05<<4),
    WDT_WINDOW_256CLK_gc = (0x06<<4),
    WDT_WINDOW_512CLK_gc = (0x07<<4),
    WDT_WINDOW_1KCLK_gc = (0x08<<4),
    WDT_WINDOW_2KCLK_gc = (0x09<<4),
    WDT_WINDOW_4KCLK_gc = (0x0A<<4),
    WDT_WINDOW_8KCLK_gc = (0x0B<<4)
} WDT_WINDOW_t;
# 539 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/io.h" 2 3
# 785 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/io.h" 3
# 1 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/portpins.h" 1 3
# 786 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/io.h" 2 3

# 1 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/common.h" 1 3
# 788 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/io.h" 2 3

# 1 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/version.h" 1 3
# 790 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/io.h" 2 3


# 1 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/xmega.h" 1 3
# 793 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/io.h" 2 3



# 1 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/fuse.h" 1 3
# 231 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/fuse.h" 3
typedef struct
{
    unsigned char byte[10];
} __fuse_t;
# 797 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/io.h" 2 3


# 1 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/lock.h" 1 3
# 800 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/io.h" 2 3
# 2 "src/main.c" 2
# 1 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/util/delay.h" 1 3
# 50 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/util/delay.h" 3
# 1 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/util/delay_basic.h" 1 3
# 40 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/util/delay_basic.h" 3
static __inline__ void _delay_loop_1(uint8_t __count) __attribute__((__always_inline__));
static __inline__ void _delay_loop_2(uint16_t __count) __attribute__((__always_inline__));
# 80 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/util/delay_basic.h" 3
void
_delay_loop_1(uint8_t __count)
{
 __asm__ volatile (
  "1: dec %0" "\n\t"
  "brne 1b"
  : "=r" (__count)
  : "0" (__count)
 );
}
# 102 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/util/delay_basic.h" 3
void
_delay_loop_2(uint16_t __count)
{
# 113 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/util/delay_basic.h" 3
 __asm__ volatile (
  "1: sbiw %0,1" "\n\t"
  "brne 1b"
  : "+w" (__count)
 );

}
# 51 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/util/delay.h" 2 3
# 151 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/util/delay.h" 3
static __inline__ __attribute__((__always_inline__)) void _delay_ms(double __ms);

void
_delay_ms(double __ms)
{
 double __tmp ;


 uint32_t __ticks_dc;
 extern void __builtin_avr_delay_cycles(uint32_t);
 __tmp = ((
# 161 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/util/delay.h"
          16000000UL
# 161 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/util/delay.h" 3
               ) / 1e3) * __ms;
# 171 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/util/delay.h" 3
  __ticks_dc = (uint32_t)(__builtin_ceil(__builtin_fabs(__tmp)));


 __builtin_avr_delay_cycles(__ticks_dc);
# 197 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/util/delay.h" 3
}
# 234 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/util/delay.h" 3
static __inline__ __attribute__((__always_inline__)) void _delay_us(double __us);

void
_delay_us(double __us)
{
 double __tmp ;


 uint32_t __ticks_dc;
 extern void __builtin_avr_delay_cycles(uint32_t);
 __tmp = ((
# 244 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/util/delay.h"
          16000000UL
# 244 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/util/delay.h" 3
               ) / 1e6) * __us;
# 254 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/util/delay.h" 3
  __ticks_dc = (uint32_t)(__builtin_ceil(__builtin_fabs(__tmp)));


 __builtin_avr_delay_cycles(__ticks_dc);
# 281 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/util/delay.h" 3
}
# 3 "src/main.c" 2
# 1 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/avr/interrupt.h" 1 3
# 4 "src/main.c" 2
# 1 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/stdio.h" 1 3
# 45 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/stdio.h" 3
# 1 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/lib/gcc/avr/15.2.0/include/stdarg.h" 1 3 4
# 40 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/lib/gcc/avr/15.2.0/include/stdarg.h" 3 4
typedef __builtin_va_list __gnuc_va_list;
# 103 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/lib/gcc/avr/15.2.0/include/stdarg.h" 3 4
typedef __gnuc_va_list va_list;
# 46 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/stdio.h" 2 3




# 1 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/lib/gcc/avr/15.2.0/include/stddef.h" 1 3 4
# 229 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/lib/gcc/avr/15.2.0/include/stddef.h" 3 4
typedef unsigned int size_t;
# 51 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/stdio.h" 2 3
# 250 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/stdio.h" 3
struct __file {
 char *buf;
 unsigned char unget;
 uint8_t flags;
# 269 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/stdio.h" 3
 int size;
 int len;
 int (*put)(char, struct __file *);
 int (*get)(struct __file *);
 void *udata;
};
# 283 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/stdio.h" 3
typedef struct __file FILE;
# 420 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/stdio.h" 3
extern struct __file *__iob[];
# 432 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/stdio.h" 3
extern FILE *fdevopen(int (*__put)(char, FILE*), int (*__get)(FILE*));
# 449 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/stdio.h" 3
extern int fclose(FILE *__stream);
# 623 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/stdio.h" 3
extern int vfprintf(FILE *__stream, const char *__fmt, va_list __ap);





extern int vfprintf_P(FILE *__stream, const char *__fmt, va_list __ap);






extern int fputc(int __c, FILE *__stream);




extern int putc(int __c, FILE *__stream);


extern int putchar(int __c);
# 664 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/stdio.h" 3
extern int printf(const char *__fmt, ...);





extern int printf_P(const char *__fmt, ...);







extern int vprintf(const char *__fmt, va_list __ap);





extern int sprintf(char *__s, const char *__fmt, ...);





extern int sprintf_P(char *__s, const char *__fmt, ...);
# 700 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/stdio.h" 3
extern int snprintf(char *__s, size_t __n, const char *__fmt, ...);





extern int snprintf_P(char *__s, size_t __n, const char *__fmt, ...);





extern int vsprintf(char *__s, const char *__fmt, va_list ap);





extern int vsprintf_P(char *__s, const char *__fmt, va_list ap);
# 728 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/stdio.h" 3
extern int vsnprintf(char *__s, size_t __n, const char *__fmt, va_list ap);





extern int vsnprintf_P(char *__s, size_t __n, const char *__fmt, va_list ap);




extern int fprintf(FILE *__stream, const char *__fmt, ...);





extern int fprintf_P(FILE *__stream, const char *__fmt, ...);






extern int fputs(const char *__str, FILE *__stream);




extern int fputs_P(const char *__str, FILE *__stream);





extern int puts(const char *__str);




extern int puts_P(const char *__str);
# 777 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/stdio.h" 3
extern size_t fwrite(const void *__ptr, size_t __size, size_t __nmemb,
         FILE *__stream);







extern int fgetc(FILE *__stream);




extern int getc(FILE *__stream);


extern int getchar(void);
# 825 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/stdio.h" 3
extern int ungetc(int __c, FILE *__stream);
# 837 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/stdio.h" 3
extern char *fgets(char *__str, int __size, FILE *__stream);






extern char *gets(char *__str);
# 855 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/stdio.h" 3
extern size_t fread(void *__ptr, size_t __size, size_t __nmemb,
        FILE *__stream);




extern void clearerr(FILE *__stream);
# 872 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/stdio.h" 3
extern int feof(FILE *__stream);
# 883 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/stdio.h" 3
extern int ferror(FILE *__stream);






extern int vfscanf(FILE *__stream, const char *__fmt, va_list __ap);




extern int vfscanf_P(FILE *__stream, const char *__fmt, va_list __ap);







extern int fscanf(FILE *__stream, const char *__fmt, ...);




extern int fscanf_P(FILE *__stream, const char *__fmt, ...);






extern int scanf(const char *__fmt, ...);




extern int scanf_P(const char *__fmt, ...);







extern int vscanf(const char *__fmt, va_list __ap);







extern int sscanf(const char *__buf, const char *__fmt, ...);




extern int sscanf_P(const char *__buf, const char *__fmt, ...);
# 953 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/avr/include/stdio.h" 3
static __inline__ int fflush(FILE *stream __attribute__((unused)))
{
 return 0;
}






__extension__ typedef long long fpos_t;
extern int fgetpos(FILE *stream, fpos_t *pos);
extern FILE *fopen(const char *path, const char *mode);
extern FILE *freopen(const char *path, const char *mode, FILE *stream);
extern FILE *fdopen(int, const char *);
extern int fseek(FILE *stream, long offset, int whence);
extern int fsetpos(FILE *stream, fpos_t *pos);
extern long ftell(FILE *stream);
extern int fileno(FILE *);
extern void perror(const char *s);
extern int remove(const char *pathname);
extern int rename(const char *oldpath, const char *newpath);
extern void rewind(FILE *stream);
extern void setbuf(FILE *stream, char *buf);
extern int setvbuf(FILE *stream, char *buf, int mode, size_t size);
extern FILE *tmpfile(void);
extern char *tmpnam (char *s);
# 5 "src/main.c" 2
# 1 "src/../include/uart.h" 1







# 7 "src/../include/uart.h"
void USART1_init(void);
void USART1_sendChar(char c);
void USART1_sendString(const char *s);
char USART1_readChar(void);


void USART2_init(void);
void USART2_sendChar(char c);
void USART2_sendString(const char *s);
char USART2_readChar(void);
# 6 "src/main.c" 2
# 1 "src/../include/twi.h" 1
# 11 "src/../include/twi.h"
void TWI0_init(void);
void TWI0_reset_bus(void);
uint8_t TWI0_start(uint8_t address);
void TWI0_stop(void);
uint8_t TWI0_write(uint8_t data);
uint8_t TWI0_read_ack(void);
uint8_t TWI0_read_nack(void);
void TWI0_debug_status(void);
void TWI0_scan(void);
# 7 "src/main.c" 2
# 1 "src/../include/mlx90640.h" 1
# 20 "src/../include/mlx90640.h"
uint16_t MLX_read16(uint16_t reg);
void debug_MLX_read16(uint16_t reg);
void MLX_read_frame(uint16_t *buffer);
void MLX_send_frame_to_pc(void);
uint8_t MLX_wait_for_data(void);
uint8_t MLX_poll_data_ready(void);
void MLX_read_row(uint8_t row_index, uint16_t *dest);
# 8 "src/main.c" 2
# 1 "src/../include/mpu6050.h" 1
# 15 "src/../include/mpu6050.h"
void MPU6050_init(void);
uint8_t MPU6050_test_connection(void);
void MPU6050_read_accel(int16_t *ax, int16_t *ay, int16_t *az);
void MPU6050_read_gyro(int16_t *gx, int16_t *gy, int16_t *gz);
void MPU6050_debug_test(void);
uint8_t MPU6050_read8(uint8_t reg);
void debug_MPU6050_read8(uint8_t reg, const char *label);
# 9 "src/main.c" 2
# 1 "src/../include/mlxProcess.h" 1
# 13 "src/../include/mlxProcess.h"
typedef struct {
    uint8_t x;
    uint8_t y;
    uint16_t max_value;
    uint16_t avg_value;
    uint8_t hot_pixel_count;
} HotspotResult;


typedef struct {
    uint16_t x_fp;
    uint16_t y_fp;
    uint16_t max_value;
    uint16_t threshold;
} CentroidResult;




void MLX_find_max_pixel(uint8_t *max_x, uint8_t *max_y, uint16_t *max_value);




CentroidResult MLX_find_hot_centroid(uint16_t threshold);



CentroidResult MLX_find_hot_centroid_auto(uint8_t percent);


void MLX_process_and_report(void);


void MLX_get_stats(uint16_t *min_val, uint16_t *max_val, uint16_t *avg_val);


void MLX_process_buffer_and_report(uint16_t *frame);
# 10 "src/main.c" 2
# 1 "src/../include/pwm.h" 1
# 12 "src/../include/pwm.h"
# 1 "src/../include/../include/flags.h" 1




typedef struct {
    volatile bool mlx_due;
    volatile bool mpu_due;
    volatile bool pwm_due;
    volatile bool led_due;
} SchedulerFlags;

extern SchedulerFlags sched_flags;


typedef enum {
    MLX_STATE_IDLE = 0,
    MLX_STATE_WAIT_READY,
    MLX_STATE_READ_ROWS,
    MLX_STATE_PROCESS
} MlxState;
# 13 "src/../include/pwm.h" 2
# 25 "src/../include/pwm.h"
void motor_init(void);




void motorA_set(int8_t speed);
void motorA_forward(void);
void motorA_backward(void);
void motorA_stop(void);


void motorB_set(int8_t speed);
void motorB_forward(void);
void motorB_backward(void);
void motorB_stop(void);

void task_pwm_update(void);
# 11 "src/main.c" 2
# 1 "src/../include/interrupt.h" 1
# 9 "src/../include/interrupt.h"
# 1 "C:/Users/SBIN/scoop/apps/avr-gcc/15.2.0/lib/gcc/avr/15.2.0/include/stdbool.h" 1 3 4
# 10 "src/../include/interrupt.h" 2

# 1 "src/../include/../include/uart.h" 1
# 12 "src/../include/interrupt.h" 2
# 1 "src/../include/../include/twi.h" 1
# 13 "src/../include/interrupt.h" 2
# 1 "src/../include/../include/mlx90640.h" 1
# 14 "src/../include/interrupt.h" 2
# 1 "src/../include/../include/mpu6050.h" 1
# 15 "src/../include/interrupt.h" 2
# 1 "src/../include/../include/mlxProcess.h" 1
# 16 "src/../include/interrupt.h" 2
# 1 "src/../include/../include/pwm.h" 1
# 17 "src/../include/interrupt.h" 2



void scheduler_init(void);


void scheduler_service_tasks(void);

void timer_init_1khz(void);
# 12 "src/main.c" 2
# 1 "src/../include/debugging.h" 1







# 1 "src/../include/../include/debugging.h" 1
# 9 "src/../include/debugging.h" 2







void initial_debugging(void);
void sensor_loop_debugging(int loop_count);
void pwm_loop_debugging(int loop_count);
# 13 "src/main.c" 2


void clock_init(void)
{







    
# 24 "src/main.c" 3
   (*(volatile uint8_t *)(0x0034)) 
# 24 "src/main.c"
       = CCP_IOREG_gc;
    
# 25 "src/main.c" 3
   (*(CLKCTRL_t *) 0x0060)
# 25 "src/main.c"
          .MCLKCTRLB = 0x00;


    while (
# 28 "src/main.c" 3
          (*(CLKCTRL_t *) 0x0060)
# 28 "src/main.c"
                 .MCLKSTATUS & 
# 28 "src/main.c" 3
                               0x01
# 28 "src/main.c"
                                              );
}

int main(void)
{

    clock_init();
    _delay_ms(100);
    USART2_init();
    timer_init_1khz();



    
# 41 "src/main.c" 3
   (*(PORT_t *) 0x04A0)
# 41 "src/main.c"
        .DIRSET = 
# 41 "src/main.c" 3
                  0x20
# 41 "src/main.c"
                         ;
    
# 42 "src/main.c" 3
   (*(PORT_t *) 0x04A0)
# 42 "src/main.c"
        .OUTCLR = 
# 42 "src/main.c" 3
                  0x20
# 42 "src/main.c"
                         ;

    TWI0_init();
    motor_init();
    scheduler_init();
    _delay_ms(100);


    for (uint8_t i = 0; i < 3; i++) {
        TWI0_reset_bus();
        _delay_ms(50);
    }

    initial_debugging();
    while (1)
    {





        scheduler_service_tasks();

        static uint16_t last_cnt = 0;
        static uint16_t dbg_print = 0;

        dbg_print++;
        if (dbg_print >= 10000) {
            dbg_print = 0;
            char b[64];
            uint16_t cnt = 
# 72 "src/main.c" 3
                          (*(TCB_t *) 0x0A80)
# 72 "src/main.c"
                              .CNT;
            uint8_t flags = 
# 73 "src/main.c" 3
                           (*(TCB_t *) 0x0A80)
# 73 "src/main.c"
                               .INTFLAGS;
            snprintf(b, sizeof(b), "CNT=%u FLAGS=%02X\r\n", cnt, flags);
            USART2_sendString(b);
            last_cnt = cnt;
        }
# 97 "src/main.c"
    }
}


# 100 "src/main.c" 3
void __vector_12 (void) __attribute__ ((__signal__,__used__, __externally_visible__)) ; void __vector_12 (void)

# 101 "src/main.c"
{
    
# 102 "src/main.c" 3
   (*(TCB_t *) 0x0A80)
# 102 "src/main.c"
       .INTFLAGS = 
# 102 "src/main.c" 3
                   0x01
# 102 "src/main.c"
                              ;
    
# 103 "src/main.c" 3
   (*(PORT_t *) 0x04A0)
# 103 "src/main.c"
        .OUTTGL = 
# 103 "src/main.c" 3
                  0x20
# 103 "src/main.c"
                         ;
}
