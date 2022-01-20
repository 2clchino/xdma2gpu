#ifndef GPUCTl_H
#define GPUCTL_H

#include "nv-p2p.h"
//-----------------------------------------------------------------------------
// for boundary alignment requirement
#define GPU_BOUND_SHIFT 16
#define GPU_BOUND_SIZE ((u64)1 << GPU_BOUND_SHIFT)
#define GPU_BOUND_OFFSET (GPU_BOUND_SIZE-1)
#define GPU_BOUND_MASK (~GPU_BOUND_OFFSET)
#define DEVICE_NAME "nvidia-fs"

//-----------------------------------------------------------------------------

struct gpudma_lock_t {
    void*    handle;
    uint64_t addr;
    uint64_t size;
    size_t   page_count;
};

//-----------------------------------------------------------------------------

struct gpudma_unlock_t {
    void*    handle;
};

//-----------------------------------------------------------------------------

struct gpudma_state_t {
    void*       handle;
    size_t      page_count;
    size_t      page_size;
    uint64_t    pages[1];
};

//-----------------------------------------------------------------------------

int say_hello(void);
struct nvidia_p2p_page_table* nv_p2p_get(unsigned long arg, struct pci_dev *pdev, struct nvidia_p2p_dma_mapping **dma_mapping);

#define nvfs_msg(KRNLVL, FMT, ARGS...) printk(KRNLVL DEVICE_NAME ":" FMT, ## ARGS)
#define nvfs_err(FMT, ARGS...)                               \
    nvfs_msg(KERN_ERR, FMT, ## ARGS)

#endif
