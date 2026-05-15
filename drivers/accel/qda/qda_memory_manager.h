/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#ifndef _QDA_MEMORY_MANAGER_H
#define _QDA_MEMORY_MANAGER_H

#include <linux/device.h>
#include <linux/refcount.h>
#include <linux/spinlock.h>
#include <linux/xarray.h>
#include <drm/drm_file.h>
#include "qda_gem.h"

/**
 * struct qda_iommu_device - IOMMU device instance for memory management
 *
 * This structure represents a single IOMMU-enabled device managed by the
 * memory manager. Each device can be assigned to a specific process.
 */
struct qda_iommu_device {
	/* Unique identifier for this IOMMU device */
	u32 id;
	/* Pointer to the underlying device */
	struct device *dev;
	/* Spinlock protecting concurrent access to device */
	spinlock_t lock;
	/* Reference counter for device */
	refcount_t refcount;
	/* Stream ID for IOMMU transactions */
	u32 sid;
	/* Process ID of the process assigned to this device */
	pid_t assigned_pid;
	/* DRM file private data for the assigned process */
	struct drm_file *assigned_file_priv;
};

/**
 * struct qda_memory_manager - Central memory management coordinator
 *
 * This is the top-level structure coordinating memory management across
 * multiple IOMMU devices. It maintains a registry of devices and ensures
 * thread-safe access to shared resources.
 */
struct qda_memory_manager {
	/* XArray storing all registered IOMMU devices */
	struct xarray device_xa;
	/* Mutex protecting process-to-device assignments */
	struct mutex process_assignment_lock;
};

int qda_memory_manager_init(struct qda_memory_manager *mem_mgr);
void qda_memory_manager_exit(struct qda_memory_manager *mem_mgr);

int qda_memory_manager_register_device(struct qda_memory_manager *mem_mgr,
				       struct qda_iommu_device *iommu_dev);
void qda_memory_manager_unregister_device(struct qda_memory_manager *mem_mgr,
					  struct qda_iommu_device *iommu_dev);

int qda_memory_manager_assign_device(struct qda_memory_manager *mem_mgr,
				     struct drm_file *file_priv);

int qda_memory_manager_alloc(struct qda_memory_manager *mem_mgr, struct qda_gem_obj *gem_obj,
			     struct drm_file *file_priv);
void qda_memory_manager_free(struct qda_memory_manager *mem_mgr, struct qda_gem_obj *gem_obj);

#endif /* _QDA_MEMORY_MANAGER_H */