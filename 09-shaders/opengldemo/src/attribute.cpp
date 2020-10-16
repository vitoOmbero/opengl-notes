#include "attribute.h"

namespace attribute
{

// clang-format off


PackDescriprionValue PackDescription<Scheme::kPosition2d>::values = PackDescriprionValue{
    scheme::position_2d, scheme::position_2d_attr_n, Type::kPosition };

PackDescriprionValue PackDescription<Scheme::kPosition3d>::values = PackDescriprionValue{
    scheme::position_3d, scheme::position_3d_attr_n, Type::kPosition };

PackDescriprionValue PackDescription<Scheme::kColorRgb>::values = PackDescriprionValue{
    scheme::color_rgb, scheme::color_rgb_attr_n, Type::kColor };

PackDescriprionValue PackDescription<Scheme::kColorRgba>::values = PackDescriprionValue{
    scheme::color_rgba, scheme::color_rgba_attr_n, Type::kColor };

// clang-format on

static PackDescriprionValue static_map[]{
    // NOTE: order must be the same as attribute::Scheme code
    PackDescription<Scheme::kPosition2d>::values,
    PackDescription<Scheme::kPosition3d>::values,
    PackDescription<Scheme::kColorRgb>::values,
    PackDescription<Scheme::kColorRgba>::values,
};

const attribute::PackDescriprionValue* attribute::RuntimeMap::Get(
    attribute::Scheme scheme)
{
    unsigned char index = static_cast<unsigned char>(scheme);
    return &static_map[index];
};

} // namespace attribute
