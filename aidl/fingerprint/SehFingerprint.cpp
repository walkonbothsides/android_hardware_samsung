// SPDX-License-Identifier: Apache-2.0
#include "SehFingerprint.h"

#include "VendorConstants.h"

namespace aidl::android::hardware::biometrics::fingerprint {

ndk::ScopedAStatus SehFingerprint::sehRequest(
        int32_t sensorId, int32_t command, int32_t param, const std::vector<uint8_t>& input,
        ::aidl::vendor::samsung::hardware::biometrics::fingerprint::SehResult* out) {
    // Only expose the read-only IFAA ID query, not arbitrary Samsung factory commands.
    static const std::vector<uint8_t> kIfaaIdQuery = {
        0x01, 0x53, 0x0c, 0x00, 0x02, 0x53, 0x04, 0x00,
        0x69, 0x66, 0x61, 0x61, 0x0a, 0x00, 0x00, 0x00,
    };
    if (sensorId != 0 || command != FINGERPRINT_REQUEST_PROCESS_FIDO || param != 0
            || input != kIfaaIdQuery) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_ARGUMENT);
    }

    out->retValue = mFingerprint->requestFido(input, &out->data);
    return ndk::ScopedAStatus::ok();
}

}  // namespace aidl::android::hardware::biometrics::fingerprint
