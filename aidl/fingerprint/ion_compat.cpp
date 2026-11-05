/*
 * Legacy libion compatibility shim.
 *
 * Samsung's libsfp_teegw.so references ion_close() (legacy libion API) but
 * declares no DT_NEEDED that provides it. The legacy libion implementation
 * defines ion_close(fd) as close(fd). Exporting it from this executable lets
 * the dynamic linker resolve the dangling reference when the fingerprint
 * backend (libsfp_sensor.so -> libsfp_teegw.so) is dlopen()ed.
 */
#include <unistd.h>

extern "C" __attribute__((visibility("default"))) int ion_close(int fd) {
    return close(fd);
}
