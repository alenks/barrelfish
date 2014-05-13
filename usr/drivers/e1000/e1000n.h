/*
 * Copyright (c) 2008, ETH Zurich. All rights reserved.
 *
 * This file is distributed under the terms in the attached LICENSE file.
 * If you do not find this file, copies can be found by writing to:
 * ETH Zurich D-INFK, Haldeneggsteig 4, CH-8092 Zurich. Attn: Systems Group.
 */

#ifndef E1000_H__
#define E1000_H__

#include <barrelfish/barrelfish.h>
#include <pci/pci.h>

#include "e1000_dev.h"      /* auto generated by Mackerel */
#include "e1000n_desc.h"
#include "e1000n_debug.h"

#define DRIVER_STRING "e1000: "

/**
 * Default message print format.
 */
#define E1000_PRINT(fmt, ...)        printf(DRIVER_STRING fmt, ##__VA_ARGS__)
#define E1000_PRINT_ERROR(fmt, ...)  fprintf(stderr, DRIVER_STRING fmt, ##__VA_ARGS__)


/*
 * Global constants
 */
#define MAC_ADDRESS_LEN     6


/*
 * e1000 (e1000) device family id's.
 */
#define E1000_DEVICE_82542               0x1000
#define E1000_DEVICE_82543GC_FIBER       0x1001
#define E1000_DEVICE_82543GC_COPPER      0x1004
#define E1000_DEVICE_82544EI_COPPER      0x1008
#define E1000_DEVICE_82544EI_FIBER       0x1009
#define E1000_DEVICE_82544GC_COPPER      0x100C
#define E1000_DEVICE_82544GC_LOM         0x100D
#define E1000_DEVICE_82540EM             0x100E
#define E1000_DEVICE_82540EM_LOM         0x1015
#define E1000_DEVICE_82540EP_LOM         0x1016
#define E1000_DEVICE_82540EP             0x1017
#define E1000_DEVICE_82540EP_LP          0x101E
#define E1000_DEVICE_82545EM_COPPER      0x100F
#define E1000_DEVICE_82545EM_FIBER       0x1011
#define E1000_DEVICE_82545GM_COPPER      0x1026
#define E1000_DEVICE_82545GM_FIBER       0x1027
#define E1000_DEVICE_82545GM_SERDES      0x1028
#define E1000_DEVICE_82546EB_COPPER      0x1010
#define E1000_DEVICE_82546EB_FIBER       0x1012
#define E1000_DEVICE_82546EB_QUAD_COPPER 0x101D
#define E1000_DEVICE_82541EI             0x1013
#define E1000_DEVICE_82541EI_MOBILE      0x1018
#define E1000_DEVICE_82541ER_LOM         0x1014
#define E1000_DEVICE_82541ER             0x1078
#define E1000_DEVICE_82547GI             0x1075
#define E1000_DEVICE_82541GI             0x1076
#define E1000_DEVICE_82541GI_MOBILE      0x1077
#define E1000_DEVICE_82541GI_LF          0x107C
#define E1000_DEVICE_82546GB_COPPER      0x1079
#define E1000_DEVICE_82546GB_FIBER       0x107A
#define E1000_DEVICE_82546GB_SERDES      0x107B
#define E1000_DEVICE_82546GB_PCIE        0x108A
#define E1000_DEVICE_82546GB_QUAD_COPPER 0x1099
#define E1000_DEVICE_82563EB             0x1096
#define E1000_DEVICE_82547EI             0x1019
#define E1000_DEVICE_82547EI_MOBILE      0x101A
#define E1000_DEVICE_82571EB_COPPER      0x105E
#define E1000_DEVICE_82571EB_FIBER       0x105F
#define E1000_DEVICE_82571EB_SERDES      0x1060
#define E1000_DEVICE_82571EB_QUAD_COPPER 0x10A4
#define E1000_DEVICE_82571EB_QUAD_FIBER  0x10A5
#define E1000_DEVICE_82571EB_QUAD_COPPER_LOWPROFILE  0x10BC
#define E1000_DEVICE_82571EB_SERDES_DUAL 0x10D9
#define E1000_DEVICE_82571EB_SERDES_QUAD 0x10DA
#define E1000_DEVICE_82572EI_COPPER      0x107D
#define E1000_DEVICE_82572EI_FIBER       0x107E
#define E1000_DEVICE_82572EI_SERDES      0x107F
#define E1000_DEVICE_82572EI             0x10B9
#define E1000_DEVICE_82573E              0x108B
#define E1000_DEVICE_82573E_IAMT         0x108C
#define E1000_DEVICE_82573L              0x109A
#define E1000_DEVICE_82574L              0x10D3
#define E1000_DEVICE_82575EB             0x10A7 // TODO(gz): This cards needs more work
#define E1000_DEVICE_82576EG             0x10C9 // TODO(gz): This cards needs more work
#define E1000_DEVICE_I210                0x1533
#define E1000_DEVICE_82546GB_QUAD_COPPER_KSP3 0x10B5


