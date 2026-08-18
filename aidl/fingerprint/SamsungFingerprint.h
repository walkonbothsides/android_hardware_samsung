/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <android/binder_interface_utils.h>
#include <android/binder_parcelable_utils.h>

#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace aidl::vendor::samsung::hardware::biometrics::fingerprint {

class SehResult {
  public:
    using fixed_size = std::false_type;
    static const char* descriptor;

    int32_t retValue = -1;
    std::vector<uint8_t> data;

    binder_status_t readFromParcel(const AParcel* parcel);
    binder_status_t writeToParcel(AParcel* parcel) const;

    static const ::ndk::parcelable_stability_t _aidl_stability = ::ndk::STABILITY_VINTF;
};

class ISehFingerprint : public ::ndk::ICInterface {
  public:
    static const char* descriptor;
    ISehFingerprint();
    virtual ~ISehFingerprint();

    static inline const int32_t version = 1;
    static inline const std::string hash = "d1ee0e678dcdec96506558215f7a8b0b2acd74a9";

    static std::shared_ptr<ISehFingerprint> fromBinder(const ::ndk::SpAIBinder& binder);
    static binder_status_t writeToParcel(AParcel* parcel,
                                         const std::shared_ptr<ISehFingerprint>& instance);
    static binder_status_t readFromParcel(const AParcel* parcel,
                                          std::shared_ptr<ISehFingerprint>* instance);
    static bool setDefaultImpl(const std::shared_ptr<ISehFingerprint>& impl);
    static const std::shared_ptr<ISehFingerprint>& getDefaultImpl();

    virtual ::ndk::ScopedAStatus sehRequest(int32_t sensorId, int32_t cmd, int32_t param,
                                            const std::vector<uint8_t>& input,
                                            SehResult* result) = 0;
    virtual ::ndk::ScopedAStatus getInterfaceVersion(int32_t* version) = 0;
    virtual ::ndk::ScopedAStatus getInterfaceHash(std::string* hash) = 0;

  private:
    static std::shared_ptr<ISehFingerprint> default_impl;
};

class BnSehFingerprint : public ::ndk::BnCInterface<ISehFingerprint> {
  public:
    BnSehFingerprint();
    virtual ~BnSehFingerprint();
    ::ndk::ScopedAStatus getInterfaceVersion(int32_t* version) final;
    ::ndk::ScopedAStatus getInterfaceHash(std::string* hash) final;

  protected:
    ::ndk::SpAIBinder createBinder() override;
};

}  // namespace aidl::vendor::samsung::hardware::biometrics::fingerprint
