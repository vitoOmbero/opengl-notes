#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace attribute
{

enum Type : unsigned char
{
    kPosition,
    kColor
};

enum Value : unsigned char
{
    kPosNormalizedXCoord,
    kPosNormalizedYCoord,
    kPosNormalizedZCoord,

    kColRedChannel,
    kColGreenChannel,
    kColBlueChannel,
    kColAlphaChannel
};

enum Scheme : unsigned char
{
    kPosition2d = 0,
    kPosition3d = 1,
    kColorRgb   = 2,
    kColorRgba  = 3,
};

namespace scheme
{

static inline const Value position_2d[]{ kPosNormalizedXCoord,
                                         kPosNormalizedYCoord };
static inline const GLint position_2d_attr_n = 2;
static inline const Value position_3d[]{ kPosNormalizedXCoord,
                                         kPosNormalizedYCoord,
                                         kPosNormalizedZCoord };
static inline const GLint position_3d_attr_n = 3;
static inline const Value color_rgb[]{ kColRedChannel, kColGreenChannel,
                                       kColBlueChannel };
static inline const GLint color_rgb_attr_n = 3;
static inline const Value color_rgba[]{ kColRedChannel, kColGreenChannel,
                                        kColBlueChannel, kColAlphaChannel };
static inline const GLint color_rgba_attr_n = 4;
} // namespace scheme

#pragma pack(push, 1)
struct PackDescriprionValue
{
    ///> value of attribute (e.g. x_coord, red_channel ...)
    const Value* values;
    ///> number of attribute values in scheme
    const GLint attributes_n;
    ///> type of attribute (e.g. position, color ...)
    const Type type;
};
#pragma pack(pop)

// clang-format off
template <Scheme key> struct PackDescription {   static PackDescriprionValue values;    };

template <> struct PackDescription<Scheme::kPosition2d> { static PackDescriprionValue values; };

template <> struct PackDescription<Scheme::kPosition3d> { static PackDescriprionValue values; };

template <> struct PackDescription<Scheme::kColorRgb> { static PackDescriprionValue values; };

template <> struct PackDescription<Scheme::kColorRgba> { static PackDescriprionValue values; };

// clang-format on

namespace RuntimeMap
{

const PackDescriprionValue* Get(Scheme scheme);

} // namespace RuntimeMap
} // namespace attribute

#endif // ATTRIBUTE_H