/**
 * Group definitions for cards that share specification and quirks.
 *
 * e1000_82542 should be split into:
 *   e1000_82542_rev_2_1 and e1000_82542_rev_2_2.
 * This can be figured out reading the PCI bus.
 */
typedef enum {
    e1000_undefined = 0,
    e1000_82542,        /* revision 2.1 and 2.2 merged */
    e1000_82543,
    e1000_82544,
    e1000_82540,
    e1000_82545,
    e1000_82545_rev_3,
    e1000_82546,
    e1000_82546_rev_3,
    e1000_82541,
    e1000_82541_rev_2,
    e1000_82547,
    e1000_82547_rev_2,
    e1000_82563,
    e1000_82571,
    e1000_82572,
    e1000_82573,
    e1000_82574,
    e1000_82575,
    e1000_82576,
    e1000_I210,
    e1000_num_macs
} e1000_mac_type_t;


/**
 * Hardware supported buffer sizes.
 */
typedef enum {
    bsize_256 = 256,
    bsize_512 = 512,
    bsize_1024 = 1024,
    bsize_2048 = 2048,
    bsize_4096 = 4096,
    bsize_8192 = 8192,
    bsize_16384 = 16384
} e1000_rx_bsize_t;

/**
 * Media types.
 */
typedef enum {
    e1000_media_type_undefined,
    e1000_media_type_fiber,
    e1000_media_type_copper,
    e1000_media_type_serdes,
    e1000_num_media_types
} e1000_media_type_t;



/**
 * Internal device info.
 */
typedef struct {
    e1000_t *device;
    uint32_t device_id;
    e1000_media_type_t media_type;
    e1000_mac_type_t mac_type;
    e1000_rx_bsize_t rx_bsize;
    bool tbi_combaility;
} e1000_device_t ;


e1000_mac_type_t e1000_get_mac_type(uint32_t vendor, uint32_t device_id);
bool e1000_supported_device(uint32_t vendor, uint32_t device_id);
bool e1000_link_up_led_status(e1000_device_t *dev);
bool e1000_check_link_up(e1000_device_t *dev);
bool e1000_auto_negotiate_link(e1000_device_t *dev);
void *alloc_map_frame(vregion_flags_t attr, size_t size, struct capref *retcap);
void e1000_hwinit(e1000_device_t *device, struct device_mem *bar_info,
                  int nr_allocated_bars,
                  volatile struct tx_desc **transmit_ring,
                  volatile union rx_desc **receive_ring,
                  int receive_buffers, int transmit_buffers,
                  uint8_t *macaddr,
                  bool user_macaddr, bool use_interrupt);


/*****************************************************************
 * On the i82541xx GPI_SP2 and GPI_SP3 are merged into one register
 * value of bits.
 *
 ****************************************************************/
static inline uint8_t i82541xx_get_icr_gpi_sdp(e1000_t *dev)
{
    e1000_intreg_t intreg = e1000_icr_rawrd(dev);
    uint8_t sdp2 = e1000_intreg_gpi_sdp2_extract(intreg);
    uint8_t sdp3 = e1000_intreg_gpi_sdp3_extract(intreg);
    uint8_t sdp = sdp2 | (sdp3 << 1);

    return sdp;
}


/*****************************************************************
 * Barrelfish has no delay. We do it like this instead.
 ****************************************************************/
static inline void usec_delay(unsigned int count)
{
    while(count) {
        __asm__ __volatile__("inb $0x80, %b0" :: "a"(0));
        count--;
    }
}


#endif
