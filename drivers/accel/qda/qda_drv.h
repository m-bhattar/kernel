/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#ifndef __QDA_DRV_H__
#define __QDA_DRV_H__

#include <linux/device.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/rpmsg.h>
#include <linux/types.h>
#include <linux/xarray.h>
#include <drm/drm_drv.h>
#include <drm/drm_file.h>
#include <drm/drm_device.h>
#include <drm/drm_accel.h>
#include <drm/drm_print.h>
#include "qda_memory_manager.h"

/* Driver identification */
#define DRIVER_NAME "qda"

/**
 * struct qda_file_priv - Per-process private data for DRM file
 */
struct qda_file_priv {
	/* Process ID for tracking */
	pid_t pid;
	/* Pointer to qda_user structure */
	struct qda_user *qda_user;
	/* IOMMU device assigned to this process */
	struct qda_iommu_device *assigned_iommu_dev;
};

/**
 * struct qda_user - Per-user context for remote processor interaction
 */
struct qda_user {
	/* Unique client identifier */
	u32 client_id;
	/* Back-pointer to device structure */
	struct qda_dev *qda_dev;
	/* GEM object for PD initialization memory */
	struct qda_gem_obj *init_mem_gem_obj;
};

/**
 * struct qda_dev - Main device structure for QDA driver
 *
 * The DRM device is embedded as the first member so that container_of()
 * can recover the qda_dev from any drm_device pointer.
 */
struct qda_dev {
	/* DRM device (embedded; recover via qda_dev_from_drm()) */
	struct drm_device drm_dev;
	/* RPMsg device for communication with remote processor */
	struct rpmsg_device *rpdev;
	/* Underlying device structure */
	struct device *dev;
	/* IOMMU/memory manager */
	struct qda_memory_manager *iommu_mgr;
	/* Atomic counter for generating unique client IDs */
	atomic_t client_id_counter;
	/* Name of the DSP (e.g., "cdsp", "adsp") */
	const char *dsp_name;
	/* Compute context-bank (CB) child devices */
	struct list_head cb_devs;
	/* XArray for FastRPC context management */
	struct xarray ctx_xa;
	/* Lock protecting prime import context */
	struct mutex import_lock;
	/* Current file_priv during prime import */
	struct drm_file *current_import_file_priv;
};

/**
 * qda_dev_from_drm - Recover qda_dev from an embedded drm_device pointer
 */
static inline struct qda_dev *qda_dev_from_drm(struct drm_device *dev)
{
	return container_of(dev, struct qda_dev, drm_dev);
}

/*
 * Device allocation (must use devm_drm_dev_alloc internally)
 */
struct qda_dev *qda_alloc_device(struct device *dev);

/*
 * Core device management functions
 */
int qda_init_device(struct qda_dev *qdev);
void qda_deinit_device(struct qda_dev *qdev);
int qda_register_device(struct qda_dev *qdev);
void qda_unregister_device(struct qda_dev *qdev);

#endif /* __QDA_DRV_H__ */
