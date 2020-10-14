#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace attribute
{

enum Type
{
    kPosition,
    kColor
};

enum Value
{
    kPosNormalizedXCoord,
    kPosNormalizedYCoord,
    kPosNormalizedZCoord,

    kColRedChannel,
    kColGreenChannel,
    kColBlueChannel,
    kColAlphaChannel
};

/* actually no need in runtime map
#include <map>
const std::multimap<Type, Values> dictionary{
{kPosition, kPosNormalizedXCoord},
{kPosition, kPosNormalizedYCoord},
{kPosition, kPosNormalizedZCoord},

{kColor, kColRedChannel},
{kColor, kColGreenChannel},
{kColor, kColBlueChannel},
{kColor, kColAlphaChannel},
};
*/

namespace scheme
{

const Value  position_2d[]{ kPosNormalizedXCoord, kPosNormalizedYCoord };
const GLint  position_2d_attr_n = 2;
const Value  position_3d[]{ kPosNormalizedXCoord, kPosNormalizedYCoord,
                           kPosNormalizedZCoord };
const GLint  position_3d_attr_n = 3;
const Value  color_rgb[]{ kColRedChannel, kColGreenChannel, kColBlueChannel };
const GLint  color_rgb_attr_n = 3;
const Value  color_rgba[]{ kColRedChannel, kColGreenChannel, kColBlueChannel,
                          kColAlphaChannel };
const GLint  color_rgba_attr_n = 4;
} // namespace scheme

} // namespace attribute

#endif // ATTRIBUTE_H
