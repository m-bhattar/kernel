/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#ifndef _QDA_MEMORY_DMA_H
#define _QDA_MEMORY_DMA_H

#include <linux/dma-mapping.h>
#include "qda_memory_manager.h"

int qda_dma_alloc(struct qda_iommu_device *iommu_dev,
		  struct qda_gem_obj *gem_obj, size_t size);
void qda_dma_free(struct qda_gem_obj *gem_obj);
int qda_dma_mmap(struct qda_gem_obj *gem_obj, struct vm_area_struct *vma);

#endif /* _QDA_MEMORY_DMA_H */
