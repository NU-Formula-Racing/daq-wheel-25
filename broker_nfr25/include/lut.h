#ifndef LUT_H
#define LUT_H

#include <Arduino.h>

#include <initializer_list>
#include <vector>

/// @brief The available interpolation modes
enum class InterpolationType : uint8_t {
    IT_LERP,
    IT_SMOOTHSTEP
};

/// @brief A simple struct to store (x, y) pairs
struct XYPair {
    float x;
    float y;
};

/// @brief A generic numeric lookup table that can linearly or smoothly
/// interpolate between data points.
class NumericLUT {
   public:
    /// @brief Constructs a NumericLUT from an initializer_list of XYPair.
    /// Example usage:
    /// @code
    /// NumericLUT lut = {
    ///   {0.0f, 10.0f},
    ///   {5.0f, 20.0f},
    ///   {10.0f, 40.0f}
    /// };
    /// @endcode
    /// @param data A list of (x, y) points
    /// @param type The interpolation mode (defaults to Lerp)
    NumericLUT(std::initializer_list<XYPair> data, InterpolationType type = InterpolationType::IT_LERP);

    /// @brief Sets the interpolation type (Lerp or Smoothstep)
    /// @param type The new interpolation type
    void setInterpolationType(InterpolationType type);

    /// @brief Gets an interpolated value for a given input x
    /// @param x The input value for which to compute the interpolated output
    /// @return The interpolated y value
    float getValue(float x) const;

   private:
    /// @brief Linear interpolation helper
    float lerp(float a, float b, float t) const;

    /// @brief Smoothstep interpolation helper
    float smoothstep(float a, float b, float t) const;

   private:
    std::vector<XYPair> _data;
    InterpolationType _type;
};

#endif
