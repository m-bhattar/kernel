/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#ifndef _QDA_IOCTL_H
#define _QDA_IOCTL_H

#include <linux/types.h>
#include <linux/kernel.h>
#include <drm/drm_ioctl.h>
#include "qda_drv.h"

int qda_ioctl_query(struct drm_device *dev, void *data, struct drm_file *file_priv);
int qda_ioctl_prime_fd_to_handle(struct drm_device *dev, struct drm_file *file_priv,
				 int prime_fd, u32 *handle);
int qda_ioctl_create(struct drm_device *dev, void *data, struct drm_file *file_priv);
int qda_release_dsp_process(struct qda_dev *qdev, struct drm_file *file_priv);
int qda_ioctl_invoke(struct drm_device *dev, void *data, struct drm_file *file_priv);
int qda_ioctl_mmap(struct drm_device *dev, void *data, struct drm_file *file_priv);
int qda_ioctl_munmap(struct drm_device *dev, void *data, struct drm_file *file_priv);

#endif /* _QDA_IOCTL_H */
