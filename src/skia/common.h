#ifndef _COMMON_H_
#define _COMMON_H_

#include <pybind11/pybind11.h>
#include <skia.h>
#include <sstream>

namespace pybind11 { class array; }  // namespace pybind11

namespace py = pybind11;


PYBIND11_DECLARE_HOLDER_TYPE(T, sk_sp<T>);

template <typename T>
sk_sp<T> CloneFlattenable(const T& flattenable) {
    auto data = flattenable.serialize();
    return T::Deserialize(data->data(), data->size());
}
template <> sk_sp<SkShader> CloneFlattenable(const SkShader& shader);

sk_sp<SkColorSpace> CloneColorSpace(const SkColorSpace* cs);

sk_sp<SkImage> CloneImage(const SkImage& image);

size_t ValidateBufferToImageInfo(
    const SkImageInfo& imageInfo, const py::buffer_info& buffer,
    size_t rowBytes = 0);

template <typename T>
bool ReadPixels(T& readable, const SkImageInfo& imageInfo, py::buffer dstPixels,
                size_t dstRowBytes, int srcX, int srcY) {
    auto info = dstPixels.request(true);
    auto rowBytes = ValidateBufferToImageInfo(imageInfo, info, dstRowBytes);
    return readable.readPixels(imageInfo, info.ptr, rowBytes, srcX, srcY);
}

SkImageInfo NumPyToImageInfo(
    py::array array, SkColorType ct, SkAlphaType at, const SkColorSpace* cs);

py::buffer_info ImageInfoToBufferInfo(
    const SkImageInfo& imageInfo, void* data, ssize_t rowBytes = 0,
    bool readonly = true);
py::dict ImageInfoToArrayInterface(
    const SkImageInfo& imageInfo, size_t rowBytes = 0);
#endif  // _COMMON_H_