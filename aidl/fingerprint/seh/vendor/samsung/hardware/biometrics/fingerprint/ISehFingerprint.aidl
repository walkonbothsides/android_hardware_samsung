// SPDX-License-Identifier: Apache-2.0
package vendor.samsung.hardware.biometrics.fingerprint;

import vendor.samsung.hardware.biometrics.fingerprint.SehResult;

@VintfStability
interface ISehFingerprint {
    SehResult sehRequest(int sensorId, int command, int param, in byte[] input);
}
