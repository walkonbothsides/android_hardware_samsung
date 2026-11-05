/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "LegacyHAL.h"

#include <android-base/logging.h>

#include <dlfcn.h>
#include <cerrno>

namespace aidl {
namespace android {
namespace hardware {
namespace biometrics {
namespace fingerprint {

bool LegacyHAL::openHal(fingerprint_notify_t notify) {
    void* handle = dlopen("libbauthserver.so", RTLD_NOW);

    if (!handle) {
        handle = dlopen("libsfp_sensor.so", RTLD_NOW);
    }

    if (!handle) {
        return false;
    }

    if (handle) {
        int err;

        ss_fingerprint_close = reinterpret_cast<typeof(ss_fingerprint_close)>(
                dlsym(handle, "ss_fingerprint_close"));
        ss_fingerprint_open =
                reinterpret_cast<typeof(ss_fingerprint_open)>(dlsym(handle, "ss_fingerprint_open"));

        ss_set_notify_callback = reinterpret_cast<typeof(ss_set_notify_callback)>(
                dlsym(handle, "ss_set_notify_callback"));
        ss_fingerprint_pre_enroll = reinterpret_cast<typeof(ss_fingerprint_pre_enroll)>(
                dlsym(handle, "ss_fingerprint_pre_enroll"));
        ss_fingerprint_enroll = reinterpret_cast<typeof(ss_fingerprint_enroll)>(
                dlsym(handle, "ss_fingerprint_enroll"));
        ss_fingerprint_post_enroll = reinterpret_cast<typeof(ss_fingerprint_post_enroll)>(
                dlsym(handle, "ss_fingerprint_post_enroll"));
        ss_fingerprint_get_auth_id = reinterpret_cast<typeof(ss_fingerprint_get_auth_id)>(
                dlsym(handle, "ss_fingerprint_get_auth_id"));
        ss_fingerprint_cancel = reinterpret_cast<typeof(ss_fingerprint_cancel)>(
                dlsym(handle, "ss_fingerprint_cancel"));
        ss_fingerprint_enumerate = reinterpret_cast<typeof(ss_fingerprint_enumerate)>(
                dlsym(handle, "ss_fingerprint_enumerate"));
        ss_fingerprint_remove = reinterpret_cast<typeof(ss_fingerprint_remove)>(
                dlsym(handle, "ss_fingerprint_remove"));
        ss_fingerprint_set_active_group = reinterpret_cast<typeof(ss_fingerprint_set_active_group)>(
                dlsym(handle, "ss_fingerprint_set_active_group"));
        ss_fingerprint_authenticate = reinterpret_cast<typeof(ss_fingerprint_authenticate)>(
                dlsym(handle, "ss_fingerprint_authenticate"));
        ss_fingerprint_request = reinterpret_cast<typeof(ss_fingerprint_request)>(
                dlsym(handle, "ss_fingerprint_request"));

        if ((err = ss_fingerprint_open(nullptr)) != 0) {
            LOG(ERROR) << "Can't open fingerprint, error: " << err;
            return false;
        }

        if ((err = ss_set_notify_callback(notify)) != 0) {
            LOG(ERROR) << "Can't register fingerprint module callback, error: " << err;
            return false;
        }

        return true;
    }

    return false;
}

int LegacyHAL::request(int cmd, int param, const std::vector<uint8_t>& input,
                       std::vector<uint8_t>* output) {
    constexpr size_t kMaxFidoBytes = 512;
    output->clear();
    if (!ss_fingerprint_request || input.empty() || input.size() > kMaxFidoBytes) {
        return -EINVAL;
    }

    // The legacy ABI uses mutable buffers even for request input.
    std::vector<uint8_t> request(input);
    output->resize(kMaxFidoBytes);
    int result = ss_fingerprint_request(cmd, reinterpret_cast<char*>(request.data()),
            request.size(), reinterpret_cast<char*>(output->data()), output->size(), param);
    if (result <= 0 || static_cast<size_t>(result) > output->size()) {
        output->clear();
        return result > 0 ? -EOVERFLOW : result;
    }
    output->resize(result);
    return result;
}

int LegacyHAL::request(int cmd, int param) {
    // TO-DO: input, output handling not implemented
    int result = ss_fingerprint_request(cmd, nullptr, 0, nullptr, 0, param);
    LOG(INFO) << "request(cmd=" << cmd << ", param=" << param << ", result=" << result << ")";
    return result;
}

}  // namespace fingerprint
}  // namespace biometrics
}  // namespace hardware
}  // namespace android
}  // namespace aidl
