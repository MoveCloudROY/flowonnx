#ifndef TENSOR_H
#define TENSOR_H

#include <vector>
#include <string>

#include <flowonnx/flowonnxglobal.h>

namespace flowonnx {

    class FLOWONNX_EXPORT Tensor {
    public:
        enum DataType {
            Float = 1,
            UInt8 = 2,
            Int8 = 3,
            UInt16 = 4,
            Int16 = 5,
            Int32 = 6,
            Int64 = 7,
            String = 8,
            Bool = 9,
            Float16 = 10,     // 0x0000000A
            Double = 11,      // 0x0000000B
            UInt32 = 12,      // 0x0000000C
            UInt64 = 13,      // 0x0000000D
            Complex64 = 14,   // 0x0000000E
            Complex128 = 15,  // 0x0000000F
            BFloat16 = 16,    // 0x00000010
            DataTypeMax = 17, // 0x00000011
        };

        std::vector<char> data;
        std::vector<int> shape;
        DataType type = Float;

        static Tensor create(float *data, size_t dataSize, int *shape, size_t shapeSize);
        static Tensor create(int64_t *data, size_t dataSize, int *shape, size_t shapeSize);
        static Tensor create(bool *data, size_t dataSize, int *shape, size_t shapeSize);
    };

}

#endif // TENSOR_H
