#pragma once

class DensityProfileLayer {
 public:
  DensityProfileLayer() : DensityProfileLayer(0.0, 0.0, 0.0, 0.0, 0.0) {}
  DensityProfileLayer(double width, double exp_term, double exp_scale,
                      double linear_term, double constant_term)
      : width(width), exp_term(exp_term), exp_scale(exp_scale),
        linear_term(linear_term), constant_term(constant_term) {
  }
  double width;
  double exp_term;
  double exp_scale;
  double linear_term;
  double constant_term;
};