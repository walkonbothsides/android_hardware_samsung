/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Fingerprint.h"
#include "SamsungFingerprint.h"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

#include <cerrno>

using ::aidl::android::hardware::biometrics::fingerprint::Fingerprint;
using ::aidl::vendor::samsung::hardware::biometrics::fingerprint::BnSehFingerprint;
using ::aidl::vendor::samsung::hardware::biometrics::fingerprint::ISehFingerprint;
using ::aidl::vendor::samsung::hardware::biometrics::fingerprint::SehResult;

namespace {
constexpr int kSensorId = 0;
constexpr int kProcessFidoCommand = 9;
constexpr size_t kMaxFidoRequestSize = 0x2800;

class SehFingerprint : public BnSehFingerprint {
  public:
    explicit SehFingerprint(std::shared_ptr<Fingerprint> fingerprint)
        : mFingerprint(std::move(fingerprint)) {}

    ndk::ScopedAStatus sehRequest(int32_t sensorId, int32_t cmd, int32_t param,
                                  const std::vector<uint8_t>& input,
                                  SehResult* result) override {
        if (sensorId != kSensorId || cmd != kProcessFidoCommand || param != 0 ||
            input.size() > kMaxFidoRequestSize) {
            result->retValue = -EINVAL;
            return ndk::ScopedAStatus::ok();
        }
        result->retValue = mFingerprint->sehRequest(cmd, param, input, &result->data);
        return ndk::ScopedAStatus::ok();
    }

  private:
    std::shared_ptr<Fingerprint> mFingerprint;
};
}  // namespace

int main() {
    ABinderProcess_setThreadPoolMaxThreadCount(0);
    std::shared_ptr<Fingerprint> fingerprint = ndk::SharedRefBase::make<Fingerprint>();

    const std::string instance = std::string() + Fingerprint::descriptor + "/default";
    binder_status_t status =
            AServiceManager_addService(fingerprint->asBinder().get(), instance.c_str());
    CHECK(status == STATUS_OK);

    std::shared_ptr<SehFingerprint> sehFingerprint =
            ndk::SharedRefBase::make<SehFingerprint>(fingerprint);
    const std::string sehInstance = std::string() + ISehFingerprint::descriptor + "/default";
    status = AServiceManager_addService(sehFingerprint->asBinder().get(), sehInstance.c_str());
    CHECK(status == STATUS_OK);

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;  // should not reach
}
