// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <aidl/vendor/samsung/hardware/biometrics/fingerprint/BnSehFingerprint.h>

#include "Fingerprint.h"

namespace aidl::android::hardware::biometrics::fingerprint {

class SehFingerprint final : public ::aidl::vendor::samsung::hardware::biometrics::fingerprint::BnSehFingerprint {
  public:
    explicit SehFingerprint(std::shared_ptr<Fingerprint> fingerprint)
        : mFingerprint(std::move(fingerprint)) {}

    ndk::ScopedAStatus sehRequest(
            int32_t sensorId, int32_t command, int32_t param, const std::vector<uint8_t>& input,
            ::aidl::vendor::samsung::hardware::biometrics::fingerprint::SehResult* out) override;

  private:
    std::shared_ptr<Fingerprint> mFingerprint;
};

}  // namespace aidl::android::hardware::biometrics::fingerprint
