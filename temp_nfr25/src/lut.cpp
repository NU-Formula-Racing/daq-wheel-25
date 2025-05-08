#include "lut.hpp"

#include <algorithm>  // For std::sort

NumericLUT::NumericLUT(std::initializer_list<XYPair> data, InterpolationType type)
    : _type(type) {
    // Copy data into our vector
    _data = data;

    // Ensure it is sorted by X (ascending), so we can do proper segment searching
    std::sort(_data.begin(), _data.end(), [](const XYPair &a, const XYPair &b) {
        return a.x < b.x;
    });
}

void NumericLUT::setInterpolationType(InterpolationType type) {
    _type = type;
}

float NumericLUT::getValue(float x) const {
    // Handle edge cases
    if (_data.empty()) {
        // No data => return 0, or handle error as desired
        return 0.0f;
    }

    // If x is before the first point, return the first y
    if (x <= _data.front().x) {
        return _data.front().y;
    }

    // If x is beyond the last point, return the last y
    if (x >= _data.back().x) {
        return _data.back().y;
    }

    // Otherwise, find which two points x is between
    for (size_t i = 0; i < _data.size() - 1; ++i) {
        if (x < _data[i + 1].x) {
            // We found the segment x is in
            float x0 = _data[i].x;
            float x1 = _data[i + 1].x;
            float y0 = _data[i].y;
            float y1 = _data[i + 1].y;

            // Avoid division by zero in case of duplicate x-values
            float range = (x1 - x0);
            if (range == 0.0f) {
                return y0;  // or y1, they’re the same if x0 == x1
            }

            // Normalized t in [0, 1]
            float t = (x - x0) / range;

            switch (_type) {
                case InterpolationType::IT_SMOOTHSTEP:
                    return smoothstep(y0, y1, t);
                case InterpolationType::IT_LERP:
                default:
                    return lerp(y0, y1, t);
            }
        }
    }

    // Should never get here if the data is sorted and has at least 2 points
    return _data.back().y;
}

float NumericLUT::getInverseValue(float y) const {
    // 1) no data?
    if (_data.empty()) {
        return 0.0f;
    }

    // 2) clamp to ends
    if (y <= _data.front().y) {
        return _data.front().x;
    }
    if (y >= _data.back().y) {
        return _data.back().x;
    }

    // 3) find the segment where y lives
    for (size_t i = 0; i < _data.size() - 1; ++i) {
        float x0 = _data[i].x, y0 = _data[i].y;
        float x1 = _data[i + 1].x, y1 = _data[i + 1].y;

        // does y lie between y0 and y1?
        if ((y >= y0 && y <= y1) || (y <= y0 && y >= y1)) {
            float dx = x1 - x0;
            float dy = y1 - y0;

            // flat segment?
            if (dy == 0.0f) {
                return x0;
            }

            float t;
            if (_type == InterpolationType::IT_LERP) {
                // linear inverse: t = (y - y0)/(y1 - y0)
                t = (y - y0) / dy;
            } else {
                // smoothstep inverse: solve  f(t)=3t^2−2t^3 = s
                float s = (y - y0) / dy;
                float lo = 0.0f, hi = 1.0f;
                // binary‐search t in [0,1] until f(t)≈s
                for (int iter = 0; iter < 20; ++iter) {
                    float mid = 0.5f * (lo + hi);
                    float f = mid * mid * (3.0f - 2.0f * mid);  // 3t^2 - 2t^3
                    if (f < s)
                        lo = mid;
                    else
                        hi = mid;
                }
                t = 0.5f * (lo + hi);
            }

            // map back to x
            return x0 + dx * t;
        }
    }

    // fallback
    return _data.back().x;
}

float NumericLUT::lerp(float a, float b, float t) const {
    return a + (b - a) * t;
}

float NumericLUT::smoothstep(float a, float b, float t) const {
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    float t2 = t * t;
    float t3 = t2 * t;
    // 3t^2 - 2t^3
    float s = 3.0f * t2 - 2.0f * t3;
    return a + (b - a) * s;
}
